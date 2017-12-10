#include <vector>
#include <string>
#include <map>
#ifndef MRCONFIG_H
#define MRCONFIG_H

class mrConfig {
    
    public:
    mrConfig(){}
    mrConfig(const std::string& configFile):fileName(configFile) {}
    bool loadTable();
    bool setConfigFile(const std::string& fileName);
    void getValue(const std::string key, uint64_t& value) const;
    void getValue(const std::string key, std::string& value) const;
    private:
    typedef std::map<std::string, std::string> table;
    typedef table::const_iterator tableIterator;
    std::string fileName;
    table configTable;
};
#endif /* MRCONFIG_H */
