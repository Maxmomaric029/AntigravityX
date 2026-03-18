#include "MainWindow.h"
#include "Theme.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <cstring>

// ─────────────────────────────────────────
//  Section definitions
// ─────────────────────────────────────────
struct SectionInfo { const char* icon; const char* label; };
static const SectionInfo kSections[] = {
    { ">>", "COMBAT"     },
    { "[]", "VISUALS"    },
    { "<>", "HITBOX"     },
    { "**", "SYSTEM"     },
    { "AX", "AntiGravity X" },
};
static const int kSectionCount = (int)(sizeof(kSections) / sizeof(kSections[0]));

// Per-section highlight animation state
static float sHighlight[5] = {};

namespace MainWindow {
    static int  sCurrentTab    = 0;
    static float sTabAlpha      = 1.0f;
    static float sTabTargetAlpha = 1.0f;
    static int  sPendingTab    = 0;
    static bool sSettingsOpen  = false;
    static float sSettingsAlpha = 0.0f;

    // ── COMBAT state ──
    static bool  sCombat_AimLockHead  = false;
    static bool  sCombat_AimLockSmooth= false;
    static bool  sCombat_SilentAim    = false;
    static bool  sCombat_FOVCircle    = false;
    static float sCombat_FOV          = 80.0f;
    static bool  sCombat_NoRecoil     = false;
    static float sCombat_Smooth       = 4.0f;
    static float sCombat_ReactionMs   = 60.0f;

    // ── VISUALS state ──
    static bool  sVis_ESPBox      = false;
    static bool  sVis_ESP3D       = false;
    static bool  sVis_Distance    = false;
    static bool  sVis_Name        = false;
    static bool  sVis_Skeleton    = false;
    static bool  sVis_Line        = false;
    static float sVis_BoxThick    = 1.0f;

    // ── HITBOX state ──
    static bool  sHit_Expand      = false;
    static float sHit_ExpandScale = 2.0f;
    static bool  sHit_Pescoco     = false;
    static bool  sHit_NPCBigHead  = false;
    static bool  sHit_Trigger     = false;
    static float sHit_TrigDelay   = 30.0f;

    // ── SYSTEM state ──
    static bool  sSys_FPSUnlock   = false;
    static bool  sSys_RAMPurge    = false;
    static bool  sSys_GPUOC       = false;
    static bool  sSys_AntiReport  = false;
    static bool  sSys_LogCleaner  = false;
    static float sSys_FPSTarget   = 120.0f;

    // ── AntiGravity X state ──
    static float sAGX_CPUUsage    = 0.23f; // stub
    static float sAGX_RAMUsage    = 0.55f; // stub
    static bool  sAGX_AutoClean   = false;
    static bool  sAGX_AutoStartup = false;
    static bool  sAGX_KillProcs   = false;
    static bool  sAGX_RegFix      = false;

