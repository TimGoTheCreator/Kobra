CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

SRC = main.cpp
OUT = kobra

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT) kobra.kb

clean:
	rm -f $(OUT)