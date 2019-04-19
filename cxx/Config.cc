#include "Config.hh"
#include <fstream>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/program_options.hpp>

namespace MultiTSP {

namespace po = boost::program_options;

namespace {

po::variables_map parse_options(int argc, char *argv[]) {
  po::options_description desc("Allowed options");
  desc.add_options()("distance-config", po::value<std::string>(),
                     "Distance configuration")("weight-x", po::value<float>(),
                                               "Weight of X")(
      "weight-y", po::value<float>(),
      "Weight of Y")("tour-cnt", po::value<unsigned int>(), "Number of tours")(
      "thread-cnt", po::value<unsigned int>(),
      "Number of threads")("spaces-per-tour-cnt", po::value<unsigned int>(),
                           "Number of spaces in one tour");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);
  return vm;
}
} // namespace

Config::Config(int argc, char *argv[]) {
  po::variables_map vm = parse_options(argc, argv);

  this->weight_x = vm["weight-x"].as<float>();
  this->weight_y = vm["weight-y"].as<float>();
  this->tour_cnt = vm["tour-cnt"].as<unsigned int>();
  this->thread_cnt = vm["thread-cnt"].as<unsigned int>();
  this->spaces_per_tour_cnt = vm["spaces-per-tour-cnt"].as<unsigned int>();

  std::fstream config_file(vm["distance-config"].as<std::string>(),
                           config_file.in);

  unsigned int line_cnt(0);
  unsigned int column_cnt(0);
  while (config_file) {
    std::string line;
    std::getline(config_file, line);
    if (!config_file) {
      break;
    }

    std::vector<std::string> sline;
    boost::split(sline, line, [](char c) { return c == '\t'; });

    if (column_cnt == 0) {
      column_cnt = sline.size();
    } else {
      if (column_cnt != sline.size()) {
        // Lines are not equal size
        abort();
      }
    }

    teams.push_back(Team(line_cnt, std::stoi(sline[0]), std::stoi(sline[1])));
    std::vector<unsigned int> dists;
    for (unsigned int i(2); i < column_cnt; ++i) {
      dists.push_back(std::stoi(sline[i]));
    }
    distances.push_back(dists);
    ++line_cnt;
  }
  assert(line_cnt == column_cnt - 2);
}
} // namespace MultiTSP
