CXX = g++
CXXFLAGS = -std=c++17 -Wall -Ilib
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lboost_system -pthread

SRC = src/main.cpp src/game.cpp src/gui.cpp src/card.cpp src/random.cpp src/servidor.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = sfml-app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
