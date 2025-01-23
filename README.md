# qwackPot -- direct transfer in DWUCK4, for dummies 

This code takes simple, human-readable inputs and produces the DWUCK4 input card to perform this calcualtion. Additionally, bundled into this is `runQwackAndDwuck.sh`, which is another step removed from DWUCK4 and generates the qwackPot inputs, runs DWUCK4, and then suitably renames the output file `for020.dat` (inelastic transfer differential cross section, in the centre-of-mass frame). 

The code takes **GENERAL** input values, defining the experimental conditions, and **SPECIFIC** input values, defining individual populated states in the heavy recoil nucleus.

- for bash script to work, the dwuck4 executable, DWUCK4.exe, has to be as the same path -- i.e. /path/to/runQwackAndDwuck.sh, /path/to/qwuackPot/... and /path/to/DWUCK4.exe. Else, edit bash.

(expand on this....)
