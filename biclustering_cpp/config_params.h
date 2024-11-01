#ifndef CLUSTERING_CONFIG_PARAMS_H
#define CLUSTERING_CONFIG_PARAMS_H

#define MUST_LINK_U 0
#define CANNOT_LINK_U 1
#define MUST_LINK_V 2
#define CANNOT_LINK_V 3

#define BEST_FIRST 0
#define DEPTH_FIRST 1
#define BREADTH_FIRST 2

/*
 * Configuration parameters. They can all be set in the 'config.txt' file
 */

// data full path
extern const char *data_path;
// full path of the log file
extern const char *log_path;
// full path of the output assignment
extern const char *result_path;
extern std::ofstream log_file;

// branch and bound tolerance
extern double branch_and_bound_tol;
// number of threads
extern int branch_and_bound_parallel;
// maximum number of nodes
extern int branch_and_bound_max_nodes;
// visiting strategy
extern int branch_and_bound_visiting_strategy;

// number of threads for the Matlab session at the root
extern int matlab_session_threads_root;
// number of threads for the Matlab session
extern int matlab_session_threads_child;

// full path of SDPNAL+
extern const char *sdp_solver_folder;
// SDPNAL+ accuracy tolerance
extern double sdp_solver_tol;
// SDPNAL+ verbosity level
extern int sdp_solver_verbose;

// cutting plane iterations
extern int cp_max_iter;
// cutting plane tolerance
extern double cp_tol;
// maximum number of valid inequalities
extern int cp_max_ineq;
// percentage of valid inequalities
extern double cp_perc_ineq;
// tolerance for checking violated constraints
extern double cp_eps_ineq;
// tolerance for checking inactive constraints
extern double cp_eps_active;

// full path of Gurobi
extern const char *gurobi_folder;
// Gurobi verbosity level
extern int heuristic_verbose;

#endif //CLUSTERING_CONFIG_PARAMS_H
