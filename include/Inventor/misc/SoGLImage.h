#ifndef COIN_SOGLIMAGE_H
#define COIN_SOGLIMAGE_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

// WARNING: This is work in progress. Do not expect this class to have
// a stable interface over a long period of time. It is installed
// because we need it in an internal project. 

// FIXME: make sure we do a design review for this class for Coin v2.0
// pederb, 2001-11-28

#include <Inventor/SbBasic.h>
#include <Inventor/SbVec2s.h>
#include <Inventor/SoType.h>
#include <stddef.h>

class SoGLDisplayList;
class SoState;
class SbImage;

class COIN_DLL_API SoGLImage {
public:

  SoGLImage();
  virtual void unref(SoState * state = NULL);

  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const ;
  virtual SbBool isOfType(SoType type) const;

  enum Wrap {
    REPEAT = 0,
    CLAMP,
    CLAMP_TO_EDGE
  };

  void applyQuality(SoGLDisplayList * dl, const float quality);

  void setData(const unsigned char * bytes,
               const SbVec2s & size,
               const int numcomponents,
               const Wrap wraps = REPEAT,
               const Wrap wrapt = REPEAT,
               const float quality = 0.5f,
               const int border = 0,
               SoState * createinstate = NULL);

  virtual void setData(const SbImage * image,
                       const Wrap wraps = REPEAT,
                       const Wrap wrapt = REPEAT,
                       const float quality = 0.5f,
                       const int border = 0,
                       SoState * createinstate = NULL);

  // these flags can be used to set texture properties.
  enum Flags {
    // mipmap, scaling and filtering settings
    SCALE_DOWN =                0x0001,
    NO_MIPMAP =                 0x0002,
    LINEAR_MAG_FILTER =         0x0004,
    LINEAR_MIN_FILTER =         0x0008,
    LINEAR_MIPMAP_FILTER =      0x0010,

    // use if you know your image properties.
    FORCE_TRANSPARENCY_TRUE   = 0x0020,
    FORCE_TRANSPARENCY_FALSE  = 0x0040,
    FORCE_ALPHA_TEST_TRUE     = 0x0080,
    FORCE_ALPHA_TEST_FALSE    = 0x0100,

    INVINCIBLE                = 0x0200, // never die of old age
    // use quality value to decide mipmap, filtering and scaling. This
    // is the default.
    USE_QUALITY_VALUE         = 0X8000
  };

  void setFlags(const uint32_t flags);
  uint32_t getFlags(void) const;

  const SbImage * getImage(void) const;

  virtual SoGLDisplayList * getGLDisplayList(SoState * state);
  SbBool hasTransparency(void) const;
  SbBool useAlphaTest(void) const;
  Wrap getWrapS(void) const;
  Wrap getWrapT(void) const;

protected:

  void incAge(void) const;
  void resetAge(void) const;
  virtual void unrefOldDL(SoState * state, const uint32_t maxage);
  virtual ~SoGLImage();

private:

  class SoGLImageP * pimpl;
  friend class SoGLImageP;
  
public:
  // internal methods for texture resource management
  static void beginFrame(SoState * state);
  static void tagImage(SoState * state, SoGLImage * image);
  static void endFrame(SoState * state);
  static void setDisplayListMaxAge(const uint32_t maxage);
  static void freeAllImages(SoState * state = NULL);

  void setEndFrameCallback(void (*cb)(void *), void * closure);
  int getNumFramesSinceUsed(void) const;

private:
  static void registerImage(SoGLImage * image);
  static void unregisterImage(SoGLImage * image);
};

#endif // !COIN_SOGLIMAGE_H
