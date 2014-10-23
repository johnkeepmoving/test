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

TestCase :: TestCase(string caseName): 
    case_name(caseName) ,case_result(false) {
    std::cout << "Construct TestCase() " << std::endl;
}
TestCase :: ~TestCase() {
    std::cout << "Destrcut TestCase() " << std::endl;
}

void TestCase::printStatus() {
    //Status are stored in data
    std::cout << "TestCase: " << case_name << std::endl;
    std::cout << "total time: "<< (data.end_time - data.start_time) << std::endl;
    std::cout << "total_latency: "<< data.total_latency << std::endl;
    std::cout << "    min_latency: " << data.min_latency << std::endl;
    std::cout << "    max_latency: " << data.max_latency << std::endl;
    std::cout << "    avg_latency: " << data.avg_latency << std::endl;
    std::cout << "    variance_latency: " << data.variance_latency << std::endl;
    std::cout << "starte_ops: " << data.started<< std::endl; 
    std::cout << "finished_ops: " << data.finished << std::endl; 
    std::cout << "in_flight_ops: " << data.in_flight<< std::endl; 
}
