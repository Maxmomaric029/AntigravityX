#pragma once
#include <vector>
#include <string>

namespace Core {
    struct StartupItem {
        std::string Name;
        std::string Path;
        std::string Publisher;
        bool Enabled;
        std::string Impact; // "High", "Medium", "Low"
    };

    class StartupManager {
    public:
        void LoadStartupItems();
        const std::vector<StartupItem>& GetItems() const;
        void ToggleItem(const std::string& name, bool enable);

    private:
        std::vector<StartupItem> m_Items;
    };
}
