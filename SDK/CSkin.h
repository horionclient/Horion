#pragma once

struct C_MinecraftImage { // Size: 0x0020
public:
	int unknown;           // 0x0000
	int width;             // 0x0004
	int height;            // 0x0008
	int unknown2;          // 0x000C
	void* imageData;       // 0x0010
	size_t imageDataSize;  // 0x0018

	unsigned int getDataAt(int x, int y) {
		if (x >= width || y >= height)
			return 0;
		if (x < 0 || y < 0)
			return 0;

		unsigned int* imageDat = reinterpret_cast<unsigned int*>(imageData);
		return imageDat[x + width * y];
	}

	C_MinecraftImage& operator=(C_MinecraftImage const& copy) {
		this->unknown = copy.unknown;
		this->width = copy.width;
		this->height = copy.height;
		this->unknown2 = copy.unknown2;
		this->imageData = new unsigned char[copy.imageDataSize];
		this->imageDataSize = copy.imageDataSize;
		memcpy(this->imageData, copy.imageData, this->imageDataSize);

		return *this;
	}
};

struct C_SerializedSkin { // Size: 0x130
public:
	TextHolder uuid1; // 0x0000
	TextHolder uuid2; // 0x0020
	TextHolder geometryResourcePatch; // 0x0040
	TextHolder geometryName; // 0x0060
	C_MinecraftImage skinImage; // 0x0080
	C_MinecraftImage unknownImage; // 0x00A0
	struct {
		void* start;	// 0x00C0
		void* end;	// 0x00C8
		void* unknown;	// 0x00D0
	} animatedImages;
	// this is a Json::Value from open-source-parsers/jsoncpp
	// It is a parsed version of the skin geometry
	struct JsonValue {
	public:
		unsigned char geometryJsonValue[0x30];  // 0x00D8

		GamerTextHolder* getAsString(GamerTextHolder& ref) const {  // Json::Value::toStyledString
			using toStyledString_t = GamerTextHolder*(__fastcall*)(const JsonValue*, GamerTextHolder&);
			static toStyledString_t toStyledString = reinterpret_cast<toStyledString_t>(Utils::FindSignature("40 55 56 57 48 81 EC ?? ?? ?? ?? 48 C7 44 24 ?? FE FF FF FF 48 89 9C 24 ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 84 24 ?? ?? ?? ?? 48 8B FA 48 8B D9 48 89 54 24 ?? 33"));
			return toStyledString(reinterpret_cast<const JsonValue*>(&geometryJsonValue), ref);
		}
	} geometry;
	
	TextHolder capeId;			// 0x0108
	bool isPremiumSkin;			// 0x0128
	bool isPersonaSkin;			// 0x0129
	bool isCapeOnClassicSkin;	// 0x012A

	void clone(C_SerializedSkin* other) {
		this->uuid1 = other->uuid1;
		this->uuid2 = other->uuid2;
		this->geometry = other->geometry;
		this->geometryName = other->geometryName;
		this->skinImage = other->skinImage;
		this->unknownImage = other->unknownImage;
		memcpy(&this->animatedImages.start, &other->animatedImages.start, sizeof(C_SerializedSkin) - 0xC0);
	}
};

struct MinecraftSkinData {
public:
	C_MinecraftImage skin; // 0x0000
	std::shared_ptr<TextHolder> geometryData; // 0x0020
	std::shared_ptr<TextHolder> resourcePatch;

	MinecraftSkinData(C_MinecraftImage skin, std::shared_ptr<TextHolder> geo, std::shared_ptr<TextHolder> res) {
		this->skin = skin;
		this->geometryData = geo;
		this->resourcePatch = res;
	}
};