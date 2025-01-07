#pragma once

// a big fucking TODO here

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
//#include "base_classes.h"
//// TODO: (?) make inherited BoxCollider and SphereCollider
//// TODO: make rotations
//
//
//// Parent for more complex collisions(pure virtual)
//class Collision: public Scene_Object, public Scene_Node, public Renderable
//{
//public:
//    Collision(const std::string name, const glm::vec3 begin_pos, const glm::vec3 rotation,
//        std::vector<Vertex> verts = std::vector<Vertex>{}, std::vector<unsigned int> indicies = std::vector<unsigned int>{});
//
//    // Copy constructor
//    Collision(const Collision& obj);
//
//    Collision(Collision&& obj) = delete;
//
//    virtual void draw(const Shaders_pack& shaders) final override;
//protected:
//    virtual void setupRender() override final;
//};
//
//class RectCollision : public Collision 
//{
//public:
//    RectCollision(const std::string name, const glm::vec3 begin_pos, const glm::vec3 rotation, glm::vec3 length);
//protected:
//    // In my vision it would be used for colliding
//    glm::vec3 m_LengthPos;
//    // Moving in world space (for matrix)
//    inline virtual void setPosition() override;
//    inline virtual void setScale() override;
//    inline virtual void setRotationDegrees(const glm::vec3 rotation) override;
//};