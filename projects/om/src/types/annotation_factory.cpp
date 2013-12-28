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
#include "dogen/utility/log/logger.hpp"
#include "dogen/utility/io/unordered_map_io.hpp"
#include "dogen/sml/types/tags.hpp"
#include "dogen/sml/types/meta_data_reader.hpp"
#include "dogen/om/types/hydration_workflow.hpp"
#include "dogen/om/io/modeline_group_io.hpp"
#include "dogen/om/types/hydration_workflow.hpp"
#include "dogen/om/types/modeline_group_hydrator.hpp"
#include "dogen/om/io/licence_io.hpp"
#include "dogen/om/types/licence_hydrator.hpp"
#include "dogen/om/types/code_generation_marker_factory.hpp"
#include "dogen/om/types/building_error.hpp"
#include "dogen/om/types/annotation_factory.hpp"

using namespace dogen::utility::log;

namespace {

auto lg(logger_factory("om.workflow"));
const std::string missing_context_ptr("Context pointer is null");
const std::string modeline_groups_dir("modeline_groups");
const std::string licence_dir("licences");

}

namespace dogen {
namespace om {

annotation_factory::annotation_factory(
    const std::list<boost::filesystem::path>& data_files_directories) :
    data_files_directories_(data_files_directories) { }

void annotation_factory::
throw_missing_item(const std::string& msg, const std::string& n) const {
    BOOST_LOG_SEV(lg, error) << msg << n;
    BOOST_THROW_EXCEPTION(building_error(msg + n));
}

boost::optional<licence> annotation_factory::
extract_licence(const boost::property_tree::ptree& meta_data) const {
    sml::meta_data_reader reader(meta_data);
    if (!reader.has_key(sml::tags::licence_name))
        return boost::optional<licence>();

    const auto licence_name(reader.get(sml::tags::licence_name));
    const auto i(licences_.find(licence_name));
    if (i == licences_.end())
        throw_missing_item("Licence not found: ", licence_name);

    licence l(i->second);
    if (reader.has_key(sml::tags::copyright_holder)) {
        const auto copyright_holder(reader.get(sml::tags::copyright_holder));
        l.copyright_holders().push_back(copyright_holder);
    }
    return l;
}

boost::optional<modeline> annotation_factory::
extract_modeline(const boost::property_tree::ptree& meta_data) const {
    sml::meta_data_reader reader(meta_data);
    if (!reader.has_key(sml::tags::modeline_group_name))
        return boost::optional<modeline>();

    const auto name(reader.get(sml::tags::modeline_group_name));
    const auto i(modeline_groups_.find(name));
    if (i == modeline_groups_.end())
        throw_missing_item("Modeline group not found: ", name);

    const auto modeline_group(i->second);
    const auto j(modeline_group.modelines().find("c++"));
    if (j == modeline_group.modelines().end())
        throw_missing_item("Modeline not found: ", name);

    return j->second;
}

std::string annotation_factory::
extract_marker(const boost::property_tree::ptree& meta_data) const {
    sml::meta_data_reader reader(meta_data);

    using cgm = sml::tags::code_generation_marker;
    const std::string message(reader.get(cgm::message));
    if (message.empty())
        return std::string();

    const bool add_date_time(reader.is_true(cgm::add_date_time));
    const bool add_warning(reader.is_true(cgm::add_warning));
    code_generation_marker_factory f(add_date_time, add_warning, message);

    return f.build();
}

std::list<boost::filesystem::path>
annotation_factory::create_directory_list(const std::string& for_whom) const {
    std::list<boost::filesystem::path> r;
    for (const auto& d : data_files_directories_)
        r.push_back(d / for_whom);
    return r;
}

void annotation_factory::hydrate_modelines() {
    const auto dirs(create_directory_list(modeline_groups_dir));
    hydration_workflow<modeline_group_hydrator> hw;
    modeline_groups_ = hw.hydrate(dirs);

    BOOST_LOG_SEV(lg, info) << "Loaded modeline groups. Found: "
                            << modeline_groups_.size();
    BOOST_LOG_SEV(lg, debug) << "contents: " << modeline_groups_;
}

void annotation_factory::hydrate_licences() {
    std::list<std::string> copyright_holders;
    licence_hydrator lh(copyright_holders);
    const auto dirs(create_directory_list(licence_dir));
    hydration_workflow<licence_hydrator> hw(lh);
    licences_ = hw.hydrate(dirs);

    BOOST_LOG_SEV(lg, info) << "Loaded licences. Found: "
                            << licences_.size();
    BOOST_LOG_SEV(lg, debug) << "contents: " << licences_;
}

void annotation_factory::load_reference_data() {
    hydrate_modelines();
    hydrate_licences();
}

annotation annotation_factory::
build(const boost::property_tree::ptree& meta_data) const {
    auto licence(extract_licence(meta_data));
    const auto modeline(extract_modeline(meta_data));
    const auto marker(extract_marker(meta_data));
    const annotation r(modeline, licence, marker);
    return r;
}

} }