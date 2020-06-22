#include "LevelFunctions.h"

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