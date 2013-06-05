/**
 * @file        Fixed.cpp
 * @brief       Fixed point arithmetic for MSP430
 * @author      Greg Larmore <larmoreg@gmail.com>
 * @date        June 4, 2013
 * @copyright   Copyright (c) 2013 Greg Larmore
 *              GNU Lesser General Public License v3.0
 */

#include "Fixed.h"

Q1_15 operator-(Q1_15 x) {
  x.full(-x.full());
  return x;
}
Q1_15 operator+(Q1_15 x, const Q1_15 &y) {
  x += y;
  return x;
}
Q1_15 operator-(Q1_15 x, const Q1_15 &y) {
  x -= y;
  return x;
}
Q1_15 operator*(Q1_15 x, const Q1_15 &y) {
  x *= y;
  return x;
}
