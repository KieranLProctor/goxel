#include "system_info.h"

#include <format>

namespace utils
{

// TODO: Implement.
auto system_info() -> SystemInfo
{
    return {
        .os_version = "REPLACE",
        .gpu_id = "REPLACE",
        .gpu_vendor = "REPLACE",
        .cpu_cores = 1,
        .system_memory = "REPLACE",
    };
}

auto to_string(const SystemInfo &info) -> std::string
{
    const auto get_or_empty = [](const auto &s) { return s.empty() ? "<unknown>" : s; };

    return std::format("os_version: {}\ngpu_id: {}\ngpu_vendor: {}\ncpu_cores: {}\nsystem_memory: {}",
                       get_or_empty(info.os_version), get_or_empty(info.gpu_id), get_or_empty(info.gpu_vendor),
                       info.cpu_cores, get_or_empty(info.system_memory));
}

} // namespace utils
