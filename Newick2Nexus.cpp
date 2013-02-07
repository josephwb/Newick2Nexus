/*

g++ -Wall Newick2Nexus.cpp -m64 -O3 -o Newick2Nexus

TODO: add translation tables (much smaller files).

*/

#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <stdlib.h> // needed on linux

using namespace std;

void processCommandLineArguments(int argc, char *argv[], vector <string> & fileNames,
	string & outName, bool & verbose);
void writeNexii (vector <string> & fileNames, string & outName, bool const& verbose);
bool checkValidInputFile (string fileName);
bool checkValidOutputFile (string & outputFileName);
bool checkWhiteSpaceOnly (string stringToParse);
string extractString (string stringToParse, int stringPosition);

int main (int argc, char *argv[]) {
	vector <string> fileNames;
	string outName = "Nexified.trees";
	bool verbose = true;
	
	processCommandLineArguments(argc, argv, fileNames, outName, verbose);
	writeNexii (fileNames, outName, verbose);
	
	if (verbose) cout << endl << "Successfully generated file '" << outName << "'." << endl << endl;
	if (verbose) cout << "Fin." << endl << endl;
	
	return 0;
}

void writeNexii (vector <string> & fileNames, string & outName, bool const& verbose) {
	ofstream outNexus;
	string currFile, line;
	int counter = 1;
	
	if (verbose) cout << endl;
	
// write header
	outNexus.open(outName.c_str());
	outNexus
	<< "#NEXUS" << endl
	<< "begin trees;" << endl;
	
	for (int i = 0; i < (int)fileNames.size(); i++) {
		bool first = true;
		ifstream treeInput;
		string currentFile = fileNames[i];
		treeInput.open(currentFile.c_str());
		if (verbose) cout << "Processing file '" << currentFile << "'..." << endl;
		while (getline(treeInput,line)) {
			if (!checkWhiteSpaceOnly(line)) {
				if (first) { // just check first non-empty line of each file
					string start = extractString (line, 0);
					if (start == "#NEXUS" || start == "Tree" || start == "tree") {
						cout << endl << "File '" << currentFile << "' appears to already be in Nexus format." << endl << endl;
						exit(0);
					}
				}
				outNexus << "tree STATE_" << counter << " = " << line << endl;
				counter++;
			}
			first = false;
		}
		treeInput.close();
	}
	outNexus << "End;" << endl;
	outNexus.close();
	
	if (verbose) cout << endl << "Processed a total of " << counter << " trees across " << fileNames.size() << " files." << endl;
}

void processCommandLineArguments(int argc, char *argv[], vector <string> & fileNames,
	string & outName, bool & verbose)
{
	if (argc < 3) {
		cout
		<< "Program description: Convert newick tree file(s) to Nexus." << endl
		<< endl
		<< "To compile, type the following in a unix prompt:" << endl
		<< endl
		<< "g++ -Wall Newick2Nexus.cpp -m64 -O3 -o Newick2Nexus" << endl
		<< endl
		<< "To run, type:" << endl
		<< endl
		<< "./Newick2Nexus -in treefile1 [treefile2 treefile3 ...] [-out outfileName]" << endl << endl;
		exit(0);
	} else {
		for (int i = 1; i < argc; i++) {
			string temp = argv[i];
			
			if (temp == "-h" || temp == "-help") {
				cout
				<< "Program description: Convert newick tree file(s) to Nexus." << endl
				<< endl
				<< "To compile, type the following in a unix prompt:" << endl
				<< endl
				<< "g++ -Wall Newick2Nexus.cpp -m64 -O3 -o Newick2Nexus" << endl
				<< endl
				<< "To run, type:" << endl
				<< endl
				<< "./Newick2Nexus -in treefile1 [treefile2 treefile3 ...] [-out outfileName]" << endl << endl;
				exit(0);
			} else if (temp == "-in") {
				i++;
				string curr = argv[i];
				while (curr != "-out" && i < argc && curr != "-q") {
					if (checkValidInputFile(curr)) {
						fileNames.push_back(curr);
					}
					i++;
					if (i < argc) curr = argv[i];
				}
				if (curr == "-out" || curr == "-q") i--;
				continue;
			} else if (temp == "-out") {
				i++;
				temp = argv[i];
				if (checkValidOutputFile(temp)) {
					outName = temp;
				}
				continue;
			} else if (temp == "-q") {
				verbose = false;
				continue;
			} else {
				cout
				<< "Unknown command-line argument '" << argv[i] << "' encountered." << endl
				<< endl
				<< "Usage:" << endl
				<< endl
				<< "./Newick2Nexus -in treefile1 [treefile2 treefile3 ...] [-out outfileName]" << endl << endl;
				exit(0);
			}
			if (verbose) cout << endl;
		}
	}
}

bool checkValidInputFile (string fileName) {
	bool validInput = false;
	ifstream tempStream;
	
	tempStream.open(fileName.c_str());
	if (tempStream.fail()) {
		cerr << endl << "Analysis failed. " << endl << "Error: unable to open file '";
		cerr << fileName << "'" <<  endl;
		exit(1);
	} else {
		//cout << "Successfully opened file '" << fileName << "'." <<  endl << endl;
		validInput = true;
		tempStream.close();
		tempStream.clear();
	}
	return validInput;
}

bool checkValidOutputFile (string & outputFileName) {
	bool testOutBool = true;
	bool fileNameAcceptable = false;
	bool keepFileName = false;
	
// First, check if file already exists, so overwriting can be prevented
	fstream testIn;
	while (!fileNameAcceptable) {
		testIn.open(outputFileName.c_str());
		if (!testIn) {
			testIn.close();
			fileNameAcceptable = true;
		} else {
			testIn.close();
			cout << endl << "Default output file '" << outputFileName << "' exists!  Change name (0) or overwrite (1)? ";
			cin >> keepFileName;
			if (!keepFileName) {
				cout << "Enter new output file name: ";
				cin >> outputFileName;
			} else {
				cout << "Overwriting existing file '" << outputFileName << "'." << endl;
				fileNameAcceptable = true;
			}
		}
	}
	
	ofstream outFile;
	outFile.open(outputFileName.c_str());
	
	if (outFile.fail()) {
		cerr << endl << "Analysis failed." << endl << "Error: unable to open file '";
		cerr << outputFileName << "'" <<  endl;
		testOutBool = false;
		exit(1);
	} else {
		outFile.close();
		outFile.clear();
	}
	return testOutBool;
}

bool checkWhiteSpaceOnly (string stringToParse) {
	bool whiteSpaceOnly = true;
	vector<string> tempVector;
	istringstream tempStream(stringToParse);
	string tempString;
	while (tempStream >> tempString) {
		if (tempString != "	" && tempString != " ") {
			whiteSpaceOnly = false;
		}
	}
	return whiteSpaceOnly;
}

string extractString (string stringToParse, int stringPosition) {
	vector<string> tempVector;
	istringstream tempStream(stringToParse);
	string tempString, returnString;
	int counter = 0;
	while (tempStream >> tempString && counter <= stringPosition) {
		tempVector.push_back(tempString);
		counter++;
	}
	
	returnString = tempVector[stringPosition];
	
	return returnString;
}
