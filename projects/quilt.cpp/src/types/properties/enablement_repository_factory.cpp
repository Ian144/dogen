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
#include <boost/throw_exception.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/utility/io/unordered_map_io.hpp"
#include "dogen/dynamic/types/field_selector.hpp"
#include "dogen/dynamic/types/repository_selector.hpp"
#include "dogen/yarn/types/object.hpp"
#include "dogen/yarn/types/concept.hpp"
#include "dogen/yarn/types/primitive.hpp"
#include "dogen/yarn/types/enumeration.hpp"
#include "dogen/yarn/types/element_visitor.hpp"
#include "dogen/yarn/types/name_factory.hpp"
#include "dogen/quilt.cpp/types/traits.hpp"
#include "dogen/quilt.cpp/io/properties/enablement_repository_io.hpp"
#include "dogen/quilt.cpp/io/properties/global_enablement_properties_io.hpp"
#include "dogen/quilt.cpp/types/properties/enablement_factory.hpp"
#include "dogen/quilt.cpp/types/properties/enablement_repository_factory.hpp"

namespace {

using namespace dogen::utility::log;
static logger lg(logger_factory(
        "quilt.cpp.properties.enablement_repository_factory"));

const std::string registrar_name("registrar");
const std::string duplicate_name("Duplicate name: ");
const std::string model_module_not_found("Model module not found for model: ");

}

namespace dogen {
namespace quilt {
namespace cpp {
namespace properties {

namespace {

/**
 * @brief Generates all inclusion dependencies.
 */
class generator final : public yarn::element_visitor {
public:
    explicit generator(const enablement_factory& f) : factory_(f) {}

private:
    template<typename YarnEntity>
    void generate(const YarnEntity& e, const bool types_only = false) {
        const auto o(e.extensions());
        result_.by_name()[e.name()] = factory_.make(o, types_only);
    }

public:
    using yarn::element_visitor::visit;
    void visit(const dogen::yarn::module& m) { generate(m); }
    void visit(const dogen::yarn::concept& c) { generate(c); }
    void visit(const dogen::yarn::primitive& p) { generate(p); }
    void visit(const dogen::yarn::enumeration& e) { generate(e); }
    void visit(const dogen::yarn::object& o) {
        const auto is_service(o.object_type() ==
            yarn::object_types::user_defined_service);
        generate(o, is_service);
    }
    void visit(const dogen::yarn::exception& e) { generate(e); }
    void visit(const dogen::yarn::visitor& v) { generate(v); }

public:
    const enablement_repository& result() const { return result_; }

private:
    const enablement_factory& factory_;
    enablement_repository result_;
};

}

std::unordered_map<std::string,
                   enablement_repository_factory::field_definitions>
enablement_repository_factory::create_field_definitions(
    const dynamic::repository& rp,
    const formatters::container& fc) const {
    const dynamic::repository_selector s(rp);
    std::unordered_map<std::string, field_definitions> r;
    for (const auto& f : fc.all_file_formatters()) {
        const auto oh(f->ownership_hierarchy());

        field_definitions fd;
        const auto& mn(oh.model_name());
        fd.model_enabled = s.select_field_by_name(mn, traits::enabled());

        const auto& fctn(oh.facet_name());
        fd.facet_enabled = s.select_field_by_name(fctn, traits::enabled());

        const auto& fn(oh.formatter_name());
        fd.formatter_enabled = s.select_field_by_name(fn, traits::enabled());

        r[fn] = fd;
    }

    return r;
}

std::unordered_map<std::string, global_enablement_properties>
enablement_repository_factory::obtain_global_properties(
    const std::unordered_map<std::string, field_definitions>& field_definitions,
    const dynamic::object& root_object) const {
    std::unordered_map<std::string, global_enablement_properties> r;
    const dynamic::field_selector fs(root_object);
    for (const auto& pair : field_definitions) {
        const auto& fn(pair.first);
        const auto& fd(pair.second);

        global_enablement_properties gep;
        gep.model_enabled(fs.get_boolean_content_or_default(fd.model_enabled));
        gep.facet_enabled(fs.get_boolean_content_or_default(fd.facet_enabled));
        gep.formatter_enabled(
            fs.get_boolean_content_or_default(fd.formatter_enabled));

        r[fn] = gep;
    }

    BOOST_LOG_SEV(lg, debug) << "Global enablement properties: " << r;
    return r;
}

enablement_repository enablement_repository_factory::make(
    const dynamic::repository& rp,
    const dynamic::object& root_object,
    const formatters::container& fc,
    const yarn::model& m) const {

    BOOST_LOG_SEV(lg, debug) << "Started computing enablement.";

    const auto fd(create_field_definitions(rp, fc));
    const auto gep(obtain_global_properties(fd, root_object));
    const enablement_factory f(rp, fc, gep);
    generator g(f);
    for (const auto& pair : m.elements()) {
        const auto& e(*pair.second);
        e.accept(g);
    }
    auto r(g.result());

    yarn::name_factory nf;
    const auto n(nf.build_element_in_model(m.name(), registrar_name));
    const auto e(f.make(root_object));
    r.by_name()[n] = e;

    for (const auto& pair : m.references()) {
        const auto origin_type(pair.second);
        if (origin_type == yarn::origin_types::system)
            continue;

        const auto ref(pair.first);
        yarn::name_factory nf;
        const auto n(nf.build_element_in_model(ref, registrar_name));
        r.by_name()[n] = e;
    }

    BOOST_LOG_SEV(lg, debug) << "Finished computing enablement:" << r;
    return r;
}

} } } }
