/*
 * =====================================================================================
 *
 *       Filename:  bench.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/08/2014 04:50:33 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#ifndef BENCH_H
#define BENCH_H

#include "testCase.h"

CephContext *g_ceph_context = NULL;
md_config_t *g_conf =NULL;

class Bench {
    public:
        //get single instance
        static Bench* getInstance(const char *user_name, const char *cluster_name);

        //destructor can be called from outside 
        ~Bench();
        
        //register test case
        int registerTestCase(TestCase *pTestCase);
        //exeucte all test case 
        int run();
        TestCase* currTestCase;
        static Bench* benchInstance; 
        
        librados::Rados cluster;
        librados::IoCtx io_ctx;
        librbd::RBD rbd;
        librbd::Image image; 

        const char *clusterName;
        const char *userName;
        const char *poolName;
        const char *imageName;
        //statistics 
        int totalResult;
        int numPassed;
        int numFailed;

    private:
        //constructor is not allowed called from outside
        Bench(const char *user_name, const char *cluster_name);
        
        std::vector<TestCase*> caseSet;
        
        //ceph cluster info
};
#endif
