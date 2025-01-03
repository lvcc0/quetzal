#pragma once

#include <sstream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "billboards.h"

// TODO: rename some stuff here

class Light
{
public:
    Light(bool enabled, const std::string& name, glm::vec3 color,
          glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    bool m_Enabled;
    std::string m_Name;
    glm::vec3 m_Color;

    glm::vec3 m_Ambient;
    glm::vec3 m_Diffuse;
    glm::vec3 m_Specular;

    // Updates all corresponging fragment shader uniforms
    virtual void updateUni(std::shared_ptr<Shader>& shader, int index) const = 0;
};

class DirLight : public Light
{
public:
    DirLight(bool enabled, const std::string& name, glm::vec3 color, glm::vec3 dir,
             glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    glm::vec3 m_Direction;

    void updateUni(std::shared_ptr<Shader>& shader, int index) const override;
};

class PointLight : public Light
{
public:
    PointLight(bool enabled, const std::string& name, glm::vec3 color, glm::vec3 pos,
               glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
               float constant, float linear, float quad);

    bool m_DrawBillboard;

    glm::vec3 m_Position;

    float m_Constant;
    float m_Linear;
    float m_Quad;
    
    void updateUni(std::shared_ptr<Shader>& shader, int index) const override;

private:
    void enableBillboard();
};

class SpotLight : public Light
{
public:
    bool m_DrawBillboard;

    glm::vec3 m_Position;
    glm::vec3 m_Direction;

    float m_Constant;
    float m_Linear;
    float m_Quad;

    float m_InnerCutoff;
    float m_OuterCutoff;

    SpotLight(bool enabled, const std::string& name, glm::vec3 color, glm::vec3 pos, glm::vec3 dir,
              glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
              float constant, float linear, float quad,
              float inner_cutoff, float outer_cutoff);

    void updateUni(std::shared_ptr<Shader>& shader, int index) const override;

private:
    void enableBillboard();
};