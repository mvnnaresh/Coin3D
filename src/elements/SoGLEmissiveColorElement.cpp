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
  \class SoGLEmissiveColorElement Inventor/elements/SoGLEmissiveColorElement.h
  \brief The SoGLEmissiveColorElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLEmissiveColorElement.h>

#if HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H
#if HAVE_WINDOWS_H
#include <windows.h>
#endif // HAVE_WINDOWS_H
#include <GL/gl.h>

#include <assert.h>

SO_ELEMENT_SOURCE(SoGLEmissiveColorElement);

/*!
  This static method initializes static data for the
  SoGLEmissiveColorElement class.
*/

void
SoGLEmissiveColorElement::initClass()
{
  SO_ELEMENT_INIT_CLASS(SoGLEmissiveColorElement, inherited);
}

/*!
  The destructor.
*/

SoGLEmissiveColorElement::~SoGLEmissiveColorElement()
{
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::init(SoState *state)
{
  inherited::init(state);
  this->current.setValue(0.0f, 0.0f, 0.0f);
  this->updategl();
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::push(SoState *state)
{
  inherited::push(state);
  SoGLEmissiveColorElement * const element =
    (SoGLEmissiveColorElement *)this->next;
  element->current = this->current;
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::pop(SoState *state, const SoElement *prevTopElement)
{
  inherited::pop(state, prevTopElement);
  ((SoGLEmissiveColorElement*)prevTopElement)->current = this->current;
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::send(const int index)
{
  int realindex = index < this->numColors ? index : this->numColors - 1;
  if (colors && colors[realindex] != this->current) {
    this->current = colors[realindex];
    this->updategl();
  }
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::setElt(const int32_t numColors,
                                 const SbColor * const colors)
{
  inherited::setElt(numColors, colors);
}

//! FIXME: write doc.

void
SoGLEmissiveColorElement::updategl()
{
  float col[4] = {0.0f, 0.0f, 0.0f, 1.0f};
  this->current.getValue(col[0], col[1], col[2]);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, col);
}
