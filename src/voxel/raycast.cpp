#include "raycast.h"

namespace goxel::voxel
{

namespace
{

auto axis_step(const float direction) -> int
{
    if (direction > 0.0f)
    {
        return 1;
    }

    return direction < 0.0f ? -1 : 0;
}

auto axis_t_delta(const float direction) -> float
{
    return direction == 0.0f ? std::numeric_limits<float>::infinity() : std::abs(1.0f / direction);
}

auto axis_t_max(const float origin, const int step, const float delta) -> float
{
    if (step == 0)
    {
        return std::numeric_limits<float>::infinity();
    }

    const float boundary = step > 0 ? std::floor(origin) + 1.0f : std::ceil(origin) - 1.0f;

    return std::abs(boundary - origin) * delta;
}

} // namespace

auto raycast(const Chunk &chunk, const glm::vec3 origin, const glm::vec3 direction, const float max_distance)
    -> std::optional<RaycastHit>
{
    glm::ivec3 current{glm::floor(origin)};
    glm::ivec3 previous = current;

    const glm::ivec3 step{axis_step(direction.x), axis_step(direction.y), axis_step(direction.z)};
    const glm::vec3 t_delta{axis_t_delta(direction.x), axis_t_delta(direction.y), axis_t_delta(direction.z)};

    glm::vec3 t_max{axis_t_max(origin.x, step.x, t_delta.x), axis_t_max(origin.y, step.y, t_delta.y),
                    axis_t_max(origin.z, step.z, t_delta.z)};

    float travelled = 0.0f;

    while (travelled < max_distance)
    {
        if (Chunk::in_bounds(glm::vec3(current)) && get_block_properties(chunk.get_block(glm::vec3(current))).is_solid)
        {
            return RaycastHit{current, previous};
        }

        previous = current;

        if (t_max.x < t_max.y && t_max.x < t_max.z)
        {
            current.x += step.x;
            travelled = t_max.x;
            t_max.x += t_delta.x;
        }
        else if (t_max.y < t_max.z)
        {
            current.y += step.y;
            travelled = t_max.y;
            t_max.y += t_delta.y;
        }
        else
        {
            current.z += step.z;
            travelled = t_max.z;
            t_max.z += t_delta.z;
        }
    }

    return std::nullopt;
}

} // namespace goxel::voxel
