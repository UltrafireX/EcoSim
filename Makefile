all: EcoSim

EcoSim:
	cd extensionFiles/; make

clean:
	rm -f EcoSim
	cd extensionFiles/; make clean
