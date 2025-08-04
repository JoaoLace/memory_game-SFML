CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -Ilib
LIBS = -lboost_system -pthread

TARGET = memory_game
SOURCE = memory_game.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run 