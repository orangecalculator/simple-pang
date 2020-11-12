#ifndef _PANG_H_
#define _PANG_H_

constexpr double tol = 1e-5;

constexpr int WindowInitWidth = 400;
constexpr int WindowInitHeight = 300;

constexpr double GameFrameLeft = -0.9;
constexpr double GameFrameRight = 0.9;

constexpr double GameFrameUp = 0.9;
constexpr double GameFrameDown = -0.8;

constexpr double Init_PlayerPosition_x = 0.0;
constexpr double Init_PlayerPosition_y = GameFrameDown;

constexpr double PlayerSpeed = 0.05;
constexpr double HarpoonSpeed = 0.01;

constexpr int fps = 30;

#ifdef NDEBUG
#define DEBUG(...) 
#else
#include <stdio.h>
#define DEBUG(...) printf(__VA_ARGS__);
#endif

#endif /* _PANG_H_ */