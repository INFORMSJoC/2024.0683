[![INFORMS Journal on Computing Logo](https://INFORMSJoC.github.io/logos/INFORMS_Journal_on_Computing_Header.jpg)](https://pubsonline.informs.org/journal/ijoc)

# An SDP-based Branch-and-Cut Algorithm for Biclustering

This archive is distributed in association with the [INFORMS Journal on
Computing](https://pubsonline.informs.org/journal/ijoc) under the [MIT License](LICENSE).

**BICL-SDP** is an exact algorithm, based on the branch-and-cut technique, for solving the biclustering problem through the $k$-densest-disjoint biclique criterion described in the paper ["An SDP-based Branch-and-Cut Algorithm for Biclustering"](). This repository contains the C++ source code and the MATLAB scripts. Gene expression data used for the experiments can be found [here](https://schlieplab.org/Supplements/CompCancer/).

**Important: This code is being developed on an on-going basis at 
https://github.com/antoniosudoso/bicl-sdp. Please go there if you would like to
get a more recent version or would like support**

## Cite

To cite the contents of this repository, please cite both the paper and this repo, using their respective DOIs.

https://doi.org/10.1287/ijoc.2024.0683

https://doi.org/10.1287/ijoc.2024.0683.cd

Below is the BibTex for citing this snapshot of the repository.

```
@misc{sudoso2024,
  author =        {A. M. Sudoso},
  publisher =     {INFORMS Journal on Computing},
  title =         {An SDP-based Branch-and-Cut Algorithm for Biclustering},
  year =          {2024},
  doi =           {10.1287/ijoc.2024.0683.cd},
  url =           {https://github.com/INFORMSJoC/2024.0683},
  note =          {Available for download at https://github.com/INFORMSJoC/2024.0683},
}  
```

## Installation
**BICL-SDP** calls the semidefinite programming solver [SDPNAL+](https://blog.nus.edu.sg/mattohkc/softwares/sdpnalplus/) by using the [MATLAB Engine API](https://www.mathworks.com/help/matlab/calling-matlab-engine-from-cpp-programs.html) for C++. It requires the MATLAB engine library *libMatlabEngine* and the Matlab Data Array library *libMatlabDataArray*. **BICL-SDP** calls the linear programming solver [Gurobi](https://www.gurobi.com/) and uses [Armadillo](http://arma.sourceforge.net/) to handle matrices and linear algebra operations efficiently.


Ubuntu and Debian instructions:

1) Install MATLAB (>= 2021b)

2) Install Gurobi (>= 10.0.2)

3) Install CMake, OpenBLAS, LAPACK and Armadillo:
 ```
sudo apt-get update
sudo apt-get install cmake libopenblas-dev liblapack-dev libarmadillo-dev
```
4) Open the makefile `biclustering_cpp/Makefile` 
	- Set the variable `matlab_path` with your MATLAB folder.

5) Compile the code:

```
cd biclustering_cpp/
make
```

4) Download SDPNAL+, move the folder `biclustering_matlab` containing the MATLAB source code of **BICL-SDP** in the SDPNAL+ main directory and set the parameter `SDP_SOLVER_FOLDER` of the configuration file accordingly. This folder and its subfolders will be automatically added to the MATLAB search path when **BICL-SDP** starts.

This code has been tested under Ubuntu 22.04 LTS with MATLAB R2021b, Gurobi 10.0.2 and Armadillo 12.6.

## Configuration
Various parameters used in **BICL-SDP** can be modified in the configuration file `biclustering_cpp/config.txt`:

- `BRANCH_AND_BOUND_TOL` - optimality tolerance of the exact algorithm
- `BRANCH_AND_BOUND_PARALLEL` -  thread pool size: single thread (1), multi-thread (> 1)
- `BRANCH_AND_BOUND_MAX_NODES` - maximum number of nodes
- `BRANCH_AND_BOUND_VISITING_STRATEGY` - best first (0),  depth first (1), breadth first (2)
- `MATLAB_SESSION_THREADS_ROOT` - number of threads for the MATLAB session at the root noee
- `MATLAB_SESSION_THREADS_CHILD` - number of threads for the MATLAB session for child nodes
- `SDP_SOLVER_FOLDER` - full path of SDPNAL+ folder
- `SDP_SOLVER_TOL` - accuracy of SDPNAL+ in the relative KKT residual
- `SDP_SOLVER_VERBOSE` - do not display log (0), display log (1)
- `CP_MAX_ITER` - maximum number of cutting-plane iterations
- `CP_TOL` - tolerance between two consecutive cutting-plane iterations
- `CP_MAX_INEQ` - maximum number of valid inequalities to separate
- `CP_PERC_INEQ` - fraction of the most violated inequalities to add
- `CP_EPS_INEQ` - tolerance for checking the violation of the inequalities
- `CP_EPS_ACTIVE` - tolerance for detecting active inequalities
- `GUROBI_FOLDER` - Gurobi solver path
- `GUROBI_VERBOSE` - do not display log (0), display log (1)

