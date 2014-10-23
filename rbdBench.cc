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
//#include "global/global_init.h"
//#include "global/global_context.h" 

RbdBench :: RbdBench(const char *user_name, const char *cluster_name, const char *pool_name, const char *image_name):
    Bench(user_name, cluster_name, pool_name, image_name) {
        std::cout << "Construct RbdBench" << std::endl;
    };
RbdBench :: ~RbdBench() {
    std::cout << "Destruct RbdBench" << std::endl;
}

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
    }
    
    i = options.find("io_total");
    uint64_t io_total;
    if (i == options.end()) {
        io_total = 102400000; //100M
    } else {
        std::stringstream ss(i->second);
        ss >> io_total;
    }

    bool filter_seq, filter_rand;
    string filter_pattern;
    i = options.find("filter-rs");
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
    
    bool filter_read, filter_write;
    i = options.find("filter-rw");
    if (i == options.end()) {
        filter_read = true;
        filter_write = true;
    } else {
        filter_pattern = i->second;
        filter_read = false;
        filter_write = false;
        if (filter_pattern == "read")
            filter_read = true;
        else if (filter_pattern == "write")
            filter_write = true;
    }

    //below construct the write case 
    string seqPattern = "seq";
    string randPattern = "rand";

    librbd::Image *image = &(rbdBench.image);
    //aio write case
    AioWriteCase aw4kseq(string("aw4kseq"), image, 4096, io_threads, io_total, seqPattern);
    AioWriteCase aw4krand(string("aw4krand"), image, 4096, io_threads, io_total, randPattern);
    
    AioWriteCase aw8kseq(string("aw8kseq"), image, 8192, io_threads, io_total, seqPattern);
    AioWriteCase aw8krand(string("aw8krand"), image, 8192, io_threads, io_total, randPattern);
    
    AioWriteCase aw16kseq(string("aw16kseq"), image, 16384, io_threads, io_total, seqPattern);
    AioWriteCase aw16krand(string("aw16krand"), image, 16384, io_threads, io_total, randPattern);

    //aio read case
    AioReadCase ar4kseq(string("ar4kseq"), image, 4096, io_threads, io_total, seqPattern);
    AioReadCase ar4krand(string("ar4krand"), image, 4096, io_threads, io_total, randPattern);
    
    AioReadCase ar8kseq(string("ar8kseq"), image, 8192, io_threads, io_total, seqPattern);
    AioReadCase ar8krand(string("ar8krand"), image, 8192, io_threads, io_total, randPattern);
    
    AioReadCase ar16kseq(string("ar16kseq"), image, 16384, io_threads, io_total, seqPattern);
    AioReadCase ar16krand(string("ar16krand"), image, 16384, io_threads, io_total, randPattern);

    if (filter_rand) {
        if (filter_write) {
            rbdBench.registerTestCase(&aw4krand); 
            rbdBench.registerTestCase(&aw8krand); 
            rbdBench.registerTestCase(&aw16krand); 
        }
        if (filter_read) {
            rbdBench.registerTestCase(&ar4krand); 
            rbdBench.registerTestCase(&ar8krand); 
            rbdBench.registerTestCase(&ar16krand); 
        }
    }
    if (filter_seq) {
        if (filter_write) {
            rbdBench.registerTestCase(&aw4kseq); 
            rbdBench.registerTestCase(&aw8kseq); 
            rbdBench.registerTestCase(&aw16kseq);
        }
        if (filter_read) {
            rbdBench.registerTestCase(&ar4kseq); 
            rbdBench.registerTestCase(&ar8kseq); 
            rbdBench.registerTestCase(&ar16kseq);
        }
    }
    rbdBench.run();
    cout << "rbdBench run() finished! " << std::endl;
} 

int main(int argc, const char *argv[])
{
    std::map<std::string, std::string> options;
    std::string val;
    vector<const char *> args;
    argv_to_vec(argc, argv, args);

    //global_init(NULL, args, CEPH_ENTITY_TYPE_CLIENT, CODE_ENVIRONMENT_UTILITY, 0);
    //common_init_finish(g_ceph_context);
    //

    std::vector<const char *> :: iterator i; 
    for(i=args.begin(); i<args.end(); ) {
        if (ceph_argparse_flag(args, i, "-h", "--help", (char *)NULL)) {
            usage(cout);
            exit(0);
        } else if (ceph_argparse_witharg(args, i, &val, 
                    "--conf", (char *)NULL)) {
            options["conf"] = val;
        } else if (ceph_argparse_witharg(args, i, &val,
                    "--filter-rw", (char *)NULL)) {
            options["filter-rw"] = val;
        } else if (ceph_argparse_witharg(args, i, &val,
                    "--filter-rs", (char *)NULL)) {
            options["filter-rs"] = val;
        } else if (ceph_argparse_witharg(args, i, &val,
                    "--io_threads", (char *)NULL)) {
            options["io_threads"] = val;
        } else if (ceph_argparse_witharg(args, i, &val,
                    "--io_total", (char *)NULL)) {
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
