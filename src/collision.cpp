#include "collision.h"

Collision::Collision(CollisionType type, glm::vec3 position, glm::vec3 size)
    : m_Type(type), m_Position(position), m_Size(size)
{ /* empty */ }

Collision::Collision(CollisionType type, glm::vec3 position, float radius)
    : m_Type(type), m_Position(position), m_Radius(radius)
{ /* empty */ }

Collision::Collision(const Collision &obj)
    : m_Type(obj.m_Type), m_Position(obj.m_Position), m_Size(obj.m_Size), m_Radius(obj.m_Radius)
{ /* empty */ }