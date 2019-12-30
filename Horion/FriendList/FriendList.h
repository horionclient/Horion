#pragma once

#include <algorithm>
#include <string>
#include <typeinfo>
#include <vector>

#include "../../Memory/GameData.h"

class FriendList {
private:
	//GameData* gameData;
	std::vector<std::string> List;

public:
	FriendList();
	~FriendList();

	static std::vector<std::string> getList();
	static bool findPlayer(std::string Name);
	static void addPlayerToList(std::string name);
	static bool removePlayer(std::string& g);
};

extern FriendList g_friend;
