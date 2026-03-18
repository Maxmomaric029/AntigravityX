#include "StartupManager.h"

namespace Core {
    void StartupManager::LoadStartupItems() {
        m_Items.clear();
        // Stub: In reality this checks HKCU\...\Run and HKLM\...\Run
        m_Items.push_back({"Discord", "C:\\Users\\User\\AppData\\Local\\Discord\\update.exe", "Discord Inc.", true, "High"});
        m_Items.push_back({"Steam", "C:\\Program Files (x86)\\Steam\\steam.exe", "Valve", true, "High"});
        m_Items.push_back({"Spotify", "C:\\Users\\User\\AppData\\Roaming\\Spotify\\Spotify.exe", "Spotify AB", false, "Medium"});
        m_Items.push_back({"One Drive", "C:\\Program Files\\Microsoft OneDrive\\OneDrive.exe", "Microsoft", true, "High"});
    }

    const std::vector<StartupItem>& StartupManager::GetItems() const {
        return m_Items;
    }

    void StartupManager::ToggleItem(const std::string& name, bool enable) {
        for (auto& item : m_Items) {
            if (item.Name == name) {
                item.Enabled = enable;
                break;
            }
        }
    }
}
