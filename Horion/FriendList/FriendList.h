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
	static bool findPlayer(const std::string& name);
	static bool findPlayer(const char* name) {
		std::string local(name);
		return findPlayer(local);
	}
	static void addPlayerToList(const std::string& name);
	static bool removePlayer(const std::string& g);
};

extern FriendList g_friend;
