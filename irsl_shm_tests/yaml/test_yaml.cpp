//#include <yaml-cpp/yaml.h>
#include "irsl/simple_yaml_parser.hpp"
#include <iostream>

using namespace irsl_common_utils;

struct HOGE
{
    int         valueint;
    double      valuedbl;
    std::string valuestr;
};

namespace YAML{
//// YAML auto conversion
template<>
struct convert<HOGE> {
    static bool decode(const Node& node, HOGE& cType){
        cType.valueint = node["valueint"].as<int>();
        cType.valuedbl = node["valuedbl"].as<double>();
        cType.valuestr = node["valuestr"].as<std::string>();
        return true;
    }
};

}

int main(int argc, char **argv)
{
    YAML::Node n;
    const std::string fname = "test.yaml";
    try {
        // check fname
        n = YAML::LoadFile(fname);
    } catch (const std::exception&) {
        std::cerr << "parameter file [" << fname << "] can not open" << std::endl;
        return -1;
    }

    //// read value
    bool res;
    int i_val;
    //res = readValue<int>(n, "int", i_val);
    res = readValue<int>(n, "int", i_val);
    if (res) {
        std::cerr << "int: " << i_val << std::endl;
    } else {
        std::cerr << "fail :int:" << std::endl;
    }

    double d_val;
    res = readValue<double>(n, "double", d_val);
    if (res) {
        std::cerr << "double: " << d_val << std::endl;
    } else {
        std::cerr << "fail :double:" << std::endl;
    }

    bool b_val;
    res = readValue<bool>(n, "bool", b_val);
    if (res) {
        std::cerr << "bool: " << b_val << std::endl;
    } else {
        std::cerr << "fail :bool:" << std::endl;
    }

    std::string s_val;
    res = readValue<std::string>(n, "string", s_val);
    if (res) {
        std::cerr << "string: " << s_val << std::endl;
    } else {
        std::cerr << "fail :string:" << std::endl;
    }

    HOGE h_val;
    res = readValue<HOGE>(n, "hoge", h_val);
    if (res) {
        std::cerr << "hoge: " << std::endl;
        std::cerr << "  valueint: " << h_val.valueint << std::endl;
        std::cerr << "  valuedbl: " << h_val.valuedbl << std::endl;
        std::cerr << "  valuestr: " << h_val.valuestr << std::endl;
    } else {
        std::cerr << "fail :struct:" << std::endl;
    }

    //// read list(single type)
    yamlList<int> i_lst;
    res = readValueList<int>(n, "listint", i_lst);
    if (res) {
        std::cerr << "listint: " << i_lst.size() << std::endl;
        for(int i = 0; i < i_lst.size(); i ++) {
            std::cerr << "[" << i << "] : "<< i_lst[i] << std::endl;
        }
    } else {
        std::cerr << "fail :listint:" << std::endl;
    }
    yamlList<double> d_lst;
    res = readValueList<double>(n, "listdouble", d_lst);
    if (res) {
        std::cerr << "listdouble: " << d_lst.size() << std::endl;
        for(int i = 0; i < i_lst.size(); i ++) {
            std::cerr << "[" << i << "] : "<< d_lst[i] << std::endl;
        }
    } else {
        std::cerr << "fail :listdouble:" << std::endl;
    }
    yamlList<std::string> s_lst;
    res = readValueList<std::string>(n, "liststring", s_lst);
    if (res) {
        std::cerr << "liststring: " << s_lst.size() << std::endl;
        for(int i = 0; i < s_lst.size(); i ++) {
            std::cerr << "[" << i << "] : "<< s_lst[i] << std::endl;
        }
    } else {
        std::cerr << "fail :liststring:" << std::endl;
    }


    //// read map(key:string, value:single-type)
    yamlMap<int> i_map;
    res = readValueMap<int>(n, "mapint", i_map);
    if (res) {
        std::cerr << "mapint: " << i_map.size() << std::endl;
        for(auto it = i_map.begin(); it != i_map.end(); it++) {
            std::cerr << "  " << it->first << ": ";
            std::cerr << it->second << std::endl;
        }
    } else {
        std::cerr << "fail :mapint:" << std::endl;
    }
    yamlMap<double> d_map;
    res = readValueMap<double>(n, "mapdouble", d_map);
    if (res) {
        std::cerr << "mapdouble: " << d_map.size() << std::endl;
        for(auto it = d_map.begin(); it != d_map.end(); it++) {
            std::cerr << "  " << it->first << ": ";
            std::cerr << it->second << std::endl;
        }
    } else {
        std::cerr << "fail :mapdouble:" << std::endl;
    }
    yamlMap<std::string> s_map;
    res = readValueMap<std::string>(n, "mapstring", s_map);
    if (res) {
        std::cerr << "mapstring: " << s_map.size() << std::endl;
        for(auto it = s_map.begin(); it != s_map.end(); it++) {
            std::cerr << "  " << it->first << ": ";
            std::cerr << it->second << std::endl;
        }
    } else {
        std::cerr << "fail :mapstring:" << std::endl;
    }

    {
        yamlList<HOGE> h_lst;
        res=readStructList(n, "structlist", h_lst);
        if(res) {
            std::cerr << "structlist: " << h_lst.size() << std::endl;
            for(auto it = h_lst.begin(); it != h_lst.end(); ++it) {
                std::cerr << "  hoge: " << std::endl;
                std::cerr << "    valueint: " << (*it).valueint << std::endl;
                std::cerr << "    valuedbl: " << (*it).valuedbl << std::endl;
                std::cerr << "    valuestr: " << (*it).valuestr << std::endl;
            }
        } else {
            std::cerr << "fail :structlist:" << std::endl;
        }
    }
    {
        yamlList<HOGE> h_lst;
        res=readValueList(n, "structlist", h_lst);
        if(res) {
            std::cerr << "structlist: " << h_lst.size() << std::endl;
            for(auto it = h_lst.begin(); it != h_lst.end(); ++it) {
                std::cerr << "  hoge: " << std::endl;
                std::cerr << "    valueint: " << (*it).valueint << std::endl;
                std::cerr << "    valuedbl: " << (*it).valuedbl << std::endl;
                std::cerr << "    valuestr: " << (*it).valuestr << std::endl;
            }
        } else {
            std::cerr << "fail :structlist:" << std::endl;
        }
    }
}
