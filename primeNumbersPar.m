function [total time] = prime(lower, upper)
%% prime counts the prime numbers between lower and upper bounds
% poolObj = parpool(4);
  poolObj = parpool('local',4);
%  lower = 1
%  upper = 100000
  total = 0;
  % start a timer to benchmark the main loop
  ticID = tic;
 % for i = lower : upper
   parfor i = lower : upper
    isprime = 1; % TRUE
        for j = 2 : i-1
            if ( mod (i, j) == 0 )
                isprime = 0; %FALSE
                break
            end
        end
        total = total + isprime;
    end

 % stop the timer
  time = toc(ticID)
%end
delete (poolObj);
