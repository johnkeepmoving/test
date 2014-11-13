/*
 * =====================================================================================
 *
 *       Filename:  testCase.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/08/2014 04:46:01 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#ifndef TEST_CASE_H
#define TEST_CASE_H


#include <rados/librados.hpp>
#include <rbd/librbd.hpp>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <iostream>
#include <vector>

using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

struct bench_interval_data {
  double min_bandwidth;
  double max_bandwidth;
};

struct bench_data {
  timeval start_time; //start time for benchmark
  timeval end_time;   //end time for benchmark
  bool done; //is the benchmark is done
  //int object_size; //the size of the objects
  //int trans_size; //size of the write/read to perform
  // same as object_size for write tests
  int started;   //number of IO requests already started
  int finished;  //number of IO requests already finished
  int in_flight; //number of reads/writes being waited on
  double min_latency;
  double max_latency;
  double avg_latency;
  double variance_latency;
  double total_latency;
  //struct bench_interval_data idata; // data that is updated by time intervals and not by events
  double cur_latency; //latency of last completed transaction
  //char *object_contents; //pointer to the contents written to each object
  bench_data() {
      done = false;
      started = 0;
      finished = 0;
      in_flight = 0;
      min_latency = 9999.0;
      max_latency = 0.0;
      avg_latency = 0.0;
      variance_latency = 0.0;
  };
};

double diff_time_ms(timeval end_time, timeval start_time);

class TestCase {
    public:
        virtual ~TestCase();
        bench_data data;
        TestCase(string caseName);
        //virtual bool run(librados::Rados &cluster, librados::IoCtx &io_ctx, librbd::RBD &rbd, librbd::Image &image);
        virtual bool run(){}
        bool case_result;
        string  case_name;
        void printStatus();
};
#endif
