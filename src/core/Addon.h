#ifndef ADDON_H
#define ADDON_H
#include <cmath>
#include <vector>
#include <filesystem>
#include <fstream>
#include <DirectXMath.h>
#include "../Settings.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_extensions.h"
#include "services/Gw2MumbleService.h"
#include "services/NexusService.h"
namespace Nekres {
	class Addon
	{
		const char* ON_ADDON_LOADED = "EV_ADDON_LOADED";
		const char* ON_ADDON_UNLOADED = "EV_ADDON_UNLOADED";

		public:
			Addon(AddonDefinition* p_addonDef, AddonAPI* p_api) : Gw2Mumble(new Services::Gw2MumbleService(p_api)), Nexus(new Services::NexusService(p_api)), m_addonDef(p_addonDef), m_api(p_api)
			{
				m_instance = this;
				ImGui::SetCurrentContext((ImGuiContext*)m_api->ImguiContext);
				ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))m_api->ImguiMalloc, (void(*)(void*, void*))m_api->ImguiFree); // on imgui 1.80+

				m_addonPath = m_api->Paths.GetAddonDirectory("Nekres/WindowResize");
				m_settingsPath = m_api->Paths.GetAddonDirectory("Nekres/WindowResize/settings.json");
				std::filesystem::create_directory(m_addonPath);
				Settings::Load(m_settingsPath);

				m_api->Renderer.Register(ERenderType_Render, AddonRender);
				m_api->Renderer.Register(ERenderType_OptionsRender, AddonOptions);
			}

			~Addon()
			{
				m_api->Renderer.Deregister(AddonOptions);
				m_api->Renderer.Deregister(AddonRender);
				delete Gw2Mumble;
				delete Nexus;
				delete m_api;
				Settings::Save(m_settingsPath);
				m_instance = nullptr;
			}

			Services::Gw2MumbleService* Gw2Mumble;
			Services::NexusService* Nexus;

			void Log(ELogLevel p_logLevel, const char* p_message) {
				if (m_instance) {
					m_instance->m_api->Log(p_logLevel, m_instance->m_addonDef->Name, p_message);
				}
			}

			void Render();
			void Options();

		private:
			static Addon* m_instance;

			AddonDefinition* m_addonDef;
			AddonAPI* m_api;

			std::filesystem::path m_addonPath;
			std::filesystem::path m_settingsPath;

			static void AddonRender() { m_instance->Render(); }
			static void AddonOptions() { m_instance->Options(); }
	};

}
#endif