#include "MainWindow.h"
#include "Theme.h"
#include "imgui.h"

namespace MainWindow {
    static int currentTab = 0;

    void RenderSidebar() {
        ImGui::BeginChild("Sidebar", ImVec2(240, 0), true);
        
        // AntiGravity X Logo area
        ImGui::Dummy(ImVec2(0, 10));
        ImGui::PushFont(Theme::FontTitle);
        ImVec2 textPos = ImVec2(ImGui::GetCursorScreenPos().x + 10, ImGui::GetCursorScreenPos().y);
        Theme::RenderGlowingText("AntiGravity X", ImColor(0.54f, 0.17f, 0.89f, 1.0f), textPos);
        ImGui::Dummy(ImVec2(0, 40));
        ImGui::PopFont();

        const char* tabs[] = {
            "Dashboard",
            "System Cleaner",
            "Startup Manager",
            "Process Monitor",
            "Registry Optimizer",
            "System Tweaks",
            "Privacy & Browser",
            "Uninstaller",
            "Disk Optimization",
            "Security",
            "Automation"
        };

        for (int i = 0; i < IM_ARRAYSIZE(tabs); i++) {
            bool selected = (currentTab == i);
            if (selected) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.54f, 0.17f, 0.89f, 0.6f)); // Purple active
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0,0,0,0)); // Transparent normal
            }

            ImGui::PushFont(Theme::FontBold);
            if (ImGui::Button(tabs[i], ImVec2(-1, 45))) {
                currentTab = i;
            }
            ImGui::PopFont();
            ImGui::PopStyleColor();
            ImGui::Dummy(ImVec2(0, 5));
        }

        ImGui::EndChild();
    }

    void RenderDashboard() {
        ImGui::PushFont(Theme::FontTitle);
        Theme::RenderGlowingText("System Dashboard", ImColor(0.00f, 0.90f, 1.00f, 1.00f), ImGui::GetCursorScreenPos());
        ImGui::Dummy(ImVec2(0, 40));
        ImGui::PopFont();
        
        ImGui::PushFont(Theme::FontSekuya);
        ImGui::Text("Overall Health: OPTIMAL");
        ImGui::PopFont();
        
        ImGui::Dummy(ImVec2(0, 30));
        
        // Big Boost Button
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 0.8f)); // Red
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f)); // Lighter red
        
        ImGui::PushFont(Theme::FontTitle);
        if (ImGui::Button("FORCE BOOST", ImVec2(250, 70))) {
            // Placeholder
        }
        ImGui::PopFont();
        ImGui::PopStyleColor(2);

        ImGui::Dummy(ImVec2(0, 40));

        ImGui::Text("Hardware Monitor (Stubs)");
        // Dummy progress bars to simulate CPU/RAM
        float cpuUse = 0.23f;
        float ramUse = 0.55f;
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.54f, 0.17f, 0.89f, 1.0f));
        ImGui::ProgressBar(cpuUse, ImVec2(300, 20), "CPU Usage 23%");
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 0.90f, 1.00f, 1.0f));
        ImGui::ProgressBar(ramUse, ImVec2(300, 20), "RAM Usage 55%");
        ImGui::PopStyleColor(2);
    }

    void RenderContent() {
        ImGui::BeginChild("Content", ImVec2(0, 0), false);
        
        // Add some padding inside the content view
        ImGui::SetCursorPos(ImVec2(30, 30));
        ImGui::BeginGroup();

        switch (currentTab) {
            case 0: RenderDashboard(); break;
            case 1: ImGui::Text("System Cleaner Mode..."); break;
            case 2: ImGui::Text("Startup Manager Module..."); break;
            case 3: ImGui::Text("Detailed Process Monitor..."); break;
            case 4: ImGui::Text("Registry Tweaks & Fixes..."); break;
            case 5: ImGui::Text("Deep System Configuration..."); break;
            case 6: ImGui::Text("Browser Traces Cleaner..."); break;
            case 7: ImGui::Text("Force Program Removal..."); break;
            case 8: ImGui::Text("SSD Trim & HDD Defrag..."); break;
            case 9: ImGui::Text("Basic Firewall / Sec Tools..."); break;
            case 10: ImGui::Text("1-Click Smart Clean..."); break;
        }

        ImGui::EndGroup();
        ImGui::EndChild();
    }

    void Render() {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        
        ImGui::Begin("MainLayout", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        Theme::ApplyStyle();
        
        ImGui::PushFont(Theme::FontRegular); // Default content font
        RenderSidebar();
        ImGui::SameLine();
        RenderContent();
        ImGui::PopFont();

        ImGui::End();
    }
}
