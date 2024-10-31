% This is the main function that is called by the C++ code. It solve the
% SDP relaxation in the child nodes
%
% :param W: weight matrix n x m
% :param k: number of biclusters
% :param init_ML_U: matrix of must-link constraints between vertices in U (n_ml_U x 2)
% :param init_CL_U: matrix of cannot-link constraints between vertices in U (n_cl_U x 2)
% :param init_ML_V: matrix of must-link constraints between vertices in V (n_ml_V x 2)
% :param init_CL_V: matrix of cannot-link constraints between vertices in V (n_cl_V x 2)
% :param init_Bcell: valid inequalities for the parent node
% :param global_lb: global lower bound
% :param global_Xu: global row-assignment matrix
% :param global_Xv: global column-assignment matrix
% :param params: configuration file (see solve_biclustering_shrinking.m)
% :returns Matlab struct that maps the C++ struct SDPResult

function result = call_solve_biclustering_child(W, k, init_ML_U, init_CL_U, init_ML_V, init_CL_V, init_B_cell, global_lb, global_Xu, global_Xv, params)
    
    disp(init_ML_U)
    disp(init_CL_U)
    disp(init_ML_V)
    disp(init_CL_V)

    [n, m] = size(W);
    result = solve_biclustering_shrinking(W, k, init_ML_U, init_CL_U, init_ML_V, init_CL_V, init_B_cell, global_lb, global_Xu, global_Xv, params);
    result.best_lb = max(result.lb_list);
    if result.cp_flag == 1
        result.branching_type = -1;
        result.i_idx = -1;
        result.j_idx = -1;
    else
        [branching_type, i_idx, j_idx, ~] = get_branching_pair(result.best_Z, n, m);
        result.branching_type = branching_type;
        result.i_idx = i_idx;
        result.j_idx = j_idx;
    end
    
    %disp(result)
    
end