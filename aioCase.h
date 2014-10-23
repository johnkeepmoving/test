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

static void rbd_bencher_completion(void *vc, void *pc);

typedef int (librbd::Image:: *ptrIOFunc) (uint64_t off, size_t len, bufferlist& bl,librbd::RBD::AioCompletion *c);

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
    map<librbd::RBD::AioCompletion*, utime_t> start_time;
    //the pointer to the member function of librbd::Image,
    //may point to librbd::Image::aio_write() or 
    //             librbd::Image::aio_read()   
    ptrIOFunc aio_function;
    AioCase(string caseName, bool ioWrite, uint64_t ioSize, uint64_t ioThreads, uint64_t ioBytes, string ioPattern, librbd::Image *pImage, ptrIOFunc aioFunction);
    ~AioCase();
    void wait_for(int max);
    bool start_io(int max, uint64_t off, uint64_t len, bufferlist& bl);
    bool run();
};
#endif
