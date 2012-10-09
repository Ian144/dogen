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
#include <ostream>
#include "dogen/utility/io/optional_io.hpp"
#include "dogen/utility/io/vector_io.hpp"
#include "dogen/dia/io/object_io.hpp"
#include "dogen/dia/io/attribute_io.hpp"
#include "dogen/dia/io/child_node_io.hpp"
#include "dogen/dia/io/connection_io.hpp"

namespace dogen {
namespace dia {

std::ostream&
operator<<(std::ostream& stream, dogen::dia::object object) {
    stream << "\"object\": {"
           << " \"type\": \"" << object.type() << "\","
           << " \"version\": \"" << object.version() << "\","
           << " \"id\": \"" << object.id() << "\", "
           << " \"attributes\":" << object.attributes() << ","
           << object.child_node()
           << " \"connections\":" << object.connections() << ","
           << object.connections()
           << " }";
    return(stream);
}

} }
