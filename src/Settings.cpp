#include "Settings.h"
#include "core/Addon.h"
#include <filesystem>
#include <fstream>

const char* SELECTED_RESOLUTION_INDEX = "SelectedResolutionIndex";

namespace Settings
{
	std::mutex	Mutex;
	json		Settings = json::object();

	void Load(std::filesystem::path aPath)
	{
		if (!std::filesystem::exists(aPath)) { return; }

		Settings::Mutex.lock();
		{
			try
			{
				std::ifstream file(aPath);
				Settings = json::parse(file);
				file.close();
			}
			catch (json::parse_error& ex)
			{
				Nekres::Addon::Log(LOGL_WARNING, "Settings.json could not be parsed.");
				Nekres::Addon::Log(LOGL_WARNING, ex.what());
			}
		}
		Settings::Mutex.unlock();

		if (!Settings[SELECTED_RESOLUTION_INDEX].is_null())
		{
			Settings[SELECTED_RESOLUTION_INDEX].get_to<int>(SelectedResolutionIndex);
		}
	}
	void Save(std::filesystem::path aPath)
	{
		Settings::Mutex.lock();
		{
			std::ofstream file(aPath);
			file << Settings.dump(1, '\t') << std::endl;
			file.close();
		}
		Settings::Mutex.unlock();
	}

	int SelectedResolutionIndex = 16; // Default to 1920x1080
}