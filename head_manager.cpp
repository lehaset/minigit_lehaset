// File: head_manager.cpp
#include "head_manager.h"
#include <iostream>
#include <stdexcept>

namespace fs = std::filesystem;

HeadManager::HeadManager(const std::string& minigitDir) 
    : minigitDir(fs::path(minigitDir)) {}

std::string HeadManager::getCurrentCommit() {
    fs::path headFile = minigitDir / "HEAD";
    
    if (!fs::exists(headFile)) {
        return ""; // First commit has no parent
    }
    
    std::string headContent = readFile(headFile);
    
    // Check if HEAD is a reference to a branch
    if (headContent.find("ref: ") == 0) {
        // Extract branch path
        std::string branchPath = headContent.substr(5);
        // Remove trailing newline if present
        if (!branchPath.empty() && branchPath.back() == '\n') {
            branchPath.pop_back();
        }
        
        fs::path branchFile = minigitDir / branchPath;
        return readFile(branchFile);
    }
    
    // Otherwise, HEAD contains the commit hash directly
    return headContent;
}

void HeadManager::updateHead(const std::string& commitHash) {
    fs::path headFile = minigitDir / "HEAD";
    std::string headContent = readFile(headFile);
    
    // If HEAD points to a branch, update the branch reference
    if (headContent.find("ref: ") == 0) {
        std::string branchPath = headContent.substr(5);
        // Remove trailing newline if present
        if (!branchPath.empty() && branchPath.back() == '\n') {
            branchPath.pop_back();
        }
        
        fs::path branchFile = minigitDir / branchPath;
        writeFile(branchFile, commitHash);
    } else {
        // Update HEAD directly
        writeFile(headFile, commitHash);
    }
}

void HeadManager::createBranch(const std::string& branchName, const std::string& commitHash) {
    fs::path branchFile = minigitDir / "refs" / "heads" / branchName;
    fs::create_directories(branchFile.parent_path());
    writeFile(branchFile, commitHash);
}

std::string HeadManager::getCurrentBranch() {
    fs::path headFile = minigitDir / "HEAD";
    if (!fs::exists(headFile)) return "";
    
    std::string headContent = readFile(headFile);
    if (headContent.find("ref: ") == 0) {
        std::string branchPath = headContent.substr(5);
        // Remove trailing newline if present
        if (!branchPath.empty() && branchPath.back() == '\n') {
            branchPath.pop_back();
        }
        return branchPath.substr(branchPath.find_last_of('/') + 1);
    }
    return ""; // Detached HEAD
}

// Helper functions
std::string HeadManager::readFile(const fs::path& path) {
    if (!fs::exists(path)) return "";
    
    std::ifstream in(path);
    std::string content((std::istreambuf_iterator<char>(in)), 
                std::istreambuf_iterator<char>());
    return content;
}

void HeadManager::writeFile(const fs::path& path, const std::string& content) {
    fs::create_directories(path.parent_path());
    std::ofstream out(path);
    out << content;
}