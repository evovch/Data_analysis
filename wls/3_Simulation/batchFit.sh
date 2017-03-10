#!/bin/bash

OUTPUTLOG=fittingLog.txt

root -l -b -q "fit2.C(\"300ttj_take3/simulation_res_300ttj_sum.root\", kTRUE, \"fitting_fixedTau.eps\")" > ${OUTPUTLOG}

root -l -b -q "fit2.C(\"300ttj_take3/simulation_res_300ttj_sum.root\", kFALSE, \"fitting_freeTau.eps\")" >> ${OUTPUTLOG}
