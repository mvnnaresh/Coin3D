#ifndef COIN_SOGLTEXTUREIMAGEELEMENT_H
#define COIN_SOGLTEXTUREIMAGEELEMENT_H

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

#include <Inventor/elements/SoTextureImageElement.h>

class SoGLImage;
class SoGLDisplayList;

class COIN_DLL_API SoGLTextureImageElement : public SoTextureImageElement {
  typedef SoTextureImageElement inherited;

  SO_ELEMENT_HEADER(SoGLTextureImageElement);
public:
  static void initClass(void);
protected:
  virtual ~SoGLTextureImageElement();

public:
  virtual void init(SoState * state);
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
                   const SoElement * prevTopElement);

  static void set(SoState * const state, SoNode * const node,
                  SoGLImage *image, const Model model,
                  const SbColor &blendColor,
                  const SbBool didapply = FALSE);

  static SoGLImage * get(SoState * state, Model & model,
                         SbColor & blendcolor);

  virtual void evaluate(const SbBool enabled, const SbBool transparency) const;
  static int32_t getMaxGLTextureSize(void);
  virtual SbBool isTextureSizeLegal(int xsize, int ysize, int zsize, 
                                    int bytespertexel);

protected:
  virtual SbBool hasTransparency(void) const;

private:
  SoGLImage * image;
  SoGLDisplayList * dlist;
  float quality;
  SbBool alphatest;
  SoState * state;
  int glmodel;
  SbBool glalphatest;
  SbColor glblendcolor;
  SbBool didapply;
};

#endif // !COIN_SOGLTEXTUREIMAGEELEMENT_H
