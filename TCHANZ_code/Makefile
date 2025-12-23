OUT = projet
CXX = g++
CXXFLAGS = -Wall -std=c++11 
LINKING = `pkg-config --cflags gtkmm-3.0`
LDLIBS = `pkg-config --libs gtkmm-3.0`
OFILES = projet.o gui.o graphic.o error_squarecell.o squarecell.o message.o nourriture.o fourmi.o fourmiliere.o simulation.o 

all: $(OUT)

gui.o: gui.cc gui.h graphic.h simulation.h fourmiliere.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

graphic.o: graphic.cc graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

projet.o: projet.cc gui.h simulation.h fourmiliere.h fourmi.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

squarecell.o: squarecell.cc squarecell.h error_squarecell.h graphic.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

error_squarecell.o: error_squarecell.cc error_squarecell.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

message.o: message.cc message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

nourriture.o: nourriture.cc nourriture.h squarecell.h error_squarecell.h message.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

fourmi.o: fourmi.cc fourmi.h squarecell.h error_squarecell.h message.h nourriture.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)

fourmiliere.o: fourmiliere.cc fourmiliere.h fourmi.h constantes.h nourriture.h message.h squarecell.h error_squarecell.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
simulation.o: simulation.cc simulation.h fourmiliere.h fourmi.h constantes.h nourriture.h squarecell.h error_squarecell.h message.h 
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
	
$(OUT): $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)

clean:
	@echo "Cleaning compilation files"
	@rm *.o $(OUT) *.cc~ *.h~






