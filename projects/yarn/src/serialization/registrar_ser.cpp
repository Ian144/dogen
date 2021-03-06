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
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include "dogen/yarn/serialization/module_ser.hpp"
#include "dogen/yarn/serialization/object_ser.hpp"
#include "dogen/yarn/serialization/concept_ser.hpp"
#include "dogen/yarn/serialization/visitor_ser.hpp"
#include "dogen/yarn/serialization/exception_ser.hpp"
#include "dogen/yarn/serialization/primitive_ser.hpp"
#include "dogen/yarn/serialization/enumeration_ser.hpp"
#include "dogen/dynamic/serialization/registrar_ser.hpp"

namespace dogen {
namespace yarn {

template<typename Archive>
void register_types(Archive& ar) {
    dogen::dynamic::register_types(ar);

    ar.template register_type<dogen::yarn::concept>();
    ar.template register_type<dogen::yarn::enumeration>();
    ar.template register_type<dogen::yarn::exception>();
    ar.template register_type<dogen::yarn::module>();
    ar.template register_type<dogen::yarn::object>();
    ar.template register_type<dogen::yarn::primitive>();
    ar.template register_type<dogen::yarn::visitor>();
}

template void register_types(boost::archive::polymorphic_oarchive& ar);
template void register_types(boost::archive::polymorphic_iarchive& ar);

template void register_types(boost::archive::text_oarchive& ar);
template void register_types(boost::archive::text_iarchive& ar);

template void register_types(boost::archive::binary_oarchive& ar);
template void register_types(boost::archive::binary_iarchive& ar);

template void register_types(boost::archive::xml_oarchive& ar);
template void register_types(boost::archive::xml_iarchive& ar);

} }
