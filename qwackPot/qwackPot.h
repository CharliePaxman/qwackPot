#include <string>
#include <functional>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "qPotentials.h"
#include "qADWAPotentials.h"

using namespace std;

// ========================
// Define global variables
// Recall this is in normal kinematics!
//   examples given for 40Ca(d,p)41Ca reaction

//-------------------------------//

// Target (i.e. 40Ca)
int AT; // Target mass
int ZT; // Target charge

// Beam (i.e. d)
int AB; // Beam mass
int ZB; // Beam charge

// Light ejectile (i.e. p)
int AL; // Light mass
int ZL; // Light charge

// Heavy ejectile (i.e. 41Ca)
int AH; // Heavy mass
int ZH; // Heavy charge

//-------------------------------//

double Qval;  // Qvalue of the reaction [MeV] (https://www.nndc.bnl.gov/qcalc/)
double Sep;   // Relevant separation energy of final nucleus 
double beamE; // Incoming beam energy [MeV/u]
double Ex;    // Heavy excitation energy [MeV]
int n, l, doubJ;

//-------------------------------//

string outputPath;
string outputName;
string outputFile;
string Pin;
string Pout;

//-------------------------------//

void ConstructOutputCards(){
  stringstream ss;
  outputPath.clear();
  outputName.clear();
  outputFile.clear();

  outputPath.append("cards/");
    outputName.append(to_string(AT)); outputName.append("-");
    outputName.append(to_string(ZT)); outputName.append("-");
    outputName.append(to_string(AB)); outputName.append("-");
    outputName.append(to_string(ZB)); outputName.append("-");
    outputName.append(to_string(AL)); outputName.append("-");
    outputName.append(to_string(ZL)); 
  outputName.append("_");
    outputName.append("bE");
    ss << setw(4) << setfill('0') << beamE*1000;
    outputName.append(ss.str()); 
    ss.str(std::string()); // clear stringstream
  outputName.append("_");
    outputName.append("Ex");
    ss << setw(4) << setfill('0') << Ex*1000;
    outputName.append(ss.str()); 
    ss.str(std::string()); // clear stringstream
  outputName.append("_");
    outputName.append("Pin-");
    outputName.append(Pin);
  outputName.append("_");
    outputName.append("Pout-");
    outputName.append(Pout);
  outputName.append("_");
    outputName.append("nl2j-");
    outputName.append(to_string(n)); outputName.append("-");
    outputName.append(to_string(l)); outputName.append("-");
    outputName.append(to_string(doubJ)); 
  outputName.append(".txt");

  outputFile.append(outputPath);
  outputFile.append(outputName);

  ofstream f;
  f.open("ListOfOutputFiles.txt",ios::app);
  f << outputFile << endl;
}

void ClearGlobalVariables_General(){
  AT=-1000;
  ZT=-1000;
  AB=-1000;
  ZB=-1000;
  AL=-1000;
  ZL=-1000;
  AH=-1000;
  ZH=-1000;
  Qval=-1000; 
  Sep=-1000;
  beamE=-1000; 
}

void LoadGlobalVariables_General(){
  // General detals for an experiment or reaction

  // Open file and read in
  string ignore;
  ifstream file("Input_General.txt");
  if(file.is_open()){
    file >> AT >> ZT >> ignore;
    file >> AB >> ZB >> ignore;
    file >> AL >> ZL >> ignore;
    file >> Sep >> ignore;
    file >> Qval >> ignore;
    file >> beamE >> ignore;
  }

  // Calculate heavy outgoing
  AH = AT+(AB-AL);
  ZH = ZT+(ZB-ZL);
  
}

void ClearGlobalVariables_Specific(){
  Ex=-1000;    
  Pin.clear();  Pout.clear();
  n=-1000;  l=1000;  doubJ=1000;
  outputFile.clear();
}

void LoadGlobalVariables_Specific(int iter){
  // Details specific to a state or transfer!

  // Open file and read in	
  ifstream file("Input_Specific.txt");
  if(file.is_open()){
    for(int i=0;  i<iter; i++){
      file.ignore(80,'\n');
    }
    file >> Ex >> n >> l >> doubJ >> Pin >> Pout;
  }

  // Define output file name for these specific variables
  ConstructOutputCards();

}

