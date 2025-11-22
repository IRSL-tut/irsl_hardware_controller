#include "irsl/simple_yaml_parser.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    std::string fname = "test.yaml";
    if (argc > 1) {
        fname = std::string(argv[1]);
    }
    YAML::Node n;
    try {
        n = YAML::LoadFile(fname);
    } catch (const std::exception &e) {
        std::cerr << "yaml: " << e.what() << std::endl;
    }
    ///
}
