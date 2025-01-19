#pragma once

#include "core/object.h"

namespace qtzl
{
    // Base class for stuff loaded by user
    class Resource : public Object
    {
    public:
        Resource(const std::string& name, const std::string& path);
        virtual ~Resource() = default;

        void setName(const std::string& name);
        void setPath(const std::string& path);

        std::string getName() const;
        std::string getPath() const;

    protected:
        std::string m_Name;
        std::string m_Path;
    };
}