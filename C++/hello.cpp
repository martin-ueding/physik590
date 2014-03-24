// Copyright © 2014 Martin Ueding <dev@martin-ueding.de>

#include <boost/program_options.hpp>

#include <iostream>

int main(int argc, char **argv) {
    boost::program_options::options_description description("");
    description.add_options()
    ("help", "Print usage and exit")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), vm);
    boost::program_options::notify(vm);

    if (vm.count("help") > 0) {
        std::cout << description << std::endl;
        return 0;
    }

    return 0;
}
