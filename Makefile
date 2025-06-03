CXX = g++
CXXFLAGS = -std=c++17 -Wall -I/usr/include -O0 -g

# SFML libraries
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files and target
SRC = src/particle_controller.cc \
	  src/particle.cc \
	  src/quadtree.cc \
	  src/main.cc
		
TARGET = colours

# Build target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

# Installation directory
PREFIX = /usr/local/bin

# Install target
install: $(TARGET)
	install -m 0755 $(TARGET) $(PREFIX)

# Clean up
clean:
	rm -f $(TARGET)
