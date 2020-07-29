.PHONY: all
all: clean cmake-cache build

.PHONY: clean
clean:
	rm -rf build

.PHONY: cmake-cache
cmake-cache:
	cmake . -Bbuild -G Ninja

.PHONY: build
build:
	cmake --build build --parallel
