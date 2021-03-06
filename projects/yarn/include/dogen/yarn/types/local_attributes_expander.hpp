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
#ifndef DOGEN_YARN_TYPES_LOCAL_ATTRIBUTES_EXPANDER_HPP
#define DOGEN_YARN_TYPES_LOCAL_ATTRIBUTES_EXPANDER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include "dogen/yarn/types/name_tree.hpp"
#include "dogen/yarn/types/name_tree_parser.hpp"
#include "dogen/yarn/types/intermediate_model.hpp"

namespace dogen {
namespace yarn {

/**
 * @brief Performs the expansion of all local attributes in the
 * supplied model.
 */
class local_attributes_expander {
private:
    /**
     * @brief Update all attributes in the supplied element.
     */
    void update_attributes(const name_tree_parser& ntp, const bool is_fluent,
        const bool is_immutable, std::list<attribute>& attrs) const;

private:
    /**
     * @brief Returns all of the top-level modules in the supplied
     * model.
     */
    std::unordered_set<std::string>
    obtain_top_level_module_names(const intermediate_model& m) const;

public:
    /**
     * Execute the property expansion against the model.
     */
    void expand(intermediate_model& m) const;
};

} }

#endif
