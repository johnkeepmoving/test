# -I /path/to/ceph/src/
bench: bench.cc testCase.cc aioCase.cc aioWriteCase.cc 
	g++ bench.cc testCase.cc aioCase.cc aioWriteCase.cc -lrados -lrbd -I /home/jh/programming/ceph/src/ -o bench
clean:
	rm bench 
