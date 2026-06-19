#include "Addon.h"
#include "WindowUtil.h"

void Nekres::Addon::Render()
{
	auto now = std::chrono::steady_clock::now();
	if (std::chrono::duration_cast<std::chrono::milliseconds>(now - m_lastCheckTime).count() > 200)
	{
		m_lastCheckTime = now;

		HWND hWnd = WindowUtil::GetWindowHandle();

		if (hWnd)
		{
			bool isWindowed = WindowUtil::IsWindowedMode(hWnd);
			if (isWindowed && !m_isWindowedMode)
			{
				ApplyResolution(Settings::ResolutionWidth, Settings::ResolutionHeight);
			}
			m_isWindowedMode = isWindowed;
		}
	}
}

void Nekres::Addon::ApplyResolution(int width, int height)
{
	Settings::ResolutionWidth = width;
	Settings::ResolutionHeight = height;
	Settings::Save(m_settingsPath);

	HWND hWnd = WindowUtil::GetWindowHandle();
	if (hWnd && WindowUtil::IsWindowedMode(hWnd))
	{
		int applyWidth = Settings::UseVerticalResolution ? height : width;
		int applyHeight = Settings::UseVerticalResolution ? width : height;
		WindowUtil::ResizeAndCenterWindow(hWnd, applyWidth, applyHeight);
	}
}

void Nekres::Addon::Options()
{
	int resIndex = -1;
	for (int i = 0; i < WindowUtil::WindowSizesCount; ++i) {
		if (WindowUtil::WindowSizes[i].Width == Settings::ResolutionWidth && WindowUtil::WindowSizes[i].Height == Settings::ResolutionHeight) {
			resIndex = i;
			break;
		}
	}

	std::string currentPreview = WindowUtil::GetLabel(Settings::ResolutionWidth, Settings::ResolutionHeight);

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Preferred Window Size:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(150.0f);

	if (ImGui::BeginCombo("##PreferredWindowSize", currentPreview.c_str()))
	{
		if (m_customResolutionWidth != -1)
		{
			bool isSelected = (resIndex == -1);
			std::string customName = WindowUtil::GetLabel(m_customResolutionWidth, m_customResolutionHeight);
			if (ImGui::Selectable(customName.c_str(), isSelected))
			{
				ApplyResolution(m_customResolutionWidth, m_customResolutionHeight);
			}
			if (isSelected) ImGui::SetItemDefaultFocus();
		}

		for (int i = 0; i < WindowUtil::WindowSizesCount; i++)
		{
			bool isSelected = (resIndex == i);
			std::string resName = WindowUtil::GetLabel(WindowUtil::WindowSizes[i].Width, WindowUtil::WindowSizes[i].Height);
			if (ImGui::Selectable(resName.c_str(), isSelected))
			{
				ApplyResolution(WindowUtil::WindowSizes[i].Width, WindowUtil::WindowSizes[i].Height);
			}
			if (isSelected) ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	if (ImGui::Checkbox("Use Vertical Resolution", &Settings::UseVerticalResolution))
	{
		Settings::Save(m_settingsPath);
		ApplyResolution(Settings::ResolutionWidth, Settings::ResolutionHeight);
	}
}