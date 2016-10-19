EXE=duck_hunt

CXXFLAGS=-std=c++0x -pedantic -Wall -ggdb $(shell sdl2-config --cflags)
LFLAGS=$(shell sdl2-config --libs) -lSDL2_image -lSDL2_ttf
ARGS=

########## below: program independant

CXX=g++

SRCS:=$(wildcard *.cpp)
OBJS:=$(patsubst %.cpp,%.o,$(SRCS))
DEPS:=$(patsubst %.cpp,%.d,$(SRCS))


$(EXE): $(DEPS) $(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(LFLAGS)

%.d: %.cpp 
	$(CXX) $(CXXFLAGS) -M $< > $@
	$(CXX) $(CXXFLAGS) -M $< | sed s/\\.o/\.d/ >> $@

clean: 
	rm $(OBJS) $(DEPS) $(EXE)

squeaky:
	rm $(OBJS) $(DEPS) $(EXE) *~

valgrind: $(EXE)
	valgrind --tool=memcheck --leak-check=full  ./$(EXE) $(ARGS)

run: $(EXE)
	./$(EXE) $(ARGS)

-include $(DEPS)