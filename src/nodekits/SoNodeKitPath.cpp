/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoNodeKitPath SoNodeKitPath.h Inventor/SoNodeKitPath.h
  \brief The SoNodeKitPath class is a path that contains only nodekit nodes.
  \ingroup nodekits

  All other nodes are hidden from the user.
*/

/*�
  We now need a "friend class SoNodeKitPath;" in the SoPath
  definition -- could we do without it? That would clean up
  the implementation a bit.
 */

#include <Inventor/SoNodeKitPath.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/actions/SoSearchAction.h>
#include <stdlib.h>

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

SoSearchAction *SoNodeKitPath::searchAction;

/*!
  A constructor.
*/
SoNodeKitPath::SoNodeKitPath(const int approxLength)
  : SoPath(approxLength)
{
#if COIN_DEBUG && 1 // debug
  int n = this->nodes.getLength();
  for (int i = 0; i < n; i++) {
    if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId()))
      return;
  }
  SoDebugError::postInfo("SoNodeKitPath::SoNodeKitPath",
                         "no nodekits in path");
#endif // debug
}

/*!
  The destructor.
*/
SoNodeKitPath::~SoNodeKitPath()
{
}

/*!
  Returns the length of the path (the number of nodekit nodes).
*/
int
SoNodeKitPath::getLength(void) const
{
  int n = this->nodes.getLength();
  int cnt = 0;
  for (int i = 0; i < n; i++) {
    if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId())) cnt++;
  }
  return cnt;
}

/*!
  Returns the tail of the path (the last nodekit in the path).
*/
SoNode *
SoNodeKitPath::getTail(void) const
{
  for (int i = this->nodes.getLength()-1; i >= 0; i--) {
    if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId()))
      return this->nodes[i];
  }
#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoNodeKitPath::getTail",
                         "no nodekit in path");
#endif // debug
  return NULL;
}

/*!
  Returns nodekit number \a idx in path.
*/
SoNode *
SoNodeKitPath::getNode(const int idx) const
{
  int n = this->nodes.getLength();
  int cnt = 0;
  for (int i = 0; i < n; i++) {
    if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId())) {
      if (cnt++ == idx) return this->nodes[i];
    }
  }
#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoNodeKitPath::getNode",
                         "index %d out of bounds", idx);
#endif // debug

  return NULL;
}

/*!
  Returns nodekit number \a idx in the path, from the tail.
*/
SoNode *
SoNodeKitPath::getNodeFromTail(const int idx) const
{
  int cnt = 0;
  for (int i = this->nodes.getLength()-1; i >=0; i--) {
    if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId())) {
      if (cnt++ == idx) return this->nodes[i];
    }
  }
#if COIN_DEBUG && 1 // debug
  SoDebugError::postInfo("SoNodeKitPath::getNodeFromTail",
                         "index %d out of bounds", idx);
#endif // debug
  return NULL;
}

/*!
  Truncates the path at nodekit number \a length.
*/
void
SoNodeKitPath::truncate(const int length)
{
  int i, n = this->nodes.getLength();
  int cnt = 0;
  for (i = 0; i < n; i++) {
    if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId())) {
      if (cnt++ == length) break;
    }
  }
  if (i < n) SoPath::truncate(i);
#if COIN_DEBUG && 1 // debug
  else {
    SoDebugError::postInfo("SoNodeKitPath::truncate",
                           "illegal length: %d", length);
  }
#endif // debug
}

/*!
  Pops off the last nodekit (truncates at last tail).
*/
void
SoNodeKitPath::pop(void)
{
  int i = this->nodes.getLength() - 1;
  for (; i >= 0; i--) {
    if (this->nodes[i]->isOfType(SoBaseKit::getClassTypeId())) break;
  }
  if (i < 0) {
#if COIN_DEBUG && 1 // debug
    SoDebugError::postInfo("SoNodeKitPath::pop",
                           "no nodekits in path");
#endif // debug
    return;
  }
  SoPath::truncate(i);
}

