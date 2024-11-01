#ifndef CLUSTERING_SDP_BRANCH_AND_BOUND_H
#define CLUSTERING_SDP_BRANCH_AND_BOUND_H

#include <armadillo>
#include "MatlabEngine.hpp"
#include "MatlabDataArray.hpp"
#include "JobQueue.h"

typedef struct MatlabStruct {

    std::unique_ptr<matlab::engine::MATLABEngine> matlabPtr; // pointer to Matlab session
    matlab::data::ArrayFactory factory; // Matlab factory

} MatlabStruct;


typedef struct SharedData {

    // Between workers and main
    std::condition_variable mainConditionVariable;
    std::vector<bool> threadStates;

    // Queue of requests waiting to be processed
    JobAbstractQueue *queue;
    // This condition variable is used for the threads to wait until there is work to do
    std::condition_variable queueConditionVariable;
    // Mutex to protect queue
    std::mutex queueMutex;

    double global_lb;
    arma::sp_mat global_Xu; // row-assignment matrix
    arma::sp_mat global_Xv; // column-assignment matrix
    double gap; // relative optimality gap
    int n_nodes; // number of nodes

} SharedData;

typedef struct InputData {

    arma::mat W; // data matrix
    int k; // number of clusters

} InputData;


void sdp_branch_and_bound(arma::mat &W, int k);
std::pair<JobData *, JobData *> build_root_problem(MatlabStruct *matlab_struct, InputData *input_data, SharedData *shared_data);
std::pair<JobData *, JobData *> build_child_problem(int job_type, NodeData *job_data, InputData *input_data, SharedData *shared_data);

#endif //CLUSTERING_SDP_BRANCH_AND_BOUND_H
