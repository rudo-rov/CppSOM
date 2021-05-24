#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <memory>

namespace som {

    class SourceFilesDir {
    public:
        SourceFilesDir(std::string sourceDirName) : m_sourceDirName(sourceDirName) {}
        ~SourceFilesDir();
        std::string getSourceDirName() const { return m_sourceDirName; }
        const std::vector<std::unique_ptr<std::ifstream>>& getSrcFiles() const { return m_srcFiles; } 

        bool loadSourceFiles();

    private:
        std::string m_sourceDirName;
        std::vector<std::unique_ptr<std::ifstream>> m_srcFiles;
    };

}