// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Settings.hpp"

#include <boost/filesystem.hpp>
#include <crypto++/hex.h>
#include <crypto++/sha.h>

#include <sstream>

void Settings::compute() {
    for (unsigned i = 0; i < time_sites / 2; i += 2 + i / 2) {
        correlation_ts.push_back(i);
    }
}

std::string Settings::generate_filename(std::string name) {
    std::ostringstream oss;
    std::string computed_hash {hash()};

    oss << "out/";
    oss << computed_hash;

    boost::filesystem::create_directories(oss.str());

    oss << "/";
    oss << computed_hash.substr(0, 6);
    oss << "-";
    oss << name;


    return oss.str();
}

std::string Settings::report() {
    std::string colon {": "};
    std::string prefix {"# "};

    std::ostringstream oss;

    oss << prefix << "This run has the following parameters:" << std::endl;
    oss << prefix << "Algorithm version" << colon << algorithm_version << std::endl;
    oss << prefix << "time sites" << colon << time_sites << std::endl;
    oss << prefix << "mass" << colon << mass << std::endl;
    oss << prefix << "time step" << colon << time_step << std::endl;
    oss << prefix << "mu_squared" << colon << mu_squared << std::endl;
    oss << prefix << "gauss height" << colon << gauss_height << std::endl;
    oss << prefix << "gauss width" << colon << gauss_width << std::endl;
    oss << prefix << "initial random width" << colon << initial_random_width << std::endl;
    oss << prefix << "margin" << colon << margin << std::endl;
    oss << prefix << "pre iterations" << colon << pre_iterations << std::endl;
    oss << prefix << "pre rounds" << colon << pre_rounds << std::endl;
    oss << prefix << "iterations" << colon << iterations << std::endl;
    oss << prefix << "rounds" << colon << rounds << std::endl;
    oss << prefix << "iterations between" << colon << iterations_between << std::endl;
    oss << prefix << "position seed" << colon << position_seed << std::endl;
    oss << prefix << "accept seed" << colon << accept_seed << std::endl;
    oss << prefix << "bootstrap samples" << colon << bootstrap_samples << std::endl;
    oss << prefix << "position_hist_bins" << colon << position_hist_bins << std::endl;
    oss << prefix << "action_hist_bins" << colon << action_hist_bins << std::endl;
    oss << prefix << "correlation_size" << colon << correlation_size << std::endl;
    oss << prefix << "correlation_ts" << colon;
    for (unsigned i: correlation_ts)
        oss << i << " ";
    oss << std::endl;
    oss << prefix << "export_potential_steps" << colon << export_potential_steps << std::endl;
    oss << prefix << "export_potential_bound" << colon << export_potential_bound << std::endl;
    oss << prefix << "----" << std::endl;

    return oss.str();
}

std::string Settings::hash() {
    std::string reported = report();
    size_t digest_size {CryptoPP::SHA1{} .DigestSize()};

    std::unique_ptr<byte> digest {new byte[digest_size]};
    CryptoPP::SHA1 {} .CalculateDigest(digest.get(), reinterpret_cast<const byte *>(reported.data()), reported.length());

    std::string digest_string {reinterpret_cast<const char *>(digest.get())};

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink {output});
    encoder.Put(digest.get(), digest_size);
    encoder.MessageEnd();

    return output;
}

unsigned Settings::max_energyvalue() {
    return 2 * correlation_size - 1;
}

unsigned Settings::matrix_to_state(unsigned i, bool even) {
    return i * 2 + (even ? 2 : 1);
}

unsigned Settings::state_to_matrix(unsigned n) {
    if (n == 1) {
        throw std::range_error {"Correlation C_00 is not contained in the matrix."};
    }
    return (n - 1) / 2;
}
