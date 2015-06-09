function dag = learn_struct_K2(data, ns, order, varargin)
% LEARN_STRUCT_K2 Greedily learn the best structure compatible with a fixed node ordering
  % best_dag = learn_struct_K2(data, node_sizes, order, ...)
%
  % data(i,m) = value of node i in case m (can be a cell array).
  % node_sizes(i) is the size of node i.
  % order(i) is the i'th node in the topological ordering.
%
% The following optional arguments can be specified in the form of name/value pairs:
% [default value in brackets]
%
% max_fan_in - this the largest number of parents we allow per node [N]
% scoring_fn - 'bayesian' or 'bic' [ 'bayesian' ]
%              Currently, only networks with all tabular nodes support Bayesian scoring.
% type       - type{i} is the type of CPD to use for node i, where the type is a string
%              of the form 'tabular', 'noisy_or', 'gaussian', etc. [ all cells contain 'tabular' ]
% params     - params{i} contains optional arguments passed to the CPD constructor for node i,
%              or [] if none.  [ all cells contain {'prior', 1}, meaning use uniform Dirichlet priors ]
% discrete   - the list of discrete nodes [ 1:N ]
% clamped    - clamped(i,m) = 1 if node i is clamped in case m [ zeros(N, ncases) ]
% verbose    - 'yes' means display output while running [ 'no' ]
%
% e.g., dag = learn_struct_K2(data, ns, order, 'scoring_fn', 'bic', 'params', [])
%
% To be backwards compatible with BNT2, you can also specify arguments as follows
%   dag = learn_struct_K2(data, node_sizes, order, max_fan_in)    
%
% This algorithm is described in
% - Cooper and Herskovits,  "A Bayesian method for the induction of probabilistic
%      networks from data", Machine Learning Journal 9:308--347, 1992

[n ncases] = size(data);

% set default params
type = cell(1,n);
params = cell(1,n);
for i=1:n
  type{i} = 'tabular';
  %params{i} = { 'prior', 1 };
  params{i} = { 'prior_type', 'dirichlet', 'dirichlet_weight', 1 };
end
scoring_fn = 'bayesian';
discrete = 1:n;
clamped = zeros(n, ncases);

max_fan_in = n;
verbose = 0;

args = varargin;
nargs = length(args);
if length(args) > 0 
  if isstr(args{1})
    for i=1:2:nargs
      switch args{i},
       case 'verbose',    verbose = strcmp(args{i+1}, 'yes');
       case 'max_fan_in', max_fan_in = args{i+1}; 
       case 'scoring_fn', scoring_fn = args{i+1};
       case 'type',       type = args{i+1}; 
       case 'discrete',   discrete = args{i+1}; 
       case 'clamped',    clamped = args{i+1}; 
