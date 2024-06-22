# pi-zero-dash Root Makefile
# --------------------------

export objFiles := Dash.o Dash_e36.o Exception.o Gauge.o GaugeDial.o GaugeHeadlight_e36.o GaugeSpeedo.o GaugeSpeedo_e36.o \
GaugeStdLight.o GaugeTacho.o \
GaugeTacho_e36.o GaugeTurnIndicator.o GaugeTurnIndicator_e36.o \
Instrument.o InstrumentOnOff.o InstrumentIndicator.o InstrumentSpeedo.o InstrumentTacho.o \
Visual.o

# NOTE: Put required libs last in the build so Debian based systems don't cry.
export libs = drm cairo

export adapsurfStaticLocn = ../libAdapSurf.a
export adapsurfStaticLocnDebug = ../libAdapSurf_debug.a

export debugExecName = piZeroDash_debug
export releaseExecName = piZeroDash

.PHONY : debug
debug :
	@ mkdir -p debug
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
	@ mkdir -p test
	cd test; $(MAKE) --makefile=../makefile_test test

.PHONY : release
release :
	@ mkdir -p release
	cd release; $(MAKE) --makefile=../makefile_release release
