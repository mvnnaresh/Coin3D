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
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

/*!
  \class SoDecomposeVec4f SoDecomposeVec4f.h Inventor/engines/SoDecomposeVec4f.h
  \brief The SoDecomposeVec4f class is used to decompose 4D vectors into four floats.
  \ingroup engines
*/

#include <Inventor/engines/SoDecomposeVec4f.h>
#include <Inventor/lists/SoEngineOutputList.h>
#include <Inventor/engines/SoSubEngineP.h>

SO_ENGINE_SOURCE(SoDecomposeVec4f);

#ifndef DOXYGEN_SKIP_THIS // No need to document these.

SoDecomposeVec4f::SoDecomposeVec4f()
{
  SO_ENGINE_INTERNAL_CONSTRUCTOR(SoDecomposeVec4f);

  SO_ENGINE_ADD_INPUT(vector,(0,0,0,0));

  SO_ENGINE_ADD_OUTPUT(x,SoMFFloat);
  SO_ENGINE_ADD_OUTPUT(y,SoMFFloat);
  SO_ENGINE_ADD_OUTPUT(z,SoMFFloat);
  SO_ENGINE_ADD_OUTPUT(w,SoMFFloat);
}

// overloaded from parent
void
SoDecomposeVec4f::initClass()
{
  SO_ENGINE_INTERNAL_INIT_CLASS(SoDecomposeVec4f);
}

//
// private members
//
SoDecomposeVec4f::~SoDecomposeVec4f()
{
}

// overloaded from parent
void
SoDecomposeVec4f::evaluate()
{
  int num = this->vector.getNum();

  SO_ENGINE_OUTPUT(x,SoMFFloat,setNum(num));
  SO_ENGINE_OUTPUT(y,SoMFFloat,setNum(num));
  SO_ENGINE_OUTPUT(z,SoMFFloat,setNum(num));
  SO_ENGINE_OUTPUT(w,SoMFFloat,setNum(num));

  int i;
  for (i = 0; i < num; i++) {
    SO_ENGINE_OUTPUT(x,SoMFFloat,set1Value(i,vector[i][0]));
    SO_ENGINE_OUTPUT(y,SoMFFloat,set1Value(i,vector[i][1]));
    SO_ENGINE_OUTPUT(z,SoMFFloat,set1Value(i,vector[i][2]));
    SO_ENGINE_OUTPUT(w,SoMFFloat,set1Value(i,vector[i][3]));
  }
}

#endif // !DOXYGEN_SKIP_THIS