/*!
  Appends \a childKit to the path. childKit should be a part in the
  tail nodekit of this path. In effect, the path from the tail to first
  occurrance of \a childKit will be appended to the path.
*/
void
SoNodeKitPath::append(SoBaseKit *childKit)
{
  if (this->getLength() == 0) this->setHead(childKit);
  else {
    SoBaseKit *tail = (SoBaseKit*) this->getTail();
    assert(tail != NULL);
    SoSearchAction *sa = this->getSearchAction();
    sa->setNode(childKit);
    SbBool oldSearch = tail->isSearchingChildren();
    tail->setSearchingChildren(TRUE);
    sa->apply(tail);
    tail->setSearchingChildren(oldSearch);
    SoPath *path = sa->getPath();
    if (path) SoPath::append(path);
    else {
#if COIN_DEBUG && 1 // debug
      SoDebugError::postInfo("SoNodeKitPath::append",
                             "childKit not found as part of tail");
#endif // debug
    }
  }
}

/*!
  Appends the nodekit path to this path. Head of \a fromPath must
  be a part in the current tail.
*/
void
SoNodeKitPath::append(const SoNodeKitPath *fromPath)
{
  int n = fromPath->getLength();
  for (int i = 0; i < n; i++) {
    this->append((SoBaseKit*)fromPath->getNode(i));
  }
}

/*!
  Returns \e TRUE if \a node is in this path.
*/
SbBool
SoNodeKitPath::containsNode(SoBaseKit *node) const
{
  return SoPath::containsNode((SoNode*)node);
}

/*!
  Returns the index of last common nodekit, or -1 if head
  node differs.
*/
int
SoNodeKitPath::findFork(const SoNodeKitPath *path) const
{
  int i, n = SbMin(this->getLength(), path->getLength());
  for (i = 0; i < n; i++) {
    if (this->getNode(i) != path->getNode(i)) break;
  }
  return i-1;
}

/*!
  Returns \e TRUE if paths are equal, \e FALSE otherwise.
*/
int
operator==(const SoNodeKitPath &p1, const SoNodeKitPath &p2)
{
  if (&p1 == &p2) return TRUE;
  int n = p1.getLength();
  if (n != p2.getLength()) return FALSE;

  for (int i = 0; i < n; i++) {
    if (p1.getNode(i) != p2.getNode(i)) return FALSE;
  }
  return TRUE;
}


//
// atexit() method
//
void
SoNodeKitPath::clean(void)
{
  delete SoNodeKitPath::searchAction;
  SoNodeKitPath::searchAction = NULL;
}

//
// returns a search action to be used while serching for nodes
//
SoSearchAction *
SoNodeKitPath::getSearchAction(void)
{
  if (SoNodeKitPath::searchAction == NULL) {
    SoNodeKitPath::searchAction = new SoSearchAction();
    searchAction->setInterest(SoSearchAction::FIRST);
    searchAction->setSearchingAll(FALSE);
    atexit(SoNodeKitPath::clean);
  }
  return SoNodeKitPath::searchAction;
}

//
// private methods, just to keep the user from making mistakes
//

void
SoNodeKitPath::append(const int)
{

}

void
SoNodeKitPath::append(SoNode *)
{
}

void
SoNodeKitPath::append(const SoPath *)
{
}

void
SoNodeKitPath::push(const int)
{
}

int
SoNodeKitPath::getIndex(const int) const
{
  return 0;
}

int
SoNodeKitPath::getIndexFromTail(const int) const
{
  return 0;
}

void
SoNodeKitPath::insertIndex(SoNode *, const int)
{
}

void
SoNodeKitPath::removeIndex(SoNode *,const int)
{
}

void
SoNodeKitPath::replaceIndex(SoNode *, const int, SoNode *)
{
}
