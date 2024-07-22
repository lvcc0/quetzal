#include "collision.h"

Collision::Collision(CollisionType type, glm::vec3 position, glm::vec3 size)
    : m_col_Type(type), m_col_Position(position), m_col_Size(size)
{ /* empty */ }

Collision::Collision(CollisionType type, glm::vec3 position, float radius)
    : m_col_Type(type), m_col_Position(position), m_col_Radius(radius)
{ /* empty */ }

Collision::Collision(const Collision& obj)
    : m_col_Type(obj.m_col_Type), m_col_Position(obj.m_col_Position), m_col_Size(obj.m_col_Size), m_col_Radius(obj.m_col_Radius)
{ /* empty */ }