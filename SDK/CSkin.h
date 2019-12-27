#pragma once

struct C_SkinData { // size: 0x0020
public:
	char pad_0x0000[0x4]; //0x0000
	unsigned int SkinWidth; //0x0004 
	unsigned int SkinHeight; //0x0008 
	char pad_0x000C[0x4]; //0x000C
	void* skinData; //0x0010 
	size_t skinSize; //0x0018 
};

struct MinecraftSkinData {
public:
	C_SkinData skin; // 0x0000
	std::shared_ptr<TextHolder> geometryData; // 0x0020
	std::shared_ptr<TextHolder> resourcePatch;

	MinecraftSkinData(C_SkinData skin, std::shared_ptr<TextHolder> geo, std::shared_ptr<TextHolder> res) {
		this->skin = skin;
		this->geometryData = geo;
		this->resourcePatch = res;
	}
};