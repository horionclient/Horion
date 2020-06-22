#include "CMinecraftUIRenderContext.h"
#include "../Utils/Utils.h"
#include "../Utils/HMath.h"

C_TexturePtr* C_MinecraftUIRenderContext::getTexture(C_TexturePtr* ptr, C_FilePath& path) {
	using getTexture_t = C_TexturePtr*(__fastcall*)(C_MinecraftUIRenderContext*,C_TexturePtr*,C_FilePath&,int);
	getTexture_t getTextureF = reinterpret_cast<getTexture_t>(FindSignature("48 89 54 24 ?? 53 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B DA 49 83 78 ?? ?? 75 1C 33 C0 48 89 02 48 89 42 ?? 48 8D 4A ?? E8"));
	return getTextureF(this, ptr, path, 0);
}
