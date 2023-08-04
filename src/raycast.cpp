#include "raycast.hpp"

#include <map>

std::vector<RaycastPlane> raycast_planes;

unsigned int raycast_add_plane(RaycastPlane plane) {
    raycast_planes.push_back(plane);

    return raycast_planes.size() - 1;
}

RaycastResult raycast_cast(glm::vec3 origin, glm::vec3 direction, float range, unsigned int ignore) {
    // using multimap so that intersect distances are sorted in order of shortest to furthest distance
    std::multimap<float, unsigned int> intersect_distances;
    for (unsigned int plane = 0; plane < raycast_planes.size(); plane++) {
        const RaycastPlane& raycast_plane = raycast_planes[plane];

        // if normal and direction are perpendicular, then ray is parallel to plane
        if (glm::dot(direction, raycast_plane.normal) == 0.0f) {
            continue;
        }

        float min_dist_from_origin = glm::dot(raycast_plane.normal, raycast_plane.a - origin);
        float intersect_distance = (min_dist_from_origin - glm::dot(origin, raycast_plane.normal)) / glm::dot(direction, raycast_plane.normal);

        // check that the plane is not "before" the ray origin
        if (intersect_distance < 0.0f) {
            continue;
        }

        if (intersect_distance <= range) {
            intersect_distances.insert(std::pair<float, unsigned int>(intersect_distance, plane));
        }
    }

    for (std::multimap<float, unsigned int>::iterator itr = intersect_distances.begin(); itr != intersect_distances.end(); ++itr) {
        const RaycastPlane& raycast_plane = raycast_planes[itr->second];
        glm::vec3 intersect_point = origin + (direction * itr->first);

        glm::vec3 b_minus_a = raycast_plane.b - raycast_plane.a;
        float a_dot_b_minus_a = glm::dot(raycast_plane.a, b_minus_a);
        float i_dot_b_minus_a = glm::dot(intersect_point, b_minus_a);
        float b_dot_b_minus_a = glm::dot(raycast_plane.b, b_minus_a);

        if (a_dot_b_minus_a > i_dot_b_minus_a || i_dot_b_minus_a > b_dot_b_minus_a) {
            continue;
        }

        glm::vec3 d_minus_a = raycast_plane.d - raycast_plane.a;
        float a_dot_d_minus_a = glm::dot(raycast_plane.a, d_minus_a);
        float i_dot_d_minus_a = glm::dot(intersect_point, d_minus_a);
        float d_dot_d_minus_a = glm::dot(raycast_plane.d, d_minus_a);

        if (a_dot_d_minus_a > i_dot_d_minus_a || i_dot_d_minus_a > d_dot_d_minus_a) {
            continue;
        }

        return {
            .hit = true,
            .point = intersect_point
        };
    }

    return {
        .hit = false,
        .point = glm::vec3(0.0f, 0.0f, 0.0f)
    };
}