#include <map>
#include <queue>
#include "serverTypes.h"

#ifndef INITIATOR_H
#define INITIATOR_H

class taskInitiator {
    public:
    taskInitiator(const strVec& inArgs, const std::string& iniId): 
                  inputArgs(inArgs), id(iniId) {}
    virtual void usage() = 0;
    virtual bool initialize() = 0;
    virtual void getNextTask(struct task& nT) = 0;
    virtual void evaluateResult(struct result& res) = 0;
    virtual bool isComplete() = 0;
    const std::string& getId() {return id;}

    protected:
    strVec inputArgs;
    std::string id;
    bool inflightError = false;
    bool complete = false;
};

class sortInitiator:public taskInitiator {
    public:
    sortInitiator(const strVec& inArgs): taskInitiator(inArgs, "sort") {}
    virtual void usage();
    virtual bool initialize();
    virtual void getNextTask(struct task& nT);
    virtual void evaluateResult(struct result& res);
    virtual bool isComplete();

    private:
    void getTaskId(std::string& id);
    void getTask(char type, const strVec& args, struct task& tsk);

    private:
    std::string inputFileName;
    std::string outputFileName;
    uint16_t numClients;
    std::queue<struct task> currTaskQ;
    std::map<std::string, struct task> inflightTasks;
    struct task mergeTask;

    uint16_t taskId = 0;
    uint32_t sortTasks = 0;
};

taskInitiator* findInitiator(const std::string& key, strVec& args);
#endif
