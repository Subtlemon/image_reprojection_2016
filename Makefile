CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LIBS := `pkg-config --libs opencv`
CFLAGS := `pkg-config --cflags opencv`

vpath .h src

bin/main: $(OBJ_FILES)
	g++ $(CFLAGS) -o $@ $^ $(LIBS)

obj/%.o: src/%.cpp
	g++ $(CFLAGS) -c -o $@ $<
