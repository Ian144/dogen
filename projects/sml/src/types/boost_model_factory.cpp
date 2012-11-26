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
#include "dogen/sml/types/boost_model_factory.hpp"

namespace {
const std::string model_name("boost");
const std::string shared_ptr_name("shared_ptr");
const std::string weak_ptr_name("weak_ptr");
const std::string scoped_ptr_name("scoped_ptr");
const std::string optional_name("optional");

const std::string asio_name("asio");
const std::string io_service_name("io_service");
const std::string ip_name("ip");
const std::string tcp_name("tcp");
const std::string socket_name("socket");
const std::string acceptor_name("acceptor");

}

namespace dogen {
namespace sml {

primitive boost_model_factory::create_primitive(const std::string& name) {
    qualified_name q;
    q.type_name(name);
    q.meta_type(meta_types::primitive);
    q.model_name(model_name);
    primitive r;
    r.name(q);
    r.generation_type(generation_types::no_generation);
    return r;
}

pod boost_model_factory::
create_pod(const std::string& name, pod_types pt,
    std::list<std::string> package_path) {
    qualified_name q;
    q.type_name(name);
    q.meta_type(meta_types::pod);
    q.model_name(model_name);
    q.package_path(package_path);
    pod r;
    r.name(q);
    r.generation_type(generation_types::no_generation);
    if (pt == pod_types::sequence_container)
        r.number_of_type_arguments(1);
    else if (pt == pod_types::associative_container)
        r.number_of_type_arguments(2);

    r.pod_type(pt);
    return r;
}

package boost_model_factory::
create_package(const std::string& name, std::list<std::string> package_path) {
    qualified_name qn;
    qn.model_name(model_name);
    qn.type_name(name);
    qn.package_path(package_path);

    package r;
    r.name(qn);

    return r;
}

model boost_model_factory::create() {
    using namespace sml;
    std::unordered_map<qualified_name, primitive> primitives;
    std::unordered_map<qualified_name, pod> pods;
    std::unordered_map<qualified_name, package> packages;

    // const auto lambda([&](std::string name){
    //         primitive p(create_primitive(name));
    //         primitives.insert(std::make_pair(p.name(), p));
    //     });

    const auto pi([&](std::string name, pod_types pt,
            std::list<std::string> package_path) {
            pod p(create_pod(name, pt, package_path));
            pods.insert(std::make_pair(p.name(), p));
        });

    const auto gamma([&](std::string name,
            std::list<std::string> package_path) {
            package p(create_package(name, package_path));
            packages.insert(std::make_pair(p.name(), p));
        });


    std::list<std::string> package_path;
    pi(shared_ptr_name, pod_types::smart_pointer, package_path);
    pi(weak_ptr_name, pod_types::smart_pointer, package_path);
    pi(scoped_ptr_name, pod_types::smart_pointer, package_path);
    pi(optional_name, pod_types::value, package_path);

    gamma(asio_name, package_path);
    package_path.push_back(asio_name);
    pi(io_service_name, pod_types::value, package_path);

    gamma(ip_name, package_path);
    package_path.push_back(ip_name);

    gamma(tcp_name, package_path);
    package_path.push_back(tcp_name);

    pi(socket_name, pod_types::value, package_path);
    pi(acceptor_name, pod_types::value, package_path);

    model r;
    r.name(model_name);
    r.primitives(primitives);
    r.pods(pods);
    r.packages(packages);
    r.is_system(true);
    return r;
}

} }
