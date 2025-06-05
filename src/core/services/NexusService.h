#ifndef NEXUSSERVICE_H
#define NEXUSSERVICE_H
#include <nexus-rtapi/RTAPI.hpp>
#include "nexus-core/Nexus.h"
namespace Nekres::Services
{
	class NexusService
	{
		const char* ADDON_LOADED = "EV_ADDON_LOADED";
		const char* ADDON_UNLOADED = "EV_ADDON_UNLOADED";
		public:
			NexusService(AddonAPI* p_api) : m_api(p_api)
			{
				m_instance = this;
				m_nexus = (NexusLinkData*)m_api->DataLink.Get("DL_NEXUS_LINK");
				m_rtdata = (RTAPI::RealTimeData*)m_api->DataLink.Get(DL_RTAPI);

				m_api->Events.Subscribe(ADDON_LOADED, OnAddonLoaded);
				m_api->Events.Subscribe(ADDON_UNLOADED, OnAddonUnloaded);
			}

			~NexusService()
			{
				m_instance = nullptr;
				m_api->Events.Unsubscribe(ADDON_LOADED, OnAddonLoaded);
				m_api->Events.Unsubscribe(ADDON_UNLOADED, OnAddonUnloaded);
			}

			RTAPI::RealTimeData* Data() const
			{
				return m_rtdata;
			}

			NexusLinkData* Core() const
			{
				return m_nexus;
			}

		private:
			static NexusService* m_instance;

			AddonAPI* m_api;
			NexusLinkData* m_nexus;
			RTAPI::RealTimeData* m_rtdata;

			static void OnAddonLoaded(void* aSignature)
			{
				if (m_instance)
				{
					int* sig = static_cast<int*>(aSignature);
					if (!sig) { return; }
					if (*sig == RTAPI_SIG)
					{
						m_instance->m_rtdata = (RTAPI::RealTimeData*)m_instance->m_api->DataLink.Get(DL_RTAPI);
					}
				}
			}

			static void OnAddonUnloaded(void* aSignature)
			{
				if (m_instance)
				{
					int* sig = static_cast<int*>(aSignature);
					if (!sig) { return; }
					if (*sig == RTAPI_SIG)
					{
						m_instance->m_rtdata = nullptr;
					}
				}
			}
	};
}
#endif
