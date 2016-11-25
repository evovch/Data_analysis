#!/bin/bash

DIRNAME=pictures_signal_eps
if [ -d "$DIRNAME" ]; then
	rm -fv $DIRNAME/*
else
	mkdir $DIRNAME
fi

DIRNAME=pictures_signal_png
if [ -d "$DIRNAME" ]; then
	rm -fv $DIRNAME/*
else
	mkdir $DIRNAME
fi

DIRNAME=pictures_noise_eps
if [ -d "$DIRNAME" ]; then
	rm -fv $DIRNAME/*
else
	mkdir $DIRNAME
fi

DIRNAME=pictures_noise_png
if [ -d "$DIRNAME" ]; then
	rm -fv $DIRNAME/*
else
	mkdir $DIRNAME
fi

root -l -b -q buildBlackboard.C