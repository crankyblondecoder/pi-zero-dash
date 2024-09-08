# pi-zero-dash Root Makefile
# --------------------------

export objFiles := Dash.o Dash_e36.o Exception.o Gauge.o GaugeDial.o GaugeBoost.o GaugeBoost_e36.o GaugeDoorOpen_e36.o \
GaugeEcuWarning_e36.o GaugeEngineTemp_e36.o GaugeFuelLevel.o GaugeFuelLevel_e36.o GaugeHeadlight_e36.o GaugeLowOilPressure_e36.o \
GaugeLowVoltage_e36.o GaugeOilTemperature_e36.o GaugeParkBrake_e36.o GaugeSpeedo.o GaugeSpeedo_e36.o GaugeStdLight.o \
GaugeTacho.o GaugeTacho_e36.o GaugeTurnIndicator.o GaugeTurnIndicator_e36.o \
Instrument.o InstrumentBoost.o InstrumentEngineTemp.o InstrumentFuelLevel.o InstrumentIndicator.o InstrumentOilPressure.o \
InstrumentOilTemperature.o InstrumentOnOff.o InstrumentSpeedo.o InstrumentTacho.o InstrumentVoltage.o \
Latcher.o LatcherPico.o Visual.o

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
