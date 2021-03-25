#include <sstream>
#include <cstring>
#include "Config.h"

namespace som {

    bool CConfig::loadFromArgs(int argc, char** argv)
    {
        if (argc <= 2) {
            // TODO: Some help message
            m_lastError = "Invalid input arguments";
            return false;
        }
        
        m_input = argv[2];
        if (strcmp("-c", argv[1]) == 0) {
            m_mode = Mode::Compile;
        } else if (strcmp("-r", argv[1]) == 0) {
            m_mode = Mode::Interpret;
        } else {
            std::stringstream errMssg;
            errMssg << "Unknown option: " << argv[1];
            return false;
        }
        return true;
    }

}