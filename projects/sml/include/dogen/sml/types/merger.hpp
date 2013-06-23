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
#ifndef DOGEN_SML_TYPES_MODELING_MERGER_HPP
#define DOGEN_SML_TYPES_MODELING_MERGER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <list>
#include <vector>
#include <string>
#include <unordered_map>
#include "dogen/sml/types/nested_qname.hpp"
#include "dogen/sml/types/model.hpp"

namespace dogen {
namespace sml {

/**
 * @brief Combines a number of models into a merged model.
 */
class merger {
private:
    typedef std::unordered_map<std::string, model> models_type;

public:
    merger(const merger&) = default;
    ~merger() = default;
    merger(merger&&) = default;
    merger& operator=(const merger&) = default;

public:
    merger();

private:
    /**
     * @brief Validates the qname.
     */
    void check_qname(const std::string& model_name,
        const meta_types meta_type, const qname& key,
        const qname& value) const;

    /**
     * @brief Ensure that all known references have been added.
     */
    void check_references() const;

    /**
     * @brief Target model must have been set.
     */
    void require_has_target() const;

    /**
     * @brief Target model must @e not have been set.
     */
    void require_not_has_target(const std::string& name) const;

    /**
     * @brief Merge must not yet have taken place.
     */
    void require_not_has_merged() const;

public:
    /**
     * @brief Returns true if the target model has already been added,
     * false otherwise.
     */
    bool has_target() const { return has_target_; }

    /**
     * @brief Returns true if the target model has already been added,
     * false otherwise.
     */
    bool has_merged() const { return has_merged_; }

    /**
     * @brief Adds the target model.
     *
     * @pre Must not already have a target model.
     * @pre Merge mustn't have taken place already.
     */
    void add_target(const model& target);

    /**
     * @brief Adds a reference model.
     *
     * @pre Merge mustn't have taken place already.
     */
    void add(const model& m);

    /**
     * @brief Combines all models into a merged model.
     *
     * @pre Merge mustn't have taken place already.
     */
    model merge();

private:
    models_type models_;
    model merged_model_;
    bool has_target_;
    bool has_merged_;
};

} }

#endif
