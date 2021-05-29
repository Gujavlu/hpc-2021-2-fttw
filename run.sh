#!/bin/bash
mpicc fftw3-mpi_helloworld.c -lfftw3_mpi -lfftw3 -lm -lmpich -Wall -Ofast

while getopts "n:o" opt
	do
		case "$opt" in
			n) N="$OPTARG" ;;
			o) FILE="$OPTARG" ;;
		esac
	done

if [ -z "$N" ]; then
	N=4;
fi

if [ -z "$FILE" ]; then
	FILE="out.txt";
fi

mpiexec -n "$N" ./a.out >> "$FILE"
