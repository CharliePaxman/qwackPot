
void TWOFNRforDWUCK_dp(double Ex, double Jf, double particleN, double particleL, double particleJ, double Ji, int model_ADWA1, int model_ADWA2){
 
  cout << "========================================================" << endl;
  cout << "           BEGINING FRONT20 AUTOMATED PROCESS           " << endl;
  cout << "========================================================" << endl;
  
  int beamA = 19, beamZ = 8;
//int model_in_A = 6; //JTandy (for now...)
//int model_in_B = 1; //Reid soft core (for now...)
//int model_out_A = 1; //BechettiGreenlees 
//int model_out_A = 2; //ChapelHill 
//int model_out_A = 3; //Menet 
//int model_out_A = 4; //Perey 
//int model_out_A = 5; //JLM
//int model_out_A = 6; //KoningDelaroche 

  string njjj;

  // Delete existing tran.jjj and 24.jjj files
  remove("tran.jjj");
  remove("24.jjj");

  // Fill in.front file
  ofstream frontinput("in.front");
  /* name */ 				frontinput << "jjj" << endl;
  /* label */				frontinput << "auto" << endl;
  /* reaction - here (d,p) */		frontinput << 2 << endl;
  /* calcualte entrance DW */		frontinput << 0 << endl;
  /* calcualte exit DW */		frontinput << 0 << endl;
  /* beam energy per nucleon */		frontinput << beamE << endl;
  /* beam A, Z */			frontinput << beamA << " " << beamZ << endl;
  /* default integration range */ 	frontinput << 1 << endl;
  /* default number of partial waves */	frontinput << 1 << endl;
  /* default c.o.m steps */		frontinput << "0 0 0" << endl;
  /* L and J of transfered nucleon */	frontinput << particleL << " " << particleJ << endl;
  /* nodes in function (0s,0p,1s...)*/	frontinput << particleN << endl;
  /* calc using reaction Q value */	frontinput << 2 << endl;
  /* input reaction Q value */		frontinput << Qval << endl;
  /* no nonlocality in incident */	frontinput << 1 << endl;
  /* spin, incident channel */		frontinput << Ji << endl;
  /* use built-in potentials */		frontinput << 1 << endl;
  /*** select deut potential ***/	frontinput << model_ADWA1 << endl;
  if(model_in_A==6){
  /*** select deut potential ***/	frontinput << model_in_B << endl;
  }
  /* no nonlocality in outgoing */	frontinput << 1 << endl;
  /* spin, outgoing channel */		frontinput << Jf << endl;
  /* use built-in potentials */		frontinput << 1 << endl;
  /*** select prot protential ***/	frontinput << model_out_A << endl;
  /*** switch case is to keep second selection consistent with first ***/
  switch(model_out_A){
    case 1:
      frontinput << 1 << endl;
      break;
    case 2:
      frontinput << 2 << endl;
      break;
    case 3:
      frontinput << 2 << endl;
      break;
    case 4:
      frontinput << 2 << endl;
      break;
    case 5:
      frontinput << 3 << endl;
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
  string exec_front = "(exec ";
  exec_front.append(twofnrDir);
  exec_front.append("/front20 < in.front > /dev/null)");

  cout << "Executing: " << exec_front << endl;
  system(exec_front.c_str());

  ifstream checkfront("tran.jjj");
  if(!checkfront){
    cout << "!!! ERROR !!! -> front20 failed" << endl;
    return 0;
  } else {
    cout << "front20 execution complete" << endl;
    checkfront.close();
  }



}