    // ─────────────────────────────────────
    static void DrawSidebar() {
        ImVec2 sidebarSize = ImVec2(188.0f, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::BeginChild("##sidebar", sidebarSize, false,
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::PopStyleVar();

        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 sb_pos  = ImGui::GetWindowPos();
        ImVec2 sb_size = ImGui::GetWindowSize();

        // Sidebar background  
        Theme::DrawGlassPanel(dl, sb_pos,
            ImVec2(sb_pos.x + sb_size.x, sb_pos.y + sb_size.y), 0, 0.95f);

        // Logo area
        ImGui::Dummy(ImVec2(0, 18));
        ImGui::PushFont(Theme::FontBold);
        ImVec2 logoPos = ImVec2(sb_pos.x + 14, ImGui::GetCursorScreenPos().y);
        Theme::RenderGlowingText("AntiGravity X",
            IM_COL32(138, 43, 226, 255), logoPos, Theme::FontBold);
        ImGui::Dummy(ImVec2(0, 34));
        ImGui::PopFont();

        // Divider
        dl->AddLine(ImVec2(sb_pos.x + 12, ImGui::GetCursorScreenPos().y),
            ImVec2(sb_pos.x + sb_size.x - 12, ImGui::GetCursorScreenPos().y),
            IM_COL32(138, 43, 226, 40), 1.0f);
        ImGui::Dummy(ImVec2(0, 8));

        // Section tabs
        ImGui::PushFont(Theme::FontBold);
        for (int i = 0; i < kSectionCount; i++) {
            bool sel = (sCurrentTab == i);
            if (Theme::SidebarTab(kSections[i].icon, kSections[i].label, sel, sHighlight[i])) {
                if (sCurrentTab != i) {
                    sPendingTab      = i;
                    sTabTargetAlpha  = 0.0f;
                }
            }
            ImGui::Dummy(ImVec2(0, 2));
        }
        ImGui::PopFont();

        // Bottom divider + settings
        dl->AddLine(ImVec2(sb_pos.x + 12, sb_pos.y + sb_size.y - 52),
            ImVec2(sb_pos.x + sb_size.x - 12, sb_pos.y + sb_size.y - 52),
            IM_COL32(138, 43, 226, 35), 1.0f);

        ImGui::SetCursorScreenPos(ImVec2(sb_pos.x + 14, sb_pos.y + sb_size.y - 42));
        ImGui::PushFont(Theme::FontSmall);
        if (Theme::GlowingButton("Settings", ImVec2(sb_size.x - 28, 30),
                IM_COL32(138, 43, 226, 200)))
            sSettingsOpen = !sSettingsOpen;
        ImGui::PopFont();

        ImGui::EndChild();
    }

    // ─────────────────────────────────────
    static void DrawTopBar(float panelW) {
        ImGui::BeginChild("##topbar", ImVec2(panelW, 48), false,
            ImGuiWindowFlags_NoScrollbar);

        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 p = ImGui::GetWindowPos();
        Theme::DrawGlassPanel(dl, p,
            ImVec2(p.x + panelW, p.y + 48), 0, 0.70f);

        // Current tab title
        ImGui::SetCursorPos(ImVec2(14, (48 - ImGui::GetFontSize()) * 0.5f));
        ImGui::PushFont(Theme::FontBold);
        ImGui::TextColored(Theme::Accent, "%s  /  AntiGravity X",
            kSections[sCurrentTab].label);
        ImGui::PopFont();

        ImGui::EndChild();
    }

    // ─────────────────────────────────────
    static void RenderCombat() {
        ImGui::Dummy(ImVec2(0, 4));
        Theme::SectionHeader("  Aimbot & AimLock");

        Theme::ToggleSwitch("AimLock Supremo  (Head)", &sCombat_AimLockHead);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("AimLock Smooth   (Neck)",  &sCombat_AimLockSmooth);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("Silent Aim  (Magic Bullet)", &sCombat_SilentAim);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("FOV Circle", &sCombat_FOVCircle);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("No Recoil 100%", &sCombat_NoRecoil);

        ImGui::Dummy(ImVec2(0, 12));
        Theme::SectionHeader("  Parameters");

        Theme::ModernSlider("Field of View", &sCombat_FOV, 10.0f, 360.0f, "%.0f");
        ImGui::Dummy(ImVec2(0, 8));
        Theme::ModernSlider("Smoothing Factor", &sCombat_Smooth, 1.0f, 20.0f, "%.1f");
        ImGui::Dummy(ImVec2(0, 8));
        Theme::ModernSlider("Reaction Time (ms)", &sCombat_ReactionMs, 0.0f, 250.0f, "%.0f ms");
    }

    // ─────────────────────────────────────
    static void RenderVisuals() {
        ImGui::Dummy(ImVec2(0, 4));
        Theme::SectionHeader("  ESP & Wallhack");

        Theme::ToggleSwitch("ESP Box 2D",      &sVis_ESPBox);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("ESP Box 3D",      &sVis_ESP3D);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("ESP Distance",    &sVis_Distance);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("ESP Name & Clan", &sVis_Name);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("ESP Skeleton  (Morado 9B)", &sVis_Skeleton);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("ESP Line  (Head-to-Head)", &sVis_Line);

        ImGui::Dummy(ImVec2(0, 12));
        Theme::SectionHeader("  Parameters");
        Theme::ModernSlider("Box Thickness", &sVis_BoxThick, 0.5f, 4.0f, "%.1f px");
    }

    // ─────────────────────────────────────
    static void RenderHitbox() {
        ImGui::Dummy(ImVec2(0, 4));
        Theme::SectionHeader("  Trigger & Scale");

        Theme::ToggleSwitch("Hitbox Expand",    &sHit_Expand);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("Hitbox Pescoco  (Neck Focus)", &sHit_Pescoco);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("NPC Big Head  (Visual Only)", &sHit_NPCBigHead);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("Triggerbot", &sHit_Trigger);

        ImGui::Dummy(ImVec2(0, 12));
        Theme::SectionHeader("  Parameters");

        const char* scaleLabels[] = { "x2", "x5", "x10" };
        ImGui::PushFont(Theme::FontSmall);
        ImGui::TextColored(ImVec4(0.6f,0.6f,0.7f,1), "Hitbox Scale");
        ImGui::PopFont();
        ImGui::Dummy(ImVec2(0, 4));

        static int sHitScale = 0;
        for (int i = 0; i < 3; i++) {
            bool sel = (sHitScale == i);
            if (sel) ImGui::PushStyleColor(ImGuiCol_Button,
                ImVec4(Theme::Accent.x, Theme::Accent.y, Theme::Accent.z, 0.55f));
            else     ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f,0.1f,0.16f,1));
            if (ImGui::Button(scaleLabels[i], ImVec2(54, 28))) sHitScale = i;
            ImGui::PopStyleColor();
            if (i < 2) ImGui::SameLine();
        }
        ImGui::Dummy(ImVec2(0, 8));
        Theme::ModernSlider("Trigger Delay (ms)", &sHit_TrigDelay, 0.0f, 300.0f, "%.0f ms");
    }

