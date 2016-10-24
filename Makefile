build_types = StaticDebug Release RelMinSize Debug

.PHONY: all
all: debug staticdebug

.PHONY: build
build:
	[ -f build/StaticRelease ]    || mkdir -p build/StaticRelease
	[ -f build/StaticRelMinSize ] || mkdir -p build/StaticRelMinSize
	[ -f build/StaticDebug ]      || mkdir -p build/StaticDebug
	[ -f build/Release ]          || mkdir -p build/Release
	[ -f build/RelMinSize ]       || mkdir -p build/RelMinSize
	[ -f build/Debug ]            || mkdir -p build/Debug

.PHONY: debug
debug: build
	cd build/Debug && cmake -DCMAKE_BUILD_TYPE=Debug ../..
	cd build/Debug && cmake --build . --target LittleInterpreter
	cd build/Debug && cmake --build . --target LittleCompiler

.PHONE: staticdebug
staticdebug: build
	cd build/StaticDebug && cmake -DCMAKE_BUILD_TYPE=Debug -DSTATIC_STACK=1 ../..
	cd build/StaticDebug && cmake --build . --target LittleInterpreter
	cd build/StaticDebug && cmake --build . --target LittleCompiler
