memsim: memsim.o fifo.o lru.o vms.o
	g++ -std=c++11 memsim.o fifo.o lru.o vms.o -o memsim

memsim.o: memsim.cpp
	g++ -std=c++11 -c memsim.cpp

fifo.o: fifo.cpp
	g++ -std=c++11 -c fifo.cpp

vms.o: vms.cpp
	g++ -std=c++11 -c vms.cpp

lru.o: lru.cpp
	g++ -std=c++11 -c lru.cpp

clean:
	rm *.o