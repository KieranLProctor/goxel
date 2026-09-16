#include "chunk_mesher.h"

namespace goxel::voxel
{

namespace
{

struct Face
{
    glm::vec3 normal;
    std::array<glm::vec3, 6> vertices;
};

// Unit-cube face vertices in block-local space [0,1]^3, wound CCW as seen from
// outside the block (matches OpenGL's default front-face winding + back-face culling).
constexpr std::array<Face, 6> k_faces = {
    {
     {{1.0f, 0.0f, 0.0f},
         {{{1.0f, 0.0f, 1.0f},
           {1.0f, 0.0f, 0.0f},
           {1.0f, 1.0f, 0.0f},
           {1.0f, 1.0f, 0.0f},
           {1.0f, 1.0f, 1.0f},
           {1.0f, 0.0f, 1.0f}}}},
     {{-1.0f, 0.0f, 0.0f},
         {{{0.0f, 0.0f, 0.0f},
           {0.0f, 0.0f, 1.0f},
           {0.0f, 1.0f, 1.0f},
           {0.0f, 1.0f, 1.0f},
           {0.0f, 1.0f, 0.0f},
           {0.0f, 0.0f, 0.0f}}}},
     {{0.0f, 1.0f, 0.0f},
         {{{0.0f, 1.0f, 0.0f},
           {0.0f, 1.0f, 1.0f},
           {1.0f, 1.0f, 1.0f},
           {1.0f, 1.0f, 1.0f},
           {1.0f, 1.0f, 0.0f},
           {0.0f, 1.0f, 0.0f}}}},
     {{0.0f, -1.0f, 0.0f},
         {{{0.0f, 0.0f, 0.0f},
           {1.0f, 0.0f, 0.0f},
           {1.0f, 0.0f, 1.0f},
           {1.0f, 0.0f, 1.0f},
           {0.0f, 0.0f, 1.0f},
           {0.0f, 0.0f, 0.0f}}}},
     {{0.0f, 0.0f, 1.0f},
         {{{0.0f, 0.0f, 1.0f},
           {1.0f, 0.0f, 1.0f},
           {1.0f, 1.0f, 1.0f},
           {1.0f, 1.0f, 1.0f},
           {0.0f, 1.0f, 1.0f},
           {0.0f, 0.0f, 1.0f}}}},
     {{0.0f, 0.0f, -1.0f},
         {{{1.0f, 0.0f, 0.0f},
           {0.0f, 0.0f, 0.0f},
           {0.0f, 1.0f, 0.0f},
           {0.0f, 1.0f, 0.0f},
           {1.0f, 1.0f, 0.0f},
           {1.0f, 0.0f, 0.0f}}}},
     }
};

} // namespace

auto build_chunk_mesh(const Chunk &chunk) -> std::vector<float>
{
    std::vector<float> vertices;

    for (int x = 0; x < S_CHUNK_SIZE; ++x)
    {
        for (int y = 0; y < S_CHUNK_SIZE; ++y)
        {
            for (int z = 0; z < S_CHUNK_SIZE; ++z)
            {
                const glm::vec3 position{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)};
                const Block block = chunk.get_block(position);
                const BlockProperties &properties = get_block_properties(block);

                if (!properties.is_solid)
                {
                    continue;
                }

                for (const Face &face : k_faces)
                {
                    const Block neighbour = chunk.get_block(position + face.normal);

                    if (get_block_properties(neighbour).is_solid)
                    {
                        continue;
                    }

                    for (const glm::vec3 &vertex : face.vertices)
                    {
                        const glm::vec3 world_vertex = position + vertex;

                        vertices.push_back(world_vertex.x);
                        vertices.push_back(world_vertex.y);
                        vertices.push_back(world_vertex.z);
                        vertices.push_back(properties.colour.x);
                        vertices.push_back(properties.colour.y);
                        vertices.push_back(properties.colour.z);
                    }
                }
            }
        }
    }

    return vertices;
}

} // namespace goxel::voxel
