#include "Theme.h"
#include <string>
#include "imgui.h"
#include "imgui_internal.h"

namespace Theme {
    ImFont* FontRegular = nullptr;
    ImFont* FontBold = nullptr;
    ImFont* FontSekuya = nullptr;
    ImFont* FontTitle = nullptr;

    void LoadFonts(ImGuiIO& io) {
        // Load fonts from the provided directories
        // In a real deployed app, these should be embedded or in an assets/ folder. 
        // We will use relative pathing assuming the exe runs near the root for now.
        
        std::string datatypeRegular = "Datatype,Sekuya/Datatype/static/Datatype-Regular.ttf";
        std::string datatypeBold = "Datatype,Sekuya/Datatype/static/Datatype-Bold.ttf";
        std::string sekuya = "Datatype,Sekuya/Sekuya/Sekuya-Regular.ttf";

        FontRegular = io.Fonts->AddFontFromFileTTF(datatypeRegular.c_str(), 16.0f);
        FontBold    = io.Fonts->AddFontFromFileTTF(datatypeBold.c_str(), 16.0f);
        FontTitle   = io.Fonts->AddFontFromFileTTF(datatypeBold.c_str(), 24.0f);
        FontSekuya  = io.Fonts->AddFontFromFileTTF(sekuya.c_str(), 18.0f);

        // Fallbacks if fonts fail to load
        if (!FontRegular) FontRegular = io.Fonts->AddFontDefault();
        if (!FontBold) FontBold = io.Fonts->AddFontDefault();
        if (!FontTitle) FontTitle = io.Fonts->AddFontDefault();
        if (!FontSekuya) FontSekuya = io.Fonts->AddFontDefault();
    }

    void ApplyStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* colors = style.Colors;

        // Custom Dark / Purple / Blue Hacker aesthetic
        // Deep dark background
        colors[ImGuiCol_WindowBg]           = ImVec4(0.05f, 0.05f, 0.08f, 1.00f);
        colors[ImGuiCol_ChildBg]            = ImVec4(0.08f, 0.08f, 0.12f, 1.00f);
        colors[ImGuiCol_PopupBg]            = ImVec4(0.08f, 0.08f, 0.12f, 0.94f);
        
