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
#ifndef DOGEN_YARN_TYPES_ALL_ATTRIBUTES_EXPANDER_HPP
#define DOGEN_YARN_TYPES_ALL_ATTRIBUTES_EXPANDER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include <unordered_set>
#include "dogen/yarn/types/intermediate_model.hpp"

namespace dogen {
namespace yarn {

/**
 * @brief Information expander that specialises in indexing attributes
 * across the model.
 *
 * @section yarn_all_attributes_expander_0 Model requirements
 *
 * The expansion of local attributes and of concepts is expected to
 * have taken place.
 *
 * @section yarn_all_attributes_expander_1 Types of attributes
 *
 * There are three sets of attributes the expander is interested in:
 *
 * @li local attributes;
 * @li inherited attributes;
 * @li all attributes.
 *
 * Lets cover each of these in more details.
 *
 * @subsection yarn_all_attributes_expander_11 Local attributes
 *
 * On a natural, pre-expanded state, local attributes encompass only
 * the attributes defined on the class directly. The expander behaves
 * differently depending on whether it is indexing an object or a
 * concept.
 *
 * For objects, the job of the expander is to expand the local
 * attributes to include all attributes obtained by modeling
 * concepts. This is because we treat them no differently from
 * attributes defined in the class for purposes of code generation. As
 * we rely on concept expansion, all we need is the set of all
 * attributes of all concepts we model. Local attributes are useful to
 * define all member variables owned by a class.
 *
 * For concepts, local attributes are untouched. This is for two
 * reasons. First, because we rely on the local attributes of concepts
 * to process the local attributes of objects, as explained above, via
 * concept expansion; thus local attributes provide a quick way to
 * access all the attributes of all concepts one models. The second
 * reason is due to the representation of concepts in source code:
 * there is no requirement for a "meta-concept" - something from which
 * concepts get attributes from. There is nothing above concepts that
 * replicate the relationship we have between concepts and
 * objects. When concepts get expressed in source code, all we require
 * is the set of all attributes for that concept (see below).
 *
 * @subsection yarn_all_attributes_expander_12 Inherited attributes
 *
 * These provide an easy way to look-up which attributes one has
 * inherited and from whom. For objects, the inherited attributes are
 * the "all attribute" set of each parent (see below). They are useful
 * to call parent constructors and the like.
 *
 * For concepts, inherited attributes are not particularly useful. We
 * still index them by, arbitrarily, adding all the local attributes
 * of the parent. However, we still haven't found a good use for them.
 *
 * @subsection yarn_all_attributes_expander_13 All attributes
 *
 * The "all attribute" set contains every single attribute. It is the
 * sum all the local attributes with all of the inherited
 * attributes. For objects it is useful for full constructors. For
 * concepts it is effectively the full interface of the concept.
 *
 */
class all_attributes_expander {
private:
    /**
     * @brief Returns the object with the given name, or throws.
     */
    object& find_object(const name& n, intermediate_model& im);

    /**
     * @brief Returns the concept with the given name, or throws.
     */
    concept& find_concept(const name& n, intermediate_model& im);

private:
    /**
     * @brief Expands a specific object.
     */
    void expand_object(object& o, intermediate_model& im,
        std::unordered_set<std::string>& processed_ids);

    /**
     * @brief Expands all objects in the model.
     */
    void expand_objects(intermediate_model& im);

    /**
     * @brief Populates index information in a concept.
     */
    void expand_concept(concept& c, intermediate_model& im,
        std::unordered_set<std::string>& processed_ids);

    /**
     * @brief Indexes all concepts in the model.
     */
    void expand_concepts(intermediate_model& im);

public:
    /**
     * @brief Indexes the supplied model.
     */
    void expand(intermediate_model& im);
};

} }

#endif
