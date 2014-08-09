TARGET := test

SRC1 :=	 $(wildcard src/*.cpp) \
		 $(wildcard src/*/*.cpp) \
		 $(wildcard src/*/*/*.cpp) \
		 $(wildcard src/*/*/*/*.cpp) \
		 $(wildcard src/*/*/*/*/*.cpp)

SRC2 :=  $(wildcard src/*.c) \
		 $(wildcard src/*/*.c) \
		 $(wildcard src/*/*/*.c) \
		 $(wildcard src/*/*/*/*.c) \
		 $(wildcard src/*/*/*/*/*.c)


OBJS := $(SRC2:%.c=obj/%.o)
OBJS += $(SRC1:%.cpp=obj/%.o)

all:$(OBJS)
	@echo [M]Link...
	@mkdir -p bin
	g++ $(OBJS) -o bin/$(TARGET) -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi


obj/%.o:%.cpp
	@echo [M]Compile CXX...
	@mkdir -p $(@D)
	g++ -c $< -o $@

obj/%.o:%.c
	@echo [M]Compile C...
	@mkdir -p $(@D)
	g++ -c $< -o $@

run:
	./bin/$(TARGET)

clean:
	rm -rf bin
	rm -rf obj
	rm -rf out















