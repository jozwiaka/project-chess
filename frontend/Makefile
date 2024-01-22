.PHONY: prepare configure build run all

prepare:
	rm -rf build
	mkdir build
configure: prepare
	cd build && cmake ../App-Chess
build: configure
	cd build && cmake --build .
run:
	cd build && ./App-Chess
all: prepare configure build run
