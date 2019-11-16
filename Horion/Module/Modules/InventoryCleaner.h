#pragma once
#include "Module.h"
class InventoryCleaner :
	public IModule
{
private:
	bool sorted = false;

	std::vector<int> findUselessItems();
	bool stackIsUseful(C_ItemStack* itemStack);
	bool isLastItem(C_Item* item);
	void findStackableItems();

	bool keepTools = true;
	bool keepArmor = true;
	bool keepBlocks = true;
	bool keepFood = true;
public:
	C_MoveInputHandler* inputHandler = nullptr;
	InventoryCleaner();
	~InventoryCleaner();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};