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
#include <boost/make_shared.hpp>
#include "dogen/yarn/types/object.hpp"
#include "dogen/quilt.cpp/types/properties/inclusion_dependencies_provider_interface.hpp"
#include "dogen/quilt.cpp/types/formatters/assistant.hpp"
#include "dogen/quilt.cpp/types/formatters/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/inclusion_constants.hpp"
#include "dogen/quilt.cpp/types/formatters/io/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/serialization/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/types/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/hash/traits.hpp"
#include "dogen/quilt.cpp/types/formatters/types/visitor_header_formatter_stitch.hpp"
#include "dogen/quilt.cpp/types/formatters/types/visitor_header_formatter.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace formatters {
namespace types {

namespace {

class provider final : public properties::
        inclusion_dependencies_provider_interface<yarn::visitor> {
public:
    std::string formatter_name() const override;

    boost::optional<std::list<std::string> >
        provide(const properties::inclusion_dependencies_builder_factory& f,
        const yarn::visitor& v) const override;
};

std::string provider::formatter_name() const {
    return visitor_header_formatter::static_formatter_name();
}

boost::optional<std::list<std::string> >
provider::provide(const properties::inclusion_dependencies_builder_factory& f,
    const yarn::visitor& v) const {

    auto builder(f.make());
    if (v.visits().empty())
        return boost::optional<std::list<std::string> >();

    const auto fwd_fn(traits::forward_declarations_formatter_name());
    builder.add(v.visits(), fwd_fn);

    return builder.build();
}

}

std::string visitor_header_formatter::static_formatter_name() {
    return traits::visitor_header_formatter_name();
}

std::string visitor_header_formatter::id() const {
    static auto r(ownership_hierarchy().formatter_name());
    return r;
}

dynamic::ownership_hierarchy
visitor_header_formatter::ownership_hierarchy() const {
    static dynamic::ownership_hierarchy
        r(formatters::traits::model_name(), traits::facet_name(),
            visitor_header_formatter::static_formatter_name(),
            formatters::traits::header_formatter_group_name());
    return r;
}

file_types visitor_header_formatter::file_type() const {
    return file_types::cpp_header;
}

properties::origin_types
visitor_header_formatter::formattable_origin_type() const {
    return properties::origin_types::external;
}

void visitor_header_formatter::register_inclusion_dependencies_provider(
    properties::registrar& rg) const {
    rg.register_provider(boost::make_shared<provider>());
}

dogen::formatters::file visitor_header_formatter::
format(const context& ctx, const yarn::visitor& v) const {
    assistant a(ctx, ownership_hierarchy(), file_type(), v.name().id());
    const auto r(visitor_header_formatter_stitch(a, v));
    return r;
}

} } } } }
