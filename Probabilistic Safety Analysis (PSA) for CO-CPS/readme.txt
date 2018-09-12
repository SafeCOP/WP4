This method of approximate abstractions of large, discrete-time labelled Markov chains (MCs) with interval-valued labelled Markov chains (IMCs) leveraging their interval Markov decision processes (IMDPs) semantics is based on the theoretical results presented in [1].

The abstraction procedures are implemented in Matlab (R2018a, tested in its maci64 version).

The main routine is provided by "abstractions.m" script file. The related configuration file "config.m" should contain the information on the concrete labelled Markov chain to be abstracted, i.e. its transition probability matrix (tpm), the cell array of the unique labels associated to each state, and the index of initial state. The example from the extended version of reference conference paper [1] is provided in "exampleConfig.m" script. 

The results of standard and novel abstraction procedures are stored in the data structures named 'stdAbstraction' and 'newAbstraction', respectively.

The concrete labelled Markov chain and its standard abstraction (with the associated abstraction error) are saved in Prism model format (by default, the produced files are named "concreteMC.pm" and "abstractMC.pm", respectively). For the model checking procedure on the abstract model refer to [2]. The model checking procedures were tested on  Prism for Mac OS X (64-bit) in version 4.4 (first released 23 Jul 2017). This version still does not support the interval Markov chains. The model checking on this model may be performed through the 

The novel model checking procedure relies on the convex optimisation presented in [3]. See its references for the related implementation.

In summary, the distribution directory contains the following files.
Where appropriate, comments/instructions are included in the files themselves.
- abstractions.m - the main Matlab script file dealing with the abstractions
- config.m - the configuration script with the info about concrete Markov chain
- createDMC4Prism.m - the function used to export the data in Prism model format
- exampleConfig.m - the example of configuration file based on example in [1]

References:

[1] Zacchia Lun, Y., Wheatley, J., D'Innocenzo, A. and Abate, A., 2018. Approximate Abstractions of Markov Chains with Interval Decision Processes. IFAC-PapersOnLine 51(16): 91--96. 6th IFAC Conference on Analysis and Design of Hybrid Systems ADHS 2018.

[2] D'Innocenzo, A., Abate, A. and Katoen, J.P., 2012. Robust PCTL model checking. In Proceedings of the 15th ACM international conference on Hybrid Systems: Computation and Control (pp. 275-286). ACM.

[3] Puggelli, A., Li, W., Sangiovanni-Vincentelli, A.L. and Seshia, S.A., 2013. Polynomial-time verification of PCTL properties of MDPs with convex uncertainties. In International Conference on Computer Aided Verification (pp. 527-542). Springer.