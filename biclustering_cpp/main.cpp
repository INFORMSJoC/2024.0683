#include <iostream>
#include <map>
#include <algorithm>
#include <armadillo>
#include "sdp_branch_and_bound.h"



// data full path
const char *data_path;
// full path of the log file
const char *log_path;
// full path of the output assignment
const char *result_path;
std::ofstream log_file;

// branch and bound tolerance
double branch_and_bound_tol;
// number of threads
int branch_and_bound_parallel;
// maximum number of nodes
int branch_and_bound_max_nodes;
// visiting strategy
int branch_and_bound_visiting_strategy;

// number of threads for the Matlab session at the root
int matlab_session_threads_root;
// number of threads for the Matlab session
int matlab_session_threads_child;

// full path of SDPNAL+
const char *sdp_solver_folder;
// SDPNAL+ accuracy tolerance
double sdp_solver_tol;
// SDPNAL+ verbosity level
int sdp_solver_verbose;

// cutting plane iterations
int cp_max_iter;
// cutting plane tolerance
double cp_tol;
// maximum number of valid inequalities
int cp_max_ineq;
// percentage of valid inequalities
double cp_perc_ineq;
// tolerance for checking violated constraints
double cp_eps_ineq;
// tolerance for checking inactive constraints
double cp_eps_active;

// full path of Gurobi
const char *gurobi_folder;
// Gurobi verbosity level
int heuristic_verbose;

/// Function that reads the parameters in the configuration file
/// @param config_file path of the configuration file
/// @return a map where the key is the parameter name and the value is its value
std::map<std::string, std::string> read_params(std::string &config_file) {

    std::map<std::string, std::string> config_map = {};

    std::ifstream cFile (config_file);
    if (cFile.is_open()) {
        std::string line;
        while (getline(cFile, line)){
            line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find('=');
            auto key = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            config_map.insert(std::pair<std::string, std::string>(key, value));
        }

    }
    else {
        std::cerr << "Couldn't open config file for reading.\n";
    }

    return config_map;
}

/// Function that reads the data matrix and its size
/// @param filename path of the data matrix
/// @param n number of rows
/// @param d number of columns
/// @return data matrix
arma::mat read_data(const char *filename, int &n, int &d) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << strerror(errno) << "\n";
        exit(EXIT_FAILURE);
    }
    // read the header n, d
    file >> n >> d;
    arma::mat Ws(n, d);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < d; j++) {
            file >> Ws(i, j);
        }
    }
    return Ws;
}

/// Run the branch-and-cut algorithm
///  <W_PATH> <K> <LOG_PATH> <RESULT_PATH>
void run(int argc, char **argv) {

    std::string config_file = "config.txt";
    std::map<std::string, std::string> config_map = read_params(config_file);

    if (argc < 5) {
        std::cerr << "Input: <W_PATH> <K> <LOG_PATH> <RESULT_PATH>" << std::endl;
        exit(EXIT_FAILURE);
    }

    data_path = argv[1];
    int n, m, k;
    arma::mat W = read_data(data_path, n, m);
    k = std::stoi(argv[2]);
    log_path = argv[3];
    log_file.open(log_path);
    result_path = argv[4];

    // branch and bound
    branch_and_bound_tol = std::stod(config_map["BRANCH_AND_BOUND_TOL"]);
    branch_and_bound_parallel = std::stoi(config_map["BRANCH_AND_BOUND_PARALLEL"]);
    branch_and_bound_max_nodes = std::stoi(config_map["BRANCH_AND_BOUND_MAX_NODES"]);
    branch_and_bound_visiting_strategy = std::stoi(config_map["BRANCH_AND_BOUND_VISITING_STRATEGY"]);

    // matlab
    matlab_session_threads_root = std::stoi(config_map["MATLAB_SESSION_THREADS_ROOT"]);
    matlab_session_threads_child = std::stoi(config_map["MATLAB_SESSION_THREADS_CHILD"]);

    // sdp solver
    sdp_solver_folder = config_map["SDP_SOLVER_FOLDER"].c_str();
    sdp_solver_tol = std::stod(config_map["SDP_SOLVER_TOL"]);
    sdp_solver_verbose = std::stoi(config_map["SDP_SOLVER_VERBOSE"]);

    // cutting plane
    cp_max_iter = std::stoi(config_map["CP_MAX_ITER"]);
    cp_tol = std::stod(config_map["CP_TOL"]);
    cp_max_ineq = std::stoi(config_map["CP_MAX_INEQ"]);
    cp_perc_ineq = std::stod(config_map["CP_PERC_INEQ"]);
    cp_eps_ineq = std::stod(config_map["CP_EPS_INEQ"]);
    cp_eps_active = std::stod(config_map["CP_EPS_ACTIVE"]);

    // heuristic
    gurobi_folder = config_map["GUROBI_FOLDER"].c_str();
    heuristic_verbose = std::stoi(config_map["GUROBI_VERBOSE"]);

    log_file << "\n" << "W_PATH: " << data_path << " " << n << " " << m << " " << k <<"\n";
    log_file << "RESULT_PATH: " << result_path << "\n";
    log_file << "LOG_PATH: " << log_path << "\n\n";

    log_file << "BRANCH_AND_BOUND_TOL: " << branch_and_bound_tol << "\n";
    log_file << "BRANCH_AND_BOUND_PARALLEL: " << branch_and_bound_parallel << "\n";
    log_file << "BRANCH_AND_BOUND_MAX_NODES: " <<  branch_and_bound_max_nodes << "\n";
    log_file << "BRANCH_AND_BOUND_VISITING_STRATEGY: " << branch_and_bound_visiting_strategy << "\n\n";

    log_file << "MATLAB_SESSION_THREADS_ROOT: " << matlab_session_threads_root << "\n";
    log_file << "MATLAB_SESSION_THREADS_CHILD: " << matlab_session_threads_child << "\n\n";

    log_file << "SDP_SOLVER_FOLDER: " << sdp_solver_folder << "\n";
    log_file << "SDP_SOLVER_TOL: " << sdp_solver_tol << "\n";
    log_file << "SDP_SOLVER_VERBOSE: " << sdp_solver_verbose << "\n\n";

    log_file << "CP_MAX_ITER: " << cp_max_iter << "\n";
    log_file << "CP_TOL: " << cp_tol << "\n";
    log_file << "CP_MAX_INEQ: " << cp_max_ineq << "\n";
    log_file << "CP_PERC_INEQ: " << cp_perc_ineq << "\n";
    log_file << "CP_EPS_INEQ: " << cp_eps_ineq << "\n";
    log_file << "CP_EPS_ACTIVE: " << cp_eps_active << "\n\n";

    log_file << "GUROBI_FOLDER: " << gurobi_folder << "\n";
    log_file << "GUROBI_VERBOSE: " << heuristic_verbose << "\n\n";

    sdp_branch_and_bound(W, k);

}

int main(int argc, char **argv) {

    run(argc, argv);

    return EXIT_SUCCESS;
}