## Source File Description

 Folder `biclustering_cpp`:

- `config_params.h` contains the configurable parameters found in the configuration file `config.txt`.
 - `JobQueue.cpp` and `JobQueue.h` provide the implementation of the queue that stores the nodes of the branch-and-bound tree.
 - `main_cpp` contains the routine that reads the parameters from the configuration file `config.txt` and executes the branch-and-cut algorithm.
 - `matlab_util.cpp` and `matlab_util.h` contain auxiliary functions that facilitate the interaction between C++ and MATLAB.
- `Node.h` contains the data structures used to represent the branch-and-bound nodes.
- `sdp_branch_and_bound.cpp` and `sdp_branch_and_bound.h` implement the overall branch-and-cut algorithm, including interfaces with the MATLAB functions `biclustering_matlab/call_solve_biclustering_root.m` and `biclustering_matlab/call_solve_biclustering_child.m`.
- `ThreadPool.cpp` and `ThreadPool.h` implement a configurable pool of POSIX threads to perform the branch-and-bound search in parallel.
- `util.cpp` and `util.h` contain auxiliary functions for formatting the branch-and-cut log file.


Folder `biclustering_matlab`:

- `add_cannot_link_Zuu.m` and `add_cannot_link_Zvv.m` construct the constraint matrix of the problem with cannot-link constraints.
- `biclustering_heuristic.m` implements the rounding algorithm using the solution of SDP relaxation.
- `build_biclustering.m` and `build_biclustering_shrinking.m` build the SDP relaxation for the root and child nodes, respectively.
- `build_T.m` builds the transformation matrix that allows reducing the size of the SDP relaxation.
- `call_solve_biclustering_root.m` and `call_solve_biclustering_child.m` contain the function calls for the bound computation, including interfaces with the C++ functions `biclustering_cpp/sdp_branch_and_bound.cpp` and `biclustering_cpp/sdp_branch_and_bound.h`.
- `get_branching_pair.m` computes the branching decision.
- `separate_inequalities.m` contains the separation routine for the hypermetric inequalities.
- `separate_pair_uu.m`, `separate_pair_vv.m`, `separate_triangle_uu.m`, and `separate_triangle_vv.m` contain the separation routines for pair and triangle inequalities for the blocks $Z_{uu}$ and $Z_{vv}$  of $Z$, respectively.
- `shrink_cuts.m` adjusts the indices of the inequalities when inheriting them from the parent to a child node.
- `solve_biclustering_root.m` and `solve_biclustering_shrinking.m` implement the SDP-based cutting-plane algorithm for the root node and the child nodes, respectively.
- `update_CL.m` updates cannot-link constraints when a size reduction is performed.
- `Z_slice_Zuu.m`, `Z_slice_Zuu_shrinking.m`, `Z_slice_Zvv.m`, and `Z_slice_Zvv_shrinking.m` add constraints to the blocks $Z_{uu}$ and $Z_{vv}$  of $Z$.

## Usage
```
cd biclustering_cpp/
./bb <W_PATH> <K> <LOG_PATH> <RESULT_PATH>
```
- `W_PATH` - full path of the data matrix
- `K` - number of biclusters
- `LOG_PATH` - path of the log file
- `RESULT_PATH` - path of the optimal bicluster assignment matrices

File `W_PATH` contains the weights `w_ij` and the must include an header line with the number of rows `n` and columns `m`:

```
n m
w_11 w_12 ... w_1m
w_21 w_22 ... w_2m
...
...
w_n1 w_n2 ... w_nm
```

## Log

The log file reports the progress of the algorithm:

- `N` - number of rows at the current node
- `M` - number of columns at the current node
- `ID_PAR` - id of the parent node
- `ID` - id of the current node
- `UB_PAR` - upper bound of the parent node
- `UB` - upper bound of the current node
- `TIME (s)` - running time in seconds of the current node
- `CP_ITER` - number of cutting-plane iterations
- `CP_FLAG` - termination flag of the cutting-plane procedure
    - `-2` - maximum number of iterations
    - `-1` - SDP not solved or partially solved successfully
    -  ` 0` - no violated inequalities
    -  ` 1` - node must be pruned
    -  ` 2` - upper bound greater than the previous one
    -  ` 3` - upper bound decrease is not sufficiently large
- `CP_INEQ` - number of inequalities added in the last cutting-plane iteration
- `LB` - current lower bound
- `BEST_LB` - global lower bound
- `SET` - vertex set selection for branching
    -  `U` - branch on the vertices in U
    -  `V` - branch on the vertices in V
    -  `-1` - branching is not needed
- `I J` - indices of branching decision
- `NODE_GAP` - gap at the current node
- `GAP` - overall gap 
- `OPEN` - number of open nodes