    // ─────────────────────────────────────
    static void RenderSystem() {
        ImGui::Dummy(ImVec2(0, 4));
        Theme::SectionHeader("  Performance & Bypass");

        Theme::ToggleSwitch("120 FPS Unlocker",       &sSys_FPSUnlock);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("RAM Purge  (Anti-Lag)",  &sSys_RAMPurge);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("GPU Overclock  (Sim)",   &sSys_GPUOC);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("Anti-Report System",     &sSys_AntiReport);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("Log Cleaner  (on exit)", &sSys_LogCleaner);

        ImGui::Dummy(ImVec2(0, 12));
        Theme::SectionHeader("  Parameters");
        Theme::ModernSlider("Target FPS", &sSys_FPSTarget, 60.0f, 240.0f, "%.0f fps");
    }

    // ─────────────────────────────────────
    static void RenderAntiGravityX() {
        ImGui::Dummy(ImVec2(0, 4));
        Theme::SectionHeader("  System Dashboard");

        ImGui::PushFont(Theme::FontSmall);
        ImGui::TextColored(ImVec4(0.6f,0.6f,0.7f,1), "CPU Usage");
        ImGui::PopFont();
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
            ImVec4(Theme::Accent.x, Theme::Accent.y, Theme::Accent.z, 1.0f));
        ImGui::ProgressBar(sAGX_CPUUsage, ImVec2(-1, 10), "");
        ImGui::PopStyleColor();
        ImGui::Dummy(ImVec2(0, 4));

