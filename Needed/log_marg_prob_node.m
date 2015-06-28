function L = log_marg_prob_node(CPD, self_ev, pev, usecell)
% LOG_MARG_PROB_NODE Compute sum_m log P(x(i,m)| x(pi_i,m)) for node i (tabular)
% L = log_marg_prob_node(CPD, self_ev, pev)
%
% This differs from log_prob_node because we integrate out the parameters.
% self_ev(m) is the evidence on this node in case m.
% pev(i,m) is the evidence on the i'th parent in case m (if there are any parents).
% (These may also be cell arrays.)

ncases = length(self_ev);
sz = CPD.sizes;
nparents = length(sz)-1;
CPD.dirichlet
  % We ignore the CPTs here and assume the prior has not been changed
  
  % We arrange the data as in the following example.
  % Let there be 2 parents and 3 cases. Let p(i,m) be parent i in case m,
  % and y(m) be the child in case m. Then we create the data matrix
  % 
  % p(1,1) p(1,2) p(1,3)
  % p(2,1) p(2,2) p(2,3)
  % y(1)   y(2)   y(3)
    data = [pev; self_ev];
  %S = struct(CPD); fprintf('log marg prob node %d, ps\n', S.self); disp(S.parents)
  counts = compute_counts(data, sz);
  L = dirichlet_score_family(counts, CPD.dirichlet);



