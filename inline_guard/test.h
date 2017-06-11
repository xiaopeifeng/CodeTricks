#ifndef TEST_H_
#define TEST_H_

#include <iostream>

class Test {
public:
    void xxxx();
};

#ifdef _INLINE_
#define INLINE inline
#include "test.ipp"
#endif

#endif
