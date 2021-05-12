CC:= g++.exe
INCLUDE:= -I./
LIBS:= -lstdc++
CXXFLAGS:= -std=c++11 -g
DIR_SRC:= ./src
DIR_OBJ:= ./obj
DIR_OBJ_WIN:= .\obj
TARGET:= ./bin/main.exe
OBJECTS:= main.o person.o grid.o game.o utils.o logger.o sim.o
OBJECTS:= $(addprefix $(DIR_OBJ)/,$(OBJECTS))

DEF_NAME:= bin/sim.def
DLL_TARGET:= ./bin/sim.dll
DLL_OBJECTS:= sim.o person.o grid.o game.o utils.o logger.o 
DLL_OBJECTS:= $(addprefix $(DIR_OBJ)/,$(DLL_OBJECTS))
DIR_BIN_WIN:= .\bin

# TEST_CODE:= test.cc
# TEST_TARGET:= ./bin/test.dll
# TEST_TARGET_WIN:= .\bin\test.dll

all: $(TARGET)

$(shell mkdir obj)

$(TARGET): $(OBJECTS)
	$(CC) -g -o $(TARGET) $(OBJECTS) $(LIBS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cc   
	$(CC) -c $(CXXFLAGS) $(INCLUDE) $< -o $@

.PHONY : test

dll: $(DLL_TARGET)


$(DLL_TARGET): $(DLL_OBJECTS)
	$(CC) -shared -Wl,--kill-at,--output-def,$(DEF_NAME) $(LIBS) -o $(DLL_TARGET) $(DLL_OBJECTS)

# test: $(TEST_TARGET)

# $(TEST_TARGET): $(DIR_SRC)/$(TEST_CODE)  
# 	$(CC) -shared -Wl,--kill-at,--output-def,bin/test.def -o $(TEST_TARGET) $(DIR_SRC)/$(TEST_CODE)

.PHONY : clean
clean:   
	-del $(DIR_OBJ_WIN)\*.o
	-del $(DIR_BIN_WIN)\*.exe $(TEST_TARGET_WIN) $(DIR_BIN_WIN)\*.def
	rmdir obj
