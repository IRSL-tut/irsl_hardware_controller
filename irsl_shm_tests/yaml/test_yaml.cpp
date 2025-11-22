//#include <yaml-cpp/yaml.h>
#include "irsl/simple_yaml_parser.hpp"
#include <iostream>

namespace icu = irsl_common_utils;

struct HOGE
{
    HOGE() : valueint(5678), valuedbl(-0.1234), valuestr("$default") {};
    int         valueint;
    double      valuedbl;
    std::string valuestr;
};

struct struct_b
{
    struct_b () : val_flt(-0.9876) {};
    double val_flt;
    icu::yamlList<int> val_list;
    HOGE val_struct;
};

namespace YAML{
//// YAML auto conversion
template<>
struct convert<HOGE> {
    static bool decode(const Node& node, HOGE& cType){
        bool r1, r2, r3;
        r1 = icu::readValue(node, "valueint", cType.valueint);
        r2 = icu::readValue(node, "valuedbl", cType.valuedbl);
        r3 = icu::readValue(node, "valuestr", cType.valuestr);
        return true;
    }
};
template <>
struct convert<struct_b>
{
    static inline bool decode(const Node &node, struct_b &cType)
    {
        bool r1, r2, r3;
        r1 = icu::readValue(node, "val_flt",  cType.val_flt);
        r2 = icu::readValueList(node, "val_list", cType.val_list);
        r3 = icu::readStruct(node, "val_struct", cType.val_struct);
        return true;
    }
};
}

