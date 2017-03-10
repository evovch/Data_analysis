INC=-I`root-config --incdir`

LIBS=`root-config --libs`

FLAGS=-std=c++11 -Wall -Wextra -Wpedantic

TARGET=prog

.PHONY: all clean

all:
	g++ gen.C $(INC) $(LIBS) $(FLAGS) -o $(TARGET)

clean:
	rm -fv $(TARGET)