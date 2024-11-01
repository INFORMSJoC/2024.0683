% This is the main function that is called by the C++ code. It solve the
% SDP relaxation at the root node
%
% :param W: weight matrix n x m
% :param k: number of biclusters
% :param params: configuration file (see solve_biclustering_shrinking.m)
% :returns Matlab struct that maps the C++ struct SDPResult

function result = call_solve_biclustering_root(W, k, params)

    disp(size(W))
    disp(k)
    disp(params)

    [n, m] = size(W);
    result = solve_biclustering_root(W, k, params);
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