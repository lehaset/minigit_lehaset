// File: log_command.cpp
#include "log_command.h"
#include <iomanip>
#include <ctime>

LogCommand::LogCommand(CommitManager& commitManager, HeadManager& headManager)
    : commitManager(commitManager), headManager(headManager) {}

void LogCommand::execute() {
    std::string currentHash = headManager.getCurrentCommit();
    
    while (!currentHash.empty()) {
        try {
            Commit commit = commitManager.loadCommit(currentHash);
            printCommit(commit);
            
            // Move to parent (for simplicity, just take first parent)
            auto parents = commit.getParents();
            currentHash = parents.empty() ? "" : parents[0];
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            break;
        }
    }
}

void LogCommand::printCommit(const Commit& commit) {
    // Format timestamp
    char buffer[80];
    std::time_t timestamp = commit.getTimestamp();
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&timestamp));
    
    // Print commit info
    std::cout << "commit " << commit.getHash() << "\n";
    std::cout << "Author: Person A <personA@minigit>\n";
    std::cout << "Date:   " << buffer << "\n";
    std::cout << "\n    " << commit.getMessage() << "\n\n";
}