#include "Config.hh"
#include <fstream>
#include <iostream>

#include <boost/program_options.hpp>

namespace MultiTSP {

namespace po = boost::program_options;

namespace {

po::variables_map parse_options(int argc, char *argv[]) {
  po::options_description desc("Allowed options");
  desc.add_options()("distance-config", po::value<std::string>(),
                     "Distance configuration");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  return vm;
}
} // namespace

Config::Config(int argc, char *argv[]) {
  po::variables_map vm = parse_options(argc, argv);

  std::fstream config_file(vm["distance-config"].as<std::string>(),
                           config_file.in);

  while (config_file) {
    std::string line;
    std::getline(config_file, line);
    std::cout << "L" << line << std::endl;
  }
}
} // namespace MultiTSP
