#pragma once
#include <vector>
#include <string>

namespace Core {
    struct CleanTarget {
        std::string Name;
        std::string Path;
        size_t SizeBytes;
        bool Selected;
    };

    class CleanerEngine {
    public:
        CleanerEngine();
        ~CleanerEngine();

        void ScanSystem();
        void CleanSelected();
        const std::vector<CleanTarget>& GetTargets() const;

    private:
        std::vector<CleanTarget> m_Targets;
    };
}
