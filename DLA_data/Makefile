SRC_ST = DLA_st.c
SRC_MT = DLA_mt.c
BIN_ST = DLA_st
BIN_MT = DLA_mt
CC = gcc
CFLAGS = -Wall -lpthread

.PHONY: all clean

all: $(BIN_ST) $(BIN_MT)

$(BIN_ST): $(SRC_ST)
	$(CC) $(CFLAGS) $< -o $@
	@echo "\033[1;35mCompilazione di $@ avvenuta con successo. Esegui con ./$@\033[0m"

$(BIN_MT): $(SRC_MT)
	$(CC) $(CFLAGS) $< -o $@
	@echo "\033[1;35mCompilazione di $@ avvenuta con successo. Esegui con ./$@\033[0m"

run_st: $(BIN_ST)
	./$(BIN_ST) $(n)

run_mt: $(BIN_MT)
	./$(BIN_MT) $(n)

clean:
	rm -f $(BIN_ST) $(BIN_MT)

