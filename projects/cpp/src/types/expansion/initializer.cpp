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
#include <boost/shared_ptr.hpp>
#include "dogen/dynamic/expansion/types/workflow.hpp"
#include "dogen/cpp/types/expansion/expander.hpp"
#include "dogen/cpp/types/expansion/initializer.hpp"

namespace dogen {
namespace cpp {
namespace expansion {

void register_expander(dynamic::expansion::expander_interface * const p) {
    dynamic::expansion::workflow::registrar()
        .register_expander(
            boost::shared_ptr<dynamic::expansion::expander_interface>(p));
}

void initializer::initialize() {
    register_expander(new expander());
}

} } }