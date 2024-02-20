#include <yaml-cpp/yaml.h>

#include <string>
#include <vector>
#include <unordered_map>

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
bool readValueList(YAML::Node &node, const std::string &key, std::vector<T> &vlist)
{
    if ( node[key] ) {
        YAML::Node n = node[key];
        if (n.IsSequence()) {
            vlist.resize(0);
            int size = n.size();
            for(int i = 0; i < size; i++) {
                try {
                    T v = n[i].as<T>();
                    vlist.push_back(v);
                } catch (const std::exception&) {
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
bool readValueMap(YAML::Node &node, const std::string &key, std::unordered_map<std::string, T> &vmap)
{
    if ( node[key] ) {
        YAML::Node n = node[key];
        if (n.IsMap()) {
            for(auto it = n.begin(); it != n.end(); ++it) {
                try {
                    std::string key = it->first.as<std::string>();
                    T value = it->second.as<T>();
                    vmap.emplace(key, value);
                } catch (const std::exception&) {
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
bool readStructList(YAML::Node &node, const std::string &key, std::vector<T> &slist)
{
    if ( node[key] ) {
        YAML::Node n = node[key];
        if (n.IsSequence()) {
            slist.resize(0);
            int size = n.size();
            for(int i = 0; i < size; i++) {
                T s;
                bool res = readStruct(n[i], s);
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

#if 0
try {
    // check fname
    YAML::Node config = YAML::LoadFile(fname);
    return parse_node(config, params);
} catch (const std::exception&) {
    std::cerr << "parameter file [" << fname << "] can not open" << std::endl;
}

///

///
Node Load(const std::string& input);
Node Load(std::istream& input);
Node LoadFile(const std::string& filename);

std::vector<Node> LoadAll(const std::string& input);
std::vector<Node> LoadAll(std::istream& input);
std::vector<Node> LoadAllFromFile(const std::string& filename);
#endif


#if 0
template <typename T>
bool readStruct(YAML::Node &node, const std::string &key, T &obj);
template <typename T>
bool readStructList(YAML::Node &node, const std::string &key, std::vector<T> &obj);
#endif

#if 0
//// auto Conversion
struct CharacterType{
  std::string attribute1;
  std::string attribute2;
};

namespace YAML{

template<>
struct convert<CharacterType> {
    static bool decode(const Node& node, CharacterType& cType){
        cType.attribute1 = node["attribute1"].as<std::string>();
        cType.attribute2 = node["attribute2"].as<std::string>();
        return true;
    }
};

}
#endif
