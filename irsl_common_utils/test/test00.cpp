#include <iostream>
#include "irsl/simple_yaml_parser.hpp"

namespace icu = irsl_common_utils;

struct struct_a
{
    struct_a () : s_int(-123), s_str() {};
    int s_int;
    std::string s_str;
};

struct struct_b
{
    struct_b () : val_flt(-0.9876) {};
    double val_flt;
    icu::yamlList<int> val_list;
    struct_a val_struct;
};

namespace YAML
{
    //// YAML auto conversion
    template <>
    struct convert<struct_a>
    {
        static inline bool decode(const Node &node, struct_a &cType)
        {
            bool r1, r2;
            r1 = icu::readValue(node, "s_int", cType.s_int);
            r2 = icu::readValue(node, "s_str", cType.s_str);
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
    {
        std::string vstr;
        bool res = icu::readValue(n, "val_str", vstr);
        std::cout << "res: " << res;
        std::cout << " | val_str = " << vstr << std::endl;
    }
    {
        std::string str;
        bool res = icu::readValue(n, "xxxx", str);
        std::cout << "res: " << res;
        std::cout << " | val_str = " << str << std::endl;
    }
    {
        int vint = 0;
        bool res = icu::readValue(n, "val_int", vint);
        std::cout << "res: " << res;
        std::cout << " | val_int = " << vint << std::endl;
    }
    {
        double vflt = 0;
        bool res = icu::readValue(n, "val_flt", vflt);
        std::cout << "res: " << res;
        std::cout << " | val_flt = " << vflt << std::endl;
    }
    {
        //std::vector<std::string> lst;
        icu::yamlList<std::string> lst;
        bool res = icu::readValueList(n, "list_str", lst);
        std::cout << "res: " << res;
        std::cout << " | [";
        for (int i = 0; i < lst.size(); i++) {
            std::cout << lst[i];
            if (i + 1 != lst.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
    {
        icu::yamlMap<double> vmap;
        bool res = icu::readValueMap(n, "map_flt", vmap);
        std::cout << "res: " << res;
        std::cout << " | {";
        for (auto it = vmap.begin(); it != vmap.end(); it++) {
            std::cout << (*it).first << ": " << (*it).second;
            std::cout << ", ";
        }
        std::cout << "}" << std::endl;;
    }
    {
        struct_a st_a;
        bool res = icu::readStruct(n, "struct_a", st_a);
        std::cout << "res: " << res;
        std::cout << "| { s_int: " << st_a.s_int;
        std::cout << ", s_str: " << st_a.s_str;
        std::cout << "}" << std::endl;
    }
    {
        struct_b st_b;
        bool res = icu::readStruct(n, "struct_b", st_b);
        std::cout << "res: " << res;
        std::cout << " | { val_flt: " << st_b.val_flt;
        std::cout << ", val_list: [";
        for (int i = 0; i < st_b.val_list.size(); i++) {
            std::cout << st_b.val_list[i];
            if (i + 1 != st_b.val_list.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "], ";
        std::cout << "{ s_int: " << st_b.val_struct.s_int;
        std::cout << ", s_str: " << st_b.val_struct.s_str;
        std::cout << "}" << std::endl;
    }
    {
        struct_b st_b;
        bool res = icu::readStruct(n, "struct_b_test", st_b);
        std::cout << "res: " << res;
        std::cout << " | { val_flt: " << st_b.val_flt;
        std::cout << ", val_list: [";
        for (int i = 0; i < st_b.val_list.size(); i++) {
            std::cout << st_b.val_list[i];
            if (i + 1 != st_b.val_list.size()) {
                std::cout << ", ";
            }
        }
        std::cout << "], ";
        std::cout << "{ s_int: " << st_b.val_struct.s_int;
        std::cout << ", s_str: " << st_b.val_struct.s_str;
        std::cout << "}" << std::endl;
    }
}
