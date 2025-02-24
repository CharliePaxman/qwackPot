#include <string>
#include <functional>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "qPotentials.h"
#include "qADWAPotentials.h"

using namespace std;

//===============================//
// Define global variables
// Recall this is in normal kinematics!
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
int doubInJ, doubOutJ;

//-------------------------------//

string outputPath;
string outputName;
string outputFile;
string Pin;
string Pout;

//-------------------------------//

double maxRad = -1000.;
double stepRad = -1000.;
int npartWav = -1000;

//===============================//

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
  outputName.append("_");
    outputName.append("i2j-");
    outputName.append(to_string(doubInJ)); 
  outputName.append("_");
    outputName.append("o2j-");
    outputName.append(to_string(doubOutJ)); 
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
    file >> Ex >> n >> l >> doubJ >> Pin >> Pout >> doubInJ >> doubOutJ;
  }

  // Define output file name for these specific variables
  ConstructOutputCards();

}

void SelectDeuteronPotential(string label, int Ax, int Zx, int InOut){
  bool x = false; 
  double energy = -1000;
  
  //For input potential, energy is simply beamE*AB
  //For output potential, energy is:
  //    Qval - Ex + beamE*AB
  //i.e.:
  //    (massB+massT) - (massL+massH) - Ex + beamE*AB
  //for now, Qval is an input, could calculate with AME tables in the future?
  if     (InOut == 1){ energy = beamE*AB; }
  else if(InOut == 2){ energy = Qval-Ex+(beamE*AB); }

  // Using ugly if-then ladder for now
  // maybe use map in the future?
  if      (label=="HSS"){   x = deut_HSS2006(Ax, Zx, energy); } 
  else if (label=="AC" ){   x = deut_AC2006 (Ax, Zx, energy); } 
  else if (label=="Bo" ){   x = deut_Bo1988 (Ax, Zx, energy); } 
  else if (label=="DCV"){   x = deut_DCV1980(Ax, Zx, energy); } 
  else if (label=="LH" ){   x = deut_LH1974 (Ax, Zx, energy); }
  else if (label=="PP" ){   x = deut_PP1963 (Ax, Zx, energy); }
  //else if (label=="ADWA" ){ x = adwa_CH1991 (Ax, Zx, energy); }

  if(!x){cout << " FAILED TO SET INCOMING PARAMETERS" << endl;}
}

