#pragma once
#include <vector>
#include <string>
#include <cstdint>

namespace Core {
    struct ProcessInfo {
        uint32_t PID;
        std::string Name;
        float CpuUsage;
        size_t RamUsageBytes;
        bool IsSuspicious;
    };

    class ProcessManager {
    public:
        void RefreshProcesses();
        const std::vector<ProcessInfo>& GetProcesses() const;
        void KillProcess(uint32_t pid);
        
        // Mode "BOOST"
        void ForceBoostSystem();

    private:
        std::vector<ProcessInfo> m_Processes;
    };
}
