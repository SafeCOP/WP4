
The repository contains code and datasets of vehicle platooning scenarios to be investigated via machine learning. 

A presentation introduces the problem with some examples (inherent code available here). 
Paper Mon18 in 'RTMA example CNR&Impara' explains the methodology. 

Two simulation environments are used: 1. differential equations of [1] and 2. Plexe.

1. is for string stability of the platoon.
2. deals with collision prediction.

Software code of 1. is available here. 2. is based on Plexe (plexe.car2x.org/) and has been automated to generate several runs under different system parameters; the inherent code is reported (Plexe automation.rar). The main script is Plexe-Automatic.sh. The inherent files, called by the script, are reported as well.

Thanks to Alessandro Fermi (a.fermi@impara-ai.com) for Plexe automation and Enrico Ferrari (e.ferrari@impara-ai.com) for usage of the data analytics platoform (www.rulex.ai).

For any question, do not hesitate to contact me: maurizio.mongelli@ieiit.cnr.it.

[1] L. Xu, L. Y.Wang, G. Yin, H. Zhang. “Communication information structures and contents for enhanced safety of highway vehicle platoons”, IEEE Transactions on Vehicular Technology, 63(9), pp. 4206–4220, (Nov 2014).