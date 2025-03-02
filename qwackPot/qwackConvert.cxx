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


////////////////////////////////////////////////////////////////////////////////////////
string extractSubstring(const string& str, const string& label, const string& delim, bool searchForward) {
  size_t labelPos = str.find(label); 
  if(labelPos == string::npos) { return ""; } // If the label is not found, return an empty string
  size_t startPos = 0;

  //----------------------------------------
  if(searchForward) {
    startPos = labelPos + label.length(); // Skip over the label for forward search
    size_t endPos = str.find(delim, startPos); // Find the delimiter after the label
    if (endPos == string::npos) { return ""; } // If no delim, return empty string
    else { return str.substr(startPos, endPos - startPos); } // Extract the substring forward
  //----------------------------------------
  } else {
    size_t startPos = str.rfind(delim, labelPos);   // Find the last delimiter before the label
    if (startPos == string::npos) { startPos = 0; } // If no delim, start from front of string
    else { return str.substr(startPos + 1, labelPos - startPos - 1); } // Extract the substring backward
  }

  return "fail";
}

////////////////////////////////////////////////////////////////////////////////////////
int main ( int n_args, const char** args ){

  // ----------------------------------------------------------
  // Read in parameters ---------------------------------------
  string unprocfile = args[1];
  double D20        = atof(args[2]); // NOTE: must already be
				     //   divided by 10^4!

  // Search for parameters in file name -----------------------
  string label, delim, buffer;
  
  label = "o2j-";  delim = ".";  buffer = extractSubstring(unprocfile,label,delim,1);
  int finalDoubJ    = atoi(buffer.c_str());

  label = "i2j-";  delim = "_";  buffer = extractSubstring(unprocfile,label,delim,1);
  int initDoubJ    = atoi(buffer.c_str());

  label = "_i2j";  delim = "-";  buffer = extractSubstring(unprocfile,label,delim,0);
  int tranDoubJ    = atoi(buffer.c_str());

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

  //// ... .txt -> ..._2Ji-#_2Jo-#_2Jt#.txt
  //string insertme = "_2Ji-" + to_string(initDoubJ ) 
  //                + "_2Jo-" + to_string(finalDoubJ)
  //                + "_2Jt-" + to_string(tranDoubJ );
  //procfile.insert(procfile.length()-4,insertme);

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
