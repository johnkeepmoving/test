CEPH_HOME = /home/jh/programming/ceph/src/
rbdBench: rbdBench.cc operate_config.cc bench.cc aioWriteCase.cc aioCase.cc testCase.cc 
	g++ -g rbdBench.cc  operate_config.cc bench.cc aioWriteCase.cc aioCase.cc testCase.cc -lrados -lrbd -I $(CEPH_HOME)  -o rbdBench
clean:
	rm rbdBench 
