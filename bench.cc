/*
 * =====================================================================================
 *
 *       Filename:  bench.cc
 *
 *    Description:  a container to run all kinds of test Case 
 *
 *        Version:  1.0
 *        Created:  08/08/2014 04:58:52 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack
 *
 * =====================================================================================
 */

#include "bench.h"

Bench::Bench(const char *user_name, const char *cluster_name, const char *pool_name, const char *image_name) {
    clusterName = cluster_name;
    userName = user_name;
    poolName = pool_name;
    imageName = image_name;
    uint64_t flags = 0;
    int ret;
    /* Initialize the cluster hadnle */
    {
        ret = cluster.init2(userName, clusterName, flags);
        if (ret < 0) {
            cerr << "Couldn't initialize the cluster handle!" << ret << std::endl;
            ret = EXIT_FAILURE;
            exit(1);
        } else {
            std::cout << "Created a cluster handle" << std::endl;
        }
    }
    /* Read a ceph conf to configure cluster handle */
    {
        ret = cluster.conf_read_file("/etc/ceph/ceph.conf");
        if (ret < 0) {
            cerr << "Couldn't read the ceph conf!" << ret << std::endl;
            ret = EXIT_FAILURE;
            exit(1);
        } else {
            std::cout << "Read the ceph conf!" << std::endl;
        }
    }
    /* Connect to the cluster */
    {
        ret = cluster.connect();
        if (ret < 0) {
            cerr << "Couldn't connect to cluster!" << ret << std::endl;
            ret = EXIT_FAILURE;
            exit(1);
        } else {
            std::cout << "Connected to the cluster." << std::endl;
        }
    }
    
    /* Create pool */
    {
        ret = cluster.pool_lookup(poolName);
        if (ret < 0)
        {
            ret = cluster.pool_create(poolName);
            if (ret < 0) {
                cerr << "Couldn't create pool !" << ret << std::endl;
                ret = EXIT_FAILURE;
                exit(1);
            } else {
                std::cout << "Create the pool" << std::endl;
            }
        } else {
            std::cout << "Pool " << poolName << " already exist" << std::endl;
        }
    }

    /* Declare an I/O Context */
    {
        ret = cluster.ioctx_create(poolName, io_ctx);
        if (ret < 0) {
            cerr << "Couldn't create ioctx !" << ret << std::endl; 
            ret = EXIT_FAILURE;
            exit(1);
        } else {
            std::cout << "Create an ioctx for the pool:" << poolName 
                << " !" << std::endl;
        }
    }
    /* Open an image, if not exist, create it! */
    { 
        ret = rbd.open(io_ctx, image, imageName);
        if (ret < 0) {
            uint64_t image_size = 102400000;
            int order = 0;
            ret = rbd.create(io_ctx, imageName, image_size, &order);
            if (ret < 0) {
                std::cout << "create image error!" << std::endl;
                exit(1);
            }
            ret = rbd.open(io_ctx, image, imageName);
            if (ret < 0) {
                std::cout << "open new image error!" << std::endl;
                exit(1);
            }
        }
        std::cout << "rbd open image:"<<imageName <<" succeed!" << std::endl;
    }
}
Bench:: ~Bench() {
    int ret;
    ret = cluster.pool_delete(poolName);
    if (ret < 0) {
        cerr << "Failed to delete our test pool" << std::endl;
        ret = EXIT_FAILURE;
    }
    cluster.shutdown();
}

int Bench::registerTestCase(TestCase *pTestCase) {
    caseSet.push_back(pTestCase);
    //deal the exception when register failed!
    return 1;
}

int Bench::run() {
    //run every test case
    vector<TestCase*> :: iterator caseIte;
    for(caseIte= caseSet.begin(); caseIte != caseSet.end(); caseIte++) {
        //TestCase* testCase = *it; 
        currTestCase = *caseIte;
        std::cout<< "==========================================" << std::endl;
        std::cout<< "Run TestCase: " << currTestCase->case_name.c_str() << std::endl;
        //currTestCase->run(cluster, io_ctx, rbd, image); 
        currTestCase->run(); 
        std::cout<< "End TestCase: " << currTestCase->case_name.c_str() << std::endl;
        //deal the case result
        if (currTestCase->case_result) {
            //success
            numPassed++;
        }
        else {
            //failure
            numFailed++;
        }
    }
    
    //print total case status 
    std::cout<< "==========================================" << std::endl;
    std::cout<< "Total test case: "<< numPassed+numFailed << std::endl;
    std::cout<< "Success: "<< numPassed << std::endl;
    std::cout<< "Failure: "<< numFailed << std::endl;
}

/*
int main(int argc, const char *argv[])
{
    const char *user_name = "client.admin";
    const char *cluster_name = "ceph";
    Bench *benchInstance = Bench::getInstance(user_name, cluster_name);
    //RbdAioBenchCase *rabc = new RbdAioBenchCase(); 
    //  
    librbd::Image *image = &(benchInstance->image);
    string seqPattern = "seq";
    string randPattern = "rand";
    uint64_t ioBytes = 10240000;
    uint64_t io_threads = 16;
    AioWriteCase aw4kseq(string("aw4kseq"), image, 4096, io_threads, ioBytes, seqPattern);
    AioWriteCase aw4krand(string("aw4krand"), image, 4096, io_threads, ioBytes, randPattern );
    benchInstance->registerTestCase(&aw4kseq); 
    benchInstance->registerTestCase(&aw4krand); 
    
    AioWriteCase aw8kseq(string("aw8kseq"), image, 8192, io_threads, ioBytes, seqPattern);
    AioWriteCase aw8krand(string("aw8krand"), image, 8192, io_threads, ioBytes, randPattern );
    benchInstance->registerTestCase(&aw8kseq); 
    benchInstance->registerTestCase(&aw8krand); 
    
    AioWriteCase aw16kseq(string("aw16kseq"), image, 16384, io_threads, ioBytes, seqPattern);
    AioWriteCase aw16krand(string("aw16krand"), image, 16384, io_threads, ioBytes, randPattern );
    benchInstance->registerTestCase(&aw16kseq); 
    benchInstance->registerTestCase(&aw16krand); 

    benchInstance->run();
    
    free(benchInstance);
    return 0;
}
*/
