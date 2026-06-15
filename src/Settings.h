#ifndef SETTINGS_H
#define SETTINGS_H

#include <mutex>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

extern const char* SELECTED_RESOLUTION_INDEX;

namespace Settings
{
	extern std::mutex	Mutex;
	extern json			Settings;

	/* Loads the settings. */
	void Load(std::filesystem::path aPath);
	/* Saves the settings. */
	void Save(std::filesystem::path aPath);

	extern int SelectedResolutionIndex;
}

#endif