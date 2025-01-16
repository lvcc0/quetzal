#include "scene/main/resource.h"

namespace qtzl
{
	Resource::Resource(const std::string& name, const std::string& path)
		: Object(Object::Type::RESOURCE), m_Name(name), m_Path(path)
	{
	}

	void Resource::setName(const std::string& name)
	{
		this->m_Name = name;
	}

	void Resource::setPath(const std::string& path)
	{
		this->m_Path = path;
	}

	std::string Resource::getName() const
	{
		return this->m_Name;
	}

	std::string Resource::getPath() const
	{
		return this->m_Path;
	}
}