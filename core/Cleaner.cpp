#include "Cleaner.h"
#include <iostream>

namespace Core {
    CleanerEngine::CleanerEngine() {
        // Initialize basic Windows paths to look at
        m_Targets.push_back({"Windows Temp", "C:\\Windows\\Temp", 0, true});
        m_Targets.push_back({"User Temp", "%LOCALAPPDATA%\\Temp", 0, true});
        m_Targets.push_back({"Prefetch", "C:\\Windows\\Prefetch", 0, true});
        m_Targets.push_back({"Browser Cache (Chrome)", "%LOCALAPPDATA%\\Google\\Chrome\\User Data\\Default\\Cache", 0, false});
        m_Targets.push_back({"System Logs", "C:\\Windows\\System32\\winevt\\Logs", 0, false});
    }

    CleanerEngine::~CleanerEngine() {}

    void CleanerEngine::ScanSystem() {
        // Dummy scan for MVP
        for (auto& target : m_Targets) {
            target.SizeBytes = (rand() % 1024) * 1024 * 512; // Fake sizes up to 512MB
        }
    }

    void CleanerEngine::CleanSelected() {
        // Dummy clean
        for (auto& target : m_Targets) {
            if (target.Selected) {
                target.SizeBytes = 0; // Cleared
                std::cout << "Cleaned: " << target.Name << std::endl;
            }
        }
    }

    const std::vector<CleanTarget>& CleanerEngine::GetTargets() const {
        return m_Targets;
    }
}
