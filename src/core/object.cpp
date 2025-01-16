#include "core/object.h"

namespace qtzl
{
	Object::Object(Type type)
		: m_Type(type)
	{
	}

	void Object::setProperty(const std::string& property_name)
	{
		// TODO
	}

	std::map<std::string, std::variant<int, float, std::string>> Object::getProperties() const
	{
		return this->m_Properties;
	}

	Object::Type Object::getType() const
	{
		return this->m_Type;
	}

	bool Object::isType(Type type) const
	{
		return this->m_Type == type;
	}
}