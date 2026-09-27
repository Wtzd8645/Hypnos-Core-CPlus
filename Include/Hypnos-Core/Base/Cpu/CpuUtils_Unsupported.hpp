#pragma once

namespace Blanketmen {

inline void CpuInfo::GetAvailableCpuIds()
{
    available_count = 0;
}

inline Status<void> CpuUtils::PinThread(uint32 cpu_index) noexcept
{
    return Status<void>::Error(ErrorCode::Unsupported, "Unsupported platform");
}

inline Status<void> CpuUtils::PinThread(Thread& thread, uint32 cpu_index) noexcept
{
    return Status<void>::Error(ErrorCode::Unsupported, "Unsupported platform");
}

} // namespace Blanketmen
