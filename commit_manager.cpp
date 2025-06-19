// File: commit_manager.cpp
#include "commit_manager.h"
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

CommitManager::CommitManager(const std::string& minigitDir) 
    : minigitDir(fs::path(minigitDir)) {
    commitsDir = this->minigitDir / "commits";
    fs::create_directories(commitsDir);
}

void CommitManager::saveCommit(const Commit& commit) {
    fs::path commitFile = commitsDir / commit.getHash();
    std::ofstream out(commitFile);
    out << commit.serialize();
}

Commit CommitManager::loadCommit(const std::string& commitHash) {
    fs::path commitFile = commitsDir / commitHash;
    if (!fs::exists(commitFile)) {
        throw std::runtime_error("Commit not found: " + commitHash);
    }
    
    std::ifstream in(commitFile);
    std::string content((std::istreambuf_iterator<char>(in)), 
                std::istreambuf_iterator<char>());
    return Commit::deserialize(content);
}