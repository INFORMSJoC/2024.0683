% Function that adds constraints to the Zvv block of smaller size
%
% :param n: number of rows
% :param m: number of columns
% :param e: vector of cardinalities in each connected component
% :returns: lhs constraint matrix in SDPNAL+ format

function [A_rowsum, A_trace] = Z_slice_Zvv_shrinking(n, m, e)

    % e_V = T_V 1_originalm (new size is m) Diag(e_V) = T_V T_V^\top

    A_rowsum = cell(1, m);
    A_trace = cell(1, 1);
    A_trace{1} = sparse([zeros(n, n), zeros(n, m); zeros(m, n), diag(e)]);
    
    %disp(full(A_trace{1}))
    
    c = 1;
    for i = (n+1):(m+n)
        A_rowsum{c} = sparse([repelem(i, m), n+1:m+n], [n+1:m+n, repelem(i, m)], [0.5 .* ones(m, 1) .* e; 0.5 .* ones(m, 1) .* e], n+m, n+m);
        %disp(full(A_rowsum{c}))
        c = c + 1;
    end
    
end