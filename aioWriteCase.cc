/*
 * =====================================================================================
 *
 *       Filename:  aioWriteCase.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/14/2014 02:10:47 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#include "aioWriteCase.h" 
AioWriteCase :: AioWriteCase(string caseName, librbd::Image *pImage, uint64_t ioSize, uint64_t ioThreads, uint64_t ioBytes, string ioPattern)
    :AioCase(caseName, true, ioSize, ioThreads, ioBytes, ioPattern, pImage, &librbd::Image::aio_write) {
        //cout << "Construct AioWriteCase" << std::endl;
}

AioWriteCase :: ~AioWriteCase() {
    //cout << "Destruct AioWriteCase" << std::endl; 
}

