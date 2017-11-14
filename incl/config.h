#include <vector>
#include <string>
#include <map>
#ifndef MRCONFIG_H
#define MRCONFIG_H

class mrConfig {
    
    public:
    mrConfig(const std::string& configFile):fileName(configFile) {}
    bool loadTable();
    void getValue(const std::string key, std::string& value) const;
    static void getLogFileName(const mrConfig& conf, const std::string stub, 
                               std::string& fileName);
    private:
    typedef std::map<std::string, std::string> table;
    typedef table::const_iterator tableIterator;
    std::string fileName;
    table configTable;
};
#endif /* MRCONFIG_H */
