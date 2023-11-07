# Some Make automatic variables
# 	$@		references the target name
# 	$^		references the dependency/prerequisite list
# 	$?		references the dependencies that are newer than the target
#	$<		references the name of the first dependency/prerequisite
# 	$%
CC = clang
CXX = clang++

BLANK :=
SLASH = \$(BLANK)

#* If adding a new dependency or vendor, add it in the following block
#* -----Denpendencies and external vendors------------
glfw = dependencies/GLFW
glfw_inc = $(glfw)/include
glfw_lib = $(glfw)/lib

glad = dependencies/GLAD
glad_inc = $(glad)/include

glm = vendor/
imgui = vendor/imgui

# Add includes when using external vendor/dependency
INCLUDES = -I$(glfw_inc) -I$(glad_inc) -I$(glm) -I$(imgui)
LIBRARIES = -L$(glfw_lib)

#* Add directory that needs to be compiled. The directory should have 
#* its own make file to compile all source code within
SOURCE_DIRS = vendor/stb vendor/imgui
#* ---------------------------------------------------

BUILD_DIR = build/
BUILD_DIR_DEL = build$(SLASH)

# Code Files
CPP_FILES = $(wildcard *.cpp)
C_FILES = $(wildcard *.c)
VENDOR_CPP_FILES = $(wildcard vendor/**/*.cpp)

# Code Binaries/Objects without directory prefix
CPP_OBJECTS = $(CPP_FILES:.cpp=.o)
C_OBJECTS = $(C_FILES:.c=.o)
VENDOR_CPP_OBJECTS = $(notdir $(VENDOR_CPP_FILES:.cpp=.o))

# Source Code Binaries/Objects (not including vendor)
SOURCE_OBJECTS = $(CPP_OBJECTS) $(C_OBJECTS)
BUILD_SOURCE_OBJECTS = $(addprefix $(BUILD_DIR),$(SOURCE_OBJECTS))

# All Code Binaries/Objects (including vendor)
OBJECTS = $(CPP_FILES:.cpp=.o) $(C_OBJECTS) $(VENDOR_CPP_OBJECTS)

BUILD_DIR_OBJECTS = $(addprefix $(BUILD_DIR),$(OBJECTS))
BUILD_DIR_OBJECTS_DEL = $(addprefix $(BUILD_DIR_DEL),$(OBJECTS))


CFLAGS = -Wall $(INCLUDES)
CXXFLAGS = -Wall -std=c++17 -g $(INCLUDES)
LDFLAGS = $(LIBRARIES) -lglfw3 -lopengl32 -lgdi32 -lglu32

TARGET = Main.exe



all: $(TARGET)

# Build rule is as follows...
# Main.exe : [ALL SOURCE CODE OBJECTS IN IMMEDIATE DIRECTORY] [SOURCE DIRECTORIES OF OTHER FOLDERS CONTAINING SOURCE CODE]
$(TARGET): $(BUILD_SOURCE_OBJECTS) $(SOURCE_DIRS)
	$(CXX) -o Main.exe $(BUILD_DIR_OBJECTS) $(LDFLAGS)


# Build all cpp files in immediate directory and place them into build directory
$(BUILD_DIR)%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


# Build all c files in immediate directory and place them into build directory
$(BUILD_DIR)%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Recursively call the make files in other directories containing source code
.PHONY: $(SOURCE_DIRS)
$(SOURCE_DIRS):
	$(MAKE) -C $@


run: $(TARGET)
	./$(TARGET)

.PHONY : clean
clean:
	del $(TARGET) $(BUILD_DIR_OBJECTS_DEL)



