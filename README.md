# qwackPot -- direct transfer in DWUCK4, for dummies 

This code takes simple, human-readable inputs and produces the DWUCK4 input card to perform this calcualtion. Additionally, bundled into this is `runQwackAndDwuck.sh`, which is another step removed from DWUCK4. This 1) generates the qwackPot inputs, 2) runs qwackPot to make DWUCK4 input cards, 3) runs DWUCK4, 4) suitably renames the output file `for020.dat` (inelastic transfer differential cross section, in the centre-of-mass frame) in `Outputs_Unprocessed/`, and then 5) scales the DWUCK4 output depending on the outgoing spin-parity, in `Outputs_Processed/`. 

This is a quick personal-use code, and has not been rigorously stress-tested. Use at your own risk!

## How to use
The code takes **GENERAL** input values, defining the experimental conditions, and **SPECIFIC** input values, defining individual populated states in the heavy recoil nucleus. These are all defined by the user in the `runQwackAndDwuck.sh` script.

- Pull this repository in the same location as the `DWUCK4.exe` executable (i.e. /path/to/runQwackAndDwuck.sh is the same as /path/to/DWUCK4.exe). Else, edit the bash script paths.
- Define the reaction conditions:
  - `AT, ZT` -- target
  - `AB, ZB` -- beam
  - `AL, ZL` -- light ejectile
  - `Qval` -- reaction Q-value
  - `Ebm`  -- beam energy in MeV/u
  - `Sep` -- currently, single neutron separation energy
- Define the states and orbitals accessed by the reaction:
  - `stateArr` -- array of all state energies to model
  - `nArr`, `lArr`, `doubJarr` -- arrays of the $n$, $\ell$ and $2j$ quantum numbers of the transferred nucleon
  - `initialDoubJ` -- array of double the initial target nucleus spin-parity
  - `inputPotArr` -- incoming potential to use
  - `outputPotArr` -- outgoing potential to use
- Run `./runQwackAndDwuck.sh`
- Outputs are in `Outputs_Unprocessed/` and `Outputs_Processed/`.

(expand on this....)


## To-do list
- Add ADWA input potentials
- Add support for (d,t), (p,d), etc.
- Finish README

## Many thanks to...

(expand on this, get permissions from various people!)

The functions to calculate the DWBA potentials in `qPotentials.h` are based on [Cleopatra for Ptolemy](https://fsunuc.physics.fsu.edu/git/rtang/PtolemyGUI/src/commit/f1b2d4a580f881893ed81447fe546bda6406421d/Cleopatra/potentials.h) by [Ryan Tang](https://fsunuc.physics.fsu.edu/git/rtang), which is itself based on the [globals_beta_v5](https://docs.google.com/spreadsheets/d/1fFDyJvTu4gAc8hc4gSqU7e7sp_mO_aK9/edit?pli=1&gid=1068274165#gid=1068274165) spreadsheet by [Ben Kay](https://sites.google.com/view/opticalpotentials/optical-potential-parameterizations). 

The functions to calculate the ADWA potentials use TWOFNR by M. Toyama, M. Igarashi, and N. Kishida and FRONT by J. A. Tostevin
