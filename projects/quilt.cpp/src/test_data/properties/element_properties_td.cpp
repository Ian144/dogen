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
#include <sstream>
#include "dogen/formatters/test_data/file_properties_td.hpp"
#include "dogen/quilt.cpp/test_data/properties/aspect_properties_td.hpp"
#include "dogen/quilt.cpp/test_data/properties/helper_properties_td.hpp"
#include "dogen/quilt.cpp/test_data/properties/element_properties_td.hpp"
#include "dogen/quilt.cpp/test_data/properties/formatter_properties_td.hpp"

namespace {

dogen::formatters::file_properties
create_dogen_formatters_file_properties(const unsigned int position) {
    return dogen::formatters::file_properties_generator::create(position);
}

boost::optional<dogen::formatters::file_properties>
create_boost_optional_dogen_formatters_file_properties(unsigned int position) {
    boost::optional<dogen::formatters::file_properties> r(
        create_dogen_formatters_file_properties(position));
    return r;
}

std::string create_std_string(const unsigned int position) {
    std::ostringstream s;
    s << "a_string_" << position;
    return s.str();
}

dogen::quilt::cpp::properties::formatter_properties
create_dogen_quilt_cpp_properties_formatter_properties(const unsigned int position) {
    return dogen::quilt::cpp::properties::formatter_properties_generator::create(position);
}

std::unordered_map<std::string, dogen::quilt::cpp::properties::formatter_properties> create_std_unordered_map_std_string_dogen_quilt_cpp_properties_formatter_properties(unsigned int position) {
    std::unordered_map<std::string, dogen::quilt::cpp::properties::formatter_properties> r;
    for (unsigned int i(0); i < 4; ++i) {
        r.insert(std::make_pair(create_std_string(position + i), create_dogen_quilt_cpp_properties_formatter_properties(position + i)));
    }
    return r;
}

dogen::quilt::cpp::properties::helper_properties
create_dogen_quilt_cpp_properties_helper_properties(const unsigned int position) {
    return dogen::quilt::cpp::properties::helper_properties_generator::create(position);
}

std::list<dogen::quilt::cpp::properties::helper_properties> create_std_list_dogen_quilt_cpp_properties_helper_properties(unsigned int position) {
    std::list<dogen::quilt::cpp::properties::helper_properties> r;
    for (unsigned int i(0); i < 4; ++i) {
        r.push_back(create_dogen_quilt_cpp_properties_helper_properties(position + i));
    }
    return r;
}

dogen::quilt::cpp::properties::aspect_properties
create_dogen_quilt_cpp_properties_aspect_properties(const unsigned int position) {
    return dogen::quilt::cpp::properties::aspect_properties_generator::create(position);
}

}

namespace dogen {
namespace quilt {
namespace cpp {
namespace properties {

element_properties_generator::element_properties_generator() : position_(0) { }

void element_properties_generator::
populate(const unsigned int position, result_type& v) {
    v.file_properties(create_boost_optional_dogen_formatters_file_properties(position + 0));
    v.formatter_properties(create_std_unordered_map_std_string_dogen_quilt_cpp_properties_formatter_properties(position + 1));
    v.helper_properties(create_std_list_dogen_quilt_cpp_properties_helper_properties(position + 2));
    v.aspect_properties(create_dogen_quilt_cpp_properties_aspect_properties(position + 3));
}

element_properties_generator::result_type
element_properties_generator::create(const unsigned int position) {
    element_properties r;
    element_properties_generator::populate(position, r);
    return r;
}

element_properties_generator::result_type*
element_properties_generator::create_ptr(const unsigned int position) {
    element_properties* p = new element_properties();
    element_properties_generator::populate(position, *p);
    return p;
}

element_properties_generator::result_type
element_properties_generator::operator()() {
    return create(position_++);
}

} } } }
