GCC = g++
DFLAGS =  -fopenmp -O3
LIBDIRS = -I.

build/main: build/main.o build/kmeans.o 
	$(GCC) $(DFLAGS) $(LIBDIRS) -o build/main build/main.o build/kmeans.o

build/main.o: src/main.cpp
	$(GCC) $(DFLAGS) $(LIBDIRS) -c -o build/main.o src/main.cpp -lm

build/kmeans.o: src/kmeans.cpp
	$(GCC) $(DFLAGS) $(LIBDIRS) -c -o build/kmeans.o src/kmeans.cpp -lm

clean:
	rm -f build/main build/*.o
	