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

// Important note: this sourcefile was in full generated by the
// Coin/scripts/templant script from the code in MFNodeEnginePath.tpl.

//$ BEGIN TEMPLATE MFNodeEnginePath(ENGINE, Engine, engine)

/*!
  \class SoMFEngine SoMFEngine.h Inventor/fields/SoMFEngine.h
  \brief The SoMFEngine class is a container for engines.
  \ingroup fields

  This field container stores an array of pointers to engines. It takes
  care of the necessary functionality for handling copy, import and
  export operations.

  Note that engine pointers stored in field instances of this type may
  be \c NULL pointers.

  \sa SoEngine, SoSFEngine

*/

// Type-specific define to be able to do #ifdef tests on type.  (Note:
// used to check the header file wrapper define, but that doesn't work
// with --enable-compact build.)
#define COIN_INTERNAL_ENGINE

#include <Inventor/fields/SoMFEngine.h>
#include <Inventor/fields/SoSubFieldP.h>
#include <Inventor/fields/SoSFEngine.h>

#include <Inventor/SoPath.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/nodes/SoNode.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


SO_MFIELD_REQUIRED_SOURCE(SoMFEngine);
SO_MFIELD_CONSTRUCTOR_SOURCE(SoMFEngine);
SO_MFIELD_MALLOC_SOURCE(SoMFEngine, SoEngine *);



// Override from parent class.
void
SoMFEngine::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFEngine);
}


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

//// From the SO_MFIELD_VALUE_SOURCE macro, start. ///////////////////////////

// We can't use the macro invocation, as we need to take care of doing
// ref() and unref() on the engines in the array.

int
SoMFEngine::fieldSizeof(void) const
{
  return sizeof(SoEngine *);
}

void *
SoMFEngine::valuesPtr(void)
{
  return (void *)this->values;
}

void
SoMFEngine::setValuesPtr(void * ptr)
{
  // We don't get any ref()'ing done here, or any notification
  // mechanisms set up -- so this function should _only_ be used for
  // initial setup of array memory.  In Coin, it's only used from
  // SoMField::allocValues().
  this->values = (SoEngine **)ptr;
}

int
SoMFEngine::find(SoEngine * value, SbBool addifnotfound)
{
  for (int i=0; i < this->num; i++) if ((*this)[i] == value) return i;

  if (addifnotfound) this->set1Value(this->num, value);
  return -1;
}

void
SoMFEngine::setValues(const int start, const int num, const SoEngine ** newvals)
{
  // Disable temporarily, so we under any circumstances will not send
  // more than one notification about the changes.
  SbBool notificstate = this->enableNotify(FALSE);

  // ref() new engines before unref()-ing old ones, in case there are
  // common engines (we don't want any premature destruction to happen).
  { for (int i=0; i < num; i++) if (newvals[i]) newvals[i]->ref(); }

  // We favor simplicity of code over performance here.
  { for (int i=0; i < num; i++)
    this->set1Value(start+i, (SoEngine *)newvals[i]); }

  // unref() to match the initial ref().
  { for (int i=0; i < num; i++) if (newvals[i]) newvals[i]->unref(); }

  // Finally, send notification.
  (void)this->enableNotify(notificstate);
  if (notificstate) this->valueChanged();
}

void
SoMFEngine::set1Value(const int idx, SoEngine * newval)
{
  // Disable temporarily, so we under no circumstances will send more
  // than one notification about the change.
  SbBool notificstate = this->enableNotify(FALSE);

  // Don't use getNum(), getValues() or operator[] to find old values,
  // since this might trigger a recursive evaluation call if the field 
  // is connected.

  // Expand array if necessary.
  if (idx >= this->num) {
#ifdef COIN_INTERNAL_PATH
    for (int i = this->num; i <= idx; i++) this->pathheads.append(NULL);
#endif // COIN_INTERNAL_PATH
    this->setNum(idx + 1);
  }

  SoEngine * oldptr = this->values[idx];
  if (oldptr == newval) return;

  if (oldptr) {
    oldptr->removeAuditor(this, SoNotRec::FIELD);
    oldptr->unref();
#ifdef COIN_INTERNAL_PATH
    SoNode * h = oldptr->getHead();
    // The path should be audited by us at all times. So don't use
    // SoMFPath to wrap SoTempPath or SoLightPath, for instance.
    assert(h==this->pathheads[idx] &&
           "Path head changed without notification!");
    if (h) {
      h->removeAuditor(this, SoNotRec::FIELD);
      h->unref();
    }
#endif // COIN_INTERNAL_PATH
  }

  if (newval) {
    newval->addAuditor(this, SoNotRec::FIELD);
    newval->ref();
#ifdef COIN_INTERNAL_PATH
    SoNode * h = newval->getHead();
    if (h) {
      h->addAuditor(this, SoNotRec::FIELD);
      h->ref();
    }
#endif // COIN_INTERNAL_PATH
  }

  this->values[idx] = newval;
#ifdef COIN_INTERNAL_PATH
  this->pathheads[idx] = newval ? newval->getHead() : NULL;
#endif // COIN_INTERNAL_PATH

  // Finally, send notification.
  (void)this->enableNotify(notificstate);
  if (notificstate) this->valueChanged();
}

void
SoMFEngine::setValue(SoEngine * value)
{
  this->deleteAllValues();
  this->set1Value(0, value);
}

