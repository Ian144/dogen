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
#include <istream>
#include <sstream>
#include <boost/test/unit_test.hpp>
#include "dogen/utility/test/logging.hpp"
#include "dogen/utility/filesystem/path.hpp"
#include "dogen/utility/io/list_io.hpp"
#include "dogen/utility/test_data/dia_sml.hpp"
#include "dogen/sml/test/mock_model_factory.hpp"
#include "dogen/om/io/file_io.hpp"
#include "dogen/om/types/workflow.hpp"

using dogen::sml::test::mock_model_factory;

namespace {

const std::string test_module("om");
const std::string test_suite("workflow_spec");

}

using namespace dogen::om;
using namespace dogen::utility::test;

BOOST_AUTO_TEST_SUITE(workflow)

BOOST_AUTO_TEST_CASE(empty_mock_model_results_in_expected_files) {
    SETUP_TEST_LOG_SOURCE("empty_mock_results_in_expected_files");

    using namespace dogen::utility::filesystem;
    dogen::om::workflow w(data_files_directory());

    auto m(mock_model_factory::build_empty_model());
    const auto r(w.execute(m));

    BOOST_LOG_SEV(lg, debug) << "result: " << r;
    // BOOST_CHECK(!r.empty());

    // for (const auto& pair : r) {
    //     BOOST_CHECK(!pair.first.empty());
    //     BOOST_CHECK(!pair.second.empty());
    // }
}

BOOST_AUTO_TEST_SUITE_END()