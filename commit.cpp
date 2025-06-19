// File: commit.cpp
#include "commit.h"
#include <sstream>
#include <iomanip>
#include <functional> // For std::hash

// Generate simple hash using std::hash
std::string Commit::generateHash(const std::string& input) {
    std::hash<std::string> hasher;
    size_t hashValue = hasher(input);
    
    // Convert to hex string
    std::stringstream ss;
    ss << std::hex << hashValue;
    return ss.str();
}

// Regular commit constructor
Commit::Commit(const std::string& message, const std::string& parent, 
               const std::map<std::string, std::string>& blobHashes)
    : message(message), blobHashes(blobHashes) {
    parents.push_back(parent);
    timestamp = std::time(nullptr);
    
    std::stringstream ss;
    ss << message << timestamp;
    for (const auto& parent : parents) ss << parent;
    for (const auto& [file, hash] : blobHashes) ss << file << hash;
    
    hash = generateHash(ss.str());
}

// Merge commit constructor
Commit::Commit(const std::string& message, const std::string& parent1, 
               const std::string& parent2, const std::map<std::string, std::string>& blobHashes)
    : message(message), blobHashes(blobHashes) {
    parents.push_back(parent1);
    parents.push_back(parent2);
    timestamp = std::time(nullptr);
    
    std::stringstream ss;
    ss << message << timestamp;
    for (const auto& parent : parents) ss << parent;
    for (const auto& [file, hash] : blobHashes) ss << file << hash;
    
    hash = generateHash(ss.str());
}

// Serialize commit to string
std::string Commit::serialize() const {
    std::stringstream ss;
    ss << "commit " << hash << "\n";
    for (const auto& parent : parents) {
        ss << "parent " << parent << "\n";
    }
    ss << "timestamp " << timestamp << "\n";
    ss << "message " << message << "\n";
    for (const auto& [file, blobHash] : blobHashes) {
        ss << "blob " << file << " " << blobHash << "\n";
    }
    return ss.str();
}

// Deserialize string to commit
Commit Commit::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string line;
    std::string hash;
    std::vector<std::string> parents;
    std::time_t timestamp = 0;
    std::string message;
    std::map<std::string, std::string> blobHashes;
    
    while (std::getline(ss, line)) {
        std::stringstream lineStream(line);
        std::string key;
        lineStream >> key;
        
        if (key == "commit") {
            lineStream >> hash;
        } else if (key == "parent") {
            std::string parent;
            lineStream >> parent;
            parents.push_back(parent);
        } else if (key == "timestamp") {
            lineStream >> timestamp;
        } else if (key == "message") {
            // Get the rest of the line as message
            std::getline(lineStream, message);
            // Remove leading space if present
            if (!message.empty() && message[0] == ' ') {
                message = message.substr(1);
            }
        } else if (key == "blob") {
            std::string filename, blobHash;
            lineStream >> filename >> blobHash;
            blobHashes[filename] = blobHash;
        }
    }
    
    // Create commit based on number of parents
    Commit commit("", "", blobHashes); // Temporary
    if (parents.size() == 1) {
        commit = Commit(message, parents[0], blobHashes);
    } else if (parents.size() >= 2) {
        commit = Commit(message, parents[0], parents[1], blobHashes);
    }
    
    // Set deserialized values
    commit.hash = hash;
    commit.timestamp = timestamp;
    return commit;
}

// Getters
std::string Commit::getHash() const { return hash; }
std::string Commit::getMessage() const { return message; }
std::vector<std::string> Commit::getParents() const { return parents; }
std::time_t Commit::getTimestamp() const { return timestamp; }
std::map<std::string, std::string> Commit::getBlobHashes() const { return blobHashes; }