// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Settings.hpp"

#include <boost/filesystem.hpp>
#include <crypto++/hex.h>
#include <crypto++/sha.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <regex>
#include <sstream>
#include <string>

#define JSON(var) root[ #var ] = (var);

void Settings::compute() {
    for (unsigned i = 0; time_step * i < corr_tau_max; i++) {
        correlation_ts.push_back(i);
    }
}

void Settings::store_json(std::string filename) {
    std::ofstream os {filename};
    os << get_json_string();
}

std::string Settings::get_json_string() {
    Json::Value root;

    JSON(accept_seed);
    JSON(action_hist_bins);
    JSON(algorithm_version);
    JSON(bootstrap_samples);
    JSON(corr_tau_max);
    JSON(correlation_size);
    JSON(export_potential_bound);
    JSON(export_potential_steps);
    JSON(gauss_width);
    JSON(initial_random_width);
    JSON(inverse_scattering_length);
    JSON(iterations);
    JSON(iterations_between);
    JSON(margin);
    JSON(mass);
    JSON(mu_squared);
    JSON(position_hist_bins);
    JSON(position_seed);
    JSON(pre_iterations);
    JSON(pre_rounds);
    JSON(rounds);
    JSON(t_0);
    JSON(t_0_mode);
    JSON(time_sites);
    JSON(time_step);

    for (auto t : correlation_ts) {
        root["correlation_ts"].append(t);
    }

    std::ostringstream oss;
    oss << root;
    return oss.str();
}

std::string Settings::generate_filename(std::string name) {
    std::ostringstream oss;
    std::string computed_hash {hash()};

    oss << "out/";
    oss << computed_hash;

    boost::filesystem::create_directories(oss.str());

    oss << "/";
    oss << computed_hash;
    oss << "-";
    oss << name;

    return oss.str();
}

std::string Settings::report() {
    std::string before {get_json_string()};
    std::regex newline {"\n"};
    std::string replacement {"\n# "};
    return std::regex_replace(before, newline, replacement);
}

std::string Settings::hash() {
    std::string reported = get_json_string();
    size_t digest_size {CryptoPP::SHA1{} .DigestSize()};

    std::unique_ptr<byte> digest {new byte[digest_size]};
    CryptoPP::SHA1 {} .CalculateDigest(digest.get(), reinterpret_cast<const byte *>(reported.data()), reported.length());

    std::string digest_string {reinterpret_cast<const char *>(digest.get())};

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink {output});
    encoder.Put(digest.get(), digest_size);
    encoder.MessageEnd();

    return output.substr(0, 6);
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

