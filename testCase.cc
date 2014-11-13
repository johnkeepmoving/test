/*
 * =====================================================================================
 *
 *       Filename:  testCase.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/14/2014 10:38:45 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#include "testCase.h" 

double diff_time_ms(timeval end_time, timeval start_time)
{
    long long ret = ((long long)(end_time.tv_sec - start_time.tv_sec)) * 1000000 +
        (long long)(end_time.tv_usec - start_time.tv_usec);
    return (double)ret/1000.0;
}

TestCase :: TestCase(string caseName): 
    case_name(caseName) ,case_result(false) {
    data = bench_data();
    //std::cout << "Construct TestCase() " << std::endl;
}
TestCase :: ~TestCase() {
    //std::cout << "Destrcut TestCase() " << std::endl;
}

void TestCase::printStatus() {
    //Status are stored in data
    std::cout << "TestCase: " << case_name << std::endl;
    std::cout << "Time measure: ms" << std::endl;
    std::cout << "total time: "<< diff_time_ms(data.end_time, data.start_time) << std::endl;
    std::cout << "total_latency: "<< data.total_latency << std::endl;
    std::cout << "    min_latency: " << data.min_latency << std::endl;
    std::cout << "    max_latency: " << data.max_latency << std::endl;
    std::cout << "    avg_latency: " << data.avg_latency << std::endl;
    std::cout << "    variance_latency: " << data.variance_latency << std::endl;
    std::cout << "starte_ops: " << data.started<< std::endl; 
    std::cout << "finished_ops: " << data.finished << std::endl; 
    std::cout << "in_flight_ops: " << data.in_flight<< std::endl; 
}

