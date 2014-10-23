/*
 * =====================================================================================
 *
 *       Filename:  aioWriteCase.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/23/2014 10:34:24 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#ifndef AIO_READ_CASE_H
#define AIO_READ_CASE_H

#include "aioCase.h" 
class AioReadCase: public AioCase {
public:
    AioReadCase(string caseName, librbd::Image *pImage, uint64_t ioSize, uint64_t ioThreads, uint64_t ioBytes, string ioPattern);
    ~AioReadCase();
};
#endif
