# $File: Makefile
# $Date: Sun Sep 09 12:40:29 2012 +0800

OBJ_DIR = obj
TARGET = main

INCLUDE_DIR = -Iinclude 
#DEFINES += -DDEBUG -DUSE_MPI
 
CXXFLAGS = -Wall -Wextra $(INCLUDE_DIR) $(DEFINES)  -fopenmp -O3 
CXXFLAGS += $(shell pkg-config --cflags --libs gtk+-2.0)

LDFLAGS = $(shell pkg-config --cflags --libs gtk+-2.0) -fopenmp 

cc = mpicxx
CXXSOURCES = $(shell find -name "*.cc" | sed 's/^\.\///g')
OBJS = $(addprefix $(OBJ_DIR)/,$(CXXSOURCES:.cc=.o))
DEPFILES = $(OBJS:.o=.d)


.PHONY: all clean run 

all: $(TARGET)

$(OBJ_DIR)/%.o: %.cc Makefile
	@echo "[cc] $< ..."
	@$(cc) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.d: %.cc  Makefile
	@mkdir -p $(dir $@)
	@echo "[dep] $< ..."
	@$(cc) $(INCLUDE_DIR) $(DEFINES) -MM -MT "$(OBJ_DIR)/$(<:.cc=.o) $(OBJ_DIR)/$(<:.cc=.d)" "$<"  > "$@"

sinclude $(DEPFILES)

$(TARGET): $(OBJS)
	@echo "Linking ..."
	@$(cc) $(OBJS) -o $@ $(LDFLAGS) 


clean:
	@rm -rf $(OBJ_DIR) $(TARGET) mpi pthread omp seq

cleanbuild:
	@rm -rf $(OBJ_DIR) $(TARGET)

run: $(TARGET)
	./$(TARGET) 

mpirun: $(TARGET)
	mpirun -n 4 ./$(TARGET)

