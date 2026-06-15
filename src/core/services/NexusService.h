#ifndef NEXUSSERVICE_H
#define NEXUSSERVICE_H

#include "nexus-core/Nexus.h"
namespace Nekres::Services
{
	class NexusService
	{
		const char* ADDON_LOADED = "EV_ADDON_LOADED";
		const char* ADDON_UNLOADED = "EV_ADDON_UNLOADED";
		public:
			NexusService(AddonAPI_t* p_api) : m_api(p_api)
			{
				m_instance = this;
				m_nexus = (NexusLinkData_t*)m_api->DataLink_Get("DL_NEXUS_LINK");

				m_api->Events_Subscribe(ADDON_LOADED, OnAddonLoaded);
				m_api->Events_Subscribe(ADDON_UNLOADED, OnAddonUnloaded);
			}

			~NexusService()
			{
				m_instance = nullptr;
				m_api->Events_Unsubscribe(ADDON_LOADED, OnAddonLoaded);
				m_api->Events_Unsubscribe(ADDON_UNLOADED, OnAddonUnloaded);
			}



			NexusLinkData_t* Core() const
			{
				return m_nexus;
			}

		private:
			inline static NexusService* m_instance = nullptr;

			AddonAPI_t* m_api;
			NexusLinkData_t* m_nexus;

			static void OnAddonLoaded(void* aSignature)
			{
				if (m_instance)
				{
					int* sig = static_cast<int*>(aSignature);
					if (!sig) { return; }
					// Do nothing
				}
			}

			static void OnAddonUnloaded(void* aSignature)
			{
				if (m_instance)
				{
					int* sig = static_cast<int*>(aSignature);
					if (!sig) { return; }
					// Do nothing
				}
			}
	};
}
#endif