int main(int argc, char **argv)
{
    icu::Node n;
    std::string fname = "test.yaml";
    if (argc > 1) {
        fname = std::string(argv[1]);
    }
    try {
        // check fname
        n = icu::LoadFile(fname);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "file: " << fname << " can not be opened" << std::endl;
        return -1;
    }

    //// read value
    bool res;
    int i_val;
    //res = readValue<int>(n, "int", i_val);
    res = icu::readValue<int>(n, "int", i_val);
    if (res) {
        std::cerr << "int: " << i_val << std::endl;
    } else {
        std::cerr << "fail :int:" << std::endl;
    }

    double d_val;
    res = icu::readValue<double>(n, "double", d_val);
    if (res) {
        std::cerr << "double: " << d_val << std::endl;
    } else {
        std::cerr << "fail :double:" << std::endl;
    }

    bool b_val;
    res = icu::readValue<bool>(n, "bool", b_val);
    if (res) {
        std::cerr << "bool: " << b_val << std::endl;
    } else {
        std::cerr << "fail :bool:" << std::endl;
    }

    std::string s_val;
    res = icu::readValue<std::string>(n, "string", s_val);
    if (res) {
        std::cerr << "string: " << s_val << std::endl;
    } else {
        std::cerr << "fail :string:" << std::endl;
    }

    HOGE h_val;
    res = icu::readValue<HOGE>(n, "hoge", h_val);
    if (res) {
        std::cerr << "hoge: " << std::endl;
        std::cerr << "  valueint: " << h_val.valueint << std::endl;
        std::cerr << "  valuedbl: " << h_val.valuedbl << std::endl;
        std::cerr << "  valuestr: " << h_val.valuestr << std::endl;
    } else {
        std::cerr << "fail :struct:" << std::endl;
    }

    //// read list(single type)
    icu::yamlList<int> i_lst;
    res = icu::readValueList<int>(n, "listint", i_lst);
    if (res) {
        std::cerr << "listint: " << i_lst.size() << std::endl;
        for(int i = 0; i < i_lst.size(); i ++) {
            std::cerr << "[" << i << "] : "<< i_lst[i] << std::endl;
        }
    } else {
        std::cerr << "fail :listint:" << std::endl;
    }
    icu::yamlList<double> d_lst;
    res = icu::readValueList<double>(n, "listdouble", d_lst);
    if (res) {
        std::cerr << "listdouble: " << d_lst.size() << std::endl;
        for(int i = 0; i < i_lst.size(); i ++) {
            std::cerr << "[" << i << "] : "<< d_lst[i] << std::endl;
        }
    } else {
        std::cerr << "fail :listdouble:" << std::endl;
    }
    icu::yamlList<std::string> s_lst;
    res = icu::readValueList<std::string>(n, "liststring", s_lst);
    if (res) {
        std::cerr << "liststring: " << s_lst.size() << std::endl;
        for(int i = 0; i < s_lst.size(); i ++) {
            std::cerr << "[" << i << "] : "<< s_lst[i] << std::endl;
        }
    } else {
        std::cerr << "fail :liststring:" << std::endl;
    }


    //// read map(key:string, value:single-type)
    icu::yamlMap<int> i_map;
    res = icu::readValueMap<int>(n, "mapint", i_map);
    if (res) {
        std::cerr << "mapint: " << i_map.size() << std::endl;
        for(auto it = i_map.begin(); it != i_map.end(); it++) {
            std::cerr << "  " << it->first << ": ";
            std::cerr << it->second << std::endl;
        }
    } else {
        std::cerr << "fail :mapint:" << std::endl;
    }
    icu::yamlMap<double> d_map;
    res = icu::readValueMap<double>(n, "mapdouble", d_map);
    if (res) {
        std::cerr << "mapdouble: " << d_map.size() << std::endl;
        for(auto it = d_map.begin(); it != d_map.end(); it++) {
            std::cerr << "  " << it->first << ": ";
            std::cerr << it->second << std::endl;
        }
    } else {
        std::cerr << "fail :mapdouble:" << std::endl;
    }
    icu::yamlMap<std::string> s_map;
    res = icu::readValueMap<std::string>(n, "mapstring", s_map);
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
        HOGE hoge;
        res = icu::readStruct(n, "HOGE", hoge);
        std::cerr << "HOGE: " << res << std::endl;
        std::cerr << "  valueint: " << hoge.valueint << std::endl;
        std::cerr << "  valuedbl: " << hoge.valuedbl << std::endl;
        std::cerr << "  valuestr: " << hoge.valuestr << std::endl;
    }
    {
        icu::yamlList<HOGE> h_lst;
        res = icu::readStructList(n, "structlist", h_lst);
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
        struct_b str_b;
        res = icu::readStruct(n, "struct_b", str_b);
        if (res) {
            std::cerr << "strut_b: " << std::endl;
            std::cerr << "  val_flt: " << str_b.val_flt << std::endl;
            std::cerr << "  val_list: " << std::endl;
            for(int i = 0; i < str_b.val_list.size(); i ++) {
                std::cerr << "    [" << i << "] : "<< str_b.val_list[i] << std::endl;
            }
            std::cerr << "  val_struct: " << std::endl;
            std::cerr << "    valueint: " << str_b.val_struct.valueint << std::endl;
            std::cerr << "    valuedbl: " << str_b.val_struct.valuedbl << std::endl;
            std::cerr << "    valuestr: " << str_b.val_struct.valuestr << std::endl;
        } else {
            std::cerr << "fail :structlist:" << std::endl;
        }
    }
    {
        struct_b str_b;
        res = icu::readStruct(n, "struct_b_test", str_b);
        if (res) {
            std::cerr << "strut_b_test: " << std::endl;
            std::cerr << "  val_flt: " << str_b.val_flt << std::endl;
            std::cerr << "  val_list: " << std::endl;
            for(int i = 0; i < str_b.val_list.size(); i ++) {
                std::cerr << "    [" << i << "] : "<< str_b.val_list[i] << std::endl;
            }
            std::cerr << "  val_struct: " << std::endl;
            std::cerr << "    valueint: " << str_b.val_struct.valueint << std::endl;
            std::cerr << "    valuedbl: " << str_b.val_struct.valuedbl << std::endl;
            std::cerr << "    valuestr: " << str_b.val_struct.valuestr << std::endl;
        } else {
            std::cerr << "fail :structlist:" << std::endl;
        }
    }
}
