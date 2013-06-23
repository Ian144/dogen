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
#include <sstream>
#include <boost/throw_exception.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/sml/types/merging_error.hpp"
#include "dogen/sml/io/qname_io.hpp"
#include "dogen/sml/io/nested_qname_io.hpp"
#include "dogen/sml/io/property_io.hpp"
#include "dogen/sml/io/pod_io.hpp"
#include "dogen/sml/io/model_io.hpp"
#include "dogen/sml/types/merger.hpp"

using namespace dogen::utility::log;

namespace {

auto lg(logger_factory("sml.merger"));

const std::string empty;
const std::string primitive_model_name("primitive_model");
const std::string msising_dependency("Cannot find target dependency: ");
const std::string missing_target("Target model not present");
}

namespace dogen {
namespace sml {

merger::merger() : has_target_(false), has_merged_(false) { }

void merger::check_qname(const std::string& model_name,
    const meta_types meta_type, const qname& key, const qname& value) const {
    if (key.model_name() != model_name) {
        std::ostringstream stream;
        stream << "Pod does not belong to this model. Model name: '"
               << model_name << "'. Pod qname: "
               << key;
        BOOST_LOG_SEV(lg, error) << stream.str();
        BOOST_THROW_EXCEPTION(merging_error(stream.str()));
    }

    if (key.meta_type() != meta_type) {
        std::ostringstream stream;
        stream << "Pod has incorrect meta_type: '" << key;
        BOOST_LOG_SEV(lg, error) << stream.str();
        BOOST_THROW_EXCEPTION(merging_error(stream.str()));
    }

    if (key != value) {
        std::ostringstream stream;
        stream << "Inconsistency between key and value qnames: "
               << " key: " << key << " value: " << value;
        BOOST_LOG_SEV(lg, error) << stream.str();
        BOOST_THROW_EXCEPTION(merging_error(stream.str()));
    }
}

void merger::check_references() const {
    for (const auto pair : merged_model_.dependencies()) {
        const auto mn(pair.second.model_name());
        const auto i(models_.find(mn));
        if (i == models_.end()) {
            BOOST_LOG_SEV(lg, error) << msising_dependency << mn;
            BOOST_THROW_EXCEPTION(merging_error(msising_dependency + mn));
        }
    }
}

void merger::require_not_has_target(const std::string& name) const {
    if (!has_target())
        return;

    std::ostringstream stream;
    stream << "Only one target expected. Last target model name: '"
           << merged_model_.name() << "'. New target model name: "
           << name;

    BOOST_LOG_SEV(lg, error) << stream.str();
    BOOST_THROW_EXCEPTION(merging_error(stream.str()));
}

void merger::require_has_target() const {
    if (has_target())
        return;

    BOOST_LOG_SEV(lg, error) << missing_target;
    BOOST_THROW_EXCEPTION(merging_error(missing_target));
}

void merger::require_not_has_merged() const {
    if (!has_merged())
        return;

    std::ostringstream stream;
    stream << "Attempt to merge more than once.";

    BOOST_LOG_SEV(lg, error) << stream.str();
    BOOST_THROW_EXCEPTION(merging_error(stream.str()));
}

void merger::add_target(const model& target) {
    require_not_has_target(target.name());

    has_target_ = true;
    merged_model_.name(target.name());
    merged_model_.documentation(target.documentation());
    merged_model_.leaves(target.leaves());
    merged_model_.modules(target.modules());
    merged_model_.external_module_path(target.external_module_path());
    merged_model_.dependencies(target.dependencies());

    add(target);
    BOOST_LOG_SEV(lg, debug) << "added target model: " << target.name();
}

void merger::add(const model& m) {
    require_not_has_merged();

    BOOST_LOG_SEV(lg, debug) << "adding model: " << m.name();
    BOOST_LOG_SEV(lg, debug) << "contents: " << m;
    models_.insert(std::make_pair(m.name(), m));
}

model merger::merge() {
    require_has_target();
    require_not_has_merged();
    check_references();

    for (const auto& pair : models_) {
        const auto& m(pair.second);
        const auto n(m.name());
        BOOST_LOG_SEV(lg, info) << "Merging model: '" << m.name()
                                << "' pods: " << m.pods().size()
                                << " primitives: " << m.primitives().size()
                                << " enumerations: " << m.enumerations().size()
                                << " exceptions: " << m.exceptions().size();

        for (const auto& p : m.pods()) {
            check_qname(n, meta_types::pod, p.first, p.second.name());
            merged_model_.pods().insert(p);
        }

        for (const auto& p : m.primitives()) {
            // FIXME: mega hack to handle primitive model.
            check_qname(
                (n == primitive_model_name ? empty : n),
                meta_types::primitive, p.first, p.second.name());
            merged_model_.primitives().insert(p);
        }

        for (const auto& p : m.enumerations()) {
            check_qname(n, meta_types::enumeration, p.first, p.second.name());
            merged_model_.enumerations().insert(p);
        }

        for (const auto& p : m.exceptions()) {
            check_qname(n, meta_types::exception, p.first, p.second.name());
            merged_model_.exceptions().insert(p);
        }
    }

    has_merged_ = true;
    return merged_model_;
}

} }
