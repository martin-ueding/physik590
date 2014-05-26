// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Settings.hpp"

#include <boost/filesystem.hpp>
#include <crypto++/hex.h>
#include <crypto++/sha.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <sstream>
#include <fstream>

void Settings::compute() {
    for (unsigned i = 0; time_step * i < corr_tau_max; i++) {
        correlation_ts.push_back(i);
    }
}

void Settings::store_json(std::string filename) {
    Json::Value root;

    root["time_step"] = time_step;
    root["iterations"] = iterations;

    std::ofstream os {filename};
    os << root;
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
    oss << prefix << "mu squared" << colon << mu_squared << std::endl;
    oss << prefix << "inverse scattering length" << colon << inverse_scattering_length << std::endl;
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
    oss << prefix << "position hist_bins" << colon << position_hist_bins << std::endl;
    oss << prefix << "action hist_bins" << colon << action_hist_bins << std::endl;
    oss << prefix << "correlation size" << colon << correlation_size << std::endl;
    oss << prefix << "correlation ts" << colon;
    for (unsigned i : correlation_ts) {
        oss << i << " ";
    }
    oss << std::endl;
    oss << prefix << "export potential steps" << colon << export_potential_steps << std::endl;
    oss << prefix << "export potential bound" << colon << export_potential_bound << std::endl;
    oss << prefix << "corr tau max" << colon << corr_tau_max << std::endl;
    oss << prefix << "t_0" << colon << t_0 << std::endl;
    oss << prefix << "t_0 mode" << colon << t_0_mode << std::endl;
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
    return 2 * correlation_size;
}

unsigned Settings::matrix_to_state(unsigned i, bool even) {
    return i * 2 + (even ? 2 : 1);
}

unsigned Settings::state_to_matrix(unsigned n) {
    if (n == 0) {
        throw std::range_error {"Correlation C_00 is not contained in the matrix."};
    }
    return (n - 1) / 2;
}

unsigned Settings::get_t_0(unsigned t) {
    switch (t_0_mode) {
        case 1:
            return get_t_0_half(t);
            break;
        default:
            return get_t_0_fixed();
            break;
    }
}

unsigned Settings::get_t_0_fixed() {
    return t_0;
}

unsigned Settings::get_t_0_half(unsigned t) {
    return t / 2;
}

