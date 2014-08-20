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
#ifndef OPERATE_CONFIG_H

#define OPERATE_CONFIG_H
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>
#define COMMENT_CHAR '#'
using std::string;

class operate_config
{
public:
    std::ifstream infile;
	operate_config(string filename);
    ~operate_config();
    bool getValue(std::map<string, string> &config);
};

#endif
