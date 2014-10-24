/*
 * =====================================================================================
 *
 *       Filename:  rbdBench.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/18/2014 11:10:15 AM
 *       Revision:  none
 *       Compiler:  g++ 
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack
 *
 * =====================================================================================
 */

#ifndef RBDBENCH_H
#define RBDBENCH_H

#include <iostream>
#include <sstream>
#include <string>
#include "bench.h"
#include "aioWriteCase.h" 
#include "aioReadCase.h" 

//void usage(ostream &out);
//void argv_to_vec(int argc, const char *argv[], std::vector<const char *> &args);

//bool read_conf(string bench_conf_name, std::map<std::string, std::string> &conf_args);

//void rbdBench_run (std::map<std::string, std::string> &options, std::vector<const char*> &args);

//CephContext *g_ceph_context = NULL;
//md_config_t *g_conf =NULL;
class RbdBench : public Bench {
    public:
        RbdBench(const char *user_name, const char *cluster_name, const char *pool_name, const char *image_name, string json_name);
        ~RbdBench();
};

#endif
