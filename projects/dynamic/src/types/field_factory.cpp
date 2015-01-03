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
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/throw_exception.hpp>
#include "dogen/utility/log/logger.hpp"
#include "dogen/dynamic/io/value_types_io.hpp"
#include "dogen/dynamic/types/building_error.hpp"
#include "dogen/dynamic/types/text.hpp"
#include "dogen/dynamic/types/text_collection.hpp"
#include "dogen/dynamic/types/number.hpp"
#include "dogen/dynamic/types/number_collection.hpp"
#include "dogen/dynamic/types/boolean.hpp"
#include "dogen/dynamic/types/boolean_collection.hpp"
#include "dogen/dynamic/types/field_factory.hpp"

namespace {

using namespace dogen::utility::log;
static logger lg(logger_factory("dynamic.field_factory"));

const std::string value_type_not_supported(
    "Value type is not supported by factory: ");
const std::string expected_at_most_one_element(
    "Expected at most one element");
const std::string invalid_numeric_value(
    "Invalid numeric value: ");
const std::string invalid_boolean_value(
    "Invalid boolean value: ");

}

namespace dogen {
namespace dynamic {

int field_factory::to_int(const std::string& s) const {
    try {
        return boost::lexical_cast<int>(s);
    } catch (boost::bad_lexical_cast& e) {
        BOOST_LOG_SEV(lg, error) << invalid_numeric_value << s;
        BOOST_THROW_EXCEPTION(building_error(invalid_numeric_value + s));
    }
}

bool field_factory::to_bool(const std::string& s) const {
    try {
        return boost::lexical_cast<bool>(s);
    } catch (boost::bad_lexical_cast& e) {
        BOOST_LOG_SEV(lg, error) << invalid_boolean_value << s;
        BOOST_THROW_EXCEPTION(building_error(invalid_numeric_value + s));
    }
}

void field_factory::ensure_at_most_one_element(
    const std::forward_list<std::string>& raw_values) const {

    if (raw_values.empty())
        return;

    const auto i(++raw_values.begin());
    if (i != raw_values.end()) {
        BOOST_LOG_SEV(lg, error) << expected_at_most_one_element;
        BOOST_THROW_EXCEPTION(building_error(expected_at_most_one_element));
    }
}

boost::shared_ptr<value> field_factory::create_text_value(
    const std::string& raw_value) const {
    return boost::make_shared<text>(raw_value);
}

boost::shared_ptr<value> field_factory::create_text_values(
    const std::forward_list<std::string>& raw_values) const {
    auto r(boost::make_shared<text_collection>());
    for (const auto& rv : raw_values)
        r->content().push_front(text(rv));

    return r;
}

boost::shared_ptr<value> field_factory::create_number_value(
    const std::string& raw_value) const {
    return boost::make_shared<number>(to_int(raw_value));
}

boost::shared_ptr<value> field_factory::create_number_values(
    const std::forward_list<std::string>& raw_values) const {
    auto r(boost::make_shared<number_collection>());
    for (const auto& rv : raw_values)
        r->content().push_front(number(to_int(rv)));

    return r;
}

boost::shared_ptr<value> field_factory::create_boolean_value(
    const std::string& raw_value) const {
    return boost::make_shared<boolean>(to_bool(raw_value));
}

boost::shared_ptr<value> field_factory::create_boolean_values(
    const std::forward_list<std::string>& raw_values) const {
    auto r(boost::make_shared<boolean_collection>());
    for (const auto& rv : raw_values)
        r->content().push_front(boolean(to_bool(rv)));

    return r;
}

field field_factory::build(const field_definition& fd,
    const std::forward_list<std::string>& raw_values) const {
    field r;

    r.name(fd.name());
    switch (fd.type()) {
    case value_types::text:
        ensure_at_most_one_element(raw_values);
        r.value(create_text_value(raw_values.front()));
        break;

    case value_types::text_collection:
        r.value(create_text_values(raw_values));
        break;

    case value_types::number:
        ensure_at_most_one_element(raw_values);
        r.value(create_text_values(raw_values));
        break;

    case value_types::number_collection:
        r.value(create_text_values(raw_values));
        break;

    case value_types::boolean:
        ensure_at_most_one_element(raw_values);
        r.value(create_text_values(raw_values));
        break;

    case value_types::boolean_collection:
        r.value(create_text_values(raw_values));
        break;

    default:
        BOOST_LOG_SEV(lg, error) << value_type_not_supported << fd.type();
        BOOST_THROW_EXCEPTION(building_error(
                value_type_not_supported +
                boost::lexical_cast<std::string>(fd.type())));
    }
    return r;
}

} }