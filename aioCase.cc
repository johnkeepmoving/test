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

void AioCase::wait_for(int max) {
    Mutex::Locker l(lock);
    while (data.in_flight > max) {
        utime_t dur;
        dur.set_from_double(.2);
        cond.WaitInterval(g_ceph_context, lock, dur);
    }
}

void rbd_bencher_completion(void *vc, void *pc)
{
    map<librbd::RBD::AioCompletion*, utime_t> :: iterator mit;

    librbd::RBD::AioCompletion *c = (librbd::RBD::AioCompletion *)vc;
    AioCase *b = static_cast<AioCase*>(pc);
    //cout << "complete " << c << std::endl;

    int ret = c->get_return_value();
    if (ret != 0) {
        cout << "aio error: " << ret << std::endl;
        assert(0 == ret);
    }
    utime_t end_time = ceph_clock_now(NULL);
    mit = b->start_time.find(c);
    if (mit == b->start_time.end())
    {
        cout << "error! when aio_write finish, no AioCompletion's start_time" << std::endl;
        exit(1);
    }

    b->lock.Lock();
    bench_data *data = &b->data;
    data->cur_latency = end_time - b->start_time[c];
    data->history.latency.push_back(data->cur_latency);
    data->total_latency += data->cur_latency;
    if( data->cur_latency > data->max_latency) data->max_latency = data->cur_latency;
    if (data->cur_latency < data->min_latency) data->min_latency = data->cur_latency;
    ++(data->finished);
    data->avg_latency = data->total_latency / data->finished;
    //clear the map 
    b->start_time.erase(mit); 
    
    data->in_flight--;
    b->cond.Signal();
    b->lock.Unlock();
    c->release();
}

bool AioCase::start_io(int max, uint64_t off, uint64_t len, bufferlist& bl)
{
    {
        Mutex::Locker l(lock);
        if (data.in_flight >= max)
            return false;
        data.in_flight++;
    }
    librbd::RBD::AioCompletion *c =
        new librbd::RBD::AioCompletion((void *)this, rbd_bencher_completion);
    if (start_time.find(c) != start_time.end()) 
    {
        cout << "error! AioCompletion pointer repeated!" << std::endl;
        exit(1);
    }
    start_time[c] = ceph_clock_now(NULL);
    //aio_function maybe aio_write or aio_read
    (image->*aio_function)(off, len, bl, c);
    data.started++;
    //cout << "start " << c << " at " << off << "~" << len << std::endl;
    return true;
}
