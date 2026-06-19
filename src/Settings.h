#ifndef SETTINGS_H
#define SETTINGS_H

#include <mutex>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

extern const char* RESOLUTION_WIDTH;
extern const char* RESOLUTION_HEIGHT;
extern const char* USE_VERTICAL_RESOLUTION;

namespace Settings
{
	extern std::mutex	Mutex;
	extern json			Settings;

	/* Loads the settings. */
	void Load(std::filesystem::path aPath);
	/* Saves the settings. */
	void Save(std::filesystem::path aPath);

	extern int ResolutionWidth;
	extern int ResolutionHeight;
	extern bool UseVerticalResolution;
}

#endif