SbBool
SoMFEngine::operator==(const SoMFEngine & field) const
{
  if (this == &field) return TRUE;
  if (this->getNum() != field.getNum()) return FALSE;

  const SoEngine ** const lhs = this->getValues(0);
  const SoEngine ** const rhs = field.getValues(0);
  for (int i = 0; i < num; i++) if (lhs[i] != rhs[i]) return FALSE;
  return TRUE;
}

void
SoMFEngine::deleteAllValues(void)
{
  // Don't use getNum(), but use this->num directly, since getNum() 
  // might trigger a recursive evaluation call if the field 
  // is connected.

  if (this->num) this->deleteValues(0);
}

// Overridden to handle unref() and removeAuditor().
void
SoMFEngine::deleteValues(int start, int num)
{
  // Note: this function overrides the one in SoMField, so if you do
  // any changes here, take a look at that method aswell.

  if (num == -1) num = this->num - start;
  for (int i=start; i < start+num; i++) {
    SoEngine * n = this->values[i];
    if (n) {
      n->removeAuditor(this, SoNotRec::FIELD);
      n->unref();
    }
#ifdef COIN_INTERNAL_PATH
    SoNode * h = this->pathheads[start];
    this->pathheads.remove(start);
    if (h) {
      h->removeAuditor(this, SoNotRec::FIELD);
      h->unref();
    }
#endif // COIN_INTERNAL_PATH
  }

  inherited::deleteValues(start, num);
}

// Overridden to insert NULL pointers in new array slots.
void
SoMFEngine::insertSpace(int start, int num)
{
  // Disable temporarily so we don't send notification prematurely
  // from inherited::insertSpace().
  SbBool notificstate = this->enableNotify(FALSE);

  inherited::insertSpace(start, num);
  for (int i=start; i < start+num; i++) {
#ifdef COIN_INTERNAL_PATH
    this->pathheads.insert(NULL, start);
#endif // COIN_INTERNAL_PATH
    this->values[i] = NULL;
  }

  // Initialization done, now send notification.
  (void)this->enableNotify(notificstate);
  if (notificstate) this->valueChanged();
}

// This is the "memmove replacement" to use copy constructors where
// they are defined.
void
SoMFEngine::copyValue(int to, int from)
{
  this->values[to] = this->values[from];
}

//// From the SO_MFIELD_VALUE_SOURCE macro, end. /////////////////////////////


// Import a single engine.
SbBool
SoMFEngine::read1Value(SoInput * in, int index)
{
  SoSFEngine sfengine;
  SbBool result = sfengine.readValue(in);
  if (result) this->set1Value(index, sfengine.getValue());
  return result;
}

// Export a single engine.
void
SoMFEngine::write1Value(SoOutput * out, int idx) const
{
  SoSFEngine sfengine;
  sfengine.setValue((*this)[idx]);
  sfengine.writeValue(out);
}

#endif // DOXYGEN_SKIP_THIS


// Overridden from parent to propagate write reference counting to
// engine.
void
SoMFEngine::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);

  for (int i=0; i < this->getNum(); i++) {
    SoEngine * n = (*this)[i];
    // Set the "from field" flag as FALSE, as that flag is meant to be
    // used for references through field-to-field connections.
    if (n) n->addWriteReference(out, FALSE);
#ifdef COIN_INTERNAL_PATH
    if (this->pathheads[i]) this->pathheads[i]->addWriteReference(out, FALSE);
#endif // COIN_INTERNAL_PATH
  }
}

// Override from parent to update our engine pointer references. This is
// necessary so 1) we're added as an auditor to the copied engines (they
// have so far only been copied as pointer bits), and 2) so we
// increase the reference count.
void
SoMFEngine::fixCopy(SbBool copyconnections)
{
  for (int i=0; i < this->getNum(); i++) {
    SoEngine * n = (*this)[i];
    if (n) {
      // There's only been a bitwise copy of the pointer; no auditing
      // has been set up, no increase in the reference count. So we do
      // that by setting the value to NULL and then re-setting with
      // setValue().
      this->values[i] = NULL;
#if defined(COIN_INTERNAL_NODE) || defined(COIN_INTERNAL_ENGINE)
      SoFieldContainer * fc = SoFieldContainer::findCopy(n, copyconnections);
      this->set1Value(i, (SoEngine *)fc);
#endif // COIN_INTERNAL_NODE || COIN_INTERNAL_ENGINE

#ifdef COIN_INTERNAL_PATH
      this->set1Value(i, n->copy());
#endif // COIN_INTERNAL_PATH
    }
  }
}

// Override from SoField to check engine pointer.
SbBool
SoMFEngine::referencesCopy(void) const
{
  if (inherited::referencesCopy()) return TRUE;

  for (int i=0; i < this->getNum(); i++) {
    SoEngine * item = (*this)[i];
    if (item) {
#if defined(COIN_INTERNAL_NODE) || defined(COIN_INTERNAL_ENGINE)
      if (SoFieldContainer::checkCopy((SoFieldContainer *)item)) return TRUE;
#endif // COIN_INTERNAL_NODE || COIN_INTERNAL_ENGINE
#ifdef COIN_INTERNAL_PATH
      if (item->getHead() && SoFieldContainer::checkCopy(item->getHead())) return TRUE;
#endif // COIN_INTERNAL_PATH
    }
  }

  return FALSE;
}

// Kill the type-specific define.
#undef COIN_INTERNAL_ENGINE
//$ END TEMPLATE MFNodeEnginePath
