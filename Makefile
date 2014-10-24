CEPH_HOME = /home/jh/programming/ceph/src/
JSON_INCLUDE = /home/jh/jsoncpp-src-0.5.0/include
JSON_LIB = /home/jh/jsoncpp-src-0.5.0/libs/linux-gcc-4.6
JSON_LIB_USED = json_linux-gcc-4.6_libmt

rbdBench: rbdBench.cc operate_config.cc bench.cc aioWriteCase.cc aioReadCase.cc aioCase.cc testCase.cc 
	g++ -g rbdBench.cc  operate_config.cc bench.cc aioWriteCase.cc aioReadCase.cc aioCase.cc testCase.cc -I$(CEPH_HOME) -I$(JSON_INCLUDE) -L$(JSON_LIB) -l$(JSON_LIB_USED) -lrados -lrbd -lyaml-cpp -o rbdBench
clean:
	rm rbdBench 
