%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % This example is taken from 
% % % [1] Zacchia Lun, Y., Wheatley, J., D'Innocenzo, A. and Abate, A., 2018. 
% % %     Approximate Abstractions of Markov Chains with Interval Decision 
% % %     Processes (Extended Version). arXiv preprint arXiv:1804.08554.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tpm = ...
      [0.05, 0.05, 0.05, 0.05, 0.15, 0.15, 0.15, 0.300, 0.020, 0.010, 0.020; 
       0.04, 0.04, 0.05, 0.05, 0.14, 0.17, 0.15, 0.280, 0.030, 0.030, 0.020;
       0.01, 0.01, 0.10, 0.05, 0.14, 0.15, 0.15, 0.200, 0.190, 0.000, 0.000;
       0.06, 0.04, 0.06, 0.07, 0.16, 0.17, 0.15, 0.070, 0.030, 0.050, 0.140;
       0.01, 0.01, 0.00, 0.00, 0.96, 0.00, 0.00, 0.005, 0.005, 0.005, 0.005;
       0.00, 0.01, 0.01, 0.01, 0.01, 0.95, 0.01, 0.000, 0.000, 0.000, 0.000;
       0.00, 0.00, 0.00, 0.00, 0.25, 0.50, 0.25, 0.000, 0.000, 0.000, 0.000;
       0.15, 0.15, 0.15, 0.00, 0.15, 0.15, 0.15, 0.020, 0.030, 0.030, 0.020;
       0.15, 0.15, 0.06, 0.06, 0.14, 0.13, 0.15, 0.040, 0.030, 0.030, 0.060;
       0.40, 0.04, 0.02, 0.01, 0.14, 0.13, 0.15, 0.100, 0.010, 0.000, 0.000;
       0.44, 0.00, 0.00, 0.00, 0.00, 0.00, 0.43, 0.000, 0.000, 0.000, 0.130];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
label={'a','a','a','a','b','b','b','c','c','c','c'};
% % % Note that labels cannot start with numbers
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
initialState=1;