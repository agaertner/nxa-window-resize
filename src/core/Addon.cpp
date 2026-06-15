#include "Addon.h"
#include "WindowUtil.h"

void Nekres::Addon::Render()
{
	auto now = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastCheckTime).count() > 200)
	{
		m_lastCheckTime = now;

		HWND hWnd = FindWindowA("ArenaNet_Dx_Window_Class", NULL);
		if (!hWnd) {
			hWnd = FindWindowA("ArenaNet_Gr_Window_Class", NULL);
		}

		if (hWnd)
		{
			bool isWindowed = WindowUtil::IsWindowedMode(hWnd);
			if (isWindowed != m_isWindowedMode)
			{
				m_isWindowedMode = isWindowed;
				if (m_isWindowedMode)
				{
					if (Settings::SelectedResolutionIndex >= 0 && Settings::SelectedResolutionIndex < WindowUtil::WindowSizesCount)
					{
						auto res = WindowUtil::WindowSizes[Settings::SelectedResolutionIndex];
						WindowUtil::ResizeAndCenterWindow(hWnd, res.Width, res.Height);
					}
				}
			}
		}
	}
}

void Nekres::Addon::Options()
{
	int resIndex = Settings::SelectedResolutionIndex;
	if (resIndex < 0 || resIndex >= WindowUtil::WindowSizesCount) resIndex = 0;
	ImGui::AlignTextToFramePadding();
	ImGui::Text("Preferred Window Size:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.0f);

	if (ImGui::BeginCombo("##PreferredWindowSize", WindowUtil::WindowSizes[resIndex].Name.c_str()))
	{
		for (int i = 0; i < WindowUtil::WindowSizesCount; i++)
		{
			bool isSelected = (Settings::SelectedResolutionIndex == i);
			if (ImGui::Selectable(WindowUtil::WindowSizes[i].Name.c_str(), isSelected))
			{
				Settings::SelectedResolutionIndex = i;
				Settings::Save(m_settingsPath);

				HWND hWnd = FindWindowA("ArenaNet_Dx_Window_Class", NULL);
				if (!hWnd) hWnd = FindWindowA("ArenaNet_Gr_Window_Class", NULL);
				if (hWnd && WindowUtil::IsWindowedMode(hWnd))
				{
					auto res = WindowUtil::WindowSizes[i];
					WindowUtil::ResizeAndCenterWindow(hWnd, res.Width, res.Height);
				}
			}
			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}