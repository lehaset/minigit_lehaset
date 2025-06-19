// File: head_manager.h
#pragma once

#include <string>
#include <fstream>
#include <filesystem>
#include <vector>

class HeadManager {
public:
    HeadManager(const std::string& minigitDir = ".minigit");
    
    std::string getCurrentCommit();
    void updateHead(const std::string& commitHash);
    void createBranch(const std::string& branchName, const std::string& commitHash);
    std::string getCurrentBranch();

private:
    std::filesystem::path minigitDir;
    
    std::string readFile(const std::filesystem::path& path);
    void writeFile(const std::filesystem::path& path, const std::string& content);
};