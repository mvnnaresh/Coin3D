/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion. All rights reserved.
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
  \class SoCallback SoCallback.h Inventor/nodes/SoCallback.h
  \brief The SoCallback class is a node type which provides a means of setting callback hooks in the scene graph.
  \ingroup nodes

  By inserting SoCallback nodes in a scene graph, the application
  programmer can set up functions to be executed at certain points in
  the traversal.

  The callback function will be executed during traversal of \e any
  action, so check the type of the \a action argument of the callback
  function if you only want to run your code at specific actions.
*/

#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/actions/SoActions.h> // SoCallback uses all of them.
#include <Inventor/elements/SoGLTextureImageElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoTransparencyElement.h>

/*!
  \typedef void SoCallbackCB(void * userdata, SoAction * action)
  Signature that callback functions need to have.
*/

SO_NODE_SOURCE(SoCallback);

/*!
  Constructor.
*/
SoCallback::SoCallback(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCallback);

  this->cbfunc = NULL;
}

/*!
  Destructor.
*/
SoCallback::~SoCallback()
{
}

// Doc from superclass.
void
SoCallback::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCallback);
}

/*!
  Set up the \a function to call at traversal of this node. \a
  userdata will be passed back as the first argument of the callback
  \a function.
*/
void
SoCallback::setCallback(SoCallbackCB * function, void * userdata)
{
  this->cbfunc = function;
  this->cbdata = userdata;
}

// Doc from superclass.
void
SoCallback::doAction(SoAction * action)
{
  if (this->cbfunc) this->cbfunc(this->cbdata, action);
}

// Doc from superclass.
void
SoCallback::callback(SoCallbackAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::GLRender(SoGLRenderAction * action)
{
  if (this->cbfunc) {
    SoState * state = action->getState();
    state->lazyEvaluate();

    SbBool transparent = SoTextureImageElement::containsTransparency(state);
    if (!transparent) {
      const SoDiffuseColorElement * diffelt =
        SoDiffuseColorElement::getInstance(state);
      if (diffelt->isPacked()) transparent = diffelt->hasPackedTransparency();
      else {
        const SoTransparencyElement * trans =
          SoTransparencyElement::getInstance(state);
        transparent = trans->getNum() > 1 ||
          trans->get(0) > 0.0f;
      }
    }
    // SoGLTextureImageElement is lazy, but needs some arguments.
    // Update manually
    const SoGLTextureImageElement * ti = (SoGLTextureImageElement *)
      state->getConstElement(SoGLTextureImageElement::getClassStackIndex());
    ti->evaluate(SoGLTextureEnabledElement::get(state),
                 transparent && !SoShapeStyleElement::isScreenDoor(state));
  }
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::getMatrix(SoGetMatrixAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::handleEvent(SoHandleEventAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::pick(SoPickAction * action)
{
  SoCallback::doAction(action);
}

// Doc from superclass.
void
SoCallback::search(SoSearchAction * action)
{
  SoCallback::doAction(action);
  SoNode::search(action);
}

// Doc from superclass.
void
SoCallback::write(SoWriteAction * action)
{
  SoCallback::doAction(action);
  SoNode::write(action);
}

// Doc from superclass.
void
SoCallback::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoCallback::doAction(action);
}

/*!
  Overloaded from parent class to also copy the callback function
  pointer and userdata.
 */
void
SoCallback::copyContents(const SoFieldContainer * from, SbBool copyconnections)
{
  inherited::copyContents(from, copyconnections);

  SoCallback * fromnode = (SoCallback *)from;
  this->cbfunc = fromnode->cbfunc;
  this->cbdata = fromnode->cbdata;
}
