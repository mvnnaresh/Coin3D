/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2000 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to aquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOTRANSFORMVEC3F_H
#define COIN_SOTRANSFORMVEC3F_H

#include <Inventor/engines/SoSubEngine.h>
#include <Inventor/engines/SoEngineOutput.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFMatrix.h>

class COIN_DLL_EXPORT SoTransformVec3f : public SoEngine {
  typedef SoEngine inherited;

  SO_ENGINE_HEADER(SoTransformVec3f);

public:

  SoMFVec3f vector;
  SoMFMatrix matrix;

  SoEngineOutput point;           // SoMFVec3f
  SoEngineOutput direction;       // SoMFVec3f
  SoEngineOutput normalDirection; // SoMFVec3f

  SoTransformVec3f(void);
  static void initClass(void);

protected:
  virtual ~SoTransformVec3f();

private:
  virtual void evaluate(void);
};

#endif // !COIN_SOTRANSFORMVEC3F_H
