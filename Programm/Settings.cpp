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

    oss << "i{" << iterations << "}";
    oss << separator;
    oss << "o{" << mu_squared << "}";
    oss << separator;
    oss << "a{" << time_step << "}";
    oss << separator;
    oss << "s{" << position_seed << "}";
    oss << separator;
    oss << "c{" << accept_seed << "}";
    oss << separator;
    oss << "margin{" << margin << "}";

    oss << suffix;

    return oss.str();
}

std::string Settings::report() {
    std::string colon {": "};

    std::ostringstream oss;

    oss << "time sites" << colon << time_sites << std::endl;
    oss << "mass" << colon << mass << std::endl;
    oss << "time step" << colon << time_step << std::endl;
    oss << "μ²" << colon << mu_squared << std::endl;
    oss << "gauss height" << colon << gauss_height << std::endl;
    oss << "gauss width" << colon << gauss_width << std::endl;
    oss << "initial random width" << colon << initial_random_width << std::endl;
    oss << "Δ" << colon << margin << std::endl;
    oss << "pre iterations" << colon << pre_iterations << std::endl;
    oss << "pre rounds" << colon << pre_rounds << std::endl;
    oss << "iterations" << colon << iterations << std::endl;
    oss << "rounds" << colon << rounds << std::endl;
    oss << "iterations between" << iterations_between << rounds << std::endl;
    oss << "position seed" << position_seed << rounds << std::endl;
    oss << "accept seed" << accept_seed << rounds << std::endl;
    oss << "bootstrap samples" << bootstrap_samples << rounds << std::endl;
    oss << "position_hist_bins" << position_hist_bins << rounds << std::endl;
    oss << "action_hist_bins" << action_hist_bins << rounds << std::endl;

    return oss.str();
}

std::string Settings::hash() {
    std::string reported = report();
    size_t digest_size {CryptoPP::SHA1().DigestSize()};

    std::shared_ptr<byte> digest {new byte[digest_size]};
    CryptoPP::SHA1().CalculateDigest(digest.get(), reinterpret_cast<const byte*>(reported.data()), reported.length());

    std::string digest_string {reinterpret_cast<const char*>(digest.get())};

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest.get(), digest_size);
    encoder.MessageEnd();


    return output;
}
