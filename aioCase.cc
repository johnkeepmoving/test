/*
 * =====================================================================================
 *
 *       Filename:  aioCase.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/14/2014 01:35:40 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#include "aioCase.h"

AioCase :: AioCase(string caseName, bool ioWrite, uint64_t ioSize, uint64_t ioThreads, 
        uint64_t ioBytes, string ioPattern, librbd::Image *pImage) 
    :TestCase(caseName), write(ioWrite), io_size(ioSize), io_threads(ioThreads), io_bytes(ioBytes), pattern(ioPattern), 
    image(pImage), lock("AioCase::lock") {
        //cout << "Construct AioCase" << std::endl;
}

AioCase :: ~AioCase() {
    //cout << "Destcut AioCase" << std::endl;
} 

void AioCase::wait_for(int max) {
    Mutex::Locker l(lock);
    while (data.in_flight > max) {
        utime_t dur;
        dur.set_from_double(.2);
        //cond.WaitInterval(g_ceph_context, lock, dur);
        cond.WaitInterval(NULL, lock, dur);
    }
}
bool AioCase::run()
{
    cout << "AioCase"
        << " io_size " << io_size
        << " io_threads " << io_threads
        << " bytes " << io_bytes
        << " pattern " << pattern
        << std::endl;

    if (pattern != "rand" && pattern != "seq")
        return -EINVAL;

    srand(time(NULL) % (unsigned long) -1);

    bufferptr bp(io_size);
    memset(bp.c_str(), rand() & 0xff, io_size);
    bufferlist bl;
    bl.push_back(bp);

    unsigned ios = 0;

    uint64_t size = 0;
    image->size(&size);

    vector<uint64_t> thread_offset;
    uint64_t i;
    uint64_t start_pos;

    // disturb all thread's offset, used by seq write
    for (i = 0; i < io_threads; i++) {
        start_pos = (rand() % (size / io_size)) * io_size;
        thread_offset.push_back(start_pos);
    }
    data.start_time = ceph_clock_now(NULL);
    uint64_t off;
    for (off = 0; off < io_bytes; ) {
        //wait_for is from aioCase.h 
        wait_for(io_threads - 1);
        i = 0;
        while (i < io_threads && off < io_bytes &&
                start_io(io_threads, thread_offset[i], io_size, bl)) {
            ++i;
            ++ios;
            off += io_size;

            if (pattern == "rand") {
                thread_offset[i] = (rand() % (size / io_size)) * io_size;
            } else {
                thread_offset[i] += io_size;
                if (thread_offset[i] + io_size > size)
                    thread_offset[i] = 0;
            }
        }
    }
    //wait for all ops to finish
    wait_for(0);
    data.done = true;
    int r = image->flush();
    if (r < 0) {
        //cerr << "Error flushing data at the end: " << cpp_strerror(r) << std::endl;
        cerr << "Error flushing data at the end: " << r << std::endl;
    }
    data.end_time = ceph_clock_now(NULL);
    case_result = true; 
    //function from testCase.h
    //when finish all ops, print statistics
    //tmp test
    cout << "ios :" << ios << std::endl;
    printStatus();
    return 0;
}
