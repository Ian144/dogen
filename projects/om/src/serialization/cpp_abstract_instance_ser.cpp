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
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/polymorphic_iarchive.hpp>
#include <boost/archive/polymorphic_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include "dogen/om/serialization/cpp_abstract_instance_ser.hpp"
#include "dogen/om/serialization/cpp_argument_ser.hpp"
#include "dogen/om/serialization/cpp_feature_ser.hpp"
#include "dogen/om/serialization/cpp_member_variable_ser.hpp"
#include "dogen/om/serialization/cpp_typeref_ser.hpp"
#include "dogen/om/serialization/cpp_variable_ser.hpp"
#include "dogen/om/serialization/doxygen_command_ser.hpp"

#ifdef __linux__
#include "eos/portable_iarchive.hpp"
#include "eos/portable_oarchive.hpp"
#endif

BOOST_CLASS_TRACKING(
    dogen::om::cpp_abstract_instance,
    boost::serialization::track_selectively)

namespace boost {
namespace serialization {

template<typename Archive>
void save(Archive& ar,
    const dogen::om::cpp_abstract_instance& v,
    const unsigned int /*version*/) {
    ar << make_nvp("cpp_feature", base_object<dogen::om::cpp_feature>(v));

    ar << make_nvp("commands", v.commands_);
    ar << make_nvp("type", v.type_);
    ar << make_nvp("default_value", v.default_value_);
}

template<typename Archive>
void load(Archive& ar,
    dogen::om::cpp_abstract_instance& v,
    const unsigned int /*version*/) {
    ar >> make_nvp("cpp_feature", base_object<dogen::om::cpp_feature>(v));

    ar >> make_nvp("commands", v.commands_);
    ar >> make_nvp("type", v.type_);
    ar >> make_nvp("default_value", v.default_value_);
}

} }

namespace boost {
namespace serialization {

template void save(archive::polymorphic_oarchive& ar, const dogen::om::cpp_abstract_instance& v, unsigned int version);
template void load(archive::polymorphic_iarchive& ar, dogen::om::cpp_abstract_instance& v, unsigned int version);

template void save(archive::text_oarchive& ar, const dogen::om::cpp_abstract_instance& v, unsigned int version);
template void load(archive::text_iarchive& ar, dogen::om::cpp_abstract_instance& v, unsigned int version);

template void save(archive::binary_oarchive& ar, const dogen::om::cpp_abstract_instance& v, unsigned int version);
template void load(archive::binary_iarchive& ar, dogen::om::cpp_abstract_instance& v, unsigned int version);

template void save(archive::xml_oarchive& ar, const dogen::om::cpp_abstract_instance& v, unsigned int version);
template void load(archive::xml_iarchive& ar, dogen::om::cpp_abstract_instance& v, unsigned int version);

#ifdef __linux__
template void save(eos::portable_oarchive& ar, const dogen::om::cpp_abstract_instance& v, unsigned int version);
template void load(eos::portable_iarchive& ar, dogen::om::cpp_abstract_instance& v, unsigned int version);
#endif

} }