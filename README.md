# qwackPot -- direct transfer in DWUCK4, for dummies 

This code takes simple, human-readable inputs and produces the DWUCK4 input card to perform this calcualtion. Additionally, bundled into this is `runQwackAndDwuck.sh`, which is another step removed from DWUCK4 and generates the qwackPot inputs, runs DWUCK4, and then suitably renames the output file `for020.dat` (inelastic transfer differential cross section, in the centre-of-mass frame). 

This is a quick personal-use code, and has not been rigorously stress-tested. Use at your own risk!

## Many thanks to...
The functions to calculate the DWBA potentials in `qPotentials.h` are from [Cleopatra for Ptolemy](https://fsunuc.physics.fsu.edu/git/rtang/PtolemyGUI/src/commit/f1b2d4a580f881893ed81447fe546bda6406421d/Cleopatra/potentials.h) by [Ryan Tang](https://fsunuc.physics.fsu.edu/git/rtang), which is itself based on the [globals_beta_v5](https://docs.google.com/spreadsheets/d/1fFDyJvTu4gAc8hc4gSqU7e7sp_mO_aK9/edit?pli=1&gid=1068274165#gid=1068274165) spreadsheet by [Ben Kay](https://sites.google.com/view/opticalpotentials/optical-potential-parameterizations). All I've done on top of this is make the potentials appropriate for DWUCK4, format the in/outputs, and a.

## How to use
The code takes **GENERAL** input values, defining the experimental conditions, and **SPECIFIC** input values, defining individual populated states in the heavy recoil nucleus.

- for bash script to work, the dwuck4 executable, DWUCK4.exe, has to be as the same path -- i.e. /path/to/runQwackAndDwuck.sh, /path/to/qwuackPot/... and /path/to/DWUCK4.exe. Else, edit bash.

(expand on this....)


## To-do list
- Add ADWA input potentials
- Add support for (d,t), (p,d), etc.
- Finish README
