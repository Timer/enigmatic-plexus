function score = score_family(j, ps, node_type, scoring_fn, ns, discrete, data, args)
% SCORE_FAMILY_COMPLETE Compute the score of a node and its parents given completely observed data
% score = score_family(j, ps, node_type, scoring_fn, ns, discrete, data, args)
%
% data(i,m) is the value of node i in case m (can be a cell array)
% args is a cell array containing optional arguments passed to the constructor,
% or is [] if none
%
% We create a whole Bayes net which only connects parents to node,
% where node has a CPD of the specified type (with default parameters).
% We then evaluate its score ('bic' or 'bayesian')

% We should use a cache to avoid unnecessary computation.
% In particular, log_marginal_prob_node for tabular CPDs calls gammaln
% and compute_counts, both of which are slow.

[n ncases] = size(data);
dag = zeros(n,n);
% SML added to sort ps b/c mk_bnet, learn_params use sorted ps to make
% CPTs
% Kevin had: if ~isempty(ps), dag(ps, j) = 1; end

% // this is sorted once here because later uses of ps should be sorted and
% // it saves time
if ~isempty(ps), dag(ps, j) = 1;, ps = sort(ps);, end
bnet = mk_bnet(dag, ns, 'discrete', discrete);

fname = sprintf('%s_CPD', node_type);
bnet.CPD{j} = feval(fname, bnet, j, args{:});
score = log_marg_prob_node(bnet.CPD{j}, data(j,:), data(ps,:));
end