        // Borders (Subtle purple/blue)
        colors[ImGuiCol_Border]             = ImVec4(0.54f, 0.17f, 0.89f, 0.30f); // #8A2BE2 with alpha
        colors[ImGuiCol_BorderShadow]       = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);

        // Frame Styling
        colors[ImGuiCol_FrameBg]            = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]     = ImVec4(0.20f, 0.20f, 0.30f, 1.00f);
        colors[ImGuiCol_FrameBgActive]      = ImVec4(0.25f, 0.25f, 0.35f, 1.00f);

        // Headers
        colors[ImGuiCol_Header]             = ImVec4(0.54f, 0.17f, 0.89f, 0.31f);
        colors[ImGuiCol_HeaderHovered]      = ImVec4(0.54f, 0.17f, 0.89f, 0.80f);
        colors[ImGuiCol_HeaderActive]       = ImVec4(0.54f, 0.17f, 0.89f, 1.00f);

        // Buttons
        colors[ImGuiCol_Button]             = ImVec4(0.15f, 0.15f, 0.22f, 1.00f);
        colors[ImGuiCol_ButtonHovered]      = ImVec4(0.00f, 0.90f, 1.00f, 0.20f); // Blue hover (Neon Cyan)
        colors[ImGuiCol_ButtonActive]       = ImVec4(0.00f, 0.90f, 1.00f, 0.40f);

        // Text
        colors[ImGuiCol_Text]               = ImVec4(0.90f, 0.90f, 0.95f, 1.00f);
        colors[ImGuiCol_TextDisabled]       = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);

        // Tabs
        colors[ImGuiCol_Tab]                = ImVec4(0.10f, 0.10f, 0.15f, 1.00f);
        colors[ImGuiCol_TabHovered]         = ImVec4(0.54f, 0.17f, 0.89f, 0.80f);
        colors[ImGuiCol_TabActive]          = ImVec4(0.54f, 0.17f, 0.89f, 1.00f);
        colors[ImGuiCol_TabUnfocused]       = ImVec4(0.10f, 0.10f, 0.15f, 1.00f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);

        // Title
        colors[ImGuiCol_TitleBg]            = ImVec4(0.05f, 0.05f, 0.08f, 1.00f);
        colors[ImGuiCol_TitleBgActive]      = ImVec4(0.08f, 0.08f, 0.12f, 1.00f);

        // Sliders & Progress
        colors[ImGuiCol_SliderGrab]         = ImVec4(0.00f, 0.90f, 1.00f, 1.00f); // Cyan
        colors[ImGuiCol_SliderGrabActive]   = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);

        // Style tweaks for minimalist aesthetic
        style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
        style.WindowPadding = ImVec2(15.0f, 15.0f);
        style.FramePadding = ImVec2(8.0f, 6.0f);
        style.ItemSpacing = ImVec2(10.0f, 10.0f);
        style.ScrollbarSize = 12.0f;
        style.WindowRounding = 8.0f;
        style.ChildRounding = 6.0f;
        style.FrameRounding = 6.0f;
        style.PopupRounding = 6.0f;
        style.ScrollbarRounding = 6.0f;
        style.GrabRounding = 6.0f;
        style.TabRounding = 6.0f;
        
        // Remove thick borders
        style.WindowBorderSize = 1.0f;
        style.FrameBorderSize = 0.0f;
        style.PopupBorderSize = 1.0f;
    }

    void RenderGlowingText(const char* text, ImU32 color, ImVec2 pos) {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        
        // Subtle offset glows for a neon effect
        ImVec2 offsets[] = {
            ImVec2(-1, 0), ImVec2(1, 0), ImVec2(0, -1), ImVec2(0, 1),
            ImVec2(-1, -1), ImVec2(1, -1), ImVec2(-1, 1), ImVec2(1, 1)
        };
        
        // Glow pass (low alpha)
        ImU32 glowColor = (color & 0x00FFFFFF) | 0x22000000; // Drop alpha
        for (auto& off : offsets) {
            draw_list->AddText(ImVec2(pos.x + off.x * 2, pos.y + off.y * 2), glowColor, text);
        }

        // Core text
        draw_list->AddText(pos, color, text);
    }

    bool GlowingButton(const char* label, const ImVec2& size, ImU32 glowColor) {
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImU32 col_bg = ImColor(ImGui::GetStyle().Colors[ImGuiCol_Button]);
        bool hovered = false;
        bool held = false;
        bool pressed = ImGui::ButtonBehavior(ImRect(p, ImVec2(p.x + size.x, p.y + size.y)), ImGui::GetID(label), &hovered, &held);
        
        if (held) { col_bg = ImColor(ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]); }
        else if (hovered) { col_bg = ImColor(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]); }

        // Draw Custom Button background
        draw_list->AddRectFilled(p, ImVec2(p.x + size.x, p.y + size.y), col_bg, ImGui::GetStyle().FrameRounding);

        // Draw Glow Border if hovered
        if (hovered) {
            draw_list->AddRect(p, ImVec2(p.x + size.x, p.y + size.y), glowColor, ImGui::GetStyle().FrameRounding, 0, 2.0f);
        }

        // Draw Text Centered
        ImVec2 textSize = ImGui::CalcTextSize(label);
        ImVec2 textPos = ImVec2(p.x + (size.x - textSize.x) * 0.5f, p.y + (size.y - textSize.y) * 0.5f);
        draw_list->AddText(textPos, ImColor(ImGui::GetStyle().Colors[ImGuiCol_Text]), label);

        // Advance cursor
        ImGui::ItemSize(size, ImGui::GetStyle().FramePadding.y);
        
        return pressed;
    }
}
