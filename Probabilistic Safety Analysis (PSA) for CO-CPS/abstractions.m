%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % This script provides a method for reducing the state space of 
% % % discrete-time labelled Markov chains (LMCs) that is based on a notion 
% % % of approximate probabilistic bisimulation. It guarantees the minimal 
% % % abstraction error for any given partitioning of the state space 
% % % dictated by the labelling. The theoretical foundations can be found in 
% % % [1] Zacchia Lun, Y., Wheatley, J., D'Innocenzo, A. and Abate, A., 2018. 
% % %     Approximate Abstractions of Markov Chains with Interval Decision 
% % % 	  Processes. IFAC-PapersOnLine 51(16): 91--96. 6th IFAC Conference on 
% % % 	  Analysis and Design of Hybrid Systems ADHS 2018.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
close all
clear
clc
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % Please remember to add the current working directory to the Matlab's 
% % % search path (type "help path" for a guidance specific to your
% % % operating system).
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % Please define the concrete labeled Markov chain in config.m file by 
% % % specifying 
% % % 
% % % -- the transition probability matrix in   tpm
% % % -- the assigned labelling in              label
% % % -- the initial state in                   initialState
% % %
% % % See exampleConfig.m for the example presented in [1].
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
prompt = 'Welcome! Do you want to load the example? Y/N [N]: ';
str = input(prompt,'s');
clear prompt
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % str = 'Y'; % % % <== For debug only, should be removed at the end...
if or(str == 'y', str == 'Y')
    str = 'Y';
else
    str = 'N';    
end
if str == 'Y'
    run('exampleConfig.m');
else
    run('config.m');
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % Initialization of the structure of a concrete labelled Markov chain:
lmc.tpm=tpm;
lmc.label=label;
lmc.initial=initialState;
clear str tpm label initialState
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % Check on consistency of the provided concrete labelled Markov chain:
if isempty(lmc.tpm)==1
    error01 = 'The concrete labelled Markov chain has empty transition probability matrix.';
    error(error01)
end
if any(cellfun('isempty',lmc.label)) == 1
    error02 = 'There are states without labels';
    error(error02)
end
if length(lmc.tpm)~=numel(lmc.label)
    error03 = 'There is a size mismatch between labelled states and transition probability matrix';
    error(error03)
end
% if 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % [labels, indStates, indLabels] = unique(lmc.label,'stable');
[labels, ~, indLabels] = unique(lmc.label,'stable');
for i = 1 : numel(labels)
    partition.(labels{i}) = find(indLabels==i);
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % Define transition probability rows:
for i = 1 : numel(labels)
    for j = 1 : length(partition.(labels{i}))
        tpr.(labels{i}){j} = zeros(1,numel(labels));
        for k = 1 : numel(labels)
            tpr.(labels{i}){j}(k) = sum(lmc.tpm(partition.(labels{i})(j),...
                partition.(labels{k})));
        end
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % Compute epsilon^i_j, i.e. abstractionError.(labels{i}){j}:
for i = 1 : numel(labels)
    evaluations = combnk(1:length(partition.(labels{i})),2);
    for j = 1 : length(partition.(labels{i}))
        [evaluatedRows,~] = find(evaluations == j);
        err = ones(1,length(evaluatedRows));
        for k = 1 : length(evaluatedRows)
            err(k) = norm(tpr.(labels{i}){evaluations(evaluatedRows(k),1)} - ...
            tpr.(labels{i}){evaluations(evaluatedRows(k),2)},Inf);
        end
        abstractionError.(labels{i})(j) = max(err);
    end    
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % Computing of the minimum error obtainable by the standard approach to
% % % approximate probabilistic bisimulation based abstaraction and the
% % % related choice of the representative points and the derivation of 
% % % the abstract transition probability matrix:
stdAbstraction.error = 0;
stdAbstraction.tpm = zeros(numel(labels));
for i = 1 : numel(labels)
    [minErr, ind] = min(abstractionError.(labels{i}));
    % % % Obs. If the values of abstractionError.(labels{i}) contain more 
    % % % than one minimal element, min returns the index of the first one.
    if minErr > stdAbstraction.error
        stdAbstraction.error = minErr;
    end
    % % % representativePoint.(labels{i}) = [minErr,ind,partition.(labels{i})(ind)];
    stdAbstraction.tpm(i,:) = tpr.(labels{i}){ind};
    stdAbstraction.label{i} = labels{i};
    if labels{i} == lmc.label{lmc.initial}
        stdAbstraction.initial = i;
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % Compute minimum theoretical error beta:
for i = 1 : numel(labels)
    beta.(labels{i}) = max(abstractionError.(labels{i}))/2;
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % Compute the starting transition set (sts) and 
% % %         the optimal transition set (ots):
for i = 1 : numel(labels)
    for j = 1 : numel(labels)
        elem = zeros(1,length(tpr.(labels{i})));
        for k = 1 : length(tpr.(labels{i}))        
            elem(k) = tpr.(labels{i}){k}(j);
        end
        sts.(labels{i}).lower(j) = min(elem);
        sts.(labels{i}).upper(j) = max(elem);
        ots.(labels{i}).upper(j) = sts.(labels{i}).lower(j) + beta.(labels{i});
        ots.(labels{i}).lower(j) = sts.(labels{i}).upper(j) - beta.(labels{i});       
    end
    if sum(ots.(labels{i}).lower) > 1
        beta.(labels{i}) = beta.(labels{i}) + (sum(ots.(labels{i}).lower)-1)/length(ots.(labels{i}).lower);
        for j = 1 : numel(labels)
        	ots.(labels{i}).upper(j) = sts.(labels{i}).lower(j) + beta.(labels{i});
            ots.(labels{i}).lower(j) = sts.(labels{i}).upper(j) - beta.(labels{i});            
        end
    elseif sum(ots.(labels{i}).upper) < 1
        beta.(labels{i}) = beta.(labels{i}) + (1-sum(ots.(labels{i}).upper))/length(ots.(labels{i}).upper);
        for j = 1 : numel(labels)
        	ots.(labels{i}).upper(j) = sts.(labels{i}).lower(j) + beta.(labels{i});
            ots.(labels{i}).lower(j) = sts.(labels{i}).upper(j) - beta.(labels{i});            
        end
    end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% % % Computing of the novel abstraction presented in [1]:
for i = 1 : numel(labels)
    newAbstraction.tpm.lower(i,:) = ots.(labels{i}).lower;
    newAbstraction.tpm.upper(i,:) = ots.(labels{i}).upper;
    newAbstraction.error.(labels{i}) = beta.(labels{i});
    newAbstraction.label{i} = labels{i};
    if labels{i} == lmc.label{lmc.initial}
        newAbstraction.initial = i;
    end
end
clear i j k evaluations evaluatedRows err elem ind minErr sts
% % %
clear abstractionError beta indLabels indStates labels ots partition
clear representativePoint tpr
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% whos % % % <=== For debug purposes only
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
createDMC4Prism(lmc.tpm,lmc.initial,lmc.label)
createDMC4Prism(stdAbstraction.tpm, stdAbstraction.initial, stdAbstraction.label, 1, stdAbstraction.error)