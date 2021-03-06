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
#include <array>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "dogen/utility/test/logging.hpp"
#include "dogen/utility/test/asserter.hpp"
#include "dogen/utility/test/exception_checkers.hpp"
#include "dogen/yarn/types/intermediate_model.hpp"
#include "dogen/yarn/io/intermediate_model_io.hpp"
#include "dogen/yarn/types/object.hpp"
#include "dogen/yarn/io/object_io.hpp"
#include "dogen/yarn/types/injection_error.hpp"
#include "dogen/yarn/test/mock_intermediate_model_factory.hpp"
#include "dogen/yarn/types/injector.hpp"

using dogen::yarn::object_types;

namespace {

using dogen::yarn::test::mock_intermediate_model_factory;
const mock_intermediate_model_factory::flags flags;
const mock_intermediate_model_factory factory(flags);

const std::string test_module("yarn");
const std::string test_suite("injector_tests");
const std::string version_name("version");
const std::string missing_identity("Identity must have at least");
const std::string no_leaves("Type marked as visitable but has no leaves");

}

using dogen::utility::test::contains_checker;
using dogen::yarn::injection_error;
using dogen::utility::test::asserter;

BOOST_AUTO_TEST_SUITE(injector_tests)

BOOST_AUTO_TEST_CASE(single_type_model_results_in_adding_only_global_module) {
    SETUP_TEST_LOG_SOURCE("single_type_model_results_in_adding_only_global_module");

    auto a(factory.make_single_type_model());
    BOOST_REQUIRE(a.objects().size() == 1);
    BOOST_CHECK(!a.objects().begin()->second.contained_by());
    BOOST_REQUIRE(a.modules().empty());
    BOOST_REQUIRE(a.primitives().empty());
    BOOST_REQUIRE(a.enumerations().empty());
    BOOST_REQUIRE(a.concepts().empty());

    dogen::yarn::injector i;
    i.inject(a);

    BOOST_CHECK(a.objects().size() == 1);
    BOOST_CHECK(a.modules().size() == 1);

    const auto n(a.modules().begin()->second.name());
    BOOST_REQUIRE(a.objects().begin()->second.contained_by());
    BOOST_REQUIRE(*a.objects().begin()->second.contained_by() == n);
    BOOST_CHECK(a.primitives().empty());
    BOOST_CHECK(a.enumerations().empty());
    BOOST_CHECK(a.concepts().empty());
}

BOOST_AUTO_TEST_CASE(visitable_object_with_no_leaves_throws) {
    SETUP_TEST_LOG_SOURCE("visitable_object_with_no_leaves_throws");

    auto m(factory.make_single_type_model());
    BOOST_REQUIRE(m.objects().size() == 1);
    auto& ob(m.objects().begin()->second);
    ob.is_visitable(true);
    BOOST_LOG_SEV(lg, debug) << "model: " << m;

    dogen::yarn::injector i;
    contains_checker<injection_error> c(no_leaves);
    BOOST_CHECK_EXCEPTION(i.inject(m), injection_error, c);
}

BOOST_AUTO_TEST_CASE(visitable_object_has_visitor_injected) {
    SETUP_TEST_LOG_SOURCE("visitable_object_has_visitor_injected");

    auto m(factory.object_with_parent_in_the_same_model());
    BOOST_REQUIRE(m.objects().size() == 2);
    for (auto& pair : m.objects()) {
        const auto& n(pair.second.name());
        if (factory.is_type_name_n(1, n)) {
            auto& o(pair.second);
            BOOST_LOG_SEV(lg, debug) << "found object: " << n.id();
            o.is_visitable(true);
        }
    }
    BOOST_LOG_SEV(lg, debug) << "before: " << m;

    dogen::yarn::injector i;
    i.inject(m);
    BOOST_LOG_SEV(lg, debug) << "after: " << m;

    BOOST_CHECK(m.objects().size() == 2);
    bool type_one(false);
    for (const auto& pair : m.objects()) {
        const auto& n(pair.second.name());
        if (factory.is_type_name_n(1, n)) {
            BOOST_LOG_SEV(lg, debug) << "found object: " << n.id();
            type_one = true;
        }
    }
    BOOST_CHECK(type_one);

    BOOST_REQUIRE(m.visitors().size() == 1);
    const auto v(m.visitors().begin()->second);
    BOOST_LOG_SEV(lg, debug) << "found visitor: " << v.name().id();
    BOOST_CHECK(factory.is_type_name_n_visitor(1, v.name()));
    BOOST_REQUIRE(v.visits().size() == 1);
    BOOST_CHECK(factory.is_type_name_n(0, v.visits().front()));
}

BOOST_AUTO_TEST_SUITE_END()
