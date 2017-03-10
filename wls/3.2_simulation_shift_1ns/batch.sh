#!/bin/bash

N=1000

for i in {1..20}
do
	./prog "300ttj_$i" $N &
	sleep 1
done

wait

hadd -f simulation_res_300ttj_sum.root simulation_res_*.root
