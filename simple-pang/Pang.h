#ifndef _PANG_H_
#define _PANG_H_

#include <array>

constexpr double tol = 1e-5;

constexpr int WindowInitWidth = 800;
constexpr int WindowInitHeight = 600;

constexpr double GameFrameLeft = -0.9;
constexpr double GameFrameRight = 0.9;

constexpr double GameFrameUp = 0.9;
constexpr double GameFrameDown = -0.8;

constexpr double Init_PlayerPosition_x = 0.0;
constexpr double Init_PlayerPosition_y = GameFrameDown;

constexpr double PlayerSpeed = 0.05;
constexpr double HarpoonSpeed = 0.01;

constexpr double BallSpeedX = 0.01;
constexpr double BallGravity = 0.0015;
constexpr int BallSlice = 50;
constexpr int BallStack = 50;

constexpr int fps = 15;

#ifdef DEBUG
#undef DEBUG
#include <stdio.h>
#define DEBUG(...) (printf(__VA_ARGS__))
#else
#define DEBUG(...)
#endif

#endif /* _PANG_H_ */