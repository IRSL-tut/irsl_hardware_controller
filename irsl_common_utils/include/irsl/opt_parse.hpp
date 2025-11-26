#pragma once

#include "thirdparty/CLI11.hpp"
#include <stdlib.h>

namespace irsl_common_utils
{

class OptParse : public CLI::App
{
private:
    uint64_t _hash;
    uint32_t _key;
public:
    OptParse(const std::string &name) : CLI::App(name)
    {
        add_option("--hash", _hash, "")->default_val("8888");
        add_option("--shm_key", _key, "")->default_val("8888");
    }
    uint64_t getHash() { return _hash; }
    uint32_t getShmKey() { return _key; }
    void opt_parse(int argc, char **argv)
    {
        try {
            this->parse(argc, argv);
        } catch(const CLI::ParseError &e) {
            ::exit( this->exit(e) );
        }
    }
};

}
