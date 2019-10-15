#include "DamageCommand.h"

DamageCommand::DamageCommand() : IMCCommand("damage", "Damage yourself (relies on falldamage)", "<amount>")
{
	registerAlias("dmg");
}

DamageCommand::~DamageCommand()
{
}

bool DamageCommand::execute(std::vector<std::string>* args)
{
	assertTrue(g_Data.getClientInstance()->getLocalPlayer() != nullptr);
	float amount = assertFloat(args->at(1));
	if (!moduleMgr->getModule<NoFall>()->isEnabled()) {
		if (amount < 1.f) {
			clientMessageF("%sPlease put a number bigger than 0", RED);
		} else g_Data.getLocalPlayer()->causeFallDamage(amount + 3.f);
	}
	else {
		clientMessageF("%sThis command will not work with NoFall toggled on.", RED);
	}
	return true;
}
