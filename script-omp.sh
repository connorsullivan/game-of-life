#!/bin/bash

printf "\nRunning Test Case: OMP\n\n"

printf "\nTrial 1 of 7:\n\n"
./gol-omp 5000 5000 1

printf "\nTrial 2 of 7:\n\n"
./gol-omp 5000 5000 2

printf "\nTrial 3 of 7:\n\n"
./gol-omp 5000 5000 4

printf "\nTrial 4 of 7:\n\n"
./gol-omp 5000 5000 8

printf "\nTrial 5 of 7:\n\n"
./gol-omp 5000 5000 10

printf "\nTrial 6 of 7:\n\n"
./gol-omp 5000 5000 16

printf "\nTrial 7 of 7:\n\n"
./gol-omp 5000 5000 20

printf "\nDone.\n\n"
