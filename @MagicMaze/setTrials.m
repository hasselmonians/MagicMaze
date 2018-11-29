function self = setTrials(self, k, n)

  % sets the trials property with a vector of n integers randomly chosen from k

  % Arguments:
    % self: the MagicMaze object
    % n: the number of trials to generate
    % k: a vector of integers to pick the random trials from
  % Outputs:
    % automatically stores the trials in the self.trials property


  if nargin < 2
    % set to defaults
    k     = 1:4;
    n     = 10;
  end

  if nargin < 3
    % set to defaults
    n     = 10;
  end

  self.trials = randsample(k, n);

end % function
