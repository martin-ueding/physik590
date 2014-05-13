// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>
// Licensed under The GNU Public License Version 2 (or later)

#include "Settings.hpp"

#include <crypto++/sha.h>
#include <crypto++/hex.h>

#include <sstream>

std::string Settings::generate_filename(std::string prefix,
                                        std::string suffix) {
    std::ostringstream oss;

    oss << prefix;
    oss << hash();
    oss << suffix;

    return oss.str();
}

std::string Settings::report() {
    std::string colon {": "};
    std::string prefix {"# "};

    std::ostringstream oss;

    oss << prefix << "This run has the following parameters:" << std::endl;
    oss << prefix << "time sites" << colon << time_sites << std::endl;
    oss << prefix << "mass" << colon << mass << std::endl;
    oss << prefix << "time step" << colon << time_step << std::endl;
    oss << prefix << "μ²" << colon << mu_squared << std::endl;
    oss << prefix << "gauss height" << colon << gauss_height << std::endl;
    oss << prefix << "gauss width" << colon << gauss_width << std::endl;
    oss << prefix << "initial random width" << colon << initial_random_width << std::endl;
    oss << prefix << "Δ" << colon << margin << std::endl;
    oss << prefix << "pre iterations" << colon << pre_iterations << std::endl;
    oss << prefix << "pre rounds" << colon << pre_rounds << std::endl;
    oss << prefix << "iterations" << colon << iterations << std::endl;
    oss << prefix << "rounds" << colon << rounds << std::endl;
    oss << prefix << "iterations between" << colon << iterations_between << std::endl;
    oss << prefix << "position seed" << colon << position_seed << std::endl;
    oss << prefix << "accept seed" << colon << accept_seed << std::endl;
    oss << prefix << "bootstrap samples" << colon << bootstrap_samples << std::endl;
    oss << prefix << "position_hist_bins" << colon << position_hist_bins << std::endl;
    oss << prefix << "action_hist_bins" << colon << action_hist_bins  << std::endl;
    oss << prefix << "----" << std::endl;

    return oss.str();
}

std::string Settings::hash() {
    std::string reported = report();
    size_t digest_size {CryptoPP::SHA1{}.DigestSize()};

    std::unique_ptr<byte> digest {new byte[digest_size]};
    CryptoPP::SHA1{}.CalculateDigest(digest.get(), reinterpret_cast<const byte*>(reported.data()), reported.length());

    std::string digest_string {reinterpret_cast<const char*>(digest.get())};

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink{output});
    encoder.Put(digest.get(), digest_size);
    encoder.MessageEnd();

    return output;
}
