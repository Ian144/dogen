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
#ifndef DOGEN_OM_TYPES_CPP_FUNCTION_HPP
#define DOGEN_OM_TYPES_CPP_FUNCTION_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <algorithm>
#include <iosfwd>
#include "dogen/om/serialization/cpp_function_fwd_ser.hpp"
#include "dogen/om/types/cpp_abstract_function.hpp"

namespace dogen {
namespace om {

class cpp_function final : public dogen::om::cpp_abstract_function {
public:
    cpp_function(const cpp_function&) = default;
    cpp_function(cpp_function&&) = default;

public:
    cpp_function();

    virtual ~cpp_function() noexcept { }

public:
    cpp_function(
        const std::string& name,
        const std::list<dogen::om::doxygen_command>& commands,
        const dogen::om::cpp_representation_types& representation_type,
        const bool is_inline,
        const dogen::om::cpp_instance_typeref& return_type,
        const std::list<dogen::om::cpp_argument>& arguments,
        const bool is_friend,
        const bool is_template_specialisation,
        const bool is_template_instantiation);

private:
    template<typename Archive>
    friend void boost::serialization::save(Archive& ar, const cpp_function& v, unsigned int version);

    template<typename Archive>
    friend void boost::serialization::load(Archive& ar, cpp_function& v, unsigned int version);

public:
    virtual void accept(const cpp_feature_visitor& v) const override {
        v.visit(*this);
    }

    virtual void accept(cpp_feature_visitor& v) const override {
        v.visit(*this);
    }

    virtual void accept(const cpp_feature_visitor& v) override {
        v.visit(*this);
    }

    virtual void accept(cpp_feature_visitor& v) override {
        v.visit(*this);
    }

public:
    void to_stream(std::ostream& s) const override;

public:
    bool is_template_specialisation() const;
    void is_template_specialisation(const bool v);

    bool is_template_instantiation() const;
    void is_template_instantiation(const bool v);

public:
    bool operator==(const cpp_function& rhs) const;
    bool operator!=(const cpp_function& rhs) const {
        return !this->operator==(rhs);
    }

public:
    bool equals(const dogen::om::cpp_feature& other) const override;

public:
    void swap(cpp_function& other) noexcept;
    cpp_function& operator=(cpp_function other);

private:
    bool is_template_specialisation_;
    bool is_template_instantiation_;
};

} }

namespace std {

template<>
inline void swap(
    dogen::om::cpp_function& lhs,
    dogen::om::cpp_function& rhs) {
    lhs.swap(rhs);
}

}

#endif
