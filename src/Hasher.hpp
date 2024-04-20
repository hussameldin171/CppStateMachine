/***************************************************************************************
 * 
 *                                State Machine Library
 * 
 *   File Name :Hasher.hpp
 * 
 *   Author : Hussam Wael
 * 
 *   Description : This file contains a simple hasher functor used for hashing the states
 * 
 * **************************************************************************************/

#ifndef __HASHER_H__
#define __HASHER_H__

#include <cstddef>

namespace sm{

template<size_t N>
struct DefaultHasher{

    template<typename Integral>
    size_t operator()(Integral number) const{
        return static_cast<size_t>(number) % N;
    }
};

}


#endif // __HASHER_H__