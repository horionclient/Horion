#include "InventoryFunctions.h"

const wchar_t* airName = L"air";

JsValueRef createItem(C_ItemStack *stack) {
	JsValueRef obj;
	chakra.JsCreateObject_(&obj);

	if (stack == nullptr || !stack->isValid()) {
		JsValueRef itemName;
		chakra.JsPointerToString_(airName, wcslen(airName), &itemName);

		chakra.addPropertyToObj(obj, L"name", itemName);
		chakra.addPropertyToObj(obj, L"id", chakra.toNumber(0));
		chakra.addPropertyToObj(obj, L"amount", chakra.toNumber(0));
	} else {
		auto name = stack->getItem()->name.getText();
		JsValueRef itemName;

		std::wstring charName = Utils::stringToWstring(name);

		chakra.JsPointerToString_(charName.c_str(), charName.size(), &itemName);

		chakra.addPropertyToObj(obj, L"name", itemName);
		chakra.addPropertyToObj(obj, L"id", chakra.toNumber(stack->getItem()->itemId));
		chakra.addPropertyToObj(obj, L"amount", chakra.toNumber(stack->count));
	}

	return obj;
}

JsValueRef CALLBACK InventoryFunctions::getItems(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<C_LocalPlayer*>(ent);

	JsValueRef inventoryArr;
	chakra.JsCreateArray_(36, &inventoryArr);

	for (int i = 0; i < 36; i++) {
		auto stack = plr->getSupplies()->inventory->getItemStack(i);
		chakra.arraySet(inventoryArr, i, createItem(stack));
	};

	return inventoryArr;
}

JsValueRef CALLBACK InventoryFunctions::getArmor(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<C_LocalPlayer*>(ent);

	JsValueRef armorArr;
	chakra.JsCreateArray_(4, &armorArr);

	for (int i = 0; i < 4; i++) {
		auto stack = plr->getArmor(i);
		chakra.arraySet(armorArr, i, createItem(stack));
	};

	return armorArr;
}

JsValueRef CALLBACK InventoryFunctions::getHeld(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<C_LocalPlayer*>(ent);
	const int currSlot = plr->getSupplies()->selectedHotbarSlot;

	auto stack = plr->getSupplies()->inventory->getItemStack(currSlot);
	return createItem(stack);
}

JsValueRef CALLBACK InventoryFunctions::isFull(JsValueRef callee, bool isConstructCall, JsValueRef* arguments, unsigned short argumentCount, void* callbackState) {
	auto ent = g_Data.getLocalPlayer();
	if (ent == nullptr) {
		THROW(L"Player not valid");
	}

	const auto plr = reinterpret_cast<C_LocalPlayer*>(ent);

	return chakra.toBoolean(plr->getSupplies()->inventory->isFull());
}