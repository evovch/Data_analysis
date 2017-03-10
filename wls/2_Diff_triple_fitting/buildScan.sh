#!/bin/bash

for i in {200..1400..100}
do
	root -l -b -q "fitting_WLSon.C(\"../WLSon_analysis_sum_rings_calib_corr.root\", $i)"
done
