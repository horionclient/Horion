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

struct C_SerializedSkin {
public:
	TextHolder uuid1; // 0x0000
	TextHolder uuid2; // 0x0020
	TextHolder geometry; // 0x0040
	TextHolder geometryName; // 0x0060
	struct {
		int unknown; // 0x0080
		int skinWidth;// 0x0084
		int skinHeight;// 0x0088
		int unknown2;// 0x008C
		void* imageData; // 0x0090
		size_t imageDataSize; // 0x0098
	} skinImage;
	struct {
		int unknown; // 0x00A0
		int skinWidth;// 0x00A4
		int skinHeight;// 0x00A8
		int unknown2;// 0x00AC
		void* imageData; // 0x00B0
		size_t imageDataSize; // 0x00B8
	} unknownImage;
	struct {
		void* start;	// 0x00C0
		void* end;	// 0x00C8
		void* unknown;	// 0x00D0
	} animatedImages;
	unsigned char imTooLazyToReverseThis[0x130 - 0xD8];
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