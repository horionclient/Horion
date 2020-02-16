#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Json.hpp"
#include "Logger.h"
#include "../Memory/GameData.h"
#include "../resource.h"

using namespace nlohmann;

namespace MeshStructs {
	struct face {
		struct facePart {
			int vertIndex = -1, normalIndex = -1, uvIndex = -1;
		} indices[8];
		int facesPresent = 4;
	};

	void to_json(json& j, const face& f);

	struct meshData {
		std::vector<std::array<float, 3>> vertices;
		std::vector<std::array<float, 3>> normals;
		std::vector<std::array<float, 2>> uvs;
		std::vector<face> faces;
	};
}

class SkinUtil {
public:
	static void importGeo(std::wstring filePath) {
		std::ifstream fileStr(filePath, std::ios::in | std::ios::binary);
		if (fileStr) {
			std::string contents;
			fileStr.seekg(0, std::ios::end);
			contents.resize(fileStr.tellg());
			fileStr.seekg(0, std::ios::beg);
			fileStr.read(&contents[0], contents.size());
			fileStr.close();

			auto hResourceGeometry = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_TEXT1), "TEXT");
			auto hMemoryGeometry = LoadResource(g_Data.getDllModule(), hResourceGeometry);

			auto sizeGeometry = SizeofResource(g_Data.getDllModule(), hResourceGeometry);
			auto ptrGeometry = LockResource(hMemoryGeometry);
			logF("Starting geometry import");
			std::string moddedGeo = SkinUtil::modGeometry(reinterpret_cast<char*>(ptrGeometry), SkinUtil::objToMesh(contents.c_str()));
			g_Data.setCustomGeometryOverride(true, std::make_shared<std::string>(moddedGeo));
			logF("Geometry import done");

			if (hMemoryGeometry)
				FreeResource(hMemoryGeometry);
		}
	}
	static std::string modGeometry(const char* oldGeoStr, MeshStructs::meshData mesh) {
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
	static MeshStructs::meshData objToMesh(const char* str) {
		std::istringstream f(str);
		std::string line;

		std::vector<std::array<float, 3>> vertices;
		std::vector<std::array<float, 3>> normals;
		std::vector<std::array<float, 2>> uvs;
		std::vector<MeshStructs::face> faces;

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
			args.push_back(line.substr(initialPos, min(pos, line.size()) - initialPos + 1));

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
				vertices.push_back({-std::stof(args[1]), std::stof(args[2]), std::stof(args[3])});
			} else if (strcmp(cmd, "f") == 0) {  // face
				if (args.size() != 5) {
					logF("Faulty face, only quads allowed: %i", args.size() - 1);
					continue;
				}

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
				faces.push_back(face);
			} else if (strcmp(cmd, "vn") == 0) {  // normal
				if (args.size() != 4) {
					logF("Faulty normal, 3 args expected: %s", line.c_str());
					continue;
				}
				normals.push_back({std::stof(args[1]), std::stof(args[2]), std::stof(args[3])});
			} else
				logF("Unknown command: %s", cmd);
		}

		MeshStructs::meshData mesh;
		mesh.faces = faces;
		mesh.normals = normals;
		mesh.uvs = uvs;
		mesh.vertices = vertices;
		return mesh;
	}
};
