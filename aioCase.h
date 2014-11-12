/*
 * =====================================================================================
 *
 *       Filename:  aioCase.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/14/2014 10:25:32 AM
 *       Revision:  none
 *       Compiler:  g++ 
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack 
 *
 * =====================================================================================
 */

#ifndef AIO_CASE_H
#define AIO_CASE_H

#include "testCase.h"
#include <rados/buffer.h>
struct aio_status {
    utime_t start_time;
    bufferlist *buf;
    aio_status(utime_t time, bufferlist *p):start_time(time), buf(p) {}
    aio_status() {}
};

class AioCase: public TestCase {
public:
    uint64_t io_size;
    uint64_t io_threads;
    uint64_t io_bytes;
    string pattern;
    bool write;//determin if it's write or read
    //data is in TestCase
    librbd::Image *image;
    Mutex lock;
    Cond cond;
    //store every aio's start time
    //map<librbd::RBD::AioCompletion*, utime_t> start_time;
    map<librbd::RBD::AioCompletion*, aio_status> status;
    //the pointer to the member function of librbd::Image,
    AioCase(string caseName, bool ioWrite, uint64_t ioSize, uint64_t ioThreads, uint64_t ioBytes, string ioPattern, librbd::Image *pImage);
    ~AioCase();
    void wait_for(int max);
    virtual bool start_io(int max, uint64_t off, uint64_t len, bufferlist& bl) = 0;
    bool run();
};
#endif
