#ifndef CLUSTERING_NODE_H
#define CLUSTERING_NODE_H

#include <armadillo>
#include <map>
#include <set>
#include <vector>
#include <MatlabDataArray/TypedArray.hpp>

#define BRANCH_ON_U 0
#define BRANCH_ON_V 1

/*
 * This script contains the data structures used to store branch-and-bound nodes
 */

class Node {

public:

    std::vector<std::pair<int, int>> global_ml_pairs_U; // must-link branching decision for U
    std::vector<std::pair<int, int>> global_cl_pairs_U; // cannot-link branching decision for U
    std::vector<std::pair<int, int>> global_ml_pairs_V; // must-link branching decision for V
    std::vector<std::pair<int, int>> global_cl_pairs_V; // cannot-link branching decision for V

    // lower bound
    double lb;
    // upper bound
    double ub;
    // node id
    int id;

};


class SDPNode : public Node {


public:

    std::vector<arma::sp_mat> B_vector; // B operator

};

typedef struct NodeData {

    SDPNode *node;
    int i; // branching index i
    int j; // branching index j

} NodeData;

typedef struct JobData {

    int type; // node type: root, must-link, cannot-link
    NodeData *node_data;

} JobData;

typedef struct SDPResult {

    int n; // current problem size
    int m; // current problem size
    double lb; // valid lower bound
    double ub; // valid upper bound
    arma::sp_mat Xu_assignment; // row-assignment matrix
    arma::sp_mat Xv_assignment; // column-assignment matrix
    int n_ineq; // number of valid inequalities
    int cp_iter; // number of cutting-plane iterations
    int cp_flag; // cutting-plane output flag
    int branching_type; // branch on rows or on columns
    int branching_i; // branching index i
    int branching_j; // branching index j
    std::vector<arma::sp_mat> B_vector; // B operator

} SDPResult;


#endif //CLUSTERING_NODE_H
