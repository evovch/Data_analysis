#!/bin/bash

OUTPUTLOG=fittingLog.txt

# Transition time jitter = 0 --------------

# Time constants fixed
root -l -b -q "fitIndivid.C(\"../3_Simulation/0ttj_take3/simulation_res_0ttj_sum.root\", kTRUE, 0., \"fitting_0ttj_fixedTaus.eps\")" > ${OUTPUTLOG}

# Time constants variable
root -l -b -q "fitIndivid.C(\"../3_Simulation/0ttj_take3/simulation_res_0ttj_sum.root\", kFALSE, 0., \"fitting_0ttj_freeTaus.eps\")" >> ${OUTPUTLOG}

# Transition time jitter = 300 ps ---------

# Time constants fixed
root -l -b -q "fitIndivid.C(\"../3_Simulation/300ttj_take3/simulation_res_300ttj_sum.root\", kTRUE, 4., \"fitting_300ttj_fixedTaus.eps\")" >> ${OUTPUTLOG}

# Time constants variable
root -l -b -q "fitIndivid.C(\"../3_Simulation/300ttj_take3/simulation_res_300ttj_sum.root\", kFALSE, 4., \"fitting_300ttj_freeTaus.eps\")" >> ${OUTPUTLOG}
