function createDMC4Prism(tpm,initial,label,varargin)
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % % % This function creates a Prism model file for discrete-time Markov chains (DTMCs), extension .pm
    % % % The mandatory inputs are:
    % % % -- the transition probability matrix in                   tpm
    % % % -- the initial state in                                   initial
    % % % -- the assigned labelling in                              label
    % % % When the optinal inputs are present, 
    % % % -- varargin{1} indicates the boolean specifying whether 
    % % %       the considered Markov chain is an abstraction
    % % % When varargin{1}==0, the considered Markov chain is a concrete one, so
    % % % -- varargin{2} is an integer number that indicates timeSteps k
    % % % -- varargin{3} is a character array indicating prismFileName (without an extension)
    % % % -- varargin{4} ia a character array that provides additional comments
    % % % When varargin{1}==1, the considered Markov chain is an abstraction, so 
    % % %   the abstraction error should be specifyied in varargin{2}.
    % % % -- varargin{2} is a fixed-point number, that will be inserted as a comment in Prism model file;
    % % % -- varargin{3} is an integer number that indicates timeSteps k
    % % % -- varargin{4} is a character array indicating prismFileName (without an extension)
    % % % -- varargin{5} ia a character array that provides additional comments
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % % % The default values:
    defaultTimeSteps = 10;
    defaultPrismFileName = 'concreteMC.pm';
    defaultPrismFileNameAbstract = 'abstractMC.pm';
    defaultComments = 'This model represent a concrete discrete-time Markov chain';
    defaultCommentsAbstract = 'This model represents a standard abstraction of a discrete-time Markov chain';
    abstractionErrorComment = 'The associated abstarction error is epsilon = ';
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % % % Error messages:
    errorMessageNarginTooSmall = 'The function createDMC4Prism requires at least 3 inputs, ';
    errorMessageNarginTooSmall = [errorMessageNarginTooSmall, ...
        'namely tpm, initial, label'];
    errorMessageVarargin1 = 'The function createDMC4Prism as a fourth inputs requires ';    
    errorMessageVarargin1 = [errorMessageVarargin1, ...
        'a boolean indicating whether the considered Markov chain is an abstraction'];
    errorMessageAbstractionError = 'The function createDMC4Prism in case of abstrac Markov chain requires ';
    errorMessageAbstractionError = [errorMessageAbstractionError, ...
        'also the abstraction error as a fifth input'];
    errorMessageNarginTooBigConcrete = 'There are too much input arguments for the function createDMC4Prism ';
    errorMessageNarginTooBigConcrete = [errorMessageNarginTooBigConcrete, 'dealing with concrete Markov chain'];
    errorMessageNarginTooBig = 'There are too much input arguments for the function createDMC4Prism ';
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    % % % varargin control:
    if nargin < 3
       error(errorMessageNarginTooSmall) 
    elseif nargin == 3
        timeSteps = defaultTimeSteps;
        prismFileName = defaultPrismFileName; 
        comments = defaultComments;
        shortComments = 1;
    elseif nargin == 4 
        if varargin{1} == 0
            timeSteps = defaultTimeSteps;
            prismFileName = defaultPrismFileName; 
            comments = defaultComments;
            shortComments = 1;
        elseif varargin{1} == 1
            error(errorMessageAbstractionError)
        else
            error(errorMessageVarargin1)
        end
    elseif nargin == 5
        if varargin{1} == 0
            timeSteps = varargin{2};
            prismFileName = defaultPrismFileName; 
            comments = defaultComments;
            shortComments = 1;
        elseif varargin{1} == 1
            comments = defaultCommentsAbstract;
            abstractionErrorComment = [abstractionErrorComment, num2str(varargin{2})];
            timeSteps = defaultTimeSteps;
            prismFileName = defaultPrismFileNameAbstract;
            shortComments = 0;
        else
            error(errorMessageVarargin1)
        end
    elseif nargin == 6
        if varargin{1} == 0
            timeSteps = varargin{2};
            prismFileName = [varargin{3},'.pm'];
            comments = defaultComments;
            shortComments = 1;
        elseif varargin{1} == 1
            comments = defaultCommentsAbstract;
            abstractionErrorComment = [abstractionErrorComment, num2str(varargin{2})];
            timeSteps = varargin{3};
            prismFileName = defaultPrismFileNameAbstract;
            shortComments = 0;
        else
            error(errorMessageVarargin1)
        end
    elseif nargin == 7 
        if varargin{1} == 0
            timeSteps = varargin{2};
            prismFileName = [varargin{3},'.pm'];
            comments = varargin{4};
            shortComments = 1;
        elseif varargin{1} == 1
            comments = defaultCommentsAbstract;
            abstractionErrorComment = [abstractionErrorComment, num2str(varargin{2})];
            timeSteps = varargin{3};
            prismFileName = [varargin{4},'.pm'];
            shortComments = 0;
        else
            error(errorMessageVarargin1)
        end
    elseif nargin == 8 
        if varargin{1} == 0
            error(errorMessageNarginTooBigConcrete)
        elseif varargin{1} == 1
            comments = varargin{5};
            abstractionErrorComment = [abstractionErrorComment, num2str(varargin{2})];
            timeSteps = varargin{3};
            prismFileName = [varargin{4},'.pm'];
            shortComments = 0;
        else
            error(errorMessageVarargin1)
        end
    else
        error(errorMessageNarginTooBig)
        % % Too much inputs
    end
    numStates = length(tpm);
    % fileID = fopen('concreteMC.pm','w');    % % Other name should be specified in the final version
    fileID = fopen(prismFileName,'w');
    if shortComments == 1
        fprintf(fileID,'// %s\n\n',comments);
        moduleName = 'ConcreteMC';
    elseif shortComments == 0
        fprintf(fileID,'// %s\n',comments);
        fprintf(fileID,'// %s\n\n',abstractionErrorComment);
        moduleName = 'AbstractMC';
    else 
        warning('Attention, ''shortComments'' variable has an anomalous value')
    end   
    
    fprintf(fileID,'dtmc \n\nconst int k = %u; \n\nmodule %s\n\n\t',timeSteps,moduleName);
                                            % % Other name should be specified in the final version
    fprintf(fileID,'s : [0..%u] init %u;\n\n\t',numStates-1,initial-1);
    for i = 1 : length(tpm)
        if any(ge(tpm(i,:),eps))
            fprintf(fileID,'[] s=%u -> ',i-1);
            index_j = find(ge(tpm(i,:),eps));
            % % % If sum ~= 1, adjust
            addend = 1;    % % % We allow up to 3 addends on the same line
            for j = index_j(1:end-1)
                if addend > 2
                    if i < 11
                        fprintf(fileID,'%8.6f : (s''=%u) + \n\t\t  ',tpm(i,j),j-1);
                    elseif i <101
                        fprintf(fileID,'%8.6f : (s''=%u) + \n\t\t   ',tpm(i,j),j-1);
                    elseif i <1001
                        fprintf(fileID,'%8.6f : (s''=%u) + \n\t\t    ',tpm(i,j),j-1);
                    elseif i <10001
                        fprintf(fileID,'%8.6f : (s''=%u) + \n\t\t     ',tpm(i,j),j-1);
                    elseif i <100001
                        fprintf(fileID,'%8.6f : (s''=%u) + \n\t\t      ',tpm(i,j),j-1);
                    elseif i <1000001
                        fprintf(fileID,'%8.6f : (s''=%u) + \n\t\t       ',tpm(i,j),j-1);
                    elseif i <10000001
                        fprintf(fileID,'%8.6f : (s''=%u) + \n\t\t        ',tpm(i,j),j-1);
                    elseif i <100000001
                        fprintf(fileID,'%8.6f : (s''=%u) + \n\t\t         ',tpm(i,j),j-1);
                    else
                        fprintf(fileID,'%8.6f : (s''=%u) + \n\t\t          ',tpm(i,j),j-1);                    
                    end                     
                    addend = 1;
                else
                    fprintf(fileID,'%8.6f : (s''=%u) + ',tpm(i,j),j-1);
                    addend = addend + 1;
                end
            end
            j = index_j(end);
            if addend > 3
                if i < 11
                    fprintf(fileID,'\n\t\t  %8.6f : (s''=%u);\n\n\t',tpm(i,j),j-1);
                elseif i <101
                    fprintf(fileID,'\n\t\t   %8.6f : (s''=%u);\n\n\t',tpm(i,j),j-1);
                elseif i <1001
                    fprintf(fileID,'\n\t\t    %8.6f : (s''=%u);\n\n\t',tpm(i,j),j-1);
                elseif i <10001
                    fprintf(fileID,'\n\t\t     %8.6f : (s''=%u);\n\n\t',tpm(i,j),j-1);
                elseif i <100001
                    fprintf(fileID,'\n\t\t      %8.6f : (s''=%u);\n\n\t',tpm(i,j),j-1);
                elseif i <1000001
                    fprintf(fileID,'\n\t\t       %8.6f : (s''=%u);\n\n\t',tpm(i,j),j-1);
                elseif i <10000001
                    fprintf(fileID,'\n\t\t        %8.6f : (s''=%u);\n\n\t',tpm(i,j),j-1);
                elseif i <100000001
                    fprintf(fileID,'\n\t\t         %8.6f : (s''=%u);\n\n\t',tpm(i,j),j-1);
                else
                    fprintf(fileID,'\n\t\t          %8.6f : (s''=%u);\n\n\t',tpm(i,j),j-1);                    
                end
            else
                fprintf(fileID,'%8.6f : (s''=%u);\n\n\t',tpm(i,j),j-1);
            end
        end
    end
    fprintf(fileID,'\nendmodule\n');
    [labels, ~, indLabels] = unique(label,'stable');
    for i = 1 : numel(labels)
        partition.(labels{i}) = find(indLabels==i)';
    end
    for i = 1 : numel(labels)
        fprintf(fileID,'\nlabel "%s" = ',labels{i});
        for j = partition.(labels{i})(1:end-1)
            fprintf(fileID,'s=%u | ',j-1);
        end 
        j = partition.(labels{i})(end);
        fprintf(fileID,'s=%u;',j-1);        
    end
end