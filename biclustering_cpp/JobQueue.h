#ifndef CLUSTERING_JOBQUEUE_H
#define CLUSTERING_JOBQUEUE_H

#include <deque>
#include "Node.h"

/*
 * This class provides the implementation of the queue that stores the nodes of the branch-and-bound tree.
 * Depending on the selected visiting strategy, this class implements: Queue, Stack, and PriorityQueue
 */

class JobAbstractQueue {

protected:
    std::deque<JobData *> queue;

private:
    static bool compareNode(JobData *a, JobData *b);

public:

    virtual void push(JobData *node) = 0;

    virtual ~JobAbstractQueue() = default;

    JobData *pop();

    bool empty();

    void sort();

    Node *getMinUb();

    Node *getMaxUb();

    int getSize();

    void print();
};

class JobQueue : public JobAbstractQueue {

public:

    void push(JobData *node) override;

};

class JobStack : public JobAbstractQueue {

public:

    void push(JobData *node) override;

};

class JobPriorityQueue : public JobAbstractQueue {

public:

    void push(JobData *node) override;

};


#endif //CLUSTERING_JOBQUEUE_H
