#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct VertexData {
    glm::vec3 position;
    glm::vec3 normal;
    unsigned int texture_index;
    glm::vec2 texture_coordinates;
};

struct Wall {
    bool exists;
    unsigned int texture_index;
    glm::vec3 normal;
};

struct PointLight {
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;
};

struct LevelBulletHole {
    glm::vec3 position;
    glm::vec3 normal;
};

struct EnemySpawn {
    glm::vec3 position;
    glm::vec2 direction;
};

struct Sector {
    std::vector<glm::vec2> vertices;
    float floor_y;
    float ceiling_y;
    unsigned int floor_texture_index;
    unsigned int ceiling_texture_index;

    std::vector<Wall> walls;
    glm::vec2 aabb_top_left;
    glm::vec2 aabb_bot_right;
    glm::vec4 aabb[8];

    bool has_generated_buffers;
    unsigned int vao, vbo;
    unsigned int vertex_data_size;

    std::vector<LevelBulletHole> bullet_holes;

    Sector();
    ~Sector();
    void add_vertex(const glm::vec2 vertex, unsigned int texture_index, bool wall_exists);
    void init_buffers(unsigned int index);
    void render();
};

struct Frustum {
    glm::vec4 plane[6];
    Frustum(const glm::mat4& projection_view_transpose);
    bool is_inside(const Sector& sector) const;
};

extern std::vector<Sector> sectors;
extern std::vector<PointLight> lights;
extern glm::vec3 player_spawn_point;
extern std::vector<EnemySpawn> enemy_spawns;

void level_save_file();
void level_init(std::string path);
void level_init_sectors();
void level_move_and_slide(glm::vec3* position, glm::vec3* velocity, float delta);
void level_render(glm::mat4 view, glm::mat4 projection, glm::vec3 view_pos, glm::vec3 flashlight_direction, bool flashlight_on);
