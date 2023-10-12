MKDIR 	:= mkdir -p --
RMDIR   := rm -Rf --
CC      := clang
WARNINGS:= -Wall -Wextra
CFLAGS  := $(WARNINGS) -std=c99
BIN     := ./bin
INCLUDE := ./include
SRC     := ./src
SRCS	:= $(wildcard $(SRC)/*.c)
OBJ		:= ./obj
OBJS    := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
OUT		:= $(BIN)/libhashedbrown.a

.PHONY: examples clean install

$(OUT): $(OBJS)
	ar rvs $(OUT) $^

$(OBJ)/%.o: $(SRC)/%.c
	$(MKDIR) $(BIN) $(OBJ)
	$(CC) -c -I$(INCLUDE) -o $@ $< $(CFLAGS) -O3

examples: $(OUT) ./example/exampletable.c
	$(CC) $(CFLAGS) -g3 -DDEBUG -I$(INCLUDE) ./example/exampletable.c -o $(BIN)/example -L$(BIN) -lhashedbrown

clean:
	$(RMDIR) $(BIN) $(OBJ)

install: $(OUT)
	install -v -s $(OUT) $(DESTDIR)