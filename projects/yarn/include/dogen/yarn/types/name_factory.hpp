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
#ifndef DOGEN_YARN_TYPES_NAME_FACTORY_HPP
#define DOGEN_YARN_TYPES_NAME_FACTORY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include "dogen/yarn/types/name.hpp"

namespace dogen {
namespace yarn {

/**
 * @brief Creates names with all the fields filled in.
 */
class name_factory {
public:
    /*
     * @brief Given a string with a model name, possibly using a
     * separator, and a string with an external module path, also
     * possibly using a separator; parses both strings and generates a
     * combined name.
     *
     * @pre @e model_name must not be empty.
     */
    name build_model_name(const std::string& model_name,
        const std::string& external_modules = std::string()) const;

    /**
     * @brief Given the components of a name for an element, creates
     * the element name.
     */
    /**@{*/
    name build_element_name(const std::string& simple_name) const;
    name build_element_name(const std::string& model_name,
        const std::string& simple_name) const;
    /**@}*/

    /**
     * @brief Given a simple name for an element and the name of the
     * model it is located in, creates the element name.
     */
    /**@{*/
    name build_element_in_model(const name& model_name,
        const std::string& simple_name) const;
    name build_element_in_model(const name& model_name,
        const std::string& simple_name,
        const std::list<std::string>& internal_modules) const;
    /**@}*/

    /**
     * @brief Given a simple name for an element and the name of the
     * module it is located in, creates the element name.
     */
    name build_element_in_module(const name& module_name,
        const std::string& simple_name) const;

    /**
     * @brief Builds a module name, given the model name and an
     * internal module path. Uses the back of the module path to name
     * the module.
     */
    /**@{*/
    name build_module_name(const name& model_name,
        const std::list<std::string>& internal_modules) const;
    name build_module_name(const name& model_name,
        const std::string& module_name,
        const std::list<std::string>& internal_modules) const;
    /**@}*/

    /**
     * @brief Combines element name with model name.
     */
    name build_combined_element_name(const name& model_name,
        const name& partial_element_name,
        const bool populate_model_name_if_blank = false) const;

    /**
     * @brief Promotes the first name of the internal module path to
     * become the model name.
     */
    name build_promoted_module_name(const name& model_name,
        const name& element_name) const;

    /**
     * @brief Builds an attribute name.
     */
    name build_attribute_name(const name& owner_name,
        const std::string& simple_name) const;
};

} }

#endif
