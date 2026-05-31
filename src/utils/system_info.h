#pragma once

#include <string>

namespace goxel::utils
{

struct SystemInfo
{
    std::string os_version;
    std::string gpu_id;
    std::string gpu_vendor;
    int cpu_cores;
    std::string system_memory;
};

auto system_info() -> SystemInfo;
auto to_string(const SystemInfo &info) -> std::string;

} // namespace utils
