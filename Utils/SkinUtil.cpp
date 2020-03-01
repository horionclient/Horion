#include "SkinUtil.h"

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

	auto hResourceGeometry = FindResourceA(g_Data.getDllModule(), MAKEINTRESOURCEA(IDR_TEXT1), "TEXT");
	auto hMemoryGeometry = LoadResource(g_Data.getDllModule(), hResourceGeometry);

	auto sizeGeometry = SizeofResource(g_Data.getDllModule(), hResourceGeometry);
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
