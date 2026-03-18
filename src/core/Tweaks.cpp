#include "Tweaks.h"

namespace Core {
    void TweakManager::LoadTweaks() {
        m_Tweaks.clear();
        m_Tweaks.push_back({"Disable Telemetry", "Disables Windows tracking and data collection (Privacy).", false});
        m_Tweaks.push_back({"Gaming Power Plan", "Enables Ultimate Performance mode.", true});
        m_Tweaks.push_back({"Disable Xbox Game Bar", "Removes Game DVR background recording.", false});
        m_Tweaks.push_back({"Optimize Visual Effects", "Disables fading and animations for better performance.", false});
        m_Tweaks.push_back({"Disable Background Apps", "Prevents universal Windows apps from running in the background.", true});
    }

    const std::vector<TweakItem>& TweakManager::GetTweaks() const {
        return m_Tweaks;
    }

    void TweakManager::ApplyTweak(const std::string& name, bool apply) {
        for (auto& tweak : m_Tweaks) {
            if (tweak.Name == name) {
                tweak.IsApplied = apply;
                break;
            }
        }
    }

    void TweakManager::ApplyGamingProfile() {
        ApplyTweak("Gaming Power Plan", true);
        ApplyTweak("Disable Xbox Game Bar", true);
        ApplyTweak("Disable Background Apps", true);
    }
}
