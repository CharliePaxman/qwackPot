////////////////////////////////////////////
// qwackPot                               //
// DWUCK4 input card automated calcualtor //
////////////////////////////////////////////

#include "qwackPot.h"

void qwackPot(){
  cout << "=======================================" << endl;
  cout << "========== STARTING QWACKPOT ==========" << endl;
  cout << "... CURRENTLY ONLY WORKING FOR (d,p) REACTIONS! ..." << endl;

  loud=false;

  // General inputs (i.e. experiment information)
  ClearGlobalVariables_General();
  LoadGlobalVariables_General();

  // Count number of inputs in specific inputs file
  int nlines = 0;
  ifstream specFile("Input_Specific.txt"); string line;
  while(getline(specFile,line)){++nlines;}

  for(int i = 0; i < nlines; i++){
    if(loud){cout << "---------------------------------------" << endl;}

    // Specific inputs (i.e. state energy and orbital)
    ClearGlobalVariables_Specific();
    LoadGlobalVariables_Specific(i);

    // 'Preamble' blocks 
    InputBlock1();
    InputBlock2(0.0, 0.5, 360.0);
    InputBlock3(15, 1);
    InputBlock4(0.1, 0.0, 50.);
  
    // Incoming channel
    SelectIncomingPotential(Pin);
    InputBlock5();
  
    // Outgoing channel
    SelectOutgoingPotential(Pout);
    InputBlock6();
  
    // Radial form factor for transfer
    InputBlock7();
   
    // END block
    InputEndBlock();  
    
    if(loud){cout << "---------------------------------------" << endl;}
  }

  cout << "================= END =================" << endl;
  cout << "=======================================" << endl;
}

