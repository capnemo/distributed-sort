#include <string>

#include "config.h"

#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

namespace globalConfig {
    bool initConfig(std::string fileName);
    void getValueByKey(std::string key, std::string& value);
    void getValueByKey(std::string key, uint64_t & value);
    void getLogFileName(const std::string stub, std::string& fileName);
}

#endif /* GLOBALCONFIG_H */
