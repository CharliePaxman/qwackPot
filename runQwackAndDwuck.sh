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
#AT=20; ZT=8; #Target
AB=2;  ZB=1; #Beam
AL=1;  ZL=1; #Light

# Define energies of the reaction
Qval=5.383; #Q-value of the reaction
Ebm=7.99;   #Energy of the beam (in MeV/u)
Sep=7.608;  #Separation energy of transfered nucleon
#Qval=1.581; #Q-value of the reaction
#Ebm=10.5;   #Energy of the beam (in MeV/u)
#Sep=3.81;  #Separation energy of transfered nucleon



# Value of D_0^2 / 10^4 [MeV fm^3] , see dw4_doc Eq. 2.10
# (usually = 1.5)
D20="1.5"

# ----------------------------------------------------
# States and Orbitals --------------------------------
#   Here, you can define arrays that are iterated over
#   in order to calculate many input cards at once

# Define the energies of the states to model (in MeV)
#stateArr=("0.000" "1.675" "3.572" "4.071" "4.456" "5.004" "5.228" "5.629" "7.252" "7.622" "7.855" "8.313" "8.561")
#stateArr=("0.000" "1.000" "2.000" "3.000" "4.000" "5.000" "6.000" "7.000" "8.000" "9.000")
stateArr=("0.000" ) #"1.21" "4.77" "6.17")

# Define the n, l, and 2*j quantum numbers of the transfered nucleon
#    Note that the input is weird here! Need to add each orbital 
#    as the same index of each of the three arrays... sorry...
nArr=(    "0" ) #"1" ) #"0" "0"  "0") #"1" ) #"0" "1" "0")
lArr=(    "2" ) #"0" ) #"2" "3"  "3") #"0" ) #"3" "1" "4")
doubJArr=("5" ) #"1" ) #"3" "7"  "5") #"1" ) #"7" "3" "9")


# Double spin of initial target state (all possible)
initialDoubJ=("5") 
#initialDoubJ=("0") 

# Double spin of final target state (all possible)
#    Note that if the initial 2*J is even, then,
#    the final 2*J must be odd, and vice versa
#            !!!! THIS IS TRUE !!!!
#          !!!! FOR DP TRANSFER !!!!
#declare -A finalDoubJ; 
#for iJ in ${initialDoubJ[@]}; do
#  for tJ in ${doubJArr[@]}; do
#    echo " initial double J = ""$iJ"", transfered dobule J = ""$tJ"
#    for ((oJ=$(($iJ-$tJ)); oJ<=$(($iJ+$tJ)); oJ++)); do
#      if [ $(($iJ%2)) -eq 0 ]; then #---  in 2*J is even
#        if [ $(($oJ%2)) -eq 1 ]; then # -- -- -- out 2*J is odd
#          echo " in even, out odd -> ""$oJ"
#          finalDoubJ+=" ""$oJ";
#	fi
#      else #----------------------------  in 2*J is odd
#        if [ $(($oJ%2)) -eq 0 ]; then # -- -- -- out 2*J is even
#          echo " in odd, out even -> ""$oJ"
#          finalDoubJ+=" ""$oJ";
#	fi
#      fi
#    done
#  done
#done
##finalDoubJ=("2" ) #"2" "4" "6" "8" "10" "12")





# Define the input and output potentials to use
inputPotArr=(  "ADWA" ) #"AC" )
outputPotArr=( "CH"   ) #"KD" )
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
        for iJ in ${initialDoubJ[@]}; do
          #=============================
          declare -A finalDoubJ; 
                                                   #echo " initial double J = ""$iJ"", transfered dobule J = ""$tJ"
          for ((oJ=$(($iJ-${doubJArr[$i]})); oJ<=$(($iJ+${doubJArr[$i]})); oJ++)); do
            if [ $(($iJ%2)) -eq 0 ]; then # ------------------------ in 2*J is even
              if [ $(($oJ%2)) -eq 1 ]; then # -- -- -- -- out 2*J is odd
                                                   #echo " in even, out odd -> ""$oJ"
                echo -e "$E""$s""${nArr[$i]}""$s""${lArr[$i]}""$s""${doubJArr[$i]}""$s""$iP""$s""$oP""$s""$iJ""$s""$oJ">> $inS
                iter=$(($iter+1))
              fi
            else #-------------------------------------------------- in 2*J is odd
              if [ $(($oJ%2)) -eq 0 ]; then # -- -- -- -- out 2*J is even
                                                   #echo " in odd, out even -> ""$oJ"
                echo -e "$E""$s""${nArr[$i]}""$s""${lArr[$i]}""$s""${doubJArr[$i]}""$s""$iP""$s""$oP""$s""$iJ""$s""$oJ">> $inS
                iter=$(($iter+1))
              fi
            fi
          done
          #=============================
        done
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
  ./DWUCK4.exe "qwackPot/""$line" #> /dev/null

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

