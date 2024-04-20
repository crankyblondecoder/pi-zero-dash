# pi-zero-dash Root Makefile
# --------------------------

export objFiles := Dash.o Exception.o Gauge.o Instrument.o Visual.o

# NOTE: Put required libs last in the build so Debian based systems don't cry.
export libs = drm cairo

export adapsurfStaticLocn = ../libAdapSurf.a
export adapsurfStaticLocnDebug = ../libAdapSurf_debug.a

export debugExecName = piZeroDash_debug
export releaseExecName = piZeroDash

.PHONY : debug
debug :
	cd debug; $(MAKE) --makefile=../makefile_debug debug

.PHONY : clean
clean :
	- cd debug; $(MAKE) --makefile=../makefile_debug clean
	- cd test; $(MAKE) --makefile=../makefile_test clean
	- cd release; $(MAKE) --makefile=../makefile_release clean
	- rm $(debugExecName)
	- rm $(releaseExecName)

.PHONY : test
test : debug
	cd test; $(MAKE) --makefile=../makefile_test test

.PHONY : release
release :
	cd release; $(MAKE) --makefile=../makefile_release release
