Newick2Nexus
====================
Lightweight program to convert newick tree file(s) to Nexus format. If more than one file is used as input, output is merged into a single file. NOTE: taxon names are not checked; assumes everything is cool.

Compile
---------------

In a  terminal prompt in the src directory, type:

	g++ -Wall Newick2Nexus.cpp -m64 -O3 -o Newick2Nexus

Usage
---------------

Call as either:

	./Newick2Nexus -in treefile1 [treefile2 treefile3 ...] [-out outfileName]

where 'outFileName' is optional (default = "Nexified.trees").

UPDATE
--------------
This is now implemented in the [phyx](https://github.com/FePhyFoFum/phyx) program `pxt2nex`.

