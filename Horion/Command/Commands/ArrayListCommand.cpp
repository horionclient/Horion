#include "ArrayListCommand.h"



ArrayListCommand::ArrayListCommand() : ICommand("arraylist", "Customize the arraylist", "<right/left/hide/lock>")
{
	registerAlias("array");
}


ArrayListCommand::~ArrayListCommand()
{
}


bool ArrayListCommand::execute(std::vector<std::string>* args)
{
	assertTrue(args->size() >= 2);
	std::string side = args->at(1);

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
	else if (side == "lock" && !GameData::shouldLock())
	{
		GameData::Lock();
		clientMessageF("[%sHorion%s] %sKeybind locked !", GOLD, WHITE, GREEN);
		return true;
	}
	else if (side == "unlock" && GameData::shouldLock())
	{
		GameData::Lock();
		clientMessageF("[%sHorion%s] %sKeybinds unlocked !", GOLD, WHITE, GREEN);
		return true;
	}
	else if (side == "show" && GameData::ShouldHide())
	{
		GameData::Hide();
		clientMessageF("[%sHorion%s] %sArrayList showed !", GOLD, WHITE, GREEN);
		return true;
	}
	else if (side == "hide" && !GameData::ShouldHide())
	{
		GameData::Hide();
		clientMessageF("[%sHorion%s] %sArrayList hided", GOLD, WHITE, GREEN);
		return true;
	}
	
	return false;
}

