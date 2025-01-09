//#include "expanded_math.h"
//
//std::vector<glm::vec3> ExpMath::makeGlobalCoordsFromVertex(const std::vector<Vertex>& local_coords, const glm::mat4& model_matrix)
//{
//    std::vector<glm::vec3> return_verts;
//    for (auto& item : local_coords)
//    {
//        return_verts.push_back(model_matrix * glm::vec4(item.Position, 1.0f));
//    }
//    return return_verts;
//}
//
//std::vector<glm::vec3> ExpMath::makeGlobalCoordsFromLocal(const std::vector<glm::vec3>& local_coords, const glm::mat4& model_matrix)
//{
//    std::vector<glm::vec3> return_verts;
//    for (auto& item : local_coords)
//    {
//        return_verts.push_back(model_matrix * glm::vec4(item, 1.0f));
//    }
//    return return_verts;
//}
//
//glm::vec3 ExpMath::getGlobalCoordsFromScreen(GLfloat screen_x, GLfloat screen_y, GLfloat width, GLfloat height, glm::mat4 projection_matrix, glm::mat4 view_matrix)
//{
//    float x = (2.0f * screen_x) / width - 1.0f;
//    float y = 1.0f - (2.0f * screen_y) / height;
//
//    glm::vec4 ray_clip = glm::vec4(x, y, -1.0, 1.0);
//
//    glm::vec4 ray_eye = glm::inverse(projection_matrix) * ray_clip;
//    ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
//
//    glm::vec4 inter_res = glm::inverse(view_matrix) * ray_eye;
//    glm::vec3 ray_wor = glm::vec3(inter_res.x, inter_res.y, inter_res.z);
//    ray_wor = glm::normalize(ray_wor);
//
//    return ray_wor;
//}
//
//std::pair<glm::vec3, glm::vec3> ExpMath::makeAABB(const std::vector<Vertex>& vector)
//{
//    if (vector.size() == 0) {
//        std::cerr << "ERROR::vector had zero size\n";
//        __debugbreak();
//    }
//
//    const auto min_corner_x = std::min_element(vector.begin(), vector.end(), [](Vertex first, Vertex min) {if (first.Position.x <= min.Position.x) return true; return false; })->Position.x;
//    const auto min_corner_y = std::min_element(vector.begin(), vector.end(), [](Vertex first, Vertex min) {if (first.Position.y <= min.Position.y) return true; return false; })->Position.y;
//    const auto min_corner_z = std::min_element(vector.begin(), vector.end(), [](Vertex first, Vertex min) {if (first.Position.z <= min.Position.z) return true; return false; })->Position.z;
//    const auto max_corner_x = std::max_element(vector.begin(), vector.end(), [](Vertex max, Vertex first) {if (max.Position.x <= first.Position.x) return true; return false; })->Position.x;
//    const auto max_corner_y = std::max_element(vector.begin(), vector.end(), [](Vertex max, Vertex first) {if (max.Position.y <= first.Position.y) return true; return false; })->Position.y;
//    const auto max_corner_z = std::max_element(vector.begin(), vector.end(), [](Vertex max, Vertex first) {if (max.Position.z <= first.Position.z) return true; return false; })->Position.z;
//
//    return std::make_pair(glm::vec3(min_corner_x, min_corner_x, min_corner_z), glm::vec3(max_corner_x, max_corner_y, max_corner_z));
//}
//
//glm::mat4 ExpMath::makeSummarizeMat4(const glm::mat4 model_matrix, const glm::mat4 view_matrix, const glm::mat4 proj_matrix)
//{
//    return (proj_matrix * view_matrix * model_matrix);
//}
//
//std::vector<glm::vec3> ExpMath::returnPositionFromVertex(const std::vector<Vertex>& vertex_vector)
//{
//    std::vector<glm::vec3> return_verts;
//    for (const auto& item : vertex_vector)
//    {
//        return_verts.push_back(item.Position);
//    }
//    return return_verts;
//}
