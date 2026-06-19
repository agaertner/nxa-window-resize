#include "Settings.h"
#include "core/Addon.h"
#include <filesystem>
#include <fstream>
#include "core/WindowUtil.h"

const char* RESOLUTION_WIDTH = "ResolutionWidth";
const char* RESOLUTION_HEIGHT = "ResolutionHeight";
const char* USE_VERTICAL_RESOLUTION = "UseVerticalResolution";

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

		if (!Settings[USE_VERTICAL_RESOLUTION].is_null())
		{
			UseVerticalResolution = Settings[USE_VERTICAL_RESOLUTION].get<bool>();
		}

		if (!Settings[RESOLUTION_WIDTH].is_null() && !Settings[RESOLUTION_HEIGHT].is_null())
		{
			int width = Settings[RESOLUTION_WIDTH].get<int>();
			int height = Settings[RESOLUTION_HEIGHT].get<int>();

			auto smallest = Nekres::WindowUtil::GetSmallestResolution();
			int minW = UseVerticalResolution ? smallest.Height : smallest.Width;
			int minH = UseVerticalResolution ? smallest.Width : smallest.Height;

			if (width < minW || height < minH) {
				auto defaultRes = Nekres::WindowUtil::GetDefaultResolution();
				ResolutionWidth = defaultRes.Width;
				ResolutionHeight = defaultRes.Height;
			} else {
				ResolutionWidth = width;
				ResolutionHeight = height;
			}
		}
	}
	void Save(std::filesystem::path aPath)
	{
		Settings::Mutex.lock();
		{
			Settings[RESOLUTION_WIDTH] = ResolutionWidth;
			Settings[RESOLUTION_HEIGHT] = ResolutionHeight;
			Settings[USE_VERTICAL_RESOLUTION] = UseVerticalResolution;
			std::ofstream file(aPath);
			file << Settings.dump(1, '\t') << std::endl;
			file.close();
		}
		Settings::Mutex.unlock();
	}

	auto defaultRes = Nekres::WindowUtil::GetDefaultResolution();
	int ResolutionWidth = defaultRes.Width;
	int ResolutionHeight = defaultRes.Height;
	bool UseVerticalResolution = false;
}