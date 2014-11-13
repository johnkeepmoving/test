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
#include "argparse.h" 
#include "operate_config.h" 
#include "yaml-cpp/yaml.h" 
//#include "global/global_init.h"
//#include "global/global_context.h" 

struct RbdCase {
    string case_name;
    bool asynchronous;//yes or no
    bool write;//write or read
    bool rand;//rand or seq
    unsigned int io_size;
    unsigned int io_total;
    unsigned int io_threads;
};

vector<RbdCase> phrase_rbdCase (string rbdCase_filename) {
    vector<RbdCase> caseSet;
    RbdCase currCase;
    YAML::Node config = YAML::LoadFile(rbdCase_filename);
    cout << "Read rbdCase file Succeed" << std::endl;
    for(int i=0; i<config.size(); i++) {
        currCase.case_name = config[i]["name"].as<string>(); 
        currCase.asynchronous = config[i]["asynchronous"].as<bool>(); 
        currCase.write = config[i]["write"].as<bool>(); 
        currCase.rand = config[i]["rand"].as<bool>(); 
        currCase.io_size = config[i]["io_size"].as<unsigned int>(); 
        currCase.io_total = config[i]["io_total"].as<unsigned int>(); 
        currCase.io_threads = config[i]["io_threads"].as<unsigned int>();
        caseSet.push_back(currCase);
    }
    return caseSet;
}

RbdBench :: RbdBench(const char *user_name, const char *cluster_name, const char *pool_name, const char *image_name, string json_name):
    Bench(user_name, cluster_name, pool_name, image_name, json_name) {
        std::cout << "Construct RbdBench" << std::endl;
    };
RbdBench :: ~RbdBench() {
    std::cout << "Destruct RbdBench" << std::endl;
}

/* usage for RbdBench */
void usage(ostream &out) {
    out << 
"usage: rbdBench [options] [commands]\n"
"     --conf path/to/file, specify the basic ceph config file(should contain cluster name ...)\n"
"     --filter-rw  read or write, just run the case which satisfy the rule\n"
"     --filter-rs  rand or seq, just run the case which satisfy the rule\n"
"     --case-file  path/to/case file which should read from, the case file should be in yaml format\n"
"     --json-file  path/to/json file which should write result to, the case file would be in json format\n"
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
    string user_name;
    string cluster_name;
    string pool_name;
    string image_name;
    std::map<std::string, std::string> conf_args;

    i = options.find("conf");
    if (i == options.end()) {
        std::cout << "Use the default bench conf!" << std::endl;
    } else {
	bench_conf_name = i->second;
	//read from conf 
	read_conf(bench_conf_name, conf_args);
    }

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
    string _json_file_name;
    i = options.find("json-file");
    if (i == options.end()) {
        _json_file_name = "default.json";
    } else {
        _json_file_name = i->second;
    }
    //construct rbdBench object
    RbdBench rbdBench(user_name.c_str(), cluster_name.c_str(), pool_name.c_str(), image_name.c_str(), _json_file_name);
    librbd::Image *image = &(rbdBench.image);
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
    
    i = options.find("case-file");
    if (i == options.end()) {
        cout << "Need to specify RbdCase file!" << std::endl;
        exit(1);
    }
    string rbdCase_filename = i->second;
    vector<RbdCase> caseSet = phrase_rbdCase(rbdCase_filename);
    //register test case
    cout << "Total case: " << caseSet.size() << std::endl;
    for(int i=0; i<caseSet.size(); i++) {
        string io_pattern = caseSet[i].rand? "rand":"seq";
        if (caseSet[i].asynchronous) {
           if (caseSet[i].write) {
               AioWriteCase *curr = new AioWriteCase(caseSet[i].case_name, image, (uint64_t)caseSet[i].io_size, 
                      (uint64_t)caseSet[i].io_threads, (uint64_t)caseSet[i].io_total, io_pattern);
               rbdBench.registerTestCase(curr); 
           } else {
               AioReadCase *curr = new AioReadCase(caseSet[i].case_name, image, (uint64_t)caseSet[i].io_size, 
                      (uint64_t)caseSet[i].io_threads, (uint64_t)caseSet[i].io_total, io_pattern);
               rbdBench.registerTestCase(curr); 
           }
        } else {
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
                    "--case-file", (char *)NULL)) {
            options["case-file"] = val;
        } else if (ceph_argparse_witharg(args, i, &val,
                    "--json-file", (char *)NULL)) {
            options["json-file"] = val;
        /*
        } else if (ceph_argparse_witharg(args, i, &val,
                    "--io_threads", (char *)NULL)) {
            options["io_threads"] = val;
        } else if (ceph_argparse_witharg(args, i, &val,
                    "--io_total", (char *)NULL)) {
            options["io_total"] = val;
        */
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
