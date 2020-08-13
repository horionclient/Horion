#include "CItem.h"

#include "../Utils/Utils.h"
#include "Tag.h"
#include "../Memory/GameData.h"

C_BaseActorRenderContext::C_BaseActorRenderContext(C_ScreenContext *ScreenCtx, C_ClientInstance *client, MinecraftGame *game) {
	memset(this, 0, sizeof(C_BaseActorRenderContext));
	using BaseActorRenderContext_t = __int64(__fastcall *)(C_BaseActorRenderContext *, C_ScreenContext *, C_ClientInstance *, MinecraftGame *);
	static BaseActorRenderContext_t BaseActorRenderContext_constructor = reinterpret_cast<BaseActorRenderContext_t>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8D 05 ? ? ? ? 33 F6 48 89 01 49 8B D8 48 89 71 ? 48 8B F9 8B 42"));
	BaseActorRenderContext_constructor(this, ScreenCtx, client, game);
}
void C_ItemRenderer::renderGuiItemNew(C_BaseActorRenderContext *BaseActorRenderCtx, C_ItemStack *item, MinecraftGame *game, float x, float y, float opacity, float scale, bool isEnchanted) {
	using renderGuiItemNew_t = void(__fastcall *)(C_ItemRenderer *, C_BaseActorRenderContext *, C_ItemStack *, MinecraftGame *, float, float, float, float, float, bool);
	static renderGuiItemNew_t renderGuiItemNew = reinterpret_cast<renderGuiItemNew_t>(FindSignature("48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 89 58 20 0F 29 70 B8 48 8B 05 ?? ?? ?? ??"));
	renderGuiItemNew(this, BaseActorRenderCtx, item, game, x, y, 1, opacity, scale, isEnchanted);
}
C_ItemStack::C_ItemStack(const C_ItemStack &src) {
	memset(this, 0x0, sizeof(C_ItemStack));
	using ItemStackCopyConstructor_t = void(__fastcall *)(C_ItemStack &, C_ItemStack const &);
	static ItemStackCopyConstructor_t ItemStackCopyConstructor = reinterpret_cast<ItemStackCopyConstructor_t>(FindSignature("48 8B C4 56 57 41 54 41 56 41 57 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? 48 89 58 ?? 48 89 68 ?? 48 8B FA 48 8B D9 48 89 48 ??"));
	ItemStackCopyConstructor(*this, src);
	this->setVtable();
}
C_ItemStack::C_ItemStack(const Tag &tag) {
	memset(this, 0x0, sizeof(C_ItemStack));
	this->setVtable();
	fromTag(tag);
	/*using ItemStackBase__loadItemF = void(__fastcall*)(C_ItemStack*,Tag const&);
	static ItemStackBase__loadItemF ItemStackBase__loadItem = reinterpret_cast<ItemStackBase__loadItemF>(FindSignature("48 8B C4 55 57 41 56 48 8D 68 A1 48 81 EC ? ? ? ? 48 C7 45 ? ? ? ? ? 48 89 58 ? 48 89 70 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B DA 48 8B F9 48 89 4D ? 33 D2 41 B8 ? ? ? ? 48 8D 4D A7 E8 ? ? ? ? 48 8D 05 ? ? ? ?"));
	ItemStackBase__loadItem(this, tag);*/
}
void C_ItemStack::fromTag(const Tag &tag) {
	using ItemStackBase__loadItemF = void(__fastcall *)(C_ItemStack *, Tag const &);
	static ItemStackBase__loadItemF fromTag = nullptr;

	if (!fromTag) {
		if (g_Data.getVersion() == GAMEVERSION::g_1_16_0)
			fromTag = reinterpret_cast<ItemStackBase__loadItemF>(FindSignature("48 8B C4 55 57 41 56 48 8D 68 A1 48 81 EC ? ? ? ? 48 C7 45 ? ? ? ? ? 48 89 58 ? 48 89 70 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B DA 48 8B F9 48 89 4D ? 33 D2 41 B8 ? ? ? ? 48 8D 4D A7 E8 ? ? ? ? 48 8D 05 ? ? ? ?"));
		else
			fromTag = reinterpret_cast<ItemStackBase__loadItemF>(FindSignature("40 55 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 C7 45 ? ? ? ? ? 48 89 9C 24 ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B DA 48 8B F9 48 89 4D ? 33 D2 41 B8 ? ? ? ?"));
	}

	fromTag(this, tag);
}
void C_ItemStack::save(CompoundTag **tag) {
	using ItemStackBase__saveF = void(__fastcall *)(C_ItemStack *, CompoundTag **);
	ItemStackBase__saveF save = reinterpret_cast<ItemStackBase__saveF>(FindSignature("40 55 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 C7 45 ? ? ? ? ? 48 89 9C 24 ? ? ? ? 48 8B F2 4C 8B F1 48 89 55"));
	return save(this, tag);
}
void C_ItemStack::setUserData(std::unique_ptr<Tag> tag) {
	using setUserData_t = void(__fastcall *)(C_ItemStack *, std::unique_ptr<Tag>);
	setUserData_t setUserData = reinterpret_cast<setUserData_t>(FindSignature("40 53 48 83 EC ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? 48 8B DA 48 8D 51 10 48 3B D3 74 20 48 8B 03 48 ?? ?? ?? ?? ?? ?? 48 8B 0A 48 89 02 48 85 C9 74 0B 48 8B 01 BA ?? ?? ?? ?? FF 10"));
	setUserData(this, std::move(tag));
}
void C_ItemStack::reinit(C_BlockLegacy &legacy, int count) {
	this->setVtable();
	Utils::CallVFunc<1, void>(this, &legacy, count);
}
void C_ItemStack::reinit(C_Item &item, int count, int itemData) {
	this->setVtable();
	Utils::CallVFunc<2, void>(this, &item, count, itemData);
}
int C_ItemStack::getEnchantValue(int enchantId) {
	using getEnchantsLevel_t = int(__fastcall *)(int, C_ItemStack *);
	static getEnchantsLevel_t getEnchantsLevel = reinterpret_cast<getEnchantsLevel_t>(FindSignature("48 8B C4 57 48 81 EC ? ? ? ? 48 C7 40 ? ? ? ? ? 48 89 58 ? 48 89 70 ? 48 8B F2 0F B6 D9 33 FF 48 8B CA E8"));
	return getEnchantsLevel(enchantId, this);
}
void C_ItemStack::setVtable(void) {
	static uintptr_t sigOffset = FindSignature("48 8D 05 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? 48 8D 0D ? ? ? ? 48 89 05 ? ? ? ? C6 05 ? ? ? ? ? 48 83 C4");
	int offset = *reinterpret_cast<int *>(sigOffset + 3);
	this->vTable = reinterpret_cast<uintptr_t **>(sigOffset + offset + /*length of instruction*/ 7);
}
C_Item ***ItemRegistry::getItemFromId(void *ptr, int itemId) {
	using getItemFromId_t = C_Item ***(__fastcall *)(void *, int);
	static getItemFromId_t getItemFromId = reinterpret_cast<getItemFromId_t>(FindSignature("40 53 48 83 EC ? 48 8B D9 66 85 D2 0F 84 ? ? ? ? 44 0F BF C2 48 B9 ? ? ? ? ? ? ? ? 44"));
	return getItemFromId(ptr, itemId);
}

C_Item ***ItemRegistry::lookUpByName(void *a1, void *a2, TextHolder &text) {
	using ItemRegistry__lookupByName_t = C_Item ***(__fastcall *)(void *, void *, TextHolder &);
	static ItemRegistry__lookupByName_t ItemRegistry__lookupByNameF = reinterpret_cast<ItemRegistry__lookupByName_t>(FindSignature("40 55 56 57 41 54 41 55 41 56 41 57 ?? ?? ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? 48 ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4 ?? ?? ?? ?? ?? ?? ?? 4C 8B F9 48 89 4D ?? 45 33 ED 44 89 6C 24 ?? 4D 39 68"));
	return ItemRegistry__lookupByNameF(a1, a2, text);
}
