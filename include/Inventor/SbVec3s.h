#ifndef COIN_SBVEC3S_H
#define COIN_SBVEC3S_H

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <stdio.h>
#include <Inventor/SbBasic.h>
#include <Inventor/SbByteBuffer.h>
#include <Inventor/SbString.h>

#ifndef NDEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // !NDEBUG

class SbVec3us;
class SbVec3b;
class SbVec3i32;
class SbVec3f;
class SbVec3d;

class COIN_DLL_API SbVec3s {
public:
  SbVec3s(void) { }
  SbVec3s(const short v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; }
  SbVec3s(short x, short y, short z) { vec[0] = x; vec[1] = y; vec[2] = z; }
  explicit SbVec3s(const SbVec3us & v) { setValue(v); }
  explicit SbVec3s(const SbVec3b & v) { setValue(v); }
  explicit SbVec3s(const SbVec3i32 & v) { setValue(v); }
  explicit SbVec3s(const SbVec3f & v) { setValue(v); }
  explicit SbVec3s(const SbVec3d & v) { setValue(v); }

  SbVec3s & setValue(const short v[3]) { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; return *this; }
  SbVec3s & setValue(short x, short y, short z) { vec[0] = x; vec[1] = y; vec[2] = z; return *this; }
  SbVec3s & setValue(const SbVec3us & v);
  SbVec3s & setValue(const SbVec3b & v);
  SbVec3s & setValue(const SbVec3i32 & v);
  SbVec3s & setValue(const SbVec3f & v);
  SbVec3s & setValue(const SbVec3d & v);

  const short * getValue(void) const { return vec; }
  void getValue(short & x, short & y, short & z) const { x = vec[0]; y = vec[1]; z = vec[2]; }

  short & operator [] (int i) { return vec[i]; }
  const short & operator [] (int i) const { return vec[i]; }

  int32_t dot(const SbVec3s & v) const { return vec[0] * v[0] + vec[1] * v[1] + vec[2] * v[2]; }
  void negate(void) { vec[0] = -vec[0]; vec[1] = -vec[1]; vec[2] = -vec[2]; }

  SbVec3s & operator *= (int d) { vec[0] *= d; vec[1] *= d; vec[2] *= d; return *this; }
  SbVec3s & operator *= (double  d);
  SbVec3s & operator /= (int d) { SbDividerChk("SbVec3s::operator/=(int)", d); vec[0] /= d; vec[1] /= d; vec[2] /= d; return *this; }
  SbVec3s & operator /= (double d) { SbDividerChk("SbVec3s::operator/=(double)", d); return operator *= (1.0 / d); }
  SbVec3s & operator += (const SbVec3s & v) { vec[0] += v[0]; vec[1] += v[1]; vec[2] += v[2]; return *this; }
  SbVec3s & operator -= (const SbVec3s & v) { vec[0] -= v[0]; vec[1] -= v[1]; vec[2] -= v[2]; return *this; }
  SbVec3s operator - (void) const { return SbVec3s(-vec[0], -vec[1], -vec[2]); }

  SbString toString() const;
  SbBool fromString(const SbString & str);
  void print(FILE * fp) const;

protected:
  short vec[3];

}; // SbVec3s

COIN_DLL_API inline SbVec3s operator * (const SbVec3s & v, int d) {
  SbVec3s val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3s operator * (const SbVec3s & v, double d) {
  SbVec3s val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3s operator * (int d, const SbVec3s & v) {
  SbVec3s val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3s operator * (double d, const SbVec3s & v) {
  SbVec3s val(v); val *= d; return val;
}

COIN_DLL_API inline SbVec3s operator / (const SbVec3s & v, int d) {
  SbDividerChk("operator/(SbVec3s,int)", d);
  SbVec3s val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3s operator / (const SbVec3s & v, double d) {
  SbDividerChk("operator/(SbVec3s,double)", d);
  SbVec3s val(v); val /= d; return val;
}

COIN_DLL_API inline SbVec3s operator + (const SbVec3s & v1, const SbVec3s & v2) {
  SbVec3s v(v1); v += v2; return v;
}

COIN_DLL_API inline SbVec3s operator - (const SbVec3s & v1, const SbVec3s & v2) {
  SbVec3s v(v1); v -= v2; return v;
}

COIN_DLL_API inline int operator == (const SbVec3s & v1, const SbVec3s & v2) {
  return ((v1[0] == v2[0]) && (v1[1] == v2[1]) && (v1[2] == v2[2]));
}

COIN_DLL_API inline int operator != (const SbVec3s & v1, const SbVec3s & v2) {
  return !(v1 == v2);
}

#endif // !COIN_SBVEC3S_H