void SelectProtonPotential(string label, int Ax, int Zx, int InOut){
  bool x = false; 
  double energy = -1000;
  
  //For input potential, energy is simply beamE*AB
  //For output potential, energy is:
  //    Qval - Ex + beamE*AB
  //i.e.:
  //    (massB+massT) - (massL+massH) - Ex + beamE*AB
  //for now, Qval is an input, could calculate with AME tables in the future?
  if     (InOut == 1){ energy = beamE*AB; }
  else if(InOut == 2){ energy = Qval-Ex+(beamE*AB); }

  // Using ugly if-then ladder for now
  // maybe use map in the future?
  if      (label=="KD"){ x = prot_KD2003(Ax, Zx, energy); } 
  else if (label=="CH"){ x = prot_CH1991(Ax, Zx, energy); } 
  else if (label=="Mt"){ x = prot_Mt1971(Ax, Zx, energy); } 
  else if (label=="BG"){ x = prot_BG1969(Ax, Zx, energy); } 
  else if (label=="P" ){ x = prot_P1963 (Ax, Zx, energy); }
  
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
    file << 1    //////  1
         << 0    //////  2
         << 0    //////  3
         << 0    //////  4
         << 0    //////  5
         << 0    //////  6
         << 0    //////  7
         << 0    //////  8
         << 0    //////  9
         << 0    ////// 10
         << 0    ////// 11
         << 0    ////// 12
         << 1    ////// 13
         << 0    ////// 14
         << 0    ////// 15
         << 0    ////// 16
         << 2    ////// 17
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

pair<vector<double>,vector<double>> ReadPotentialFromTWOFNRFile(string potFile){
  string line;
  vector<double> real, imag;
  pair<vector<double>, vector<double>> returnMe;

  ifstream infile(potFile.c_str());
  if(!infile){cout << "ERROR! Cannot open file: " << potFile << endl;}
  
  // Ignore TWOFNR preamble
  int limit = 16; //For (d,p), ignore first 16 lines
  for(int n=0; n<limit; n++){infile.ignore(500,'\n');}

  int counter = 0;
  // Read & write next chunk
  while(getline(infile,line)){
    if(line.length()<13){ break; }
    if(line.length()>13){ real.push_back(stod(line.substr( 0,14))); }
    if(line.length()>27){ real.push_back(stod(line.substr(14,14))); }
    if(line.length()>41){ real.push_back(stod(line.substr(28,14))); }
    if(line.length()>55){ real.push_back(stod(line.substr(42,14))); }
    if(line.length()>69){ real.push_back(stod(line.substr(56,14))); }
  }

  while(getline(infile,line)){
    if(line.length()<13){ break; }
    if(line.length()>13){ imag.push_back(stod(line.substr( 0,14))); }
    if(line.length()>27){ imag.push_back(stod(line.substr(14,14))); }
    if(line.length()>41){ imag.push_back(stod(line.substr(28,14))); }
    if(line.length()>55){ imag.push_back(stod(line.substr(42,14))); }
    if(line.length()>69){ imag.push_back(stod(line.substr(56,14))); }
  }

  returnMe.first  = real;
  returnMe.second = imag;


  if(loud){
    cout << " !!!!!!!! ------------>> " << real.size() << "    " << imag.size() << endl;
    for (int i = 0; i<real.size(); i++){ cout << real.at(i) << " " << imag.at(i) << endl;}
  }

  return returnMe;

}

void WritePotentialFromTWOFNRFile(vector<double> potential, ofstream& file, double scale){
  file << scientific;  file.precision(7);
  
  for(int r = 0; r<potential.size()/5; r++){
    if(loud){cout << r*5+0 << " " << r*5+1 << " " << r*5+2 << " " << r*5+3 << " " << r*5+4  << endl;}
    file << setw(16) << potential.at(r*5 + 0)            * scale 
         << setw(16) << potential.at(r*5 + 1)            * scale 
         << setw(16) << potential.at(r*5 + 2)            * scale 
         << setw(16) << potential.at(r*5 + 3)            * scale 
         << setw(16) << potential.at(r*5 + 4)            * scale; 
    file << "\n"; 
  }

  for(int r = potential.size()%5+1; r --> 1;){
    if(loud){cout << potential.size()-r  << " ";}
    file << setw(16) << potential.at(potential.size()-r) * scale; 
  } 

  file << "\n"; if(loud){cout << endl;}
}

void RunTWOFNR_dp(double Ji, double Jf){
  cout << "========================================================" << endl;
  cout << "           BEGINING FRONT20 AUTOMATED PROCESS           " << endl;
  cout << "========================================================" << endl;
  
//int model_ADWA1 = 5; //JSoper
  int model_ADWA1 = 6; //JTandy (for now...)
  int model_ADWA2 = 1; //Reid soft core (for now...)
  int model_out; 

       if(Pout=="KD"){ model_out   = 6; } // KoningDelaroche
  else if(Pout=="BG"){ model_out   = 1; } // BechettiGreenlees
  else               { model_out   = 2; } // Default to ChapelHill
					  


  double stateQval;
  if(Ex<Sep){ // bound, calculate as normal
    stateQval = Qval - Ex;
  } else {   // unbound, use WBNA -- see Sen et al. (1974) doi: 10.1016/0375-9474(74)90110-9
    stateQval = Qval - (Sep - 0.01);
  }

  string njjj;

  // Move to subdirectory, in order for the output files of TWOFNR to be in a reasonablem place
  system("pwd");  chdir("./adwa/");  system("pwd");
  //system("pwd");  chdir("./adwa15/");  system("pwd");

  // Delete existing tran.adwa file
  remove("tran.adwa");

  // Fill in.front file
  ofstream frontinput("in.front");
  /* name */ 				frontinput << "adwa" << endl;
  /* label */				frontinput << "adwa automated" << endl;
  /* reaction - here (d,p) */		frontinput << 2 << endl;
  /* calcualte entrance DW */		frontinput << 3 << endl;  //0 << endl;
  /* calcualte exit DW */		frontinput << 3 << endl;  //0 << endl;
  /* beam energy per nucleon */		frontinput << beamE << endl;
  /* beam A, Z */			frontinput << AT << " " << ZT << endl;
  	/* default integration range */ 	//frontinput << 1 << endl;
  /* define integration range */ 	frontinput << 2 << endl;
                                        frontinput << maxRad << " " << stepRad << endl;
  	/* default number of partial waves */	//frontinput << 1 << endl;
  /* define number of partial waves */	frontinput << 2 << endl;
                                      	frontinput << npartWav << endl;
  /* default c.o.m steps */		frontinput << "0 0 0" << endl;
  /* L and J of transfered nucleon */	frontinput << l << " " << (double)doubJ/2. << endl;
  /* nodes in function (0s,0p,1s...)*/	frontinput << n << endl;
  /* calc using reaction Q value */	frontinput << 2 << endl;
  /* input reaction Q value */		frontinput << stateQval << endl;
  /* no nonlocality in incident */	frontinput << 1 << endl;
  /* spin, incident channel */		frontinput << Ji << endl;
  /* use built-in potentials */		frontinput << 1 << endl;
  /*** select deut potential ***/	frontinput << model_ADWA1 << endl;
  if(model_ADWA1==6){
  /*** select deut potential ***/	frontinput << model_ADWA2 << endl;
  }
  /* no nonlocality in outgoing */	frontinput << 1 << endl;
  /* spin, outgoing channel */		frontinput << Jf << endl;
  /* use built-in potentials */		frontinput << 1 << endl;
  /*** select prot protential ***/	frontinput << model_out << endl;
  /*** switch case is to keep second selection consistent with first ***/
  switch(model_out){
    case 1:
      frontinput << 1 << endl;
      break;
    case 2:
      frontinput << 2 << endl;
      break;
    case 6:
      frontinput << 4 << endl;
      break;
  }
  /* default <p|d> vertex value */	frontinput << 1 << endl;
  /* use that value */			frontinput << 1 << endl;
  /* use zero range <d|p> vertex */	frontinput << 1 << endl;
  /* default neutron binding potent. */	frontinput << "1.25 0.65" << endl;
  /* default spin-orbit strengh */	frontinput << 6.0 << endl;
  /* default bound st. non-locality */	frontinput << 0 << endl;
  /* default bound st. s-o radius */	frontinput << 0 << endl;

  // Close in.front file
  frontinput.close();

  // Execute front20
  system("./front20 < in.front > /dev/null");
  //system("./front15 < in.front > /dev/null");

  ifstream checkfront("tran.adwa");
  if(!checkfront){
    cout << "!!! ERROR !!! -> front20 failed" << endl;
    return;
  } else {
    cout << "front20 execution complete" << endl;
    checkfront.close();
  }

  // Move back out of subdirectory
  system("pwd");  chdir("..");  system("pwd");

}

void InputBlock5_ADWA(){
  
  // Run TWOFNR
  if(loud){
    cout << "   !!!!! SPINS HERE:::::  ->>> " << ((double)doubInJ/2.0) 
	 << "    " << ((double)doubOutJ/2.0) << endl;
  }

  RunTWOFNR_dp( ((double)doubInJ/2.0) , ((double)doubOutJ/2.0) );

  // Read in the TWOFNR file to get real central and imaginary central
  pair<vector<double>,vector<double>> pairPot;
  pairPot = ReadPotentialFromTWOFNRFile("./adwa/tran.adwa");
  
  // Write to output file
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
    
    //--------------------------------------------------------
    // Real central potential --------------------------------
    file.setf(ios::fixed, ios::floatfield);
    file.precision(3);
    
    // LINE 1 
    file << setw(8) << (double)  8.0
         << setw(8) << (double) -1.0 // Scale by -1  
         << setw(8) << (double)    0  
         << setw(8) << (double)    0  
         << setw(8) << (double)    0  
         << setw(8) << (double)  1.0 // Don't scale yet 
         << setw(8) << (double)    0  
         << setw(8) << (double)    0;  
    file << "\n"; 

    // LINE 2
    file << setw(8) << (double) pairPot.first.size()  //num radial points to be read in
         << setw(8) << (double) 0.0;  // 0 = real, 1 = imag
    file << "\n"; 

    // LINE X
    WritePotentialFromTWOFNRFile(pairPot.first, file, 1.0);
   
    //--------------------------------------------------------
    // Imaginary central potential ---------------------------
    file.setf(ios::fixed, ios::floatfield);
    file.precision(3);

    // LINE 1 
    file << setw(8) << (double)  8.0
         << setw(8) << (double)  1.0 // Don't scale  
         << setw(8) << (double)    0  
         << setw(8) << (double)    0  
         << setw(8) << (double)    0  
         << setw(8) << (double) -1.0 // Slace by -1
         << setw(8) << (double)    0  
         << setw(8) << (double)    0;  
   file << "\n"; 

    // LINE 2
    file << setw(8) << (double) pairPot.second.size()  //num angles to be read in
         << setw(8) << (double) 1.0;  // 0 = real, 1 = imag
    file << "\n"; 

    // LINE X
    WritePotentialFromTWOFNRFile(pairPot.second, file, 1.0);

    //----------------------------------------------------------
    // Real & imaginary spin-orbit potential -------------------
    file.setf(ios::fixed, ios::floatfield);
    file.precision(3);
  
    SelectProtonPotential(Pout, AT, ZT, 1);
    //SelectProtonPotential(Pout, AH, ZH, 1);
    file << setw(8) << (double) -4.0
         << setw(8) << (double) vso    
         << setw(8) << (double) rso0
         << setw(8) << (double) aso
         << setw(8) << (double) 0.0
         << setw(8) << (double) vsoi   
         << setw(8) << (double) rsoi0
         << setw(8) << (double) asoi;
    file << "\n"; 
    // for some reason, this is not being correctrly read as the end of PARTICLE 1 input?

    file.close(); 
    if(loud){cout << "Written Input Block 5 -- ADWA" << endl;}
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
    file << setw(8) << (double) Ex-Sep  // Binding energy of single particle
         << setw(8) << (double) AB-AL   // Single particle mass
         << setw(8) << (double) ZB-ZL   // Single particle charge
         << setw(8) << (double) AT      // Core nucleus mass
         << setw(8) << (double) ZT      // Core nucleus charge
         << setw(8) << (double) 1.25    // Reduced charge radius
         << setw(8) << (double) 0.0     // Diffuseness (not implemented)
         << setw(8) << (double) 0.0     // Nonlocality parameter
         << setw(8) << (double) AB-AL;  // TWICE SPIN OF NEUTRON! Convenient for now
         //<< setw(8) << (double) abs(doubFinalSpin-doubInitSpin);  //TWICE SPIN OF NEUTRON! Convenient for now
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

