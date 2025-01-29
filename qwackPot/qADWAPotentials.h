// Potentials r qwackPot calculator (by C. J. Paxman)

bool adwa_CH1991(int A, int Z, double ELab){
  //Pulled from A. Matta's easyDWUCK

  double N = A - Z;
//ELab = ELab/2.;

  double V0 = 52.9, Vt = 13.1, Ve = -0.299;
  double r0 = 1.250, r00= -0.225;
  double a0 = 0.690, rc = 1.24, rc0= 0.12;
  double V_SO = 5.9, r_SO = 1.34, r0_SO= -1.2, a_SO = 0.63;

  double Wv0 = 7.8, Wve0 = 35, Wvew = 16;
  double Ws0 = 10.0, Wst = 18, Wse0 = 36, Wsew = 37;
  double rw = 1.33, rw0= -0.42, aw = 0.69;

  double r_mean2 = 0.51;   //(valeur moyenne de (r au carre))

  //Real part
  double R0  = r0  * pow(A, 1./3) + r00 ;   //real potential radius
  double Rc  = rc  * pow(A, 1./3) + rc0 ;   //coulomb potential radius
  double RSO= r_SO* pow(A, 1./3) + r0_SO ;  //spin-orbit potential radius
  
  //Coulomb Energy
  //proton
  double Ec_p = 1.73 *Z / Rc;
  double Up = V0 + Vt * (N - Z) / A + (ELab - Ec_p) * Ve;
  
  //neutron
  double Ec_n = 0;
  double Un = V0 - Vt * (N - Z) / A + (ELab - Ec_n) * Ve;


  
  //Imaginary part
  double Rw = rw  * pow(A, 1./3) + rw0 ; 
  double Wv_p = Wv0 / ( 1 + exp((Wve0 -(ELab - Ec_p))/(Wvew)) ) ;
  double Wv_n = Wv0 / ( 1 + exp((Wve0 -(ELab - Ec_n))/(Wvew)) ) ;

  double Ws = 0, Ws_n = 0, Ws_p = 0;

  Ws_p = ( Ws0 + Wst * (N - Z) /A ) / (1 + exp (((ELab - Ec_p) - Wse0)/(Wsew)));
  Ws_n = ( Ws0 - Wst * (N - Z) /A ) / (1 + exp (((ELab - Ec_n) - Wse0)/(Wsew)));

  //Adiabatic calculations 
  //Real Part of the potential
  double da_u = 5. /(14 * M_PI*M_PI) * r_mean2 / a0; 
  double  a_u = a0 + da_u;
  
  double dU = (Un + Up)* pow(M_PI/R0, 2.) * (a0*a0 - a_u*a_u) / (1 + pow(M_PI*a_u/R0, 2.));
  //cout << Up << " " << Un << " " << dU << endl;
  double U = Un + Up + dU;
  
  //Imaginary part of the potential
  double da_w = 3./(10 * M_PI*M_PI) * r_mean2 / aw; 
  double a_w = aw + da_w;
  double dW = - da_w / a_w * (1 + 2/3 * pow(M_PI*aw/Rw, 2.));
  double W = Wv_p + Wv_n + dW;

  //Imaginary part for surface interaction
  Ws = Ws_p + Ws_n + dW;

  //Spin-orbit part of the potential do not change (hypothesis of the paper)
  double da_so =  5. /(14 * M_PI*M_PI) * r_mean2 / a_SO; ;
  double a_so  = a_SO + da_so ;
  
  double dVSO = 2 * V_SO * pow(M_PI/RSO,2.) * (a_SO*a_SO - a_so*a_so) / (1 + pow(M_PI*a_so/RSO,2.));
  double VSO  = 2 * V_SO + dVSO;

  R0 = R0 / pow(A, 1./3) ;
  Rc = Rc / pow(A, 1./3) ;
  RSO = RSO / pow(A, 1./3) ;
  Rw = Rw /  pow(A, 1./3) ;     

  // Set global variables
  v = -U;
  r0 = R0;
  a = a_u;

  vi = 4*W;
  ri0 = R0;
  ai = a_u;

  vsi = 4.*Ws;
  rsi0 = Rw;
  asi = a_w;

  vso = 2.*VSO;
  rso0 = RSO;
  aso = a_so;

  vsoi = 0;
  rsoi0 = 0;
  asoi = 0;

  rc0 = Rc;

  return true;

}






