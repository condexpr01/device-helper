all:
	-cmake -B build -G Ninja && cmake --build build

clean:
	-rm -rf build
	
	-rm -rf .cache
	-rm -rf src/imgui/.cache
	-rm -rf src/core/.cache

run:
	./build/prog
