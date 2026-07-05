CXX = clang++
CXXFLAGS = -fmodules -I /usr/local/include -std=c++20
LDFLAGS = -L/usr/local/lib -lraylib

build: src/public/main.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c src/public/main.cpp -o bin/main.o

run: build
	$(CXX) bin/main.o -o bin/game $(LDFLAGS)
	chmod +x bin/game
	./bin/game

clean:
	rm -rf bin

.PHONY: build run clean
