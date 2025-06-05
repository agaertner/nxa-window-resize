#include "Settings.h"

#include "core/Addon.h"

#include <filesystem>
#include <fstream>

const char* WINDOW_WIDTH = "WindowWidth";
const char* WINDOW_HEIGHT = "WindowHeight";

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
				Nekres::Addon::Log(ELogLevel_WARNING, "Settings.json could not be parsed.");
				Nekres::Addon::Log(ELogLevel_WARNING, ex.what());
			}
		}
		Settings::Mutex.unlock();

		if (!Settings[WINDOW_WIDTH].is_null())
		{
			Settings[WINDOW_WIDTH].get_to<float>(WindowWidth);
		}
		if (!Settings[WINDOW_HEIGHT].is_null())
		{
			Settings[WINDOW_HEIGHT].get_to <float>(WindowHeight);
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

	float WindowWidth = 1280.0f;
	float WindowHeight = 920.0f;
}