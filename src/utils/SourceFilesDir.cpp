#include <experimental/filesystem>
#include <iostream>
#include <memory>

#include "SourceFilesDir.h"
#include "Config.h"

namespace som {

    SourceFilesDir::~SourceFilesDir()
    {

    }

    bool SourceFilesDir::loadSourceFiles()
    {
        for (auto& file : std::experimental::filesystem::directory_iterator(m_sourceDirName)) {
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

        for (auto& file : std::experimental::filesystem::directory_iterator(coreLibPath)) {
            if (file.path().extension().string() == ".som") {
                std::cout << "Core library file: " << file.path().filename().string() << std::endl;
                std::unique_ptr<std::ifstream> fileStream(new std::ifstream(file.path().string()));
                if (!fileStream->is_open()) {
                    std::cerr << "Cannot open core library source file: " << file.path().string() << std::endl;
                    return false;
                }
                m_srcFiles.push_back(std::move(fileStream));
            }
        }

        std::cout << m_srcFiles.size() << " source file" << (m_srcFiles.size() == 1 ? " " : " s" ) << "loaded.\n"; 
        return true;
    }

}