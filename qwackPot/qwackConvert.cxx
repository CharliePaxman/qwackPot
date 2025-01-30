#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;


//Compile me in location:
//   cd /path/to/DWUCK4
//With command:
//   g++ -o qwackPot/qwackConvert qwackPot/qwackConvert.cxx

int main ( int n_args, const char** args ){

  // ----------------------------------------------------------
  // Read in parameters ---------------------------------------
  string unprocfile = args[1];
  int initDoubJ     = atoi(args[2]);
  int finalDoubJ    = atoi(args[3]);
  int tranDoubJ     = atoi(args[4]);
  double D20        = atof(args[5]); // NOTE: must already be
				     //   divided by 10^4!

  // ----------------------------------------------------------
  // Calculate multiplier (incl. fm2/sr -> mb/sr) -------------

  // Numerators and denominators
  int varIn   = (initDoubJ)  + 1;
  int varOut  = (finalDoubJ) + 1;
  int varTran = (tranDoubJ)  + 1;

  // Calculate equation 2.10 in dw4_doc
  double multiplier = (double) varOut * (1./(double) varIn) * (1./(double)varTran) * D20;

  // Convert fm2/sr -> mb/sr
  multiplier *= 10.;

  // ----------------------------------------------------------
  // Read in unprocessed file ---------------------------------
  vector<double> angle;
  vector<double> sigma;
  string line;

  // Read in file
  ifstream infile(unprocfile.c_str());    
  if (!infile) { cerr << "Unable to open file" << endl; return 1; }

  // Ignore first two lines (headers)
  for(int i=0; i<2 && getline(infile, line); ++i) { /* Skip */  }

  while(getline(infile, line)) {
    // Separate variables, delimited by comma
    istringstream iss(line);
    string substring{};
    vector<string> substrings{};

    while (getline(iss, substring, ',')){
      substrings.push_back(substring);
    }

    angle.push_back(stod(substrings[0]));
    sigma.push_back(stod(substrings[2]));
  }

  // ----------------------------------------------------------
  // Write processed data file --------------------------------

  // Make file name
  string procfile = unprocfile;

  // Output_Unprocessed -> Output_Processed
  procfile.replace(7,3,"P");

  // ... .txt -> ..._2Ji-#_2Jo-#_2Jt#.txt
  string insertme = "_2Ji-" + to_string(initDoubJ ) 
	          + "_2Jo-" + to_string(finalDoubJ)
	          + "_2Jt-" + to_string(tranDoubJ );
  procfile.insert(procfile.length()-4,insertme);

  // Write to file
  ofstream outfile(procfile.c_str(),ios::out); //overwrite
  
  for(int i=0; i<angle.size(); i++){
    outfile << fixed;       outfile.precision(1);
    outfile << angle[i] << "  ";
    outfile << scientific;  outfile.precision(4);
    outfile << sigma[i]*multiplier << endl;
  }

  // ----------------------------------------------------------
  // End ------------------------------------------------------
  
  return 0;
}
