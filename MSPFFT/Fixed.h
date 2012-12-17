#ifndef FIXED_H_
#define FIXED_H_

#include <msp430.h>
#include <stdint.h>

// Copyright (c) 2012, Greg Larmore
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met: 
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer. 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution. 
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// The views and conclusions contained in the software and documentation are those
// of the authors and should not be interpreted as representing official policies, 
// either expressed or implied, of the FreeBSD Project.


/*
 * Fixed point arithmetic
 *
 * Greg Larmore
 */

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
//  Q1_15& operator*=(const Q1_15 &x) {
//    //Q1.15 fixed point multiply (software)
//    _full = ((int32_t)_full*x._full+16384)>>15;
//    return *this;
//  };
  Q1_15& operator*=(const Q1_15 &x) {
    //Q1.15 fixed point multiply (hardware)
    MPYS = _full;
    OP2 = x._full;
    _full = RESHI;
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
