#include "JobQueue.h"

// Function that compares the upper bounds of two branch-and-bound nodes
bool JobAbstractQueue::compareNode(JobData *a, JobData *b) {
    return a->node_data->node->ub > b->node_data->node->ub;
}

// Function that extracts a node from the queue
JobData *JobAbstractQueue::pop() {
    JobData *node = queue.front();
    queue.pop_front();
    return node;
}

// Function that checks if the queue is empty
bool JobAbstractQueue::empty() {
    return queue.empty();
}

// Function that sorts the nodes in the queue according to the upper bound
void JobAbstractQueue::sort() {
    std::sort(queue.begin(), queue.end(), compareNode);
}

// Function that extracts the node with the smallest upper bound
Node *JobAbstractQueue::getMinUb() {
    double min_ub = std::numeric_limits<double>::infinity();
    Node *min_node = nullptr;
    for (auto current : queue) {
        if (current->node_data->node->ub < min_ub) {
            min_ub = current->node_data->node->ub;
            min_node = current->node_data->node;
        }
    }
    return min_node;
}

// Function that extracts the node with the largest upper bound
Node *JobAbstractQueue::getMaxUb() {
    double max_ub = - std::numeric_limits<double>::infinity();
    Node *max_node = nullptr;
    for (auto current : queue) {
        if (current->node_data->node->ub > max_ub) {
            max_ub = current->node_data->node->ub;
            max_node = current->node_data->node;
        }
    }
    return max_node;
}

// For each node in the queue it prints its associated upper bound
void JobAbstractQueue::print() {
    for (auto &elem : queue) {
        std::cout << elem->node_data->node->ub << " ";
    }
    std::cout << "\n";
}

// It returns the size of the queue
int JobAbstractQueue::getSize() {
    return (int) queue.size();
}

// It pushes a new node in the queue
void JobQueue::push(JobData *node) {
    queue.push_back(node);
}

// It pushes a new node in the queue
void JobStack::push(JobData *node) {
    queue.push_front(node);
}

// It pushes a new node in the queue
void JobPriorityQueue::push(JobData *node) {
    queue.push_back(node);
    sort();
}
