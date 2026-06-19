#ifndef ADDON_H
#define ADDON_H
#include <cmath>
#include <vector>
#include <filesystem>
#include <fstream>
#include <DirectXMath.h>
#include <chrono>
#include "../Settings.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_extensions.h"
#include "services/NexusService.h"
#include "WindowUtil.h"
namespace Nekres {
	class Addon
	{
		const char* ON_ADDON_LOADED = "EV_ADDON_LOADED";
		const char* ON_ADDON_UNLOADED = "EV_ADDON_UNLOADED";

		public:
			Addon(AddonDefinition_t* p_addonDef, AddonAPI_t* p_api) : Nexus(new Services::NexusService(p_api)), m_addonDef(p_addonDef), m_api(p_api)
			{
				m_instance = this;
				ImGui::SetCurrentContext((ImGuiContext*)m_api->ImguiContext);
				ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))m_api->ImguiMalloc, (void(*)(void*, void*))m_api->ImguiFree); // on imgui 1.80+

				m_addonPath = m_api->Paths_GetAddonDirectory("WindowResize");
				m_settingsPath = m_api->Paths_GetAddonDirectory("WindowResize/settings.json");
				std::error_code ec;
				std::filesystem::create_directories(m_addonPath, ec);
				Settings::Load(m_settingsPath);

				bool isCustom = true;
				for (int i = 0; i < WindowUtil::WindowSizesCount; ++i) {
					if (WindowUtil::WindowSizes[i].Width == Settings::ResolutionWidth && WindowUtil::WindowSizes[i].Height == Settings::ResolutionHeight) {
						isCustom = false;
						break;
					}
				}
				if (isCustom) {
					m_customResolutionWidth = Settings::ResolutionWidth;
					m_customResolutionHeight = Settings::ResolutionHeight;
				}

				m_api->GUI_Register(RT_Render, AddonRender);
				m_api->GUI_Register(RT_OptionsRender, AddonOptions);
			}

			~Addon()
			{
				m_api->GUI_Deregister(AddonOptions);
				m_api->GUI_Deregister(AddonRender);
				delete Nexus;
				Settings::Save(m_settingsPath);
				m_instance = nullptr;
			}

			Services::NexusService* Nexus;

			static void Log(ELogLevel p_logLevel, const char* p_message) {
				if (m_instance) {
					m_instance->m_api->Log(p_logLevel, m_instance->m_addonDef->Name, p_message);
				}
			}

			void Render();
			void Options();

		private:
			inline static Addon* m_instance = nullptr;

			AddonDefinition_t* m_addonDef;
			AddonAPI_t* m_api;

			std::filesystem::path m_addonPath;
			std::filesystem::path m_settingsPath;

			bool m_isWindowedMode = false;
			std::chrono::steady_clock::time_point m_lastCheckTime = std::chrono::steady_clock::now();

			int m_customResolutionWidth = -1;
			int m_customResolutionHeight = -1;

			void ApplyResolution(int width, int height);

			static void AddonRender() { m_instance->Render(); }
			static void AddonOptions() { m_instance->Options(); }
	};

}
#endif