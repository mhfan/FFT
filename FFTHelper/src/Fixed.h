#ifndef FIXED_H_
#define FIXED_H_

#include <stdint.h>

#define CONST_Q1_15(A,B) (int16_t)((A<<15) + ((B + 0.0000152587890625)*32768))

class Q1_15 {
private:
  int16_t _full;
public:
  Q1_15(int16_t full = 0) {
    _full = full;
  };
  Q1_15& operator+=(const Q1_15 &x) {
    _full += x._full;
    return *this;
  };
  Q1_15& operator-=(const Q1_15 &x) {
    _full -= x._full;
    return *this;
  };
  Q1_15& operator*=(const Q1_15 &x) {
    //Q1.15 fixed point multiply (software)
    _full = ((int32_t)_full*x._full+16384)>>15;
    return *this;
  };
  inline int16_t full() const {
    return _full;
  };
  void full(int16_t full) {
    _full = full;
  }
  inline float toFloat(void) const {
    return (float)(_full >> 15) + ((float)(0x7FFF&_full)) / 32768;
  };
};

inline int operator!=(const Q1_15 &x, const Q1_15 &y) {
  return (x.full()!=y.full());
}
inline int operator==(const Q1_15 &x, const Q1_15 &y) {
  return (x.full()==y.full());
}

Q1_15 operator-(Q1_15 x);
Q1_15 operator+(Q1_15 x, const Q1_15 &y);
Q1_15 operator-(Q1_15 x, const Q1_15 &y);
Q1_15 operator*(Q1_15 x, const Q1_15 &y);

#endif /* FIXED_H_ */
