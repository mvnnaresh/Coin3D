#ifndef COIN_SOSFVEC4D_H
#define COIN_SOSFVEC4D_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/fields/SoSField.h>
#include <Inventor/fields/SoSubField.h>
#include <Inventor/SbVec4d.h>

class COIN_DLL_API SoSFVec4d : public SoSField {
  typedef SoSField inherited;

  SO_SFIELD_HEADER(SoSFVec4d, SbVec4d, const SbVec4d &);

public:
  static void initClass(void);

  void setValue(double x, double y, double z, double w);
  void setValue(const double xyzw[4]);

}; // SoSFVec4d

#endif // !COIN_SOSFVEC4D_H
