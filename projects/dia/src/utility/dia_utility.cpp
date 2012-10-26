/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda <info@kitanda.co.uk>
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
#include "dogen/utility/exception/invalid_enum_value.hpp"
#include "dogen/dia/utility/dia_utility.hpp"

namespace {

const std::string uml_large_package("UML - LargePackage");
const std::string uml_class("UML - Class");
const std::string uml_generalization("UML - Generalization");
const std::string uml_association("UML - Association");
const std::string invalid_dia_type("Invalid or unsupported Dia type");

const std::string enumeration("enumeration");

}

namespace dogen {
namespace dia {
namespace utility {

object_types parse_object_type(const std::string& ot) {
    if (ot == uml_large_package)
        return object_types::uml_large_package;

    if (ot == uml_class)
        return object_types::uml_class;

    if (ot == uml_generalization)
        return object_types::uml_generalization;

    if (ot == uml_association)
        return object_types::uml_association;

    throw dogen::utility::exception::exception(invalid_dia_type);
}

stereotypes parse_stereotype(const std::string& st) {
    if (st == enumeration)
        return stereotypes::enumeration;

    throw dogen::utility::exception::exception(invalid_dia_type);
}

} } }
