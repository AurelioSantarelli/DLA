# DLA
Diffusion-limited aggregation project (DLA) in C.

# Creator: Aurelio Santarelli #

Il progetto e' strutturato in due parti:

-DLA_graphic: Sono due istanze dello stesso programma che simulano la crescita cristallina DLA e stampano a schermo 
              una rappresentazione grafica utilizzando la libreria C, SDL. Le due versioni una multithread (DLA_mt_graphic.c)
              e una single-thread. 
              #NOTA#: Entrambe le versioni chiedono come primo e unico argomento da terminale il numero di particelle.
              
-DLA_data: Sono due istanze dello stesso programma che simulano la crescita cristallina DLA ma salvano in due log separati
           (log_mt.txt per la vesrione multithread e log_st.txt per la versione single-thread) il tempo in cui vengono 
           fatte le operazioni di collisione randomica delle particelle rispettivamente per DLA_st e DLA_mt e salva i risultati,
           nei rispettivi log, insieme alla data/ora e il num. di particelle che hanno colliso.
           
Componenti ausiliarie (DLA_data):

-Launcher.sh: E' uno script che lancia DLA_mt.c e DLA_st.c in sequenza entrambi con 4 valori come argomenti da terminale 
              (1000,5000,10000,15000).
           
-logProcessor.c: E' un programma che legge i tempi dai log di entrambe le versioni e ne calcola efficenza e speedup,
                 e stampa su terminale tali risultati di ogni riga del log.