void SelectIncomingPotential(string label){
  //Note that, for input potential, energy is simply beamE*AB
  //This is not the case for output potential!
  bool x = false; 

  // Using ugly if-then ladder for now
  // maybe use map in the future?
  if      (label=="HSS"){   x = deut_HSS2006(AT, ZT, beamE*AB); } 
  else if (label=="AC" ){   x = deut_AC2006 (AT, ZT, beamE*AB); } 
  else if (label=="Bo" ){   x = deut_Bo1988 (AT, ZT, beamE*AB); } 
  else if (label=="DCV"){   x = deut_DCV1980(AT, ZT, beamE*AB); } 
  else if (label=="LH" ){   x = deut_LH1974 (AT, ZT, beamE*AB); }
  else if (label=="PP" ){   x = deut_PP1963 (AT, ZT, beamE*AB); }
  else if (label=="ADWA" ){ x = adwa_CH1991 (AT, ZT, beamE*AB); }

  if(!x){cout << " FAILED TO SET INCOMING PARAMETERS" << endl;}

}

void SelectOutgoingPotential(string label){
  //Note that, for output potential, energy is:
  //    Qval - Ex + beamE*AB
  //i.e.:
  //    (massB+massT) - (massL+massH) - Ex + beamE*AB
  //for now, Qval is an input, could calculate with AME tables in the future?
  bool x = false; 

  double Ein = Qval-Ex+(beamE*AB); //cout <<"Ein=====" << Ein << endl;

  // Using ugly if-then ladder for now
  // maybe use map in the future?
  if      (label=="KD"){ x = prot_KD2003(AT, ZT, Ein); } 
  else if (label=="CH"){ x = prot_CH1991(AT, ZT, Ein); } 
  else if (label=="Mt"){ x = prot_Mt1971(AT, ZT, Ein); } 
  else if (label=="BG"){ x = prot_BG1969(AT, ZT, Ein); } 
  else if (label=="P" ){ x = prot_P1963 (AT, ZT, Ein); }

  if(!x){cout << " FAILED TO SET INCOMING PARAMETERS" << endl;}

}

void InputBlock1(){
  auto t = std::time(nullptr);
  auto tm = *std::localtime(&t);

  stringstream buffer;
  buffer << setw(8) << setfill('0') << 0;
  stringstream title;
  title << " Date: "
	<< put_time(&tm, "%Y-%m-%d %H:%M:%S")
	<< ". Label: " << outputName;

  ofstream file;
  file.open(outputFile.c_str());
 
  if(file.is_open()){
    file << 1 //  1
         << 0 //  2
         << 0 //  3
         << 0 //  4
         << 0 //  5
         << 0 //  6
         << 0 //  7
         << 0 //  8
         << 0 //  9
         << 0 // 10
         << 0 // 11
         << 0 // 12
         << 1 // 13
         << 0 // 14
         << 0 // 15
         << 0 // 16
         << 2 // 17
         << buffer.str()
         << title.str();

    file << "\n";; 
    file.close(); 
    if(loud){cout << "Written Input Block 1" << endl;}
  } else {
    cout << "ERROR! File not opened" << endl;
  }
}

void InputBlock2(double initAng, double stepAng, double numAng){
  ofstream file;
  file.open(outputFile.c_str(),ios::app);
  file.setf(ios::fixed, ios::floatfield);
  file.precision(3);
  if(file.is_open()){
    file << setw(8) << numAng
         << setw(8) << initAng
         << setw(8) << stepAng;

    file << "\n"; 
    file.close(); 
    if(loud){cout << "Written Input Block 2" << endl;}
  } else {
    cout << "ERROR! File not opened" << endl;
  } 
}

void InputBlock3(int maxPartWave, int maxAngMomTransfer){
  ofstream file;
  file.open(outputFile.c_str(),ios::app);
  file.setf(ios::fixed, ios::floatfield);
  file.precision(0);
  if(file.is_open()){
    file << setw(3) << maxPartWave
         << setw(3) << maxAngMomTransfer
         << setw(3) << l
         << setw(3) << doubJ;

    file << "\n"; 
    file.close(); 
    if(loud){cout << "Written Input Block 3" << endl;}
  } else {
    cout << "ERROR! File not opened" << endl;
  } 
}

void InputBlock4(double radStep, double radMin, double radMax){
  ofstream file;
  file.open(outputFile.c_str(),ios::app);
  file.setf(ios::fixed, ios::floatfield);
  file.precision(3);
  if(file.is_open()){
    file << setw(8) << radStep
         << setw(8) << radMin 
         << setw(8) << radMax;

    file << "\n"; 
    file.close(); 
    if(loud){cout << "Written Input Block 4" << endl;}
  } else {
    cout << "ERROR! File not opened" << endl;
  } 
}

