#ifndef __IRSL_SIMPLE_YAML_PARSER_HPP__
#define __IRSL_SIMPLE_YAML_PARSER_HPP__

#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>
#include <unordered_map>

namespace irsl_common_utils
{

template <typename T>
using yamlList = std::vector<T>;

template <typename T>
using yamlMap  = std::unordered_map<std::string, T>;

//// read simple (require to define convert)
template <typename T>
bool readValue(YAML::Node &node, const std::string &key, T &value)
{
    if ( node[key] ) {
        YAML::Node n = node[key];
        try {
            T res = n.as<T>();
            value = res;
            return true;
        } catch (const std::exception&) {
            return false;
        }
    }
    return false;
}

template <typename T>
bool readList(const YAML::Node &node, yamlList<T> &vlist)
{
    if (node.IsSequence()) {
        vlist.resize(0);
        int size = node.size();
        for(auto it = node.begin(); it != node.end(); ++it) {
            try {
                T v = (*it).as<T>();
                vlist.push_back(v);
            } catch (const std::exception&) {
                return false;
            }
        }
        return true;
    }
    return false;
}

template <typename T>
bool readValueList(YAML::Node &node, const std::string &key, yamlList<T> &vlist)
{
    if ( node[key] ) {
        YAML::Node n = node[key];
        return readList<T>(n, vlist);
    }
    return false;
}

template <typename T>
bool readMap(const YAML::Node &node, yamlMap<T> &vmap)
{
    if (node.IsMap()) {
        for(auto it = node.begin(); it != node.end(); ++it) {
            try {
                std::string key = it->first.as<std::string>();
                T value = it->second.as<T>();
                vmap.emplace(key, value);
            } catch (const std::exception&) {
                return false;
            }
        }
        return true;
    }
    return false;
}

template <typename T>
bool readValueMap(YAML::Node &node, const std::string &key, yamlMap<T> &vmap)
{
    if ( node[key] ) {
        YAML::Node n = node[key];
        return readMap(node, vmap);
    }
    return false;
}

//// read complex (require to define readSingleStruct)
template <typename T>
bool readSingleStruct(const YAML::Node &node, T &obj)
{
    try {
        obj = node.as<T>();
    } catch (const std::exception&) {
        return false;
    }
    return true;
}
template <typename T>
bool readStruct(YAML::Node &node, const std::string &key, T &obj)
{
    if ( node[key] ) {
        YAML::Node n = node[key];
        bool res = readSingleStruct(node, obj);
        if (res) {
            return true;
        }
    }
    return false;
}
template<> bool readStruct(YAML::Node &node, const std::string &key, yamlList<int> &obj)
{
    return readValueList(node, key, obj);
}
template<> bool readStruct(YAML::Node &node, const std::string &key, yamlList<double> &obj)
{
    return readValueList(node, key, obj);
}
template<> bool readStruct(YAML::Node &node, const std::string &key, yamlList<bool> &obj)
{
    return readValueList(node, key, obj);
}
template<> bool readStruct(YAML::Node &node, const std::string &key, yamlList<std::string> &obj)
{
    return readValueList(node, key, obj);
}

template <typename T>
bool readStructList(YAML::Node &node, const std::string &key, yamlList<T> &slist)
{
    if ( node[key] ) {
        YAML::Node n = node[key];
        if (n.IsSequence()) {
            slist.resize(0);
            int size = n.size();
            for(auto it = n.begin(); it != n.end(); ++it) {
                T s;
                bool res = readSingleStruct<T>(*it, s);
                if (res) {
                    slist.push_back(s);
                } else {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }
    return false;
}

template <typename T>
bool readStructMap(YAML::Node &node, const std::string &key, yamlMap<T> &smap)
{
    if ( node[key] ) {
        YAML::Node n = node[key];
        if (n.IsMap()) {
            for(auto it = n.begin(); it != n.end(); ++it) {
                std::string key = it->first.as<std::string>();
                T s;
                bool res = readSingleStruct<T>(it->second, s);
                if(res) {
                    smap.emplace(key, s);
                } else {
                    return false;
                }
            }
            return true;
        } else {
            return false;
        }
    }
    return false;
}

}// namespace

#endif // __IRSL_SIMPLE_YAML_PARSER_HPP__

#if 0
//// loading parsing yaml
Node Load(const std::string& input);
Node Load(std::istream& input);
Node LoadFile(const std::string& filename);

std::vector<Node> LoadAll(const std::string& input);
std::vector<Node> LoadAll(std::istream& input);
std::vector<Node> LoadAllFromFile(const std::string& filename);
#endif

#if 0
//// YAML auto Conversion
struct MyType{
  std::string attribute1;
  std::string attribute2;
};

namespace YAML{

template<>
struct convert<MyType> {
    static bool decode(const Node& node, MyType& myType){
        myType.attribute1 = node["attribute1"].as<std::string>();
        myType.attribute2 = node["attribute2"].as<std::string>();
        return true;
    }
};

}
#endif
