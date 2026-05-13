#pragma once

namespace Blanketmen {

struct CpuUtils
{
    static constexpr uint32 MaxCpuIdCount = 1024;

    [[nodiscard]] static uint32 GetLogicalCount()
    {
        uint32 count = static_cast<uint32>(Thread::hardware_concurrency());
        return count == 0 ? 1 : count;
    }

    [[nodiscard]] static uint32 GetAvailableCpuIds(uint32* cpu_ids);

    [[nodiscard]] static Status<void> PinThread(uint32 cpu_id) noexcept;
    [[nodiscard]] static Status<void> PinThread(Thread& thread, uint32 cpu_id) noexcept;
};

} // namespace Blanketmen

#if defined(_WIN32)
#include "CpuUtils_Windows.hpp"
#elif defined(__linux__)
#include "CpuUtils_Linux.hpp"
#else
#include "CpuUtils_Unsupported.hpp"
#endif
