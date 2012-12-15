#include "Fixed.h"

/*
 * Fixed point arithmetic
 *
 * Greg Larmore
 */

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