void InputBlock5(){
  ofstream file;
  file.open(outputFile.c_str(),ios::app);
  file.setf(ios::fixed, ios::floatfield);
  file.precision(3);
  if(file.is_open()){
    // LINE 1
    file << setw(8) << (double) beamE*AB
         << setw(8) << (double) AB
         << setw(8) << (double) ZB
         << setw(8) << (double) AT
         << setw(8) << (double) ZT
         << setw(8) << (double) rc0
         << setw(8) << (double) 0.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) AB;  //TWICE SPIN OF DEUTERON! convenient for now
    file << "\n"; 
    
    // LINE 2
    file << setw(8) << (double) 1.0
         << setw(8) << (double) v 
         << setw(8) << (double) r0
         << setw(8) << (double) a 
         << setw(8) << (double) 0.0 
         << setw(8) << (double) vi
         << setw(8) << (double) ri0
         << setw(8) << (double) ai;
    file << "\n"; 
    
    // LINE 3
    file << setw(8) << (double) 2.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) vsi
         << setw(8) << (double) rsi0
         << setw(8) << (double) asi;
    file << "\n"; 

    // LINE 4
    file << setw(8) << (double) -4.0
         << setw(8) << (double) vso
         << setw(8) << (double) rso0
         << setw(8) << (double) aso
         << setw(8) << (double) 0.0
         << setw(8) << (double) vsoi
         << setw(8) << (double) rsoi0
         << setw(8) << (double) asoi;
    file << "\n"; 

    file.close(); 
    if(loud){cout << "Written Input Block 5" << endl;}
  } else {
    cout << "ERROR! File not opened" << endl;
  } 
}

void InputBlock6(){
  ofstream file;
  file.open(outputFile.c_str(),ios::app);
  file.setf(ios::fixed, ios::floatfield);
  file.precision(3);
  if(file.is_open()){
    // LINE 1
    file << setw(8) << (double) Qval-Ex
         << setw(8) << (double) AL
         << setw(8) << (double) ZL
         << setw(8) << (double) AH
         << setw(8) << (double) ZH
         << setw(8) << (double) rc0
         << setw(8) << (double) 0.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) AL;  //TWICE SPIN OF PROTON! Convenient for now
    file << "\n"; 
 
    // LINE 2
    file << setw(8) << (double) 1.0
         << setw(8) << (double) v 
         << setw(8) << (double) r0
         << setw(8) << (double) a 
         << setw(8) << (double) 0.0 
         << setw(8) << (double) vi
         << setw(8) << (double) ri0
         << setw(8) << (double) ai;
    file << "\n"; 
    
    // LINE 3
    file << setw(8) << (double) 2.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) vsi
         << setw(8) << (double) rsi0
         << setw(8) << (double) asi;
    file << "\n"; 

    // LINE 4
    file << setw(8) << (double) -4.0
         << setw(8) << (double) vso
         << setw(8) << (double) rso0
         << setw(8) << (double) aso
         << setw(8) << (double) 0.0
         << setw(8) << (double) vsoi
         << setw(8) << (double) rsoi0
         << setw(8) << (double) asoi;
    file << "\n"; 
   
    file.close(); 
    if(loud){cout << "Written Input Block 6" << endl;}
  } else {
    cout << "ERROR! File not opened" << endl;
  } 
}

void InputBlock7(){
  // FISW = 0 for bound states, but =2 for unbound states
  double FISW = 0.0;  if(Ex>=Sep){FISW=2.0;}
  
  ofstream file;
  file.open(outputFile.c_str(),ios::app);
  file.setf(ios::fixed, ios::floatfield);
  file.precision(3);
  if(file.is_open()){
    // LINE 1
    file << setw(8) << (double) Ex-Sep
         << setw(8) << (double) AB-AL
         << setw(8) << (double) ZB-ZL
         << setw(8) << (double) AT
         << setw(8) << (double) ZT
         << setw(8) << (double) 1.25
         << setw(8) << (double) 0.0
         << setw(8) << (double) 0.0
         << setw(8) << (double) AB-AL;  //TWICE SPIN OF NEUTRON! Convenient for now
    file << "\n"; 
 
    // LINE 2
    file << setw(8) << (double) -1.0
         << setw(8) << (double) -1.0
         << setw(8) << (double) 1.25
         << setw(8) << (double) 0.65;
    file << "\n"; 
    
    // LINE 3
    file << setw(8) << (double) n
         << setw(8) << (double) l
         << setw(8) << (double) doubJ
         << setw(8) << (double) AB-AL   //TWICE SPIN OF NEUTRON! Convenient for now
         << setw(8) << (double) 0.0     //empty
         << setw(8) << (double) FISW;   //bound or unbound?
    file << "\n"; 
   
    file.close(); 
    if(loud){cout << "Written Input Block 7" << endl;}
  } else {
    cout << "ERROR! File not opened" << endl;
  } 
}

void InputEndBlock(){
  ofstream file;
  file.open(outputFile.c_str(),ios::app);
  file.setf(ios::fixed, ios::floatfield);
  if(file.is_open()){
    file << 9 << "\n"; 
    file.close(); 
    if(loud){cout << "Written End Block" << endl;}
  } else {
    cout << "ERROR! File not opened" << endl;
  } 
}

