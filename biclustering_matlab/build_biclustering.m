% Build the SDP relaxation of the k-densest disjoint biclique problem
%
% :param W: data matrix (n x m)
% :param k: number of bicliques
% :returns [blk, At, b, c, L]: problem data in SDPNAL+ format

function [blk, At, b, C, L] = build_biclustering(W, k)

    [n, m] = size(W);
        
    W_full = 0.5*[zeros(n, n), W; W', zeros(m, m)];
    
    [Zuu_rowsum, Zuu_trace] = Z_slice_Zuu(n, m);
    [Zvv_rowsum, Zvv_trace] = Z_slice_Zvv(n, m);

    Acell = [Zuu_rowsum, Zuu_trace, Zvv_rowsum, Zvv_trace];
    b = [ones(n, 1); k; ones(m, 1); k];
        
    blk = cell(1);
    blk{1,1} = 's';
    blk{1,2} = n+m;
    
    At = svec(blk, Acell, 1);

    C = cell(1);
    C{1} = -sparse(W_full);

    L = cell(1);
    L{1} = 0; % we want Z >= 0
    
end