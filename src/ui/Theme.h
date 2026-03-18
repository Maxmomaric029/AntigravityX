#pragma once
#include "imgui.h"
#include "imgui_internal.h"

namespace Theme {
    // Fonts
    extern ImFont* FontRegular;
    extern ImFont* FontBold;
    extern ImFont* FontSekuya;
    extern ImFont* FontTitle;
    extern ImFont* FontSmall;

    // Accent color (user-customizable)
    extern ImVec4 Accent;

    // Call before main loop to load .ttf files
    void LoadFonts(ImGuiIO& io);

    // Apply the dark hacker aesthetic
    void ApplyStyle();

    // Easing: smoothly move 'current' toward 'target'
    void EaseValue(float& current, float target, float speed);

    // Draw a soft glowing rectangle (for shadows/depth)
    void DrawGlowRect(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, ImU32 color, float size, float rounding = 0.0f);

    // Draw a vertical or horizontal gradient
    void DrawGradientRect(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, ImU32 col1, ImU32 col2, bool vertical = true);

    // Draw a glass-style rounded background panel
    void DrawGlassPanel(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, float rounding, float alpha = 0.85f);

    // Draw glowing text using draw list
    void RenderGlowingText(const char* text, ImU32 color, ImVec2 pos, ImFont* font = nullptr);

    // Draw a fully custom glowing button. Returns true if clicked.
    bool GlowingButton(const char* label, const ImVec2& size, ImU32 glowColor);

    // Draw a sidebar tab item with animated highlight. Returns true if clicked.
    bool SidebarTab(const char* icon, const char* label, bool selected, float& highlightAlpha);

    // Draw a toggle switch. Returns true when value changed.
    bool ToggleSwitch(const char* label, bool* value);

    // Draw a modern slider. Returns true when value changed.
    bool ModernSlider(const char* label, float* v, float v_min, float v_max, const char* fmt = "%.0f");

    // Draw a labeled separator
    void SectionHeader(const char* label);

    // Helper to get font file path from assets folder or current dir
    std::string GetFontPath(const std::string& fileName);
}
