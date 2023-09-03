PROJECT_NAME=cpplisp

CC=clang++
LD=clang++

BIN_DIR=bin
INC_DIR=include
SRC_DIR=src
OBJ_DIR=obj
SUBDIRS=. Lisp Lisp/Util Lisp/Funcs Lisp/Primitives Lisp/Exceptions

EXEC_NAME=$(PROJECT_NAME)
EXEC_PATH=$(BIN_DIR)/$(EXEC_NAME)

INCLUDES_DIRS=
LIBS_DIRS=/usr/local/lib

LIBS=
DEFINES= 

CFLAGS=-g -Wall -gdwarf-4 -std=c++20 -fstandalone-debug
LDFLAGS=

DEPFLAGS = -MT $@ -MMD -MP -MF $(OBJ_DIR)/$*.d

FULL_EXEC=$(EXEC_PATH)
FULL_CFLAGS=$(CFLAGS) -I$(INC_DIR) $(addprefix -I, $(INCLUDES_DIRS)) $(addprefix -D, $(DEFINES)) $(DEPFLAGS)
FULL_LDFLAGS=$(LDFLAGS) $(addprefix -L, $(LIBS_DIRS)) $(addprefix -l, $(LIBS))

SRC_SUBDIRS=$(addprefix $(SRC_DIR)/, $(SUBDIRS))
INC_SUBDIRS=$(addprefix $(INC_DIR)/, $(SUBDIRS))

INCS=$(wildcard *.hpp $(foreach fd, $(INC_SUBDIRS), $(fd)/*.hpp))
SRCS=$(wildcard *.cpp $(foreach fd, $(SRC_SUBDIRS), $(fd)/*.cpp))

OBJS=$(subst $(SRC_DIR), $(OBJ_DIR), $(SRCS:.cpp=.o))
DEPFILES := $(OBJS:.o=.d)

all: $(FULL_EXEC)

obj_subdirs:
	mkdir -p $(addprefix $(OBJ_DIR)/, $(SUBDIRS))

$(FULL_EXEC): $(OBJS)
	$(CC) $(OBJS) $(FULL_LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(FULL_CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXEC_PATH) $(OBJ_DIR)/*
	mkdir -p $(addprefix $(OBJ_DIR)/, $(SUBDIRS))

run: $(FULL_EXEC)
	@$(EXEC_PATH)

debug: $(FULL_EXEC)
	@lldb $(EXEC_PATH)

valgrind: $(FULL_EXEC)
	@valgrind $(EXEC_PATH)

valgrind_massif: $(FULL_EXEC)
	@valgrind --tool=massif $(EXEC_PATH)

$(DEPFILES):

include $(wildcard $(DEPFILES))
