/*
 * =====================================================================================
 *
 *       Filename:  aioWriteCase.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/23/2014 10:35:47 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  jiangheng , jiangheng@UnitedStack.com 
 *        Company:  United Stack 
 *
 * =====================================================================================
 */
#include "aioReadCase.h" 
AioReadCase :: AioReadCase(string caseName, librbd::Image *pImage, uint64_t ioSize, uint64_t ioThreads, uint64_t ioBytes, string ioPattern)
    :AioCase(caseName, false, ioSize, ioThreads, ioBytes, ioPattern, pImage, &librbd::Image::aio_read) {
        //cout << "Construct AioReadCase" << std::endl;
}

AioReadCase :: ~AioReadCase() {
    //cout << "Destruct AioReadCase" << std::endl; 
}
