// File: main.cpp - Test Program
#include "commit_manager.h"
#include "head_manager.h"
#include "log_command.h"
#include <iostream>
#include <map>

// Helper function to create test environment
void setupTestEnvironment() {
    namespace fs = std::filesystem;
    fs::create_directory(".minigit");
    fs::create_directory(".minigit/refs");
    fs::create_directory(".minigit/refs/heads");
    fs::create_directory(".minigit/commits");
    
    // Create initial HEAD pointing to main branch
    std::ofstream head(".minigit/HEAD");
    head << "ref: refs/heads/main\n";
    
    // Create main branch file
    std::ofstream mainBranch(".minigit/refs/heads/main");
    mainBranch << ""; // Empty for first commit
}

int main() {
    try {
        // Setup test environment
        setupTestEnvironment();
        
        // Initialize managers
        HeadManager headManager;
        CommitManager commitManager;
        
        // Create sample blob hashes (would normally come from Person B)
        std::map<std::string, std::string> blobHashes1 = {
            {"file1.txt", "a1b2c3d4"},
            {"file2.txt", "e5f6a7b8"}
        };
        
        std::map<std::string, std::string> blobHashes2 = {
            {"file1.txt", "c3d4e5f6"},
            {"file3.txt", "a7b8c9d0"}
        };
        
        // Create first commit
        std::string parent = headManager.getCurrentCommit(); // Should be empty
        Commit commit1("Initial commit", parent, blobHashes1);
        commitManager.saveCommit(commit1);
        headManager.updateHead(commit1.getHash());
        
        // Create second commit
        Commit commit2("Add new feature", commit1.getHash(), blobHashes2);
        commitManager.saveCommit(commit2);
        headManager.updateHead(commit2.getHash());
        
        // Create a branch
        headManager.createBranch("feature", commit1.getHash());
        
        // Test log command
        std::cout << "=== Commit Log ===\n";
        LogCommand log(commitManager, headManager);
        log.execute();
        
        // Test loading a commit
        std::cout << "\n=== Inspecting Commit ===\n";
        Commit loaded = commitManager.loadCommit(commit1.getHash());
        std::cout << "Commit " << loaded.getHash() << " has "
                  << loaded.getBlobHashes().size() << " files\n";
        
        // Test branch information
        std::cout << "\n=== Branch Information ===\n";
        std::cout << "Current branch: " << headManager.getCurrentBranch() << "\n";
        std::cout << "Current commit: " << headManager.getCurrentCommit() << "\n";
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}