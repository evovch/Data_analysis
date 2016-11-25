#!/bin/bash

DIR=/media/evovch/Data/DATA/threshold_scan_2

CONFIGFOLDER=config

# If the config directory exists - clean it
# Otherwise create it
if [ -d "$CONFIGFOLDER" ]; then
	rm -fv ${CONFIGFOLDER}/config${i}.cfg
else
	mkdir $CONFIGFOLDER
fi

let i=1

for FILE in ${DIR}/*.hld
do
	echo "HLDFILE=${FILE}" > ${CONFIGFOLDER}/config${i}.cfg
	echo "CALIBFILE=/home/evovch/Documents/Analysis_Sep2016/threshold_scan_2/calibration_files/" >> ${CONFIGFOLDER}/config${i}.cfg
	echo "MATCHEDEDGESFILE=/home/evovch/Documents/Analysis_Sep2016/threshold_scan_2/hits_files/" >> ${CONFIGFOLDER}/config${i}.cfg
	echo "UNPACKINFOFILE=/home/evovch/Documents/Analysis_Sep2016/threshold_scan_2/analysis_results/" >> ${CONFIGFOLDER}/config${i}.cfg
	echo "EDGEMATCHINFOFILE=/home/evovch/Documents/Analysis_Sep2016/threshold_scan_2/analysis_results/" >> ${CONFIGFOLDER}/config${i}.cfg
	echo "EVBLDINFOFILE=/home/evovch/Documents/Analysis_Sep2016/threshold_scan_2/analysis_results/" >> ${CONFIGFOLDER}/config${i}.cfg
	echo "ANALYSISINFOFILE=/home/evovch/Documents/Analysis_Sep2016/threshold_scan_2/analysis_results/" >> ${CONFIGFOLDER}/config${i}.cfg
	echo "CORRECTIONSFILE=/home/evovch/Documents/Analysis_Sep2016/threshold_scan_2/corrections/" >> ${CONFIGFOLDER}/config${i}.cfg
	echo "RINGSANALYSISINFOFILE=/home/evovch/Documents/Analysis_Sep2016/threshold_scan_2/analysis_results/" >> ${CONFIGFOLDER}/config${i}.cfg

	let i=i+1

done