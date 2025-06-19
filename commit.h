// File: commit.h
#pragma once

#include <string>
#include <vector>
#include <map>
#include <ctime>

class Commit {
public:
    // Constructor for regular commits
    Commit(const std::string& message, const std::string& parent, 
           const std::map<std::string, std::string>& blobHashes);
    
    // Constructor for merge commits
    Commit(const std::string& message, const std::string& parent1, 
           const std::string& parent2, const std::map<std::string, std::string>& blobHashes);
    
    // Getters
    std::string getHash() const;
    std::string getMessage() const;
    std::vector<std::string> getParents() const;
    std::time_t getTimestamp() const;
    std::map<std::string, std::string> getBlobHashes() const;
    
    // Serialization
    std::string serialize() const;
    static Commit deserialize(const std::string& data);

private:
    std::string hash;
    std::vector<std::string> parents;
    std::string message;
    std::time_t timestamp;
    std::map<std::string, std::string> blobHashes;
    
    static std::string generateHash(const std::string& input);
};