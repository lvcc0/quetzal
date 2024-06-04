#ifndef LIGHTS_H
#define LIGHTS_H

#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.h"
#include "billboard.h"

class DirLight
{
public:
    bool m_enabled;
    const char* m_name;
    
    glm::vec3 m_dir;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;

    glm::vec3 m_color;

    // Constructor
    DirLight( bool enabled, const char* name, glm::vec3 dir,
              glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
              glm::vec3 color) :
        m_enabled(enabled), m_name(name), m_dir(dir),
        m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
        m_color(color) { /* empty */ }

    // Updates all corresponging fragment shader uniforms
    void UpdateUni(Shader& shader, int index);

    // (const char*) cast operator
    operator const char* () { return m_name; }
};

class PointLight
{
public:
    bool m_enabled;
    bool m_draw_billboard;

    const char* m_name;

    glm::vec3 m_pos;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;

    float m_constant;
    float m_linear;
    float m_quad;

    glm::vec3 m_color;

    // Constructor
    PointLight( bool enabled, const char* name, glm::vec3 pos, 
                glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
                float constant, float linear, float quad,
                glm::vec3 color ) :
        m_enabled(enabled), m_draw_billboard(true),
        m_name(name), m_pos(pos),
        m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
        m_constant(constant), m_linear(linear), m_quad(quad),
        m_color(color) { /* empty */ }

    // Updates all corresponging fragment shader uniforms
    void UpdateUni(Shader& shader, int index);

    // (const char*) cast operator
    operator const char* () { return m_name; }

private:
    void enableBillboard();
};

class SpotLight
{
public:
    bool m_enabled;
    bool m_draw_billboard;

    const char* m_name;

    glm::vec3 m_pos;
    glm::vec3 m_dir;

    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;

    float m_constant;
    float m_linear;
    float m_quad;

    float m_innerCutoff;
    float m_outerCutoff;

    glm::vec3 m_color;

    // Constructor
    SpotLight( bool enabled, const char* name, glm::vec3 pos, glm::vec3 dir,
               glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular,
               float constant, float linear, float quad,
               float innerCutoff, float outerCutoff,
               glm::vec3 color ) :
        m_enabled(enabled), m_draw_billboard(true),
        m_name(name), m_pos(pos), m_dir(dir),
        m_ambient(ambient), m_diffuse(diffuse), m_specular(specular),
        m_constant(constant), m_linear(linear), m_quad(quad),
        m_innerCutoff(innerCutoff), m_outerCutoff(outerCutoff),
        m_color(color) { /* empty */ }

    // Updates all corresponging fragment shader uniforms
    void UpdateUni(Shader& shader, int index);

    // (const char*) cast operator
    operator const char* () { return m_name; }

private:
    void enableBillboard();
};

#endif