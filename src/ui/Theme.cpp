#include "Theme.h"
#include <string>
#include <cmath>

namespace Theme {
    ImFont* FontRegular = nullptr;
    ImFont* FontBold    = nullptr;
    ImFont* FontSekuya  = nullptr;
    ImFont* FontTitle   = nullptr;
    ImFont* FontSmall   = nullptr;

    // Default accent: electric purple
    ImVec4 Accent = ImVec4(0.54f, 0.17f, 0.89f, 1.0f);

    bool FileExists(const char* name) {
        if (FILE* file = fopen(name, "r")) {
            fclose(file);
            return true;
        }
        return false;
    }

    std::string GetFontPath(const std::string& fileName) {
        std::string paths[] = {
            fileName,
            "Datatype,Sekuya/" + fileName,
            "../" + fileName,
            "../Datatype,Sekuya/" + fileName,
            "C:/Users/Mxzzy/Downloads/AntiGravity X/Datatype,Sekuya/" + fileName
        };
        for (const auto& p : paths) {
            if (FileExists(p.c_str())) return p;
        }
        return "";
    }

    void LoadFonts(ImGuiIO& io) {
        std::string regularPath = GetFontPath("Datatype/static/Datatype-Regular.ttf");
        std::string boldPath    = GetFontPath("Datatype/static/Datatype-Bold.ttf");
        std::string sekuyaPath  = GetFontPath("Sekuya/Sekuya-Regular.ttf");

        if (!regularPath.empty()) {
            FontRegular = io.Fonts->AddFontFromFileTTF(regularPath.c_str(), 15.0f);
            FontSmall   = io.Fonts->AddFontFromFileTTF(regularPath.c_str(), 12.0f);
        }
        if (!boldPath.empty()) {
            FontBold    = io.Fonts->AddFontFromFileTTF(boldPath.c_str(), 15.0f);
            FontTitle   = io.Fonts->AddFontFromFileTTF(boldPath.c_str(), 22.0f);
        }
        if (!sekuyaPath.empty()) {
            FontSekuya  = io.Fonts->AddFontFromFileTTF(sekuyaPath.c_str(), 17.0f);
        }

        if (!FontRegular) FontRegular = io.Fonts->AddFontDefault();
        if (!FontBold)    FontBold    = io.Fonts->AddFontDefault();
        if (!FontTitle)   FontTitle   = io.Fonts->AddFontDefault();
        if (!FontSmall)   FontSmall   = io.Fonts->AddFontDefault();
        if (!FontSekuya)  FontSekuya  = io.Fonts->AddFontDefault();
    }

