//#pragma once
//
//#include <string>
//#include <iostream>
//#include <sstream>
//#include <fstream>
//#include <map>
//#include <vector>
//#include <algorithm>
//
//#include <glad/glad.h>
//#include <stb_image/stb_image.h>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtx/string_cast.hpp>
//
//#include "expanded_math.h"
//#include "collision.h"
//
//
//class Ray
//{
//public:
//    float vec_coef = 0.5;
//
//    glm::vec3 m_RayOrigin;
//    glm::vec3 m_RayDirection; // normalized
//
//    Ray(glm::vec3 m_ray_origin, glm::vec3 m_direction);
//
//    bool TestRayOBBIntersection(
//        glm::vec3 aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
//        glm::vec3 aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
//        glm::mat4 ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
//        float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
//    );
//};
//
//class CollidingRay : Ray
//{
//public:
//    using Ray::Ray;
//    bool checkCollision(const Collision& object);
//};