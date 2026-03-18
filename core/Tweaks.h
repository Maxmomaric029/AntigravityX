#pragma once
#include <vector>
#include <string>

namespace Core {
    struct TweakItem {
        std::string Name;
        std::string Description;
        bool IsApplied;
    };

    class TweakManager {
    public:
        void LoadTweaks();
        const std::vector<TweakItem>& GetTweaks() const;
        void ApplyTweak(const std::string& name, bool apply);
        void ApplyGamingProfile();

    private:
        std::vector<TweakItem> m_Tweaks;
    };
}
