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
#ifndef DOGEN_CPP_TYPES_CONCEPT_INFO_HPP
#define DOGEN_CPP_TYPES_CONCEPT_INFO_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <algorithm>
#include <iosfwd>
#include "dogen/cpp/serialization/concept_info_fwd_ser.hpp"
#include "dogen/cpp/types/entity.hpp"
#include "dogen/cpp/types/state.hpp"

namespace dogen {
namespace cpp {

class concept_info final : public dogen::cpp::entity {
public:
    concept_info() = default;
    concept_info(const concept_info&) = default;
    concept_info(concept_info&&) = default;

    virtual ~concept_info() noexcept { }

public:
    concept_info(
        const std::string& name,
        const std::string& documentation,
        const std::list<std::string>& namespaces,
        const std::unordered_map<std::string, dogen::cpp::path_spec_details>& path_spec_details_for_formatter,
        const dogen::cpp::state& state);

private:
    template<typename Archive>
    friend void boost::serialization::save(Archive& ar, const concept_info& v, unsigned int version);

    template<typename Archive>
    friend void boost::serialization::load(Archive& ar, concept_info& v, unsigned int version);

public:
    virtual void accept(const entity_visitor& v) const override {
        v.visit(*this);
    }

    virtual void accept(entity_visitor& v) const override {
        v.visit(*this);
    }

    virtual void accept(const entity_visitor& v) override {
        v.visit(*this);
    }

    virtual void accept(entity_visitor& v) override {
        v.visit(*this);
    }

public:
    void to_stream(std::ostream& s) const override;

public:
    const dogen::cpp::state& state() const;
    dogen::cpp::state& state();
    void state(const dogen::cpp::state& v);
    void state(const dogen::cpp::state&& v);

public:
    bool operator==(const concept_info& rhs) const;
    bool operator!=(const concept_info& rhs) const {
        return !this->operator==(rhs);
    }

public:
    bool equals(const dogen::cpp::entity& other) const override;

public:
    void swap(concept_info& other) noexcept;
    concept_info& operator=(concept_info other);

private:
    dogen::cpp::state state_;
};

} }

namespace std {

template<>
inline void swap(
    dogen::cpp::concept_info& lhs,
    dogen::cpp::concept_info& rhs) {
    lhs.swap(rhs);
}

}

#endif