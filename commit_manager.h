// File: commit_manager.h
#pragma once

#include "commit.h"
#include <filesystem>
#include <string>

class CommitManager {
public:
    CommitManager(const std::string& minigitDir = ".minigit");
    
    void saveCommit(const Commit& commit);
    Commit loadCommit(const std::string& commitHash);
    
private:
    std::filesystem::path minigitDir;
    std::filesystem::path commitsDir;
};