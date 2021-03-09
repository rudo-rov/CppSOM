#include <filesystem>
#include <iostream>
#include <memory>

#include "SourceFilesDir.h"

namespace som {

    SourceFilesDir::~SourceFilesDir()
    {

    }

    bool SourceFilesDir::loadSourceFiles()
    {
        for (auto& file : std::filesystem::directory_iterator(m_sourceDirName)) {
            if (file.path().extension().string() == ".som") {
                std::cout << "Detected source file: " << file.path().filename().string() << std::endl;
                std::unique_ptr<std::ifstream> fileStream(new std::ifstream(file.path().string()));
                if (!fileStream->is_open()) {
                    std::cerr << "Cannot open source file: " << file.path().string() << std::endl;
                    return false;
                }
                m_srcFiles.push_back(std::move(fileStream));
            }
        }
        std::cout << m_srcFiles.size() << " source files loaded.\n"; 
        return true;
    }

}