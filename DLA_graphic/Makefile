SRC_ST = DLA_st_graphic.c
SRC_MT = DLA_mt_graphic.c
BIN_ST = DLA_st_graphic
BIN_MT = DLA_mt_graphic
CC = gcc
CFLAGS = -lpthread 
DFLAGS = -lSDL2

.PHONY: all clean

all: $(BIN_ST) $(BIN_MT)

$(BIN_ST): $(SRC_ST)
	$(CC) $< -o $@ $(CFLAGS) $(DFLAGS)
	@echo "\033[1;35mCompilazione di $@ avvenuta con successo. Esegui con ./$@\033[0m"

$(BIN_MT): $(SRC_MT)
	$(CC) $< -o $@ $(CFLAGS) $(DFLAGS)
	@echo "\033[1;35mCompilazione di $@ avvenuta con successo. Esegui con ./$@\033[0m"

run_st: $(BIN_ST)
	./$(BIN_ST) $(n)

run_mt: $(BIN_MT)
	./$(BIN_MT) $(n)

clean:
	rm -f $(BIN_ST) $(BIN_MT)

