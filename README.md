# **MiniGit Commit System Technical Report**  
**Lehaset Yohannes(Person A): Commit System & Core Logic**  

---

## **1. Overview**  
This document explains the implementation of the **Commit System** in MiniGit, focusing on the data structures, algorithms, and integration points with other team members. The system manages:  
- Commit creation and storage  
- Commit history traversal (`log` command)  
- HEAD reference management  
- Support for merge commits  

---

## **2. Data Structures & Algorithms**  

### **2.1 Commit DAG (Directed Acyclic Graph)**  
- **Structure**: Commits form a DAG where:  
  - Normal commits have **1 parent** (linear history)  
  - Merge commits have **2 parents** (branch merging)  
- **Traversal**:  
  - `log` walks backward from `HEAD` using parent pointers (linked-list traversal for linear history, DAG traversal for merges).  
  - Implemented via recursive/iterative depth-first search.  

### **2.2 Key Data Structures**  
| Structure       | Purpose                          | DSA Concept        |  
|----------------|----------------------------------|-------------------|  
| `Commit`       | Stores metadata + file snapshots | Node in DAG       |  
| `std::map`     | Tracks filenames → blob hashes   | Hash Table        |  
| `std::vector`  | Stores parent commit hashes      | Dynamic Array     |  
| File System    | Persists commits/blobs           | Tree (directories)|  

### **2.3 Hashing**  
- Uses **`std::hash`** (C++ built-in) for commit IDs.  
- Blob hashes (from Person B) are stored in commits but not computed here.  

---

## **3. How the Code Works**  

### **3.1 Class Initialization**  
#### **CommitManager**  
- **Purpose**: Saves/loads commits to/from `.minigit/commits/`.  
- **How to Initialize**:  
  ```cpp
  // Default path: .minigit/
  CommitManager commitManager; 

  // OR specify custom path:
  CommitManager commitManager("path/to/repo");
  ```

#### **HeadManager**  
- **Purpose**: Manages `HEAD` and branch references.  
- **How to Initialize**:  
  ```cpp
  // Default path: .minigit/
  HeadManager headManager;

  // OR specify custom path:
  HeadManager headManager("path/to/repo");
  ```

### **3.2 Commit Creation**  
1. **Input**: Commit message, parent hashes, and `blobHashes` (from Person B).  
2. **Process**:  
   - Generates a unique hash from metadata.  
   - Saves to `.minigit/commits/<hash>`.  
3. **Example**:  
   ```cpp
   // Initialize managers
   CommitManager commitManager;
   HeadManager headManager;

   // Get blob hashes from Person B's staging system
   std::map<std::string, std::string> blobs = {
       {"file1.txt", "a1b2c3"},
       {"file2.txt", "d4e5f6"}
   };

   // Get current HEAD commit as parent (empty string if first commit)
   std::string parentHash = headManager.getCurrentCommit();

   // Create and save commit
   Commit commit("Initial commit", parentHash, blobs);
   commitManager.saveCommit(commit);

   // Update HEAD to point to the new commit
   headManager.updateHead(commit.getHash());
   ```

### **3.3 Log Command**  
- Traverses from `HEAD` backward:  
  ```cpp
  // Initialize managers
  CommitManager commitManager;
  HeadManager headManager;

  // Print commit history
  LogCommand log(commitManager, headManager);
  log.execute();
  ```
- **Output**:  
  ```
  commit 1a2b3c
  Author: Person A
  Date: 2023-10-20
      Initial commit
  ```

---

## **4. Integration with Team Members**  

### **4.1 Person B (Branching & Merge System)**  
#### **Branch Creation**  
```cpp
HeadManager headManager;
headManager.createBranch("feature", commitHash); 
```

#### **Merge Commit**  
```cpp
CommitManager commitManager;
std::map<std::string, std::string> mergedBlobs = /* from merge logic */;

// parent1 = Current branch tip, parent2 = Other branch tip
Commit mergeCommit("Merge feature", parent1, parent2, mergedBlobs);
commitManager.saveCommit(mergeCommit);
headManager.updateHead(mergeCommit.getHash());
```

### **4.2 Person C (CLI & File Handling)**  
#### **Initialization**  
```cpp
// In Person C's init command:
CommitManager commitManager;  // Creates .minigit/commits/
HeadManager headManager;      // Creates .minigit/HEAD
```

#### **Staging Files**  
```cpp
// After hashing files, pass blobs to Person A:
std::map<std::string, std::string> blobs = ...;
Commit commit(message, parentHash, blobs);
```

---

## **5. Usage Examples**  

### **5.1 Full Workflow Example**  
```cpp
#include "commit_manager.h"
#include "head_manager.h"

int main() {
    // Initialize (Person C's init would do this)
    CommitManager commitManager;
    HeadManager headManager;

    // Person B stages files (example data)
    std::map<std::string, std::string> blobs = {
        {"main.cpp", "3a4b5c"},
        {"README.md", "1f2e3d"}
    };

    // Person A creates commit
    std::string parent = headManager.getCurrentCommit(); // Empty for first commit
    Commit commit("Add core files", parent, blobs);
    commitManager.saveCommit(commit);
    headManager.updateHead(commit.getHash());

    // Person A prints history
    LogCommand log(commitManager, headManager);
    log.execute();
}
```

---

## **6. Conclusion**  
This system provides:  
✅ Commit storage and retrieval  
✅ Branch-aware HEAD management  
✅ History traversal (`log`)  
✅ Clear integration points for the team  

---

## **7.Key Takeaways for the Team**  
1. **Initialize managers once** (in `main` or CLI setup).  
2. **Always update HEAD** after creating a commit.  
3. **Pass blob hashes** from staging area to `Commit`.  
4. **Merge commits require 2 parents** (Person B’s responsibility).  
5. **Use the same `.minigit` path** across all components for consistency.  

**Compile & Test**:  
```bash
g++ -std=c++17 *.cpp -o minigit
./minigit
```  

**Dependencies**: None (uses C++17 standard library).  

**End of Report**  
*MiniGit Team | Lehaset Yohannes (Person A, Commit System)*