CXX = clang++
CXXFLAGS = -Wall -std=c++17 -g 

CODEDIRS = .

BLANK :=
SLASH = \$(BLANK)

glfw = ../../dependencies/GLFW
glfw_inc = $(glfw)/include
INCLUDES = -I$(glfw_inc)
LDFLAGS = $(LIBRARIES)

CPP_FILES = $(wildcard *.cpp)
OBJECTS = $(CPP_FILES:.cpp=.o)
BUILD_DIR = ../../build/
BUILD_DIR_DEL = ..\..\build$(SLASH)
BUILD_DIR_OBJECTS = $(addprefix $(BUILD_DIR),$(OBJECTS))

all: $(BUILD_DIR_OBJECTS)

$(BUILD_DIR)%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(INCLUDES)

.PHONY : clean
clean:
	del $(addprefix $(BUILD_DIR_DEL),$(OBJECTS))