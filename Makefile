# Основной используемый компилятор: g++
# -O3 использутеся для оптимизации кода
# -Wall используется для отображение предупреждений
# -lSDL библиотека SDL
# -lgomp библотека OpenMP
# -lglut -lGL библиотеки OpenGL
# -lcuda -lcudart библиотеки CUDA
GPP = g++ -O3 -w
GUI = layout.o display.o element.o graph.o text.o infobox.o main.o
PHYS = LBmethod.o field.o parallel.o sound_map.o timer.o wrapper.o emitter.o receiver.o wav_codec.o spectral_analysis.o unit_conversion.o verification.o research_acoustic_source.o research_porosity.o
LIBS = 	-lSDL -lgomp -lglut -lGL

all : LBM 

LBM : $(GUI) $(PHYS)
	$(GPP) $(GUI) $(PHYS) $(LIBS) -o LBM
	strip -s LBM

layout.o : layout.cpp layout.h
	$(GPP) -c layout.cpp

display.o : display.cpp display.h
	$(GPP) -c display.cpp

element.o : element.cpp element.h
	$(GPP) -c element.cpp

graph.o : graph.cpp graph.h
	$(GPP) -c graph.cpp

demograph.o : demograph.cpp graph.h
	$(GPP) -c demograph.cpp

text.o : text.cpp text.h
	$(GPP) -c text.cpp

infobox.o : infobox.cpp infobox.h
	$(GPP) -c infobox.cpp

main.o : main.cpp
	$(GPP) -c main.cpp

field.o : field.cpp field.h constants.h sound_map.h
	$(GPP) -c field.cpp

LBmethod.o : field.h constants.h LBmethod.cpp
	$(GPP) -c LBmethod.cpp

parallel.o : parallel.cpp field.h constants.h
	$(GPP) -c parallel.cpp -fopenmp

sound_map.o : sound_map.cpp sound_map.h
	$(GPP) -c sound_map.cpp

timer.o : timer.cpp timer.h
	$(GPP) -c timer.cpp

wrapper.o : wrapper.cpp wrapper.h
	$(GPP) -c wrapper.cpp

emitter.o : emitter.cpp emitter.h constants.h
	$(GPP) -c emitter.cpp

receiver.o : receiver.cpp receiver.h constants.h
	$(GPP) -c receiver.cpp

spectral_analysis.o : spectral_analysis.cpp spectral_analysis.h
	$(GPP) -c spectral_analysis.cpp

unit_conversion.o : unit_conversion.cpp unit_conversion.h constants.h
	$(GPP) -c unit_conversion.cpp

wav_codec.o : wav_codec.cpp wav_codec.h 
	$(GPP) -c wav_codec.cpp

verification : field.h verification.cpp
	$(GPP) -c verification.cpp

speed_test.o : speed_test.cpp field.h field.cpp
	$(GPP) -c speed_test.cpp

research_acoustic_source.o : research_acoustic_source.h \
			     research_acoustic_source.cpp \
	                     field.cpp constants.h
	$(GPP) -c research_acoustic_source.cpp

research_porosity.o : research_porosity.h research_porosity.cpp \
		      field.cpp constants.h
	$(GPP) -c research_porosity.cpp

MSG = Hello, $(USER)!
info : 
	@echo $(MSG) 

clean : 
	rm -f $(PHYS) $(GUI)
