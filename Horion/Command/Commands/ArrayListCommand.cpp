#include "ArrayListCommand.h"



ArrayListCommand::ArrayListCommand() : ICommand("arraylist", "Choose the place of the arrayList", "<side>")
{
	registerAlias("lock");
	registerAlias("unlock");
	registerAlias("hide");
	registerAlias("show");
}


ArrayListCommand::~ArrayListCommand()
{
}


bool ArrayListCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() >= 1);
	if (args->size() >= 2)
	{
		std::string side = args->at(1);
		assertTrue(side.size() > 0);

		if (side == "right" && !GameData::shouldOnTheRight())
		{
			GameData::OnTheRight();
			clientMessageF("[%sHorion%s] %sArrayList placed to the right !", GOLD, WHITE, GREEN);
			return true;

		}
		else if (side == "left" && GameData::shouldOnTheRight())
		{
			GameData::OnTheRight();
			clientMessageF("[%sHorion%s] %sArrayList placed to the left !", GOLD, WHITE, GREEN);
			return true;
		}
		return true;
	}
	else if (args->at(0) == "lock" && !GameData::shouldLock())
	{
			GameData::Lock();
			clientMessageF("[%sHorion%s] %sKeybind locked !", GOLD, WHITE, GREEN);
			return true;
	}
	else if (args->at(0) == "unlock" && GameData::shouldLock())
	{
		GameData::Lock();
		clientMessageF("[%sHorion%s] %sKeybinds unlocked !", GOLD, WHITE, GREEN);
		return true;
	}
	else if (args->at(0) == "show" && GameData::ShouldHide())
	{
		GameData::Hide();
		clientMessageF("[%sHorion%s] %sArrayList showed !", GOLD, WHITE, GREEN);
		return true;
	}
	else if (args->at(0) == "hide" && !GameData::ShouldHide())
	{
		GameData::Hide();
		clientMessageF("[%sHorion%s] %sArrayList hided", GOLD, WHITE, GREEN);
		return true;
	}
	clientMessageF("[%sHorion%s] %sUnknow Command !", GOLD, WHITE, GREEN);
	return true;
}

