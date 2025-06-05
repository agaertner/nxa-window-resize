#include <Windows.h>
#include "resource.h"
#include "Version.h"
#include "core/Addon.h"

void AddonLoad(AddonAPI* aApi);
void AddonUnload();

HMODULE hSelf;

AddonDefinition AddonDef{};

Nekres::Addon* g_addon = nullptr;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
	    case DLL_PROCESS_ATTACH: hSelf = hModule; break;
	    case DLL_THREAD_ATTACH:
	    case DLL_THREAD_DETACH:
	    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) AddonDefinition* GetAddonDef()
{
	AddonDef.Signature = 17;
	AddonDef.APIVersion = NEXUS_API_VERSION;
	AddonDef.Name = "GW2 Window Resize";
	AddonDef.Version.Major = V_MAJOR;
	AddonDef.Version.Minor = V_MINOR;
	AddonDef.Version.Build = V_BUILD;
	AddonDef.Version.Revision = V_REVISION;
	AddonDef.Author = "Nekres.1934";
	AddonDef.Description = "Resizes the game when you switch it to windowed mode.";
	AddonDef.Load = AddonLoad;
	AddonDef.Unload = AddonUnload;
	AddonDef.Flags = EAddonFlags_None;
	AddonDef.Provider = EUpdateProvider_GitHub;
	AddonDef.UpdateLink = "https://github.com/agaertner/nxa-window-resize";
	return &AddonDef;
}

void AddonLoad(AddonAPI* aApi)
{
	g_addon = new Nekres::Addon(&AddonDef, aApi);
}

void AddonUnload()
{
	delete g_addon;
}
