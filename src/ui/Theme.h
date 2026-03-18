#pragma once
#include "imgui.h"

namespace Theme {
    // Fonts
    extern ImFont* FontRegular;
    extern ImFont* FontBold;
    extern ImFont* FontSekuya;
    extern ImFont* FontTitle;

    // Call this before ImGui_ImplXXX_Init to load the .ttf files
    // Make sure paths are relative to the executable or absolute
    void LoadFonts(ImGuiIO& io);
    
    // Call this to set the dark purple/blue aesthetic
    void ApplyStyle();

    // Helper for rendering a glowing text
    void RenderGlowingText(const char* text, ImU32 color, ImVec2 pos);

    // Helper for rendering a glowing button
    bool GlowingButton(const char* label, const ImVec2& size, ImU32 glowColor);
}
