#pragma once

#include "irsl/thirdparty/CLI11.hpp"

class OptParse : public CLI::App
{
private:
    uint64_t _hash;
    uint32_t _key;
public:
    OptParse(const std::string name) : CLI::App(name)
    {
        add_option("--hash", _hash, "")->default_val("8888");
        add_option("--shm_key", _key, "")->default_val("8888");
        allow_extras(true);
    }
    uint64_t getHash() { return _hash; }
    uint32_t getShmKey() { return _key; }
};
