#ifndef TP_WOLFENSTEIN_MAPPARSER_H
#define TP_WOLFENSTEIN_MAPPARSER_H


#include "yaml-cpp/yaml.h"
#include <string>
#include <unordered_map>
#include "server/game/coordinate.h"
class MapParser {
    private:

    public:
        YAML::Node config;
        explicit MapParser(const std::string& path);
        std::unordered_map<std::string, std::vector<Coordinate>> getCategory(const std::string& node);
        Coordinate getDimensions();
        std::unordered_map<std::string,
            std::vector<Coordinate>> getSpecificCategory(const std::string& category);

        ~MapParser();
};


#endif //TP_WOLFENSTEIN_MAPPARSER_H
