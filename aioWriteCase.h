/*
 * =====================================================================================
 *
 *       Filename:  aioWriteCase.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/14/2014 01:50:24 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#ifndef AIO_WRITE_CASE_H
#define AIO_WRITE_CASE_H

#include "aioCase.h" 
class AioWriteCase: public AioCase {
public:
    uint64_t io_size;
    uint64_t io_threads;
    uint64_t io_bytes;
    string pattern;
   AioWriteCase(string caseName, librbd::Image *pImage, uint64_t ioSize, uint64_t ioThreads, uint64_t ioBytes, string ioPattern)
       :AioCase(caseName, pImage, &librbd::Image::aio_write), 
       io_size(ioSize), io_threads(ioThreads), io_bytes(ioBytes), 
       pattern(ioPattern)
    {};
   virtual bool run();
};
#endif
