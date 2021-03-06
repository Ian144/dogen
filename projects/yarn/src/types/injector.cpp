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
#include <memory>
#include <functional>
#include <boost/lexical_cast.hpp>
#include <boost/throw_exception.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/yarn/types/name_builder.hpp"
#include "dogen/yarn/types/intermediate_model.hpp"
#include "dogen/yarn/types/object.hpp"
#include "dogen/yarn/types/injection_error.hpp"
#include "dogen/yarn/types/injector.hpp"

using namespace dogen::utility::log;

namespace {

auto lg(logger_factory("yarn.injector"));

const std::string empty;
const std::string visitor_name("visitor");
const std::string visitor_argument_name("v");
const std::string visitor_doc("Visitor for ");
const std::string visit_operation_doc("Accept visits for type ");
const std::string global_module_doc("Module that represents the global scope.");
const std::string duplicate_name(
    "Attempt to add object with a name that already exists in model: ");
const std::string zero_leaves("Type marked as visitable but has no leaves: ");
const std::string leaf_not_found("Could not find leaf object: ");
const std::string model_already_has_global_module(
    "Found a global module in model: ");
const std::string no_visitees("Visitor is not visiting any types: ");

}

namespace dogen {
namespace yarn {

template<typename AssociativeContainerOfContainable>
inline void add_containing_module_to_non_contained_entities(
    const name& container_name, AssociativeContainerOfContainable& c) {
    for (auto& pair : c) {
        auto& s(pair.second);
        if (!s.contained_by())
            s.contained_by(container_name);
    }
}

module injector::create_global_module() const {
    module r;
    r.generation_type(generation_types::no_generation);
    r.origin_type(origin_types::system);
    r.documentation(global_module_doc);
    return r;
}

visitor injector::
create_visitor(const object& o, const std::list<name>& leaves) const {
    name_builder b;
    b.simple_name(o.name().simple() + "_" + visitor_name);
    b.location(o.name().location());

    const auto n(b.build());
    BOOST_LOG_SEV(lg, debug) << "Creating visitor: " << n.id();

    visitor r;
    r.name(n);
    r.generation_type(o.generation_type());
    r.origin_type(origin_types::system);
    r.documentation(visitor_doc + o.name().simple());

    if (leaves.empty()) {
        BOOST_LOG_SEV(lg, error) << no_visitees << n.id();
        BOOST_THROW_EXCEPTION(injection_error(no_visitees + n.id()));
    }

    for (const auto& l : leaves)
        r.visits().push_back(l);

    BOOST_LOG_SEV(lg, debug) << "Created visitor: " << n.id();
    return r;
}

void injector::inject_visitable_by(object& root, const std::list<name>& leaves,
    const name& visitor, intermediate_model& m) const {

    root.visitable_by(visitor);

    for (const auto& l : leaves) {
        auto i(m.objects().find(l.id()));
        if (i == m.objects().end()) {
            BOOST_LOG_SEV(lg, error) << leaf_not_found << l.id();
            BOOST_THROW_EXCEPTION(injection_error(leaf_not_found + l.id()));
        }

        auto& leaf(i->second);
        leaf.visitable_by(visitor);
    }
}

void injector::inject_visitors(intermediate_model& m) {
    BOOST_LOG_SEV(lg, debug) << "Injecting visitors.";

    std::list<visitor> visitors;
    for (auto& pair : m.objects()) {
        auto& o(pair.second);

        if (!o.is_visitable())
            continue;

        if (o.leaves().empty()) {
            const auto id(o.name().id());
            BOOST_LOG_SEV(lg, error) << zero_leaves << id;
            BOOST_THROW_EXCEPTION(injection_error(zero_leaves + id));
        }

        const auto v(create_visitor(o, o.leaves()));
        inject_visitable_by(o, o.leaves(), v.name(), m);
        visitors.push_back(v);
    }

    for (const auto v : visitors) {
        BOOST_LOG_SEV(lg, debug) << "Adding visitor: "
                                 << v.name().id();

        const auto pair(std::make_pair(v.name().id(), v));
        const auto i(m.visitors().insert(pair));
        if (!i.second) {
            const auto id(v.name().id());
            BOOST_LOG_SEV(lg, error) << duplicate_name << id;
            BOOST_THROW_EXCEPTION(injection_error(duplicate_name + id));
        }
    }

    BOOST_LOG_SEV(lg, debug) << "Done injecting visitors.";
}

void injector::inject_global_module(intermediate_model& m) {
    const auto gm(create_global_module());

    const auto gmn(gm.name());
    const auto i(m.modules().find(gmn.id()));
    if (i != m.modules().end()) {
        const auto id(m.name().id());
        BOOST_LOG_SEV(lg, error) << model_already_has_global_module << id;
        BOOST_THROW_EXCEPTION(
            injection_error(model_already_has_global_module + id));
    }
    m.modules().insert(std::make_pair(gmn.id(), gm));

    add_containing_module_to_non_contained_entities(gmn, m.modules());
    add_containing_module_to_non_contained_entities(gmn, m.concepts());
    add_containing_module_to_non_contained_entities(gmn, m.primitives());
    add_containing_module_to_non_contained_entities(gmn, m.enumerations());
    add_containing_module_to_non_contained_entities(gmn, m.objects());
    add_containing_module_to_non_contained_entities(gmn, m.exceptions());
    add_containing_module_to_non_contained_entities(gmn, m.visitors());
}

void injector::inject(intermediate_model& m) {
    inject_visitors(m);
    inject_global_module(m);
}

} }
