CEPH_HOME = /root/work/ceph/src/
RADOS_LIB = /root/work/ceph/src/.libs/librados.so
RBD_LIB = /root/work/ceph/src/.libs/librbd.so
JSON_INCLUDE = /root/work/jsoncpp-src-0.5.0/include
JSON_LIB = /root/work/jsoncpp-src-0.5.0/libs/linux-gcc-4.4.7/libjson_linux-gcc-4.4.7_libmt.so

%.o:%.cc
	gcc -g -c $^ -o $@ -I$(CEPH_HOME) -I$(JSON_INCLUDE)

rbdBench: rbdBench.o operate_config.o bench.o aioWriteCase.o aioReadCase.o aioCase.o testCase.o
	g++ -g -o $@ $^ $(JSON_LIB) $(RADOS_LIB) $(RBD_LIB) -lyaml-cpp
clean:
	rm -f *.o
	rm -f rbdBench 

