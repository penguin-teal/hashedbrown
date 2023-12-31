MKDIR 			:= mkdir -p --
RMDIR   		:= rm -Rf --

EXTRA_CFLAGS		:= 
WARNINGS		:= -Wall -Wextra
ERRORS			:= -Werror=pointer-arith
CFLAGS  		:= $(WARNINGS) $(ERRORS) -std=c99 $(EXTRA_CFLAGS)
CFLAGSRELEASE		:= -O3
CFLAGSDEBUG		:= -g3 -DDEBUG
CURRENTCFLAGS		:=
BIN     		:= ./bin
INCLUDE 		:= ./include
SRC     		:= ./src
SRCS			:= $(wildcard $(SRC)/*.c)
OBJ				:= ./obj
OBJS    		:= $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
OUT				:= $(BIN)/libhashedbrown.a

.PHONY: release debug examples clean

release: CURRENTCFLAGS += $(CFLAGSRELEASE)
release: $(OUT)

debug: CURRENTCFLAGS += $(CFLAGSDEBUG)
debug: $(OUT)

$(OUT): $(OBJS)
	ar rvs $(OUT) $^

$(OBJ)/%.o: $(SRC)/%.c
	$(MKDIR) $(BIN) $(OBJ)
	$(CC) -c -I$(INCLUDE) $(CURRENTCFLAGS) -o $@ $< $(CFLAGS)

examples: debug ./example/exampletable.c
	$(CC) $(CFLAGS) $(CFLAGSDEBUG) -I$(INCLUDE) ./example/exampletable.c -o $(BIN)/example -L$(BIN) -lhashedbrown

clean:
	$(RMDIR) $(BIN) $(OBJ)

