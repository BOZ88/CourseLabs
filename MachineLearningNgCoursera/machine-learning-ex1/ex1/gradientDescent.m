function [theta, J_history] = gradientDescent(X, y, theta, alpha, num_iters)
%GRADIENTDESCENT Performs gradient descent to learn theta
%   theta = GRADIENTDESENT(X, y, theta, alpha, num_iters) updates theta by 
%   taking num_iters gradient steps with learning rate alpha

% Initialize some useful values
m = length(y); % number of training examples
J_history = zeros(num_iters, 1);

for iter = 1:num_iters

    % ====================== YOUR CODE HERE ======================
    % Instructions: Perform a single gradient step on the parameter vector
    %               theta. 
    %
    % Hint: While debugging, it can be useful to print out the values
    %       of the cost function (computeCost) and gradient here.
    %

    oldTheta = theta;
    %theta(1) = oldTheta(1) - alpha ./ m .* (ones(1, m) * ((X * oldTheta - y) .* X(:,1)));
    %theta(2) = oldTheta(2) - alpha ./ m .* (ones(1, m) * ((X * oldTheta - y) .* X(:,2)));
    theta = oldTheta - alpha ./ m .* (ones(1,m) * ((X * oldTheta - y) .* X))';

    oldTheta
    theta
    cost = computeCost(X, y, theta);
    

    % ============================================================

    % Save the cost J in every iteration    
    J_history(iter) = cost;
    J_history;

end

end