    void ApplyStyle() {
        ImGuiStyle& style = ImGui::GetStyle();
        ImVec4* c = style.Colors;

        c[ImGuiCol_WindowBg]          = ImVec4(0.04f, 0.04f, 0.07f, 1.00f);
        c[ImGuiCol_ChildBg]           = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
        c[ImGuiCol_PopupBg]           = ImVec4(0.07f, 0.07f, 0.11f, 0.97f);
        c[ImGuiCol_Border]            = ImVec4(Accent.x, Accent.y, Accent.z, 0.22f);
        c[ImGuiCol_BorderShadow]      = ImVec4(0, 0, 0, 0);
        c[ImGuiCol_FrameBg]           = ImVec4(0.10f, 0.10f, 0.16f, 1.00f);
        c[ImGuiCol_FrameBgHovered]    = ImVec4(0.18f, 0.18f, 0.28f, 1.00f);
        c[ImGuiCol_FrameBgActive]     = ImVec4(0.22f, 0.22f, 0.34f, 1.00f);
        c[ImGuiCol_Header]            = ImVec4(Accent.x, Accent.y, Accent.z, 0.25f);
        c[ImGuiCol_HeaderHovered]     = ImVec4(Accent.x, Accent.y, Accent.z, 0.60f);
        c[ImGuiCol_HeaderActive]      = ImVec4(Accent.x, Accent.y, Accent.z, 0.90f);
        c[ImGuiCol_Button]            = ImVec4(0.13f, 0.13f, 0.20f, 1.00f);
        c[ImGuiCol_ButtonHovered]     = ImVec4(Accent.x, Accent.y, Accent.z, 0.30f);
        c[ImGuiCol_ButtonActive]      = ImVec4(Accent.x, Accent.y, Accent.z, 0.55f);
        c[ImGuiCol_Text]              = ImVec4(0.92f, 0.92f, 0.96f, 1.00f);
        c[ImGuiCol_TextDisabled]      = ImVec4(0.45f, 0.45f, 0.55f, 1.00f);
        c[ImGuiCol_SliderGrab]        = ImVec4(Accent.x, Accent.y, Accent.z, 1.00f);
        c[ImGuiCol_SliderGrabActive]  = ImVec4(1.0f, 1.0f, 1.0f, 0.90f);
        c[ImGuiCol_CheckMark]         = ImVec4(Accent.x, Accent.y, Accent.z, 1.00f);
        c[ImGuiCol_Tab]               = ImVec4(0.09f, 0.09f, 0.14f, 1.00f);
        c[ImGuiCol_TabHovered]        = ImVec4(Accent.x, Accent.y, Accent.z, 0.70f);
        c[ImGuiCol_TabActive]         = ImVec4(Accent.x, Accent.y, Accent.z, 1.00f);
        c[ImGuiCol_TitleBg]           = ImVec4(0.04f, 0.04f, 0.07f, 1.00f);
        c[ImGuiCol_TitleBgActive]     = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
        c[ImGuiCol_ScrollbarBg]       = ImVec4(0.03f, 0.03f, 0.06f, 1.00f);
        c[ImGuiCol_ScrollbarGrab]     = ImVec4(Accent.x, Accent.y, Accent.z, 0.50f);
        c[ImGuiCol_ScrollbarGrabHovered] = ImVec4(Accent.x, Accent.y, Accent.z, 0.80f);
        c[ImGuiCol_ScrollbarGrabActive]  = ImVec4(1.0f, 1.0f, 1.0f, 0.80f);
        c[ImGuiCol_Separator]         = ImVec4(Accent.x, Accent.y, Accent.z, 0.20f);
        c[ImGuiCol_PlotHistogram]     = ImVec4(Accent.x, Accent.y, Accent.z, 1.00f);

        style.WindowTitleAlign    = ImVec2(0.5f, 0.5f);
        style.WindowPadding       = ImVec2(0.0f, 0.0f);
        style.FramePadding        = ImVec2(10.0f, 5.0f);
        style.ItemSpacing         = ImVec2(10.0f, 8.0f);
        style.ItemInnerSpacing    = ImVec2(6.0f, 4.0f);
        style.ScrollbarSize       = 6.0f;
        style.WindowRounding      = 12.0f;
        style.ChildRounding       = 8.0f;
        style.FrameRounding       = 6.0f;
        style.PopupRounding       = 8.0f;
        style.ScrollbarRounding   = 6.0f;
        style.GrabRounding        = 4.0f;
        style.TabRounding         = 6.0f;
        style.WindowBorderSize    = 1.0f;
        style.FrameBorderSize     = 0.0f;
    }

    void EaseValue(float& current, float target, float speed) {
        float dt = ImGui::GetIO().DeltaTime;
        current += (target - current) * (1.0f - expf(-speed * dt));
    }

    void DrawGlowRect(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, ImU32 color, float size, float rounding) {
        for (int i = 1; i <= (int)size; i++) {
            float alpha = (1.0f - (float)i / size) * 0.12f;
            ImU32 col = (color & 0x00FFFFFF) | ((ImU32)(alpha * 255) << 24);
            dl->AddRect(ImVec2(p_min.x - i, p_min.y - i), ImVec2(p_max.x + i, p_max.y + i), col, rounding, 0, 1.0f);
        }
    }

    void DrawGradientRect(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, ImU32 col1, ImU32 col2, bool vertical) {
        dl->AddRectFilledMultiColor(p_min, p_max, col1, vertical ? col1 : col2, col2, vertical ? col2 : col1);
    }

    void DrawGlassPanel(ImDrawList* dl, ImVec2 p_min, ImVec2 p_max, float rounding, float alpha) {
        // Drop Shadow
        DrawGlowRect(dl, p_min, p_max, IM_COL32(0, 0, 0, 150), 15.0f, rounding);

        // Core Glass Background
        dl->AddRectFilled(p_min, p_max, IM_COL32(10, 10, 20, (int)(alpha * 250)), rounding);

        // Top Highlight (Sheen)
        DrawGradientRect(dl, p_min, ImVec2(p_max.x, p_min.y + 40),
            IM_COL32(255, 255, 255, 12), IM_COL32(255, 255, 255, 0), true);

        // Edge light
        dl->AddRect(p_min, p_max, IM_COL32(138, 43, 226, 60), rounding, 0, 1.0f);
    }

