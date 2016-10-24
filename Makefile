.PHONY: all
all: debug staticdebug release staticrelease

.PHONY: build
build:
	[ -f build/Debug ]            || mkdir -p build/Debug
	[ -f build/StaticDebug ]      || mkdir -p build/StaticDebug
	[ -f build/Release ]          || mkdir -p build/Release
	[ -f build/StaticRelease ]    || mkdir -p build/StaticRelease

.PHONY: release
release: build
	cd build/Release && cmake -DCMAKE_BUILD_TYPE=Release ../..
	cd build/Release && cmake --build . --target LittleInterpreter
	cd build/Release && cmake --build . --target LittleCompiler

.PHONY: staticrelease
staticrelease: build
	cd build/StaticRelease && cmake -DCMAKE_BUILD_TYPE=Release -DSTATIC_STACK=1 -DSTATIC_PROGRAM=1 -DSTATIC_MEMORY=1 ../..
	cd build/StaticRelease && cmake --build . --target LittleInterpreter
	cd build/StaticRelease && cmake --build . --target LittleCompiler

.PHONY: clean
clean:

.PHONY: debug
debug: build
	cd build/Debug && cmake -DCMAKE_BUILD_TYPE=Debug ../..
	cd build/Debug && cmake --build . --target LittleInterpreter
	cd build/Debug && cmake --build . --target LittleCompiler

.PHONY: staticdebug
staticdebug: build
	cd build/StaticDebug && cmake -DCMAKE_BUILD_TYPE=Debug -DSTATIC_STACK=1 -DSTATIC_PROGRAM=1 -DSTATIC_MEMORY=1 ../..
	cd build/StaticDebug && cmake --build . --target LittleInterpreter
	cd build/StaticDebug && cmake --build . --target LittleCompiler

.PHONY: clean
clean:
	rm -rf build/*

.PHONY: runrel
runrel: release
	build/Release/LittleInterpreter ExampleProgram/TestOut

.PHONY: rundeb
rundeb: debug
	build/Debug/LittleInterpreter ExampleProgram/TestOut

.PHONY: runrelsta
runrelsta: staticrelease
	build/StaticRelease/LittleInterpreter ExampleProgram/TestOut

.PHONY: rundebsta
rundebsta: staticdebug
	build/StaticDebug/LittleInterpreter ExampleProgram/TestOut

.PHONY: run
run: runrel rundeb runrelsta rundebsta

.PHONY: size
size: all
	@ls -lah build/Debug/LittleInterpreter
	@ls -lah build/StaticDebug/LittleInterpreter
	@ls -lah build/Release/LittleInterpreter
	@ls -lah build/StaticRelease/LittleInterpreter
