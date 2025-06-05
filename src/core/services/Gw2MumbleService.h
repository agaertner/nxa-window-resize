#ifndef GW2MUMBLESERVICE_H
#define GW2MUMBLESERVICE_H
#include "nexus-core/Nexus.h"
#include "nexus-mumble/Mumble.h"
namespace Nekres::Services
{
	class Gw2MumbleService
	{
		const char* MUMBLE_IDENTITY_UPDATED = "EV_MUMBLE_IDENTITY_UPDATED";
		public:
			Gw2MumbleService(AddonAPI* p_api)
				: m_api(p_api),
				  m_link((Mumble::Data*)p_api->DataLink.Get("DL_MUMBLE_LINK")),
				  m_identity()

			{
				m_instance = this;
				m_api->Events.Subscribe(MUMBLE_IDENTITY_UPDATED, OnMumbleIdentityUpdated);
			}
			~Gw2MumbleService()
			{
				m_api->Events.Unsubscribe(MUMBLE_IDENTITY_UPDATED, OnMumbleIdentityUpdated);
				m_instance = nullptr;
			}

			Mumble::Data* Data() const
			{
				return m_link;
			}

			Mumble::Identity* Identity() const
			{
				return m_identity;
			}

		private:
			static Gw2MumbleService* m_instance;

			AddonAPI* m_api;
			Mumble::Data* m_link;
			Mumble::Identity* m_identity;

			static void OnMumbleIdentityUpdated(void* aEventArgs)
			{
				if (m_instance)
				{
					m_instance->m_identity = (Mumble::Identity*)aEventArgs;
				}
			}
	};

}
#endif
