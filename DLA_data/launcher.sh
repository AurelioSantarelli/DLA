#!/bin/bash

arg1="1000"
arg2="5000"
arg3="10000"
arg4="15000"

echo -n " I"

# Avvia il primo programma nel primo terminale
./DLA_st $arg1 > /dev/null

# Attendi prima di avviare il programma successivo
wait

echo -n " I"

# Avvia il secondo programma nel secondo terminale
./DLA_mt $arg1 > /dev/null

# Attendi prima di avviare il programma successivo
wait

echo -n " I"

# Avvia il primo programma nel primo terminale
./DLA_st $arg2 > /dev/null

# Attendi prima di avviare il programma successivo
wait

echo -n " I"

# Avvia il secondo programma nel secondo terminale
./DLA_mt $arg2 > /dev/null 

# Attendi prima di avviare il programma successivo
wait

echo -n " I"

# Avvia il primo programma nel primo terminale
./DLA_st $arg3 > /dev/null

# Attendi prima di avviare il programma successivo
wait

echo -n " I"

# Avvia il secondo programma nel secondo terminale
./DLA_mt $arg3 > /dev/null

# Attendi prima di avviare il programma successivo
wait

echo -n " I"

# Avvia il primo programma nel primo terminale
./DLA_st $arg4 > /dev/null

# Attendi prima di avviare il programma successivo
wait

echo -n " I"

# Avvia il secondo programma nel secondo terminale
./DLA_mt $arg4 > /dev/null

# Attendi prima di chiudere
wait

echo " I"

./logProcessor  > /dev/null

echo "./logProcessor lanciato con successo!" 

wait






