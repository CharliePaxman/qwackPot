#!/bin/bash

#### qwackPot ####
# Code for generating DWUCK4 input cards rapidly and flexibly

#=====================================================
################# USER DEFINITIONS ###################
# ----------------------------------------------------
# Experimental Conditions ----------------------------
#   The experimental conditions are applied to every
#   calcualtion performed in this bash script run

# Define the reaction
AT=19; ZT=8; #Target
AB=2;  ZB=1; #Beam
AL=1;  ZL=1; #Light

doubInitSpin=5; #Twice spin of the initial state

# Define energies of the reaction
Qval=5.383; #Q-value of the reaction
Ebm=7.99;   #Energy of the beam (in MeV/u)
Sep=7.608;  #Separation energy of transfered nucleon

# Double spin of initial target state (all possible)
initialDoubJ=("5") 

# Double spin of final target state (all possible)
finalDoubJ=("0" "1" "2" "3" "4" "5" "6")

# Value of D_0^2 / 10^4 [MeV fm^3] , see dw4_doc Eq. 2.10
# (usually = 1.5)
D20="1.5"

# ----------------------------------------------------
# States and Orbitals --------------------------------
#   Here, you can define arrays that are iterated over
#   in order to calculate many input cards at once

# Define the energies of the states to model (in MeV)
stateArr=("0.000" "1.675" "3.572" "4.071" "4.456" "5.004" "5.228" "5.629" "7.252" "7.622" "7.855" "8.313" "8.561")
#stateArr=("0.000" "1.000" "2.000" "3.000" "4.000" "5.000" "6.000" "7.000" "8.000" "9.000")

# Define the n, l, and 2*j quantum numbers of the transfered nucleon
#    Note that the input is weird here! Need to add each orbital 
#    as the same index of each of the three arrays... sorry...
nArr=(    "0" "1" "0" ) #"1" ) #"0" "1" "0")
lArr=(    "2" "0" "2" ) #"0" ) #"3" "1" "4")
doubJArr=("5" "1" "3" ) #"1" ) #"7" "3" "9")

# Define the input and output potentials to use
inputPotArr=( "LH" ) #"ADWA" "DCV")
outputPotArr=("BG" ) #"CH"   "KD")
#inputPotArr=("ADWA" "AC" "HSS")
#outputPotArr=("CH" "CH" "CH")
#inputPotArr=("HSS" "AC" "Bo" "DCV" "LH" "PP")
#outputPotArr=("KD" "CH" "Mt" "BG" "P")

#=====================================================
############# GENERATING QWACKPOT FILES ##############
# ----------------------------------------------------
# Preparations ---------------------------------------
cd qwackPot
rm ListOfOutputFiles.txt
inG="./Input_General.txt"
inS="./Input_Specific.txt"
s=" "
iter=0

# ----------------------------------------------------
# Experimental Conditions ----------------------------
rm $inG
echo -e "$AT""$s""$ZT""$s"   "#TARGET"                  >> $inG
echo -e "$AB""$s""$ZB""$s"   "#BEAM"                    >> $inG
echo -e "$AL""$s""$ZL""$s"   "#LIGHT"                   >> $inG
echo -e "$Sep""$s"           "#SEPARATION-ENERGY"       >> $inG
echo -e "$Qval""$s"          "#Q-VALUE"                 >> $inG
echo -e "$Ebm""$s"           "#BEAM-ENERG-MeVu"         >> $inG

# ----------------------------------------------------
# States and Orbitals --------------------------------
rm $inS
for E in ${stateArr[@]}; do
  for ((i=0;i<${#nArr[@]};i++)); do
    for iP in ${inputPotArr[@]}; do
      for oP in ${outputPotArr[@]}; do
        echo -e "$E""$s""${nArr[$i]}""$s""${lArr[$i]}""$s""${doubJArr[$i]}""$s""$iP""$s""$oP" >> $inS
	iter=$(($iter+1))
      done
    done
  done
done

#=====================================================
################# RUNNING QWACKPOT ###################
# ----------------------------------------------------
# Finally! -------------------------------------------
root -q qwackPot.cxx
cd ..

#=====================================================
########## RUN DWUCK4 WITH QWACKPOT FILES ############
echo "=========== STARTING DWUCK4 ==========="

# ----------------------------------------------------
# Loop over qwackPot files and run DWUCK4 ------------
while IFS= read -r line; do
  # Run DWUCK4 on the qwackPot files (suppress output)
  ./DWUCK4.exe "qwackPot/""$line" > /dev/null

  # Move and rename output file
  # Note: need to remove 'cards/' from front of string
  mv for020.dat "Output_Unprocessed/""${line:6}"
  #echo "  Moved output to Output_Unprocessed/""${line:6}"
done < "qwackPot/ListOfOutputFiles.txt"

# ----------------------------------------------------
# Spring cleaning ------------------------------------
rm 'for021.dat'
rm 'fort.7'

echo "================= END ================="
echo "======================================="

#=====================================================
########## CONVERT DWUCK4 OUTPUT TO mb/sr ############
echo "========== CONVERTING FORMAT =========="

# ----------------------------------------------------
# Loop over unprocessed output files -----------------
while IFS= read -r line; do
  unprocessedfile="Output_Unprocessed/""${line:6}"
 
  # Compile qwackConvert
  g++ -o qwackPot/qwackConvert qwackPot/qwackConvert.cxx

#  { read 
#    read
#    while IFS="," read -r angle cos sigma pol asy Dnn Knn Inn; do
      for inJ in ${initialDoubJ[@]}; do
        for outJ in ${finalDoubJ[@]}; do
          for tranJ in ${doubJArr[@]}; do
            ./qwackPot/qwackConvert $unprocessedfile $inJ $outJ $tranJ $D20
          done
        done
      done
#    done
#  } < $unprocessedfile



done < "qwackPot/ListOfOutputFiles.txt"

echo "================= END ================="
echo "======================================="

#=====================================================

