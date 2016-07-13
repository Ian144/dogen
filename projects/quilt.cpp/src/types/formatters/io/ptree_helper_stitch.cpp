/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012-2015 Marco Craveiro <marco.craveiro@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 */
#include "dogen/quilt.cpp/types/properties/helper_properties.hpp"
#include "dogen/quilt.cpp/types/formatters/io/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/assistant.hpp"
#include "dogen/quilt.cpp/types/formatters/io/ptree_helper_stitch.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace formatters {
namespace io {

std::string ptree_helper::family() const {
    static std::string r("PTree");
    return r;
}

std::list<std::string> ptree_helper::owning_formatters() const {
    static auto r(std::list<std::string> {
        traits::class_implementation_formatter_name()
    });
    return r;
}

bool ptree_helper::requires_explicit_call() const {
    return false;
}

std::string ptree_helper::function_name() const {
    static std::string r("operator==");
    return r;
}

bool ptree_helper::is_enabled(const assistant& /*a*/,
    const properties::helper_properties& /*hp*/) const {
    return true;
}

void ptree_helper::
format(assistant& a, const properties::helper_properties& hp) const {
    {
        const auto d(hp.current());
        const auto nt_qn(d.name_tree_qualified());
        auto snf(a.make_scoped_namespace_formatter(d.namespaces()));
a.stream() << std::endl;
a.stream() << "inline std::ostream& operator<<(std::ostream& s, const " << nt_qn << "& v) {" << std::endl;
a.stream() << "    std::ostringstream ss;" << std::endl;
a.stream() << "    boost::property_tree::write_json(ss, v);" << std::endl;
a.stream() << std::endl;
a.stream() << "    std::string content(ss.str());" << std::endl;
a.stream() << "    boost::replace_all(content, \"\\r\\n\", \"\");" << std::endl;
a.stream() << "    boost::replace_all(content, \"\\n\", \"\");" << std::endl;
a.stream() << std::endl;
a.stream() << "    s << content;" << std::endl;
a.stream() << "    return s;" << std::endl;
a.stream() << "}" << std::endl;
a.stream() << std::endl;
    }
a.stream() << std::endl;
}

void ptree_helper_stitch(
    nested_type_formatting_assistant& a,
    const properties::nested_type_info& t) {

    {
        auto snf(a.make_scoped_namespace_formatter(t.namespaces()));
a.stream() << std::endl;
a.stream() << "inline std::ostream& operator<<(std::ostream& s, const " << t.complete_name() << "& v) {" << std::endl;
a.stream() << "    std::ostringstream ss;" << std::endl;
a.stream() << "    boost::property_tree::write_json(ss, v);" << std::endl;
a.stream() << std::endl;
a.stream() << "    std::string content(ss.str());" << std::endl;
a.stream() << "    boost::replace_all(content, \"\\r\\n\", \"\");" << std::endl;
a.stream() << "    boost::replace_all(content, \"\\n\", \"\");" << std::endl;
a.stream() << std::endl;
a.stream() << "    s << content;" << std::endl;
a.stream() << "    return s;" << std::endl;
a.stream() << "}" << std::endl;
a.stream() << std::endl;
    }
a.stream() << std::endl;
}
} } } } }
