CC:= g++.exe
INCLUDE:= -I./
LIBS:= -lstdc++ -lwsock32 
CXXFLAGS:= -std=c++11 -g
DIR_SRC:= ./src
DIR_OBJ:= ./obj
DIR_OBJ_WIN:= .\obj
TARGET:= main.exe
OBJECTS := main.o person.o grid.o game.o utils.o
OBJECTS := $(addprefix $(DIR_OBJ)/,$(OBJECTS))

all: $(TARGET)

$(shell mkdir obj)

$(TARGET): $(OBJECTS)
	$(CC) -g -o $(TARGET) $(OBJECTS) $(LIBS)

$(DIR_OBJ)/%.o: $(DIR_SRC)/%.cc   
	$(CC) -c $(CXXFLAGS) $(INCLUDE) $< -o $@
  
.PHONY : clean
clean:   
	-del $(DIR_OBJ_WIN)\*.o
	-del $(TARGET)
	rmdir obj
