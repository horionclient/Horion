#pragma once
//#include <string>
#include <vector>

#include "../Memory/GameData.h"
#include "../resource.h"

struct SkinData {
public:
	char pad_0x0000[0x4];     //0x0000
	unsigned int SkinWidth;   //0x0004
	unsigned int SkinHeight;  //0x0008
	char pad_0x000C[0x4];     //0x000C
	void* skinData;           //0x0010
	size_t skinSize;          //0x0018
};

namespace MeshStructs {
struct face {
	struct facePart {
		int vertIndex = -1, normalIndex = -1, uvIndex = -1;
	} indices[8];
	int facesPresent = 4;
};

/*
using namespace nlohmann;
class json;
void to_json(json& j, const face& f);*/

struct meshData {
	std::vector<std::array<float, 3>> vertices;
	std::vector<std::array<float, 3>> normals;
	std::vector<std::array<float, 2>> uvs;
	std::vector<face> faces;
};
}  // namespace MeshStructs

class SkinUtil {
public:
	static void importGeo(std::wstring filePath);
	static std::string modGeometry(const char* oldGeoStr, MeshStructs::meshData mesh);
	static MeshStructs::meshData objToMesh(const char* str, bool convertTrisToQuads = true);
};
