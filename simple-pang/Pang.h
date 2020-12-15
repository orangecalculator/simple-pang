#ifndef _PANG_H_
#define _PANG_H_

#include <array>

constexpr double tol = 1e-8;

constexpr int WindowInitWidth = 1000;
constexpr int WindowInitHeight = 1000;

constexpr double GameFrameLeft = -0.9;
constexpr double GameFrameRight = 0.9;

constexpr double GameFrameUp = 0.9;
constexpr double GameFrameDown = -0.8;

constexpr double Init_PlayerPosition_x = 0.0;
constexpr double Init_PlayerPosition_y = 0.1 + GameFrameDown;

constexpr double PlayerSpeed = 0.05;
constexpr double HarpoonSpeed = 0.02;

constexpr double BallSpeedX = 0.005;
constexpr double BallGravity = 0.0004;
constexpr int BallSlice = 50;
constexpr int BallStack = 50;

constexpr double BallMaxSpeedY = 0.03;
constexpr double BallMaxSize = 0.1;
constexpr double BallMinSize = 0.02;

constexpr int fps = 60;

constexpr int LifeInit = 5;

// 무적 시간을 3초로 함.
constexpr long CollideDelay = fps * 3;
constexpr double PlayerCollideBoxSize = 0.03;

////// HELPERS //////

#ifdef DEBUG
#undef DEBUG
#include <stdio.h>
#define DEBUG(...) (printf(__VA_ARGS__))
#else
#define DEBUG(...)
#endif

template<typename T>
static inline T square(T x) { return x * x; }

#endif /* _PANG_H_ */