        ImGui::PushFont(Theme::FontSmall);
        ImGui::TextColored(ImVec4(0.6f,0.6f,0.7f,1), "RAM Usage");
        ImGui::PopFont();
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram,
            ImVec4(0.0f, 0.9f, 1.0f, 1.0f));
        ImGui::ProgressBar(sAGX_RAMUsage, ImVec2(-1, 10), "");
        ImGui::PopStyleColor();

        ImGui::Dummy(ImVec2(0, 12));
        Theme::SectionHeader("  Automation");

        Theme::ToggleSwitch("Auto Clean on Launch",    &sAGX_AutoClean);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("Disable Startup Bloat",  &sAGX_AutoStartup);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("Kill Background Procs",  &sAGX_KillProcs);
        ImGui::Dummy(ImVec2(0, 4));
        Theme::ToggleSwitch("Registry Optimizer",     &sAGX_RegFix);

        ImGui::Dummy(ImVec2(0, 16));
        ImGui::PushStyleColor(ImGuiCol_Button,
            ImVec4(0.8f, 0.1f, 0.1f, 0.75f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
            ImVec4(1.0f, 0.2f, 0.2f, 0.90f));
        ImGui::PushFont(Theme::FontBold);
        if (ImGui::Button("  FORCE BOOST  ", ImVec2(-1, 46))) { /* Action */ }
        ImGui::PopFont();
        ImGui::PopStyleColor(2);
    }

    // ─────────────────────────────────────
    static void DrawSettingsPopup() {
        Theme::EaseValue(sSettingsAlpha, sSettingsOpen ? 1.0f : 0.0f, 12.0f);
        if (sSettingsAlpha < 0.01f) return;

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, sSettingsAlpha);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14, 14));
        ImGui::SetNextWindowSize(ImVec2(220, 140), ImGuiCond_Always);

        ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(
            ImVec2(vp->Pos.x + 14, vp->Pos.y + vp->Size.y - 154),
            ImGuiCond_Always);

        if (ImGui::Begin("##settings_popup", nullptr,
                ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav)) {

            ImDrawList* dl = ImGui::GetWindowDrawList();
            Theme::DrawGlassPanel(dl, ImGui::GetWindowPos(),
                ImVec2(ImGui::GetWindowPos().x + 220,
                       ImGui::GetWindowPos().y + 140), 10.0f);

            ImGui::TextColored(Theme::Accent, "Settings");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 4));
            ImGui::TextColored(ImVec4(0.6f,0.6f,0.7f,1), "Accent Color");
            ImGui::SameLine();
            ImGui::ColorEdit3("##accent", &Theme::Accent.x,
                ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

            if (!ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows)
                && ImGui::IsMouseClicked(0))
                sSettingsOpen = false;
        }
        ImGui::End();
        ImGui::PopStyleVar(2);
    }

    // ─────────────────────────────────────
    void Render() {
        // Tab fade-in transition
        Theme::EaseValue(sTabAlpha, sTabTargetAlpha, 14.0f);
        if (sTabAlpha < 0.05f && sTabTargetAlpha < 0.5f) {
            sCurrentTab  = sPendingTab;
            sTabTargetAlpha = 1.0f;
        }

        ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(vp->Pos);
        ImGui::SetNextWindowSize(vp->Size);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("##root", nullptr,
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize   | ImGuiWindowFlags_NoMove     |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
        ImGui::PopStyleVar(3);

        // Global background gradient-like dark fill
        ImDrawList* bdl = ImGui::GetBackgroundDrawList();
        bdl->AddRectFilled(vp->Pos,
            ImVec2(vp->Pos.x + vp->Size.x, vp->Pos.y + vp->Size.y),
            IM_COL32(8, 8, 14, 255));

        // ── Layout ──
        ImGui::PushFont(Theme::FontRegular);

        // Sidebar
        DrawSidebar();
        ImGui::SameLine(0, 0);

        // Right panel
        float panelW = vp->Size.x - 188.0f;
        ImGui::BeginGroup();

        // Top Bar
        DrawTopBar(panelW);

        // Content area
        float contentH = vp->Size.y - 48.0f;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(18, 14));
        ImGui::BeginChild("##content", ImVec2(panelW, contentH), false);
        ImGui::PopStyleVar();

        // Fade via global alpha
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, sTabAlpha);
        switch (sCurrentTab) {
            case 0: RenderCombat();      break;
            case 1: RenderVisuals();     break;
            case 2: RenderHitbox();      break;
            case 3: RenderSystem();      break;
            case 4: RenderAntiGravityX();break;
        }
        ImGui::PopStyleVar();

        ImGui::EndChild();
        ImGui::EndGroup();

        ImGui::PopFont();
        ImGui::End();

        // Settings popup (outside main window to float above)
        DrawSettingsPopup();
    }
}
