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
#ifndef DOGEN_OM_TYPES_CMAKE_INSTALL_HPP
#define DOGEN_OM_TYPES_CMAKE_INSTALL_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <algorithm>
#include <iosfwd>
#include "dogen/om/serialization/cmake_install_fwd_ser.hpp"
#include "dogen/om/types/cmake_feature.hpp"

namespace dogen {
namespace om {

class cmake_install final : public dogen::om::cmake_feature {
public:
    cmake_install() = default;
    cmake_install(const cmake_install&) = default;
    cmake_install(cmake_install&&) = default;
    cmake_install& operator=(const cmake_install&) = default;

    virtual ~cmake_install() noexcept { }

private:
    template<typename Archive>
    friend void boost::serialization::save(Archive& ar, const cmake_install& v, unsigned int version);

    template<typename Archive>
    friend void boost::serialization::load(Archive& ar, cmake_install& v, unsigned int version);

public:
    void to_stream(std::ostream& s) const override;

public:
    bool operator==(const cmake_install& rhs) const;
    bool operator!=(const cmake_install& rhs) const {
        return !this->operator==(rhs);
    }

public:
    bool equals(const dogen::om::cmake_feature& other) const override;

};

} }

#endif
