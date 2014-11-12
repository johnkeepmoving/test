/*
 * =====================================================================================
 *
 *       Filename:  aioWriteCase.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/14/2014 02:10:47 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#include "aioWriteCase.h" 
AioWriteCase :: AioWriteCase(string caseName, librbd::Image *pImage, uint64_t ioSize, uint64_t ioThreads, uint64_t ioBytes, string ioPattern)
    :AioCase(caseName, true, ioSize, ioThreads, ioBytes, ioPattern, pImage) {
        //cout << "Construct AioWriteCase" << std::endl;
}

AioWriteCase :: ~AioWriteCase() {
    //cout << "Destruct AioWriteCase" << std::endl; 
}

void aio_write_completion(void *vc, void *pc)
{
    map<librbd::RBD::AioCompletion*, aio_status> :: iterator mit;

    librbd::RBD::AioCompletion *c = (librbd::RBD::AioCompletion *)vc;
    AioCase *b = static_cast<AioCase*>(pc);

    int ret = c->get_return_value();
    if (ret != 0) {
        cout << "aio write error: " << ret << std::endl;
        assert(0 == ret);
    }
    utime_t end_time = ceph_clock_now(NULL);
    b->lock.Lock();
    mit = b->status.find(c);
    if (mit == b->status.end())
    {
        cout << "error! when aio_write finish, no AioCompletion's status" << std::endl;
        exit(1);
    }

    bench_data *data = &b->data;
    data->cur_latency = end_time - b->status[c].start_time;
    double delta = data->cur_latency - data->avg_latency;
    data->total_latency += data->cur_latency;
    if( data->cur_latency > data->max_latency) data->max_latency = data->cur_latency;
    if (data->cur_latency < data->min_latency) data->min_latency = data->cur_latency;
    ++(data->finished);
    data->avg_latency = data->total_latency / data->finished;
    data->variance_latency += (delta * (data->cur_latency - data->avg_latency)); 
    //clear the map 
    b->status.erase(mit); 
    
    data->in_flight--;
    b->cond.Signal();
    b->lock.Unlock();
    c->release();
}

bool AioWriteCase::start_io(int max, uint64_t off, uint64_t len, bufferlist& bl)
{
    librbd::RBD::AioCompletion *c;
    {
        Mutex::Locker l(lock);
        if (data.in_flight >= max)
            return false;
        data.in_flight++;
    //}
    c = new librbd::RBD::AioCompletion((void *)this, aio_write_completion);
    if (status.find(c) != status.end()) 
    {
        cout << "error! AioCompletion pointer repeated!" << std::endl;
        exit(1);
    }
    aio_status stat(ceph_clock_now(NULL), NULL);
    status[c] = stat;
    }
    image->aio_write(off, len, bl, c);
    data.started++;
    return true;
}
