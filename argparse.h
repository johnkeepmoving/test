/*
 * =====================================================================================
 *
 *       Filename:  argparse.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/13/2014 02:22:37 PM
 *       Revision:  none
 *       Compiler:  g++ 
 *
 *         Author:  jiangheng , jiangheng@unitedstack.com
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <string>
#include <string.h>
#include <stdarg.h>

using namespace std;

bool ceph_argparse_flag(vector<const char*> &args,
	vector<const char*>::iterator &i, ...);
bool ceph_argparse_witharg(vector<const char*> &args,
	vector<const char*>::iterator &i, string *ret, ...);
