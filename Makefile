%.o:%.cc
	gcc -g -c $^ -o $@ 

rbdBench: rbdBench.o operate_config.o bench.o aioWriteCase.o aioReadCase.o aioCase.o testCase.o
	g++ -g -o $@ $^ -ljson_linux -lrados -lrbd -lyaml-cpp
clean:
	rm -f *.o
	rm -f rbdBench 

