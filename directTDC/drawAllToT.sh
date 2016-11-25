#!/bin/bash

OUTDIRNAME=pictures_ToT
if [ -d "$OUTDIRNAME" ]; then
	rm -fv $OUTDIRNAME/*
else
	mkdir $OUTDIRNAME
fi


INDIR=results_calib_real

let iChannel=2

for FILE in ${INDIR}/analysis_info_*.root
do
	FILENAME=$(basename ${FILE})
	root -l -b -q "drawToT.C(${iChannel}, \"${FILENAME}\")"

	let iChannel=iChannel+1
done
