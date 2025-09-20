#pragma once

// Manager.hpp structure by acaruso
// reused with explicit permission and strong encouragement

using namespace geode::prelude;

class Manager {

protected:
	static Manager* instance;
public:

	gd::vector<int> managerSFXIDs; // FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID FUCK ANDROID
	std::string managerLevelName = "";

	static Manager* get() {
		if (!instance) instance = new Manager();
		return instance;
	}

};