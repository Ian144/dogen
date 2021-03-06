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
#include <boost/throw_exception.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/utility/io/list_io.hpp"
#include "dogen/utility/string/splitter.hpp"
#include "dogen/yarn/io/location_io.hpp"
#include "dogen/yarn/types/languages.hpp"
#include "dogen/yarn/types/separators.hpp"
#include "dogen/yarn/types/building_error.hpp"
#include "dogen/yarn/types/string_processor.hpp"
#include "dogen/yarn/types/pretty_printer.hpp"
#include "dogen/yarn/types/name_builder.hpp"

namespace {

using namespace dogen::utility::log;
auto lg(logger_factory("yarn.name_builder"));

const std::string dot(".");
const std::string empty_type_name("Type name is empty.");
const std::string empty_model_name("Model name is empty.");
const std::string empty_internal_modules("Internal modules are empty.");
const std::string empty_external_modules("External modules are empty.");
const std::string unexpected_simple_name("Simple name is not expected.");

}

namespace dogen {
namespace yarn {

name_builder::name_builder(const bool model_name_mode)
    : model_name_mode_(model_name_mode) { }

name_builder::name_builder(const name& n, const bool model_name_mode)
    : model_name_mode_(model_name_mode), name_(n) { }

std::string name_builder::compute_id() {
    pretty_printer pp;
    pp.add(name_, model_name_mode_);
    const auto r(pp.print());
    BOOST_LOG_SEV(lg, debug) << "Computed id: " << r;
    return r;
}

std::string name_builder::compute_qualified_for_cpp() {
    pretty_printer pp(separators::double_colons);
    pp.add(name_, model_name_mode_);
    const auto r(pp.print());
    BOOST_LOG_SEV(lg, debug) << "Computed qualified: " << r;
    return r;
}

std::string name_builder::compute_identifiable(const std::string& qualified) {
    string_processor sp;
    const auto r(sp.to_identifiable(qualified));
    BOOST_LOG_SEV(lg, debug) << "Computed identifiable: " << r;
    return r;
}

void name_builder::setup_computed_properties() {
    name_.id(compute_id());
    const auto q(compute_qualified_for_cpp());
    name_.qualified()[languages::cpp] = q;
    name_.identifiable(compute_identifiable(q));
}

name_builder& name_builder::simple_name(const std::string& sn) {
    if (model_name_mode_) {
        BOOST_LOG_SEV(lg, error) << unexpected_simple_name;
        BOOST_THROW_EXCEPTION(building_error(unexpected_simple_name));
    }

    name_.simple(sn);
    BOOST_LOG_SEV(lg, debug) << "Added simple name: " << sn;
    return *this;
}

name_builder& name_builder::model_name(const std::string& mn) {
    if (mn.empty()) {
        BOOST_LOG_SEV(lg, error) << empty_model_name;
        BOOST_THROW_EXCEPTION(building_error(empty_model_name));
    }

    using utility::string::splitter;
    name_.location().model_modules(splitter::split_scoped(mn, dot));

    if (model_name_mode_)
        name_.simple(*name_.location().model_modules().rbegin());

    BOOST_LOG_SEV(lg, debug) << "Added model name: " << mn;
    return *this;
}

name_builder& name_builder::model_name(const yarn::location& l) {
    name_.location().model_modules(l.model_modules());

    BOOST_LOG_SEV(lg, debug) << "Added model name from location: " << l;
    return *this;
}

name_builder& name_builder::
model_modules(const std::list<std::string>& mmp) {
    name_.location().model_modules(mmp);
    BOOST_LOG_SEV(lg, debug) << "Added model module path: " << mmp;
    return *this;
}

name_builder& name_builder::internal_modules(const std::string& imp) {
    if (imp.empty()) {
        BOOST_LOG_SEV(lg, error) << empty_internal_modules;
        BOOST_THROW_EXCEPTION(building_error(empty_internal_modules));
    }

    using utility::string::splitter;
    name_.location().internal_modules(splitter::split_scoped(imp));
    BOOST_LOG_SEV(lg, debug) << "Added internal model path: " << imp;
    return *this;
}

name_builder& name_builder::internal_modules(
    const std::list<std::string>& imp) {
    name_.location().internal_modules(imp);
    BOOST_LOG_SEV(lg, debug) << "Added external model path: " << imp;
    return *this;
}

name_builder& name_builder::external_modules(const std::string& emp) {
    if (emp.empty()) {
        BOOST_LOG_SEV(lg, error) << empty_external_modules;
        BOOST_THROW_EXCEPTION(building_error(empty_external_modules));
    }

    using utility::string::splitter;
    name_.location().external_modules(splitter::split_scoped(emp));
    BOOST_LOG_SEV(lg, debug) << "Added external model path: " << emp;
    return *this;
}

name_builder& name_builder::external_modules(
    const std::list<std::string>& emp) {
    name_.location().external_modules(emp);
    BOOST_LOG_SEV(lg, debug) << "Added external model path: " << emp;
    return *this;
}

name_builder& name_builder::location(const yarn::location& l) {
    BOOST_LOG_SEV(lg, debug) << "Added location: " << l;
    name_.location(l);
    return *this;
}

name name_builder::build() {
    setup_computed_properties();
    return name_;
}

} }
