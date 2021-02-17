#include "LevelFunctions.h"

JsValueRef createBlock(C_BlockLegacy *block) {
	JsValueRef ref;
	chakra.JsCreateObject_(&ref);

	std::wstring blockName = Utils::stringToWstring(block->name.getText());

	JsValueRef bName;
	chakra.JsPointerToString_(blockName.c_str(), blockName.size(), &bName);

	std::wstring tileName = Utils::stringToWstring(block->tileName.getText());

	JsValueRef tName;
	chakra.JsPointerToString_(tileName.c_str(), tileName.size(), &tName);

	JsValueRef id = chakra.toNumber((double)block->blockId);

	chakra.addPropertyToObj(ref, L"name", bName);
	chakra.addPropertyToObj(ref, L"tileName", tName);
	chakra.addPropertyToObj(ref, L"blockId", id);

	return ref;
}

JsValueRef CALLBACK LevelFunctions::isValid(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	return chakra.toBoolean(g_Data.isInGame() && g_Data.getLocalPlayer()->isAlive());
}

JsValueRef CALLBACK LevelFunctions::getAllEntities(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	std::vector<C_Entity*> entList;
	g_Data.forEachEntity([&](auto ent, bool isNew) {
		entList.push_back(ent);
	});

	JsValueRef jsList;
	chakra.JsCreateArray_((unsigned int)entList.size(), &jsList);
	
	for (int i = 0; i < entList.size(); i++) {
		auto entityRef = scriptMgr.prepareEntity(entList[i]->entityRuntimeId, reinterpret_cast<ContextObjects*>(callbackState));
		chakra.arraySet(jsList, i, entityRef);
	}

	return jsList;
}

JsValueRef CALLBACK LevelFunctions::getAllTargetEntities(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	std::vector<C_Entity*> entList;
	g_Data.forEachEntity([&](auto ent, bool isNew) {
		static auto noFriendsMod = moduleMgr->getModule<NoFriends>();
		if (!noFriendsMod->isEnabled() && FriendList::findPlayer(ent->getNameTag()->getText()))
			return;

		if (!Target::isValidTarget(ent))
			return;

		entList.push_back(ent);
	});

	JsValueRef jsList;
	chakra.JsCreateArray_((unsigned int)entList.size(), &jsList);

	for (int i = 0; i < entList.size(); i++) {
		auto entityRef = scriptMgr.prepareEntity(entList[i]->entityRuntimeId, reinterpret_cast<ContextObjects*>(callbackState));
		chakra.arraySet(jsList, i, entityRef);
	}

	return jsList;
}

JsValueRef CALLBACK LevelFunctions::getBlock(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto plr = g_Data.getLocalPlayer();

	if (plr == nullptr) {
		THROW(L"Player not valid");
	}

	auto vecOpt = Vector3Functions::getVec3FromArguments(&arguments[1], argumentCount - 1);
	if (!vecOpt.has_value()) {
		THROW(L"Invalid vector!");
	}

	auto block = plr->region->getBlock(vecOpt.value())->toLegacy();
	return createBlock(block);
}