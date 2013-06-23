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
#include <array>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "dogen/utility/test/logging.hpp"
#include "dogen/sml/types/all.hpp"
#include "dogen/sml/io/all_io.hpp"
#include "dogen/utility/test/equality_tester.hpp"
#include "dogen/sml/types/model.hpp"
#include "dogen/sml/types/merging_error.hpp"
#include "dogen/sml/types/merger.hpp"
#include "dogen/utility/test/exception_checkers.hpp"
#include "dogen/sml/test/mock_model_factory.hpp"

using dogen::sml::test::mock_model_factory;

namespace {

const std::string test_module("sml");
const std::string test_suite("merger_spec");

const std::string invalid_type_name("INVALID");
const std::string invalid_model_name("INVALID");

const std::string incorrect_model("Pod does not belong to this model");
const std::string double_merging("Attempt to merge more than once");
const std::string inconsistent_kvp("Inconsistency between key and value");
const std::string missing_target("Target model not present");
const std::string too_many_targets("Only one target expected.");
const std::string incorrect_meta_type("Pod has incorrect meta_type");

}

using dogen::utility::test::contains_checker;
using dogen::sml::merging_error;

BOOST_AUTO_TEST_SUITE(merger)

BOOST_AUTO_TEST_CASE(merging_n_distinct_models_with_one_pod_each_results_in_n_pods_in_merged_model) {
    SETUP_TEST_LOG_SOURCE("merging_n_distinct_models_with_one_pod_each_results_in_n_pods_in_merged_model");
    const unsigned int n(5);
    dogen::sml::merger mg;
    for (unsigned int i(0); i < n; ++i) {
        const auto m(mock_model_factory::build_single_type_model(i));
        i == 0 ? mg.add_target(m) : mg.add(m);
    }

    BOOST_CHECK(!mg.has_merged());
    const auto combined(mg.merge());
    BOOST_CHECK(mg.has_merged());
    BOOST_CHECK(combined.pods().size() == n);
    BOOST_CHECK(combined.primitives().empty());

    std::set<std::string> pod_names;
    std::set<std::string> model_names;
    for (const auto& pair : combined.pods()) {
        const auto& qn(pair.first);
        pod_names.insert(qn.model_name() + "_" + qn.type_name());
        model_names.insert(qn.model_name());
    }

    BOOST_REQUIRE(pod_names.size() == n);
    BOOST_REQUIRE(model_names.size() == n);

    auto pod_i(pod_names.cbegin());
    auto model_i(model_names.cbegin());
    for (unsigned int i(0); i < n; ++i) {
        BOOST_REQUIRE(pod_i != pod_names.cend());
        BOOST_REQUIRE(model_i != model_names.cend());

        BOOST_LOG_SEV(lg, debug) << "pod name: " << *pod_i;
        BOOST_LOG_SEV(lg, debug) << "model name: " << *model_i;

        const auto expected_model_name(mock_model_factory::model_name(i));
        const auto expected_pod_name(mock_model_factory::type_name(0));
        BOOST_LOG_SEV(lg, debug) << "expected pod name: "
                                 << expected_pod_name;
        BOOST_LOG_SEV(lg, debug) << "expected model name: "
                                 << expected_model_name;

        BOOST_CHECK(boost::ends_with(*pod_i, expected_pod_name));
        BOOST_CHECK(boost::starts_with(*pod_i, expected_model_name));
        BOOST_CHECK(*model_i == expected_model_name);
        ++pod_i;
        ++model_i;
    }
}

BOOST_AUTO_TEST_CASE(merging_empty_model_results_in_empty_merged_model) {
    SETUP_TEST_LOG("merging_empty_model_results_in_empty_merged_model");
    dogen::sml::merger mg;
    dogen::sml::model m;
    BOOST_CHECK(!mg.has_target());
    mg.add_target(m);
    BOOST_CHECK(mg.has_target());

    const auto combined(mg.merge());
    BOOST_CHECK(combined.pods().empty());
    BOOST_CHECK(combined.primitives().empty());
}

BOOST_AUTO_TEST_CASE(type_with_incorrect_model_name_throws) {
    SETUP_TEST_LOG("type_with_incorrect_model_name_throws");
    auto m(mock_model_factory::build_single_type_model());
    m.name(invalid_model_name);

    dogen::sml::merger mg;
    mg.add_target(m);

    contains_checker<merging_error> c(incorrect_model);
    BOOST_CHECK_EXCEPTION(mg.merge(), merging_error, c);
}

BOOST_AUTO_TEST_CASE(type_with_inconsistent_key_value_pair_throws) {
    SETUP_TEST_LOG("type_with_inconsistent_key_value_pair_throws");

    auto m(mock_model_factory::build_multi_type_model(0, 2));
    m.pods().begin()->second.name().type_name(invalid_type_name);

    dogen::sml::merger mg;
    mg.add_target(m);

    contains_checker<merging_error> c(inconsistent_kvp);
    BOOST_CHECK_EXCEPTION(mg.merge(), merging_error, c);
}

BOOST_AUTO_TEST_CASE(not_adding_a_target_throws) {
    SETUP_TEST_LOG("not_adding_a_target_throws");

    const auto m(mock_model_factory::build_single_type_model());
    dogen::sml::merger mg;
    mg.add(m);

    contains_checker<merging_error> c(missing_target);
    BOOST_CHECK_EXCEPTION(mg.merge(), merging_error, c);
}

BOOST_AUTO_TEST_CASE(adding_more_than_one_target_throws) {
    SETUP_TEST_LOG("adding_more_than_one_target_throws");
    const auto m0(mock_model_factory::build_single_type_model(0));
    const auto m1(mock_model_factory::build_single_type_model(1));

    dogen::sml::merger mg;
    mg.add_target(m0);

    contains_checker<merging_error> c(too_many_targets);
    BOOST_CHECK_EXCEPTION(mg.add_target(m1), merging_error, c);
}

BOOST_AUTO_TEST_CASE(merging_more_than_once_throws) {
    SETUP_TEST_LOG("merging_more_than_once_throws");
    const auto m(mock_model_factory::build_single_type_model());
    dogen::sml::merger mg;
    mg.add_target(m);
    mg.merge();

    contains_checker<merging_error> c(double_merging);
    BOOST_CHECK_EXCEPTION(mg.merge(), merging_error, c);
}

BOOST_AUTO_TEST_CASE(pod_incorrect_meta_type_throws) {
    SETUP_TEST_LOG("pod_incorrect_meta_type_throws");

    auto m(mock_model_factory::build_single_type_model());
    BOOST_CHECK(m.pods().size() == 1);

    auto p(m.pods().begin()->second);
    p.name().meta_type(dogen::sml::meta_types::primitive);
    m.pods().clear();
    m.pods().insert(std::make_pair(p.name(), p));

    dogen::sml::merger mg;
    mg.add_target(m);

    contains_checker<merging_error> c(incorrect_meta_type);
    BOOST_CHECK_EXCEPTION(mg.merge(), merging_error, c);
}

BOOST_AUTO_TEST_SUITE_END()
