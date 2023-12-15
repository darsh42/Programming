#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define TEST_BIT(a, b) ((a & (1 << b)) == (1 << b))

#define CPU_CLOCK 4194304

// memory location defines
#define mLCDC 0XFF40
#define mLCDS 0XFF41
#define mSCX 0XFF42
#define mSCY 0XFF43
#define mWX 0XFF4A
#define mWY 0XFF4B
#define mLY 0XFF44
#define mLYC 0XFF45

#define mDIV 0XFF04
#define mTIMA 0XFF05
#define mTMA 0XFF06
#define mTAC 0XFF07

#endif // COMMON_H_INCLUDED
