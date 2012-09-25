/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 *
 * Copyright (C) 2012 Kitanda
 *
 * This file is distributed under the Kitanda Proprietary Software
 * Licence. See doc/LICENCE.TXT for details.
 *
 */
#ifndef DOGEN_GENERATOR_GENERATOR_HPP
#define DOGEN_GENERATOR_GENERATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif

#include <map>
#include <string>
#include <utility>
#include <functional>
#include <boost/optional.hpp>
#include <boost/filesystem/path.hpp>
#include "dogen/sml/domain/model.hpp"
#include "dogen/generator/config/settings.hpp"
#include "dogen/generator/backends/backend.hpp"
#include "dogen/generator/outputters/outputter.hpp"
#include "dogen/utility/serialization/archive_types.hpp"

namespace dogen {
namespace generator {

/**
 * @brief Core of Dogen. Implements the code generation workflow.
 *
 * The model which one intends to generate is known as the @e target
 * model. The code generator starts by reading in the target model and
 * all of its dependencies, of which there are two types:
 *
 * @li @e explicit: specified by the settings passed in; these are
 * models created by the user and any models that they, in turn,
 * depend on.
 *
 * @li @e implicit: these are added automatically. At present the only
 * implicit dependency is the primitives model, but its expected to be
 * extended further in the future.
 *
 * Collectively, all these models are referred to as the @e input
 * models. The input models are merged it into a single, unified SML
 * model, called the merged model, and all dependencies are resolved
 * and validated.
 *
 * The code generator then instantiates all backends requested by the
 * settings passed in. They use the merged model to generate source
 * code into a string. This is then outputted to the desired output
 * destination.
 */
class generator {
public:
    generator() = delete;
    generator& operator=(const generator&) = default;
    generator(const generator&) = default;

public:
    typedef std::function<std::ostream& ()> output_fn;

public:
    generator(generator&& rhs)
    : verbose_(std::move(rhs.verbose_)), settings_(std::move(rhs.settings_)) { }

    generator(const config::settings& s);
    generator(const config::settings& s, const output_fn& o);

private:
    /**
     * @brief Outputs the pair file name and contents to its output
     * destination.
     */
    void output(outputters::outputter::value_type o) const;

public: // public section for testing purposes only
    /**
     * @brief Returns true if the housekeeping of generated files is
     * required, false otherwise.
     */
    bool housekeeping_required() const;

    /**
     * @brief Merges all of the input models into the merged model.
     */
    boost::optional<sml::model> merge_models() const;

    /**
     * @brief Transforms the model into generated code, according to
     * the backend passed in.
     */
    void generate(backends::backend& b) const;

    /**
     * @brief Given a merged model, generates all of its
     * representations.
     */
    void generate(sml::model m) const;

public:
    /**
     * @brief Perform the entire code generation workflow.
     */
    void generate() const;

private:
    const bool verbose_;
    const config::settings settings_;
    const output_fn output_;
};

} }

#endif
