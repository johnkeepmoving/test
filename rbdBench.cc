/*
 * =====================================================================================
 *
 *       Filename:  rbdBench.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/18/2014 03:08:30 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack
 *
 * =====================================================================================
 */
#include "rbdBench.h" 
#include <common/ceph_argparse.h>
#include "operate_config.h" 
#include "global/global_init.h"
#include "global/global_context.h" 


/* usage for RbdBench */
void usage(ostream &out) {
    out << 
"usage: rbdBench [options] [commands]\n"
"     \n"
;
    //need to add more
}

void argv_to_vec(int argc, const char *argv[], 
        std::vector<const char *> &args) {
    for(int i=1; i<argc; i++)
        args.push_back(argv[i]);
}

bool read_conf(string bench_conf_name, std::map<std::string, std::string> &conf_args) {
    operate_config oc(bench_conf_name);
    oc.getValue(conf_args);
    return true;
}
/* 
 * read bench conf, select testCase to register accroding to some filter rules
 * finally run bench
 */
void rbdBench_run (std::map<std::string, std::string> &options, std::vector<const char*> &args) {
    
    std::map<std::string, std::string> :: const_iterator i; 
    //read bench conf
    string bench_conf_name;
    i = options.find("conf");
    if (i == options.end()) {
        std::cout << "Error! Must specify the bench conf!" << std::endl;
        exit(1);
    }
    bench_conf_name = i->second;
    string user_name;
    string cluster_name;
    string pool_name;
    string image_name;
    std::map<std::string, std::string> conf_args;
    //read from conf 
    read_conf(bench_conf_name, conf_args);

    i = conf_args.find("user_name");
    if (i == conf_args.end()) {
        user_name = "client.admin";
    } else {
        user_name = i->second.c_str();
    }

    i = conf_args.find("cluster_name");
    if (i == conf_args.end()) {
        cluster_name = "ceph";
    } else {
        cluster_name = i->second.c_str();
    }

    i = conf_args.find("pool_name");
    if (i == conf_args.end()) {
        pool_name = "TestPool";
    } else {
        pool_name = i->second.c_str();
    }
    
    i = conf_args.find("image_name");
    if (i == conf_args.end()) {
        image_name = "TestImage";
    } else {
        image_name = i->second.c_str();
    }

    //construct rbdBench object
    RbdBench rbdBench(user_name.c_str(), cluster_name.c_str(), pool_name.c_str(), image_name.c_str());

    //
    i = options.find("io_threads");
    uint64_t io_threads;
    if (i == options.end()) {
        io_threads = 16;
    } else {
        std::stringstream ss(i->second);
        ss >> io_threads;
        std::cout << "in ss, io_threads: " << io_threads << std::endl;
    }
    
    i = options.find("io_total");
    uint64_t io_total;
    if (i == options.end()) {
        io_total = 102400000; //100M
    } else {
        std::stringstream ss(i->second);
        ss >> io_total;
        std::cout << "in ss, io_total: " << io_total << std::endl;
    }

    bool filter_seq;
    bool filter_rand;
    string filter_pattern;
    i = options.find("filter");
    if (i == options.end()) {
        filter_seq = true;
        filter_rand = true;
    } else {
        filter_pattern = i->second;
        filter_seq = false;
        filter_rand = false;
        if (filter_pattern == "seq")
            filter_seq = true;
        else if (filter_pattern == "rand")
            filter_rand = true;
    }
    //below construct the write case 
    string seqPattern = "seq";
    string randPattern = "rand";
    librbd::Image *image = &(rbdBench.image);
    AioWriteCase aw4kseq(string("aw4kseq"), image, 4096, io_threads, io_total, seqPattern);
    AioWriteCase aw4krand(string("aw4krand"), image, 4096, io_threads, io_total, randPattern );
    
    AioWriteCase aw8kseq(string("aw8kseq"), image, 8192, io_threads, io_total, seqPattern);
    AioWriteCase aw8krand(string("aw8krand"), image, 8192, io_threads, io_total, randPattern );
    
    AioWriteCase aw16kseq(string("aw16kseq"), image, 16384, io_threads, io_total, seqPattern);
    AioWriteCase aw16krand(string("aw16krand"), image, 16384, io_threads, io_total, randPattern );
    if (filter_rand) {
        rbdBench.registerTestCase(&aw4krand); 
        rbdBench.registerTestCase(&aw8krand); 
        rbdBench.registerTestCase(&aw16krand); 
    }
    if (filter_seq) {
        rbdBench.registerTestCase(&aw4kseq); 
        rbdBench.registerTestCase(&aw8kseq); 
        rbdBench.registerTestCase(&aw16kseq);
    }
    rbdBench.run();
} 

int main(int argc, const char *argv[])
{
    std::map< std::string, std::string> options;
    std::string val;
    vector<const char *> args;
    argv_to_vec(argc, argv, args);


    //global_init(NULL, args, CEPH_ENTITY_TYPE_CLIENT, CODE_ENVIRONMENT_UTILITY, 0);
    //common_init_finish(g_ceph_context);
    //


    std::vector<const char *> :: iterator i; 
    for(i=args.begin(); i<args.end(); ) {
        std::cout << "argv: " << *i << std::endl;
        if (ceph_argparse_flag(args, i, "-h", "--help", (char *)NULL)) {
            usage(cout);
            exit(0);
        } else if (ceph_argparse_witharg(args, i, &val, 
                    "--conf", (char *)NULL)) {
            std::cout << "conf: " << val << std::endl;
            options["conf"] = val;
        } else if (ceph_argparse_witharg(args, i, &val,
                    "--filter", (char *)NULL)) {
            std::cout << "filter: " << val << std::endl;
            options["filter"] = val;
        } else if (ceph_argparse_witharg(args, i, &val,
                    "--io_threads", (char *)NULL)) {
            std::cout << "io_threads: " << val << std::endl;
            options["io_threads"] = val;
        } else if (ceph_argparse_witharg(args, i, &val,
                    "--io_total", (char *)NULL)) {
            std::cout << "io_total: " << val << std::endl;
            options["io_total"] = val;
        } else {
            if (val[0] != '-') {
                usage(cout);
                exit(1);
            }
            i++;
        }
    }
    
    /*
     * if (args.empty()) {
        std::cout << "rbdBench: you must specify the bench conf file" << std::endl;
        return 1;
    }
    */
    rbdBench_run(options, args);
    return 0;
}
