#include "SkinUtil.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "Utils.h"
#include "Logger.h"
#include "Json.hpp"
#include <fstream>
#include <sstream>
#include <string>

using namespace nlohmann;

namespace MeshStructs {
	void to_json(json& j, const face& f) {
		std::vector<std::array<int, 3>> partArray;

		for (int i = 0; i < f.facesPresent; i++) {
			auto ind = f.indices[i];
			partArray.push_back({ind.vertIndex, ind.normalIndex, ind.uvIndex});
		}
		j = partArray;
	}
}

void SkinUtil::importGeo(std::wstring filePath) {
	std::string contents = Utils::readFileContents(filePath);
	if (contents.size() == 0)
		return;

	auto hResourceGeometry = FindResourceA((HMODULE)g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_TEXT1), "TEXT");
	auto hMemoryGeometry = LoadResource((HMODULE)g_Data.getDllModule(), hResourceGeometry);

	auto sizeGeometry = SizeofResource((HMODULE)g_Data.getDllModule(), hResourceGeometry);
	auto ptrGeometry = LockResource(hMemoryGeometry);
	logF("Starting geometry import");
	auto mesh = SkinUtil::objToMesh(contents.c_str());
	logF("Mesh created (verts: %i, uvs: %i, normals: %i, faces: %i)", mesh.vertices.size(), mesh.uvs.size(), mesh.normals.size(), mesh.faces.size());
	std::string moddedGeo = SkinUtil::modGeometry(reinterpret_cast<char*>(ptrGeometry), mesh);
	g_Data.setCustomGeometryOverride(true, std::make_shared<std::string>(moddedGeo));
	logF("Geometry import done");

	if (hMemoryGeometry)
		FreeResource(hMemoryGeometry);
}
std::string SkinUtil::modGeometry(const char* oldGeoStr, MeshStructs::meshData mesh) {
	auto oldGeo = std::string(oldGeoStr);
	json geoMod = json::parse(oldGeo);  // If this crashes, coolroblox json is invalid
	auto geoParts = &geoMod.at("minecraft:geometry");

	for (auto it = geoParts->begin(); it != geoParts->end(); it++) {
		auto part = it.value();
		std::string identifier = part["description"]["identifier"].get<std::string>();
		if (identifier.find("animated") != std::string::npos)
			continue;

		auto bones = &part.at("bones");
		for (auto boneIt = bones->begin(); boneIt != bones->end(); boneIt++) {
			auto bone = *boneIt;
			if (bone["cubes"].is_array() || bone["poly_mesh"].is_object())
				continue;
			std::string name = bone["name"];
			std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });
			if (name == "body") {
				json polyMesh;
				polyMesh["normalized_uvs"] = true;  // blender does that apparently
				polyMesh["normals"] = mesh.normals;
				polyMesh["positions"] = mesh.vertices;
				polyMesh["uvs"] = mesh.uvs;
				polyMesh["polys"] = mesh.faces;

				boneIt->emplace("poly_mesh", polyMesh);
			}
		}

		it->swap(part);
	}
	return geoMod.dump();
}
MeshStructs::meshData SkinUtil::objToMesh(const char* str, bool doWierdMogangStuff) {
	std::istringstream f(str);
	std::string line;

	std::vector<std::array<float, 3>> vertices;
	std::vector<std::array<float, 3>> normals;
	std::vector<std::array<float, 2>> uvs;
	std::vector<MeshStructs::face> faces;
	std::vector<int> faceFixList;  // Fix faces without uvs

	while (std::getline(f, line)) {
		// Remove trailing whitespace
		// left out for performance reasons
		// clang-format off
		/*{
			size_t startpos = line.find_first_not_of(" \t");
			if (std::string::npos != startpos) {
				line = line.substr(startpos);
			}
		}*/
		// clang-format on

		if (line[0] != 'f' && line[0] != 'v')
			continue;

		auto firstWhiteSpace = line.find(" ");
		if (firstWhiteSpace == std::string::npos)  // comment
			continue;

		std::vector<std::string> args;
		size_t pos = firstWhiteSpace, initialPos = 0;
		while (pos != std::string::npos) {
			args.push_back(line.substr(initialPos, pos - initialPos));
			initialPos = pos + 1;

			pos = line.find(" ", initialPos);
		}
		args.push_back(line.substr(initialPos, std::min(pos, line.size()) - initialPos + 1));

		auto cmd = args[0].c_str();

		if (strcmp(cmd, "vt") == 0) {  // uv
			if (args.size() != 3) {
				logF("Faulty uv, 2 args expected: %s", line.c_str());
				continue;
			}
			uvs.push_back({std::stof(args[1]), std::stof(args[2])});
		} else if (strcmp(cmd, "v") == 0) {  // vertex
			if (args.size() != 4) {
				logF("Faulty vertex, 3 args expected: %s", line.c_str());
				continue;
			}
			if (doWierdMogangStuff) // flip x axis
				vertices.push_back({-std::stof(args[1]), std::stof(args[2]), std::stof(args[3])});
			else
				vertices.push_back({std::stof(args[1]), std::stof(args[2]), std::stof(args[3])});
		} else if (strcmp(cmd, "f") == 0) {  // face
			if (args.size() != 5 && args.size() != 4) {
				logF("Faulty face, only quads or tris allowed: %i", args.size() - 1);
				continue;
			}

			bool needsFix = false;
			MeshStructs::face face;
			face.facesPresent = (int)args.size() - 1;
			for (int i = 1; i < args.size(); i++) {
				MeshStructs::face::facePart part;
				auto arg = args[i];
				size_t doubleOff = arg.find("//");
				if (doubleOff != std::string::npos) {  //  vertex // normal
					int vertex = std::stoi(arg.substr(0, doubleOff)) - 1;
					int normal = std::stoi(arg.substr(doubleOff + 2)) - 1;
					assert(vertex >= 0);
					assert(normal >= 0);  // negative indices mean relative from last, too lazy for that tbh
					part.vertIndex = vertex;
					part.normalIndex = normal;
					part.uvIndex = -1;
					needsFix = true;
				} else {
					size_t firstOff = arg.find("/");
					if (firstOff != std::string::npos) {
						int vertex = std::stoi(arg.substr(0, firstOff)) - 1;
						assert(vertex >= 0);
						part.vertIndex = vertex;

						auto afterSingle = arg.substr(firstOff + 1);
						size_t secondOff = afterSingle.find("/");
						if (secondOff != std::string::npos) {  // vertex / uv / normal
							int uv = std::stoi(afterSingle.substr(0, secondOff)) - 1;
							int normal = std::stoi(afterSingle.substr(secondOff + 1)) - 1;
							assert(uv >= 0);
							assert(normal >= 0);

							part.uvIndex = uv;
							part.normalIndex = normal;
						} else {  // vertex / uv
							assert(false);
						}
					} else
						assert(false);  // vertex only, don't think minecraft even supports this
				}

				face.indices[i - 1] = part;
			}
			if (args.size() == 4 && doWierdMogangStuff) {  // Convert triangles to quads
				face.facesPresent++;
				face.indices[face.facesPresent - 1] = face.indices[face.facesPresent - 2];
			}
			faces.push_back(face);
			if (needsFix)
				faceFixList.push_back((int)faces.size() - 1);

		} else if (strcmp(cmd, "vn") == 0) {  // normal
			if (args.size() != 4) {
				logF("Faulty normal, 3 args expected: %s", line.c_str());
				continue;
			}
			normals.push_back({std::stof(args[1]), std::stof(args[2]), std::stof(args[3])});
		} else
			logF("Unknown command: %s", cmd);
	}

	if (faceFixList.size() > 0) { // Fix faces without uv's
		uvs.push_back({0, 0});
		logF("Fixed %i (%i%% of faces) missing uvs (texture mappings) in mesh, please fix them yourself next time", faceFixList.size(), faceFixList.size() * 100 / faces.size());
		for (auto it = faceFixList.begin(); it != faceFixList.end(); it++) {
			auto fac = &faces[*it];
			for (int i = 0; i < fac->facesPresent; i++) {
				fac->indices[i].uvIndex = (int)uvs.size() - 1;
			}
		}
	}

	MeshStructs::meshData mesh;
	mesh.faces = faces;
	mesh.normals = normals;
	mesh.uvs = uvs;
	mesh.vertices = vertices;
	return mesh;
}
