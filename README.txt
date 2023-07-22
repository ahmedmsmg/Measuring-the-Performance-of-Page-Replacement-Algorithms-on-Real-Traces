In order to compile the code you must use
	g++ -std=c++11 -o memsim memsim.cpp lru.cpp fifo.cpp vms.cpp
In order to run the code
	./memsim tracefile nFrames policy quiet/debug

When running debug you can use the extra commented code at the end of the file to 
see an accurate hit/miss count.

You can also compile this code using command make which will run the compilation for each file
individually to create the memsim executable