#ifndef GP2Y10_H
#define GP2Y10_H

#include "Arduino.h"

/**
 空气质量图：小颗粒读数（大于0.5微米）
 3000 +     = 很差很差
 1050-3000  = 差
 300-1050   = 还行
 150-300    = 好
 75-150     = 非常好
 0-75       = 棒极了
 */

void initGP2Y10();
float getDustDensity();

#endif
