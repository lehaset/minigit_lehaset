// File: log_command.h
#pragma once

#include "commit_manager.h"
#include "head_manager.h"
#include <iostream>

class LogCommand {
public:
    LogCommand(CommitManager& commitManager, HeadManager& headManager);
    void execute();
    
private:
    CommitManager& commitManager;
    HeadManager& headManager;
    
    void printCommit(const Commit& commit);
};