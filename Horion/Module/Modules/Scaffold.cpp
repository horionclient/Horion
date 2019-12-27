#include "Scaffold.h"

Scaffold::Scaffold() : IModule(VK_NUMPAD1, Category::BUILD, "Automatically build blocks beneath you")
{
	registerBoolSetting("AutoSelect", &this->autoselect, this->autoselect);
}


Scaffold::~Scaffold()
{
}

const char* Scaffold::getModuleName()
{
	return ("Scaffold");
}

bool Scaffold::tryScaffold(vec3_t blockBelow)
{
	blockBelow = blockBelow.floor();

	DrawUtils::drawBox(blockBelow, vec3_t(blockBelow).add(1), 0.4f);

	// BlockSource::getBlock()::getMaterial()::isReplaceable()
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blockBelow));
	//logF("block: %llX", block);
	C_BlockLegacy* blockLegacy = *(block->blockLegacy);
	//logF("blockLegacy: %llX", blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		
		vec3_ti* blok = new vec3_ti(blockBelow);

		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.size() == 0) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}
		bool foundCandidate = false;
		int i = 0;
		for (auto it = checklist.begin(); it != checklist.end(); ++it) {
			vec3_ti* current = *it;

			vec3_ti* calc = blok->subAndReturn(*current);
			if (!(*(g_Data.getLocalPlayer()->region->getBlock(*calc)->blockLegacy))->material->isReplaceable)
			{
				// Found a solid block to click
				foundCandidate = true;
				blok->set(calc);
				delete calc;
				break;
			}
			delete calc;
			i++;
		}
		if (foundCandidate) {
			if (autoselect) {
				int slot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
				findBlock();
				g_Data.getCGameMode()->buildBlock(blok, i);
				g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = slot;
			}
			else {
				g_Data.getCGameMode()->buildBlock(blok, i);
			}
			delete blok;
			
			return true;
		}
		delete blok;
	}
	return false;
}

bool Scaffold::findBlock() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != NULL) {
			if ((*stack->item)->itemId < 256 && (*stack->item)->itemId != 0) {
				supplies->selectedHotbarSlot = n;
				return true;
			}
		}
	}
	return false;
}

void Scaffold::onPostRender()
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	if ((g_Data.getLocalPlayer()->itemId == 0 || g_Data.getLocalPlayer()->itemId > 255) && !autoselect) // Block in hand?
		return;

	vec3_t blockBelow = g_Data.getLocalPlayer()->eyePos0; // Block below the player
	blockBelow.y -= g_Data.getLocalPlayer()->height;     
	blockBelow.y -= 0.5f;


	// Adjustment by velocity
	float speed = g_Data.getLocalPlayer()->velocity.magnitudexz();
	vec3_t vel = g_Data.getLocalPlayer()->velocity;
	vel.normalize(); // Only use values from 0 - 1
	
	DrawUtils::setColor(0.3f, 0.2f, 0.8f, 1);
	if (!tryScaffold(blockBelow)) {
		if (speed > 0.05f) { // Are we actually walking?
			DrawUtils::setColor(0.8f, 0.8f, 0.2f, 1);
			blockBelow.z -= vel.z * 0.4f;
			if (!tryScaffold(blockBelow)) {
				blockBelow.x -= vel.x * 0.4f;
				if (!tryScaffold(blockBelow) && g_Data.getLocalPlayer()->isSprinting()) {
					blockBelow.z += vel.z;
					blockBelow.x += vel.x;
					tryScaffold(blockBelow);
				}
			}
		}
		
	}
}
