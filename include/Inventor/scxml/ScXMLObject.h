#ifndef COIN_SCXMLOBJECT_H
#define COIN_SCXMLOBJECT_H

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

#include <map>

#include <Inventor/SbBasic.h>
#include <Inventor/SoType.h>

#include <Inventor/scxml/ScXMLSubObject.h>

class COIN_DLL_API ScXMLObject {
  SCXML_OBJECT_ABSTRACT_HEADER(ScXMLObject);
public:
  static void initClass(void);

  ScXMLObject(void);
  virtual ~ScXMLObject(void);

  SbBool isOfType(SoType type) const;

  void setAttribute(const char * attribute, const char * value);
  const char * getAttribute(const char * attribute) const;

  virtual SbBool handleXMLAttributes(void);

  virtual void setContainer(const ScXMLObject * container);
  const ScXMLObject * getContainer(void) const { return this->containerptr; }

  SbBool isContainedIn(const ScXMLObject * object) const;

protected:
  static void registerClassType(const char * xmlns,
                                const char * classname, SoType type);
  static void registerInvokeClassType(const char * xmlns,
                                      const char * targettype,
                                      const char * source, SoType type);

private:
  const ScXMLObject * containerptr;

  typedef std::map<const char *, const char *> AttrDict;
  typedef std::pair<const char *, const char *> AttrEntry;
  AttrDict attributedict;

}; // ScXMLObject

#endif // !COIN_SCXMLOBJECT_H