    void RenderGlowingText(const char* text, ImU32 color, ImVec2 pos, ImFont* font) {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImU32 glow = (color & 0x00FFFFFF) | 0x28000000;
        float font_size = font ? ImGui::GetFontSize() : ImGui::GetFontSize(); // Use current context size

        static const ImVec2 offsets[] = {
            {-2,0},{2,0},{0,-2},{0,2},{-1,-1},{1,-1},{-1,1},{1,1}
        };
        for (auto& o : offsets)
            dl->AddText(font, font_size,
                ImVec2(pos.x + o.x, pos.y + o.y), glow, text);
        dl->AddText(font, font_size, pos, color, text);
    }

    bool GlowingButton(const char* label, const ImVec2& size, ImU32 glowColor) {
        ImVec2 p = ImGui::GetCursorScreenPos();
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImU32 col_bg = ImColor(ImGui::GetStyle().Colors[ImGuiCol_Button]);
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(
            ImRect(p, ImVec2(p.x + size.x, p.y + size.y)),
            ImGui::GetID(label), &hovered, &held);

        if (held)        col_bg = ImColor(ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
        else if (hovered) col_bg = ImColor(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);

        float rnd = ImGui::GetStyle().FrameRounding;
        dl->AddRectFilled(p, ImVec2(p.x+size.x, p.y+size.y), col_bg, rnd);
        if (hovered)
            dl->AddRect(p, ImVec2(p.x+size.x, p.y+size.y), glowColor, rnd, 0, 1.5f);

        ImVec2 ts = ImGui::CalcTextSize(label);
        dl->AddText(ImVec2(p.x + (size.x - ts.x) * 0.5f,
                           p.y + (size.y - ts.y) * 0.5f),
                    ImColor(ImGui::GetStyle().Colors[ImGuiCol_Text]), label);

        ImGui::ItemSize(size, ImGui::GetStyle().FramePadding.y);
        return pressed;
    }

    bool SidebarTab(const char* icon, const char* label, bool selected, float& highlightAlpha) {
        EaseValue(highlightAlpha, selected ? 1.0f : 0.0f, 15.0f);

        ImVec2 p      = ImGui::GetCursorScreenPos();
        ImVec2 size   = ImVec2(ImGui::GetContentRegionAvail().x - 10, 44.0f);
        p.x += 5; // Horizontal offset for centering feel
        ImDrawList* dl = ImGui::GetWindowDrawList();

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(
            ImRect(p, ImVec2(p.x + size.x, p.y + size.y)),
            ImGui::GetID(label), &hovered, &held);

        // Sidebar background gradient when selected/hovered
        float bgAlpha = highlightAlpha * 0.25f + (hovered ? 0.10f : 0.0f);
        if (bgAlpha > 0.01f) {
            ImU32 col1 = IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), (int)(bgAlpha * 255));
            ImU32 col2 = IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), 0);
            DrawGradientRect(dl, p, ImVec2(p.x + size.x, p.y + size.y), col1, col2, false);
        }

        // Left accent bar (The "active" indicator)
        if (highlightAlpha > 0.01f) {
            float barH = size.y * 0.5f * highlightAlpha;
            dl->AddRectFilled(ImVec2(p.x - 2, p.y + (size.y - barH) * 0.5f),
                ImVec2(p.x + 1, p.y + (size.y + barH) * 0.5f),
                IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), (int)(highlightAlpha * 255)), 2.0f);
            
            // Outer glow for the bar
            DrawGlowRect(dl, ImVec2(p.x - 3, p.y + (size.y - barH) * 0.5f),
                ImVec2(p.x, p.y + (size.y + barH) * 0.5f),
                IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), 100), 4.0f, 2.0f);
        }

        ImU32 textCol = selected ? IM_COL32(255, 255, 255, 255) : IM_COL32(160, 160, 180, 200);
        
        // Icon (slightly larger)
        ImGui::PushFont(FontSekuya ? FontSekuya : FontBold);
        dl->AddText(ImVec2(p.x + 14, p.y + (size.y - ImGui::GetFontSize()) * 0.5f), textCol, icon);
        ImGui::PopFont();

        // Label
        dl->AddText(ImVec2(p.x + 42, p.y + (size.y - ImGui::GetFontSize()) * 0.5f), textCol, label);

        ImGui::ItemSize(size);
        return pressed;
    }

    bool ToggleSwitch(const char* label, bool* value) {
        ImVec2 p   = ImGui::GetCursorScreenPos();
        float  w   = 40.0f, h = 20.0f, r = h * 0.5f;
        ImDrawList* dl = ImGui::GetWindowDrawList();

        ImGui::InvisibleButton(label, ImVec2(w + 8 + ImGui::CalcTextSize(label).x, h));
        bool clicked = ImGui::IsItemClicked();
        if (clicked) *value = !*value;
        bool hov = ImGui::IsItemHovered();

        // Track
        ImU32 trackCol = *value
            ? IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), hov ? 240 : 200)
            : IM_COL32(40, 40, 60, 200);
        dl->AddRectFilled(p, ImVec2(p.x + w, p.y + h), trackCol, r);
        dl->AddRect(p, ImVec2(p.x + w, p.y + h),
            IM_COL32(255,255,255, *value ? 40 : 20), r, 0, 1.0f);

        // Thumb
        float thumbX = *value ? p.x + w - r - 2 : p.x + r + 2;
        dl->AddCircleFilled(ImVec2(thumbX, p.y + r), r - 3,
            IM_COL32(255, 255, 255, *value ? 255 : 180));

        // Label
        dl->AddText(ImVec2(p.x + w + 8, p.y + (h - ImGui::GetFontSize()) * 0.5f),
            IM_COL32(220, 220, 240, 255), label);

        return clicked;
    }

    bool ModernSlider(const char* label, float* v, float v_min, float v_max, const char* fmt) {
        ImVec2 p   = ImGui::GetCursorScreenPos();
        float  w   = ImGui::GetContentRegionAvail().x;
        float  h   = 28.0f;
        float  tw  = w - 10.0f;
        float  th  = 4.0f;
        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Label + value
        char valBuf[32];
        snprintf(valBuf, sizeof(valBuf), fmt, *v);
        ImVec2 ts = ImGui::CalcTextSize(label);
        ImVec2 vs = ImGui::CalcTextSize(valBuf);
        dl->AddText(ImVec2(p.x, p.y + 2),
            IM_COL32(190, 190, 210, 255), label);
        dl->AddText(ImVec2(p.x + tw - vs.x, p.y + 2),
            IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), 255), valBuf);

        // Track
        ImVec2 tp = ImVec2(p.x, p.y + h - 8);
        dl->AddRectFilled(tp, ImVec2(tp.x + tw, tp.y + th),
            IM_COL32(30, 30, 50, 255), 2.0f);

        // Fill
        float t = (*v - v_min) / (v_max - v_min);
        if (t > 0.0f) {
            DrawGradientRect(dl, tp, ImVec2(tp.x + tw * t, tp.y + th),
                IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), 255),
                IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), 150), false);
            
            // Glow on the fill
            DrawGlowRect(dl, tp, ImVec2(tp.x + tw * t, tp.y + th),
                IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), 80), 3.0f, 2.0f);
        }

        // Thumb
        float tx = tp.x + tw * t;
        dl->AddCircleFilled(ImVec2(tx, tp.y + th * 0.5f), 7.0f,
            IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), 255));
        dl->AddCircleFilled(ImVec2(tx, tp.y + th * 0.5f), 4.5f,
            IM_COL32(255, 255, 255, 220));

        // Interaction
        ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + h - 14));
        ImGui::InvisibleButton(label, ImVec2(tw, 14.0f));
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
            float mx = ImGui::GetIO().MousePos.x;
            *v = v_min + (v_max - v_min) * ImClamp((mx - tp.x) / tw, 0.0f, 1.0f);
        }
        ImGui::SetCursorScreenPos(ImVec2(p.x, p.y + h));
        return ImGui::IsItemDeactivatedAfterEdit();
    }

    void SectionHeader(const char* label) {
        ImVec2 p  = ImGui::GetCursorScreenPos();
        float  w  = ImGui::GetContentRegionAvail().x;
        ImDrawList* dl = ImGui::GetWindowDrawList();

        ImVec2 ts = ImGui::CalcTextSize(label);
        dl->AddText(p, IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), 200), label);
        dl->AddLine(ImVec2(p.x + ts.x + 8, p.y + ts.y * 0.5f),
                    ImVec2(p.x + w, p.y + ts.y * 0.5f),
                    IM_COL32((int)(Accent.x*255),(int)(Accent.y*255),(int)(Accent.z*255), 55), 1.0f);
        ImGui::Dummy(ImVec2(0, ts.y + 4));
    }
}
