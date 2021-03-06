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
#include <boost/graph/depth_first_search.hpp>
#include "dogen/dia/types/diagram.hpp"
#include "dogen/yarn/types/intermediate_model.hpp"
#include "dogen/yarn/io/intermediate_model_io.hpp"
#include "dogen/yarn/types/name_factory.hpp"
#include "dogen/utility/log/logger.hpp"
#include "dogen/yarn.dia/types/grapher.hpp"
#include "dogen/yarn.dia/types/context.hpp"
#include "dogen/yarn.dia/types/visitor.hpp"
#include "dogen/yarn.dia/types/transformer.hpp"
#include "dogen/yarn.dia/types/object_processor.hpp"
#include "dogen/yarn.dia/types/workflow.hpp"

namespace {

using namespace dogen::utility::log;
static logger lg(logger_factory("yarn.dia.workflow"));

}

namespace dogen {
namespace yarn {
namespace dia {

workflow::workflow(const dynamic::workflow& w) : dynamic_workflow_(w) { }

yarn::name
workflow::create_name_for_model(const std::string& model_name,
    const std::string& external_modules) const {

    yarn::name_factory nf;
    return nf.build_model_name(model_name, external_modules);
}

yarn::module workflow::create_module_for_model(const yarn::name& n,
    const bool is_target) const {

    yarn::module r;
    r.name(n);
    r.origin_type(yarn::origin_types::user);
    r.generation_type(is_target ?
        yarn::generation_types::full_generation :
        yarn::generation_types::no_generation);
    return r;
}

void workflow::initialise_context_activity(const std::string& model_name,
    const std::string& external_modules, bool is_target) {

    context_ = context();
    auto& m(context_.model());
    m.name(create_name_for_model(model_name, external_modules));
    m.original_model_name(model_name);
    BOOST_LOG_SEV(lg, debug) << "Model: " << m.name().id();

    m.origin_type(yarn::origin_types::user);
    m.is_target(is_target);

    const auto mm(create_module_for_model(m.name(), is_target));
    m.modules().insert(std::make_pair(mm.name().id(), mm));
}

graph_type workflow::
generate_graph_activity(const dogen::dia::diagram& diagram) {
    grapher g;
    object_processor op;
    for (const auto& l : diagram.layers()) {
        for (const auto& o : l.objects()) {
            const auto po(op.process(o));
            if (g.is_relevant(po))
                g.add(po);
        }
    }

    g.generate();
    context_.child_id_to_parent_ids(g.child_id_to_parent_ids());
    return g.graph();
}

void workflow::transformation_activity(const processed_object& o) {
    auto p(profiler_.generate(o));

    bool nothing_set(!p.is_enumeration() && !p.is_exception() &&
        !p.is_value_object() && !p.is_service() && !p.is_concept());
    if (p.is_uml_class() && nothing_set)
        p.is_value_object(true);

    validator_.validate(p);
    transformer_->transform(o, p);
}

void workflow::graph_to_context_activity(const graph_type& g) {
    transformer_ = std::unique_ptr<transformer>(
        new transformer(dynamic_workflow_, context_));
    const auto f(std::bind(&workflow::transformation_activity, this,
            std::placeholders::_1));
    visitor v(f);
    boost::depth_first_search(g, boost::visitor(v));
}

yarn::intermediate_model workflow::execute(const dogen::dia::diagram& diagram,
    const std::string& model_name, const std::string& external_modules,
    bool is_target) {

    initialise_context_activity(model_name, external_modules, is_target);
    graph_to_context_activity(generate_graph_activity(diagram));

    BOOST_LOG_SEV(lg, debug) << "Final model: " << context_.model();
    return context_.model();
}

} } }
