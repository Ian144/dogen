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
#ifndef DOGEN_QUILT_CPP_TYPES_SETTINGS_STREAMING_SETTINGS_REPOSITORY_HPP
#define DOGEN_QUILT_CPP_TYPES_SETTINGS_STREAMING_SETTINGS_REPOSITORY_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <string>
#include <algorithm>
#include <unordered_map>
#include "dogen/quilt.cpp/types/settings/streaming_settings.hpp"
#include "dogen/quilt.cpp/serialization/settings/streaming_settings_repository_fwd_ser.hpp"

namespace dogen {
namespace quilt {
namespace cpp {
namespace settings {

class streaming_settings_repository final {
public:
    streaming_settings_repository() = default;
    streaming_settings_repository(const streaming_settings_repository&) = default;
    streaming_settings_repository(streaming_settings_repository&&) = default;
    ~streaming_settings_repository() = default;

public:
    explicit streaming_settings_repository(const std::unordered_map<std::string, dogen::quilt::cpp::settings::streaming_settings>& by_id);

private:
    template<typename Archive>
    friend void boost::serialization::save(Archive& ar, const dogen::quilt::cpp::settings::streaming_settings_repository& v, unsigned int version);

    template<typename Archive>
    friend void boost::serialization::load(Archive& ar, dogen::quilt::cpp::settings::streaming_settings_repository& v, unsigned int version);

public:
    const std::unordered_map<std::string, dogen::quilt::cpp::settings::streaming_settings>& by_id() const;
    std::unordered_map<std::string, dogen::quilt::cpp::settings::streaming_settings>& by_id();
    void by_id(const std::unordered_map<std::string, dogen::quilt::cpp::settings::streaming_settings>& v);
    void by_id(const std::unordered_map<std::string, dogen::quilt::cpp::settings::streaming_settings>&& v);

public:
    bool operator==(const streaming_settings_repository& rhs) const;
    bool operator!=(const streaming_settings_repository& rhs) const {
        return !this->operator==(rhs);
    }

public:
    void swap(streaming_settings_repository& other) noexcept;
    streaming_settings_repository& operator=(streaming_settings_repository other);

private:
    std::unordered_map<std::string, dogen::quilt::cpp::settings::streaming_settings> by_id_;
};

} } } }

namespace std {

template<>
inline void swap(
    dogen::quilt::cpp::settings::streaming_settings_repository& lhs,
    dogen::quilt::cpp::settings::streaming_settings_repository& rhs) {
    lhs.swap(rhs);
}

}

#endif
