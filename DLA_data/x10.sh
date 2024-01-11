#!/bin/bash

for i in {1..10}
do
  echo -n "$i iterazione:   "
  ./launcher.sh &  # Esegui lo script in background
  wait              # Attendere il completamento dello script precedente
  echo -n ""
  echo "Simulazione completata con successo!"
done
