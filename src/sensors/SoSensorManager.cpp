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

/*!
  \class SoSensorManager SoSensorManager.h Inventor/sensors/SoSensorManager.h
  \brief The SoSensorManager class handles the sensor queues.
  \ingroup sensors

  There are two major sensor types in Coin: delay-sensors and
  timer-sensors. Each of these two types has its own queue, which is
  handled by the SoSensorManager. The queues are kept in sorted order by
  SoSensorManager, either according to trigger-time (for timer-sensors)
  or by priority (for delay-sensors).

  Delay-sensors trigger when the application is otherwise idle. In
  addition, to avoid starvation in applications that are continually
  busy, the delay-sensor queue also has a timeout which, when reached,
  will empty the queue anyhow.

  Timer-sensors are set up to trigger at specific, abolute times.


  FIXME: doc

  ..provides methods for inserting, removing, processing (emptying)
  queues of sensors..

  ..the sensor mechanism is crucial in Coin for (automatic redrawal upon
  changes,)..

  ..should usually be considered as an internal class in the Coin
  system, only interesting for "normal users" when implementing new
  windowsystem-specific libraries (like SoQt,) ... which usually goes
  like this: (register change callback -- drop in delay queue sensor
  of type XXX for redraw queueing -- call process*Queue when... --
  ...) ... confer the SoQt sourcecode to see a complete example of use
  (list interesting files)...

  \sa SoSensor SoTimerQueueSensor SoDelayQueueSensor
  \sa SoTimerSensor SoAlarmSensor
  \sa SoIdleSensor SoDataSensor SoOneShotSensor
  \sa SoPathSensor SoFieldSensor SoNodeSensor
*/


#include <Inventor/sensors/SoSensorManager.h>
#include <Inventor/sensors/SoDelayQueueSensor.h>
#include <Inventor/sensors/SoTimerSensor.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SbTime.h>
#include <coindefs.h> // COIN_STUB()

#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG

#include <assert.h>

// FIXME: find fd_set definition properly through something configure
// based.  19991214 mortene.  (Note: fd_set is in time.h under AIX?)
#if HAVE_UNISTD_H
#include <unistd.h> // fd_set (?)
#endif // HAVE_UNISTD_H


// Keep these around. Even though the SoSensorManager code seems to be
// working as it should now, a lot of other stuff around in the Coin
// library depends on getting the "local" sensor handling correct, and
// enabling the debuginfo in this class can help immensely. --mortene.
#define DEBUG_DELAY_SENSORHANDLING 0
#define DEBUG_TIMER_SENSORHANDLING 0

#ifndef DOXYGEN_SKIP_THIS

class SoSensorManagerP {
public:

  SbBool processingtimerqueue, processingdelayqueue;
  SbBool processingimmediatequeue;

  // immediatequeue - stores SoDelayQueueSensors with priority 0. FIFO.
  // reinsertlist - temporary storage for idle sensors during processing
  // delayqueue   - stores SoDelayQueueSensor's in sorted order.
  // timerqueue - stores SoTimerSensors in sorted order.
  // triggerlist - stores sensors that have been triggered in processDelayQueue().

  // FIXME: replace reinsertlist and triggerlist by SbDict instances
  // as soon as the SbDict has been reimplemented to avoid continuous
  // memory allocation/deallocation.  pederb, 2002-01-29
  SbList <SoDelayQueueSensor *> immediatequeue;
  SbList <SoDelayQueueSensor *> reinsertlist;
  SbList <SoDelayQueueSensor *> delayqueue;
  SbList <SoTimerQueueSensor *> timerqueue;
  SbList <SoDelayQueueSensor *> triggerlist;
  SbList <SoTimerSensor*> reschedulelist;

  void (*queueChangedCB)(void *);
  void * queueChangedCBData;

  SbTime delaysensortimeout;
};

#endif // DOXYGEN_SKIP_THIS 

#undef THIS
#define THIS this->pimpl

/*!
  Constructor.
 */
SoSensorManager::SoSensorManager(void)
{
  THIS = new SoSensorManagerP;

  THIS->queueChangedCB = NULL;
  THIS->queueChangedCBData = NULL;

  THIS->processingtimerqueue = FALSE;
  THIS->processingdelayqueue = FALSE;
  THIS->processingimmediatequeue = FALSE;

  THIS->delaysensortimeout.setValue(1.0/12.0);
}

/*!
  Destructor.
 */
SoSensorManager::~SoSensorManager()
{
  // FIXME: remove entries. 19990225 mortene.
  if(THIS->delayqueue.getLength() != 0) {}
  if(THIS->timerqueue.getLength() != 0) {}

  delete THIS;
}

/*!
  Add a new entry to the queue of delay sensors.

  \sa removeFromQueue()
 */
void
SoSensorManager::insertDelaySensor(SoDelayQueueSensor * newentry)
{
  assert(newentry);

  // immediate sensors are stored in a separate list. We don't need to
  // sort them based on SoSensor::isBefore(), but just use a FIFO
  // strategy.
  if (newentry->getPriority() == 0) {
    THIS->immediatequeue.append(newentry);
  }
  else {
    SbList <SoDelayQueueSensor *> & delayqueue = THIS->delayqueue;
    
    int pos = 0;
    while((pos < delayqueue.getLength()) &&
          ((SoSensor*)delayqueue[pos])->isBefore(newentry)) {
      pos++;
    }
    delayqueue.insert(newentry, pos);
    this->notifyChanged();
  }
#if DEBUG_DELAY_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::insertDelaySensor",
                         "inserted delay sensor #%d -- %p -- "
                         "%sprocessing queue",
                         THIS->delayqueue.getLength() +
                         THIS->delaywaitqueue.getLength() - 1,
                         newentry,
                         THIS->processingdelayqueue ? "" : "not ");
#endif // debug
}

/*!
  Add a new entry to the timer queue of sensors. The queue will be sorted in
  order of supposed trigger time.

  \sa removeFromQueue()
 */
void
SoSensorManager::insertTimerSensor(SoTimerQueueSensor * newentry)
{
  assert(newentry);

  SbList <SoTimerQueueSensor *> & timerqueue = THIS->timerqueue;
  int i = 0;
  while (i < timerqueue.getLength() &&
         ((SoSensor*)timerqueue[i])->isBefore(newentry)) {
    i++;
  }
  timerqueue.insert(newentry, i);
  
#if DEBUG_TIMER_SENSORHANDLING || 0 // debug
  SoDebugError::postInfo("SoSensorManager::insertTimerSensor",
                         "inserted timer sensor #%d -- %p "
                         "(triggertime %f) -- "
                         "%sprocessing queue",
                         THIS->timerqueue.getLength() +
                         THIS->timerwaitqueue.getLength() - 1,
                         newentry, newentry->getTriggerTime().getValue(),
                         THIS->processingtimerqueue ? "" : "not ");
#endif // debug

  if (!THIS->processingtimerqueue) {
    this->notifyChanged();
  }
}

/*!
  Remove an entry from the sensor queue.

  \sa addToQueue()
 */
void
SoSensorManager::removeDelaySensor(SoDelayQueueSensor * entry)
{
  // Check "real" queue first..
  int idx = THIS->delayqueue.find(entry);
  if (idx != -1) THIS->delayqueue.remove(idx);

  // ..then the immediate queue.
  if (idx == -1) {
    idx = THIS->immediatequeue.find(entry);
    if (idx != -1) THIS->immediatequeue.remove(idx);
  }
  // ..then the reinsert list
  if (idx == -1) {
    idx = THIS->reinsertlist.find(entry);
    if (idx != -1) THIS->reinsertlist.remove(idx);
  }
  
  if (idx != -1) this->notifyChanged();

#if COIN_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoSensorManager::removeDelaySensor",
                              "trying to remove element not in list");
  }
#endif // COIN_DEBUG
}

/*!
  \overload
 */
void
SoSensorManager::removeTimerSensor(SoTimerQueueSensor * entry)
{
  int idx = THIS->timerqueue.find(entry);
  if (idx != -1) THIS->timerqueue.remove(idx);

  if (idx != -1) this->notifyChanged();

#if COIN_DEBUG
  if (idx == -1) {
    SoDebugError::postWarning("SoSensorManager::removeTimerSensor",
                              "trying to remove element not in list");
  }
#endif // COIN_DEBUG
}

/*!
  Trigger all the timers which has expired.
 */
void
SoSensorManager::processTimerQueue(void)
{
  if (THIS->processingtimerqueue || THIS->timerqueue.getLength() == 0)
    return;

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "start: %d elements", THIS->timerqueue.getLength());
#endif // debug

  assert(THIS->reschedulelist.getLength() == 0);
  THIS->processingtimerqueue = TRUE;

  SbTime currenttime = SbTime::getTimeOfDay();
  while (THIS->timerqueue.getLength() > 0 &&
         THIS->timerqueue[0]->getTriggerTime() <= currenttime) {
#if DEBUG_TIMER_SENSORHANDLING // debug
    SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                           "process element with triggertime %s",
                           THIS->timerqueue[0]->getTriggerTime().format().getString());
#endif // debug
    SoSensor * sensor = THIS->timerqueue[0];
    THIS->timerqueue.remove(0);
    sensor->trigger();
  }

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "end, before merge: %d elements",
                         THIS->timerqueue.getLength());
#endif // debug

  int n = THIS->reschedulelist.getLength();
  if (n) {
    SbTime time = SbTime::getTimeOfDay();
    for (int i = 0; i < n; i++) {
      THIS->reschedulelist[i]->reschedule(time);
    }
    THIS->reschedulelist.truncate(0);
  }

  THIS->processingtimerqueue = FALSE;

#if DEBUG_TIMER_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processTimerQueue",
                         "end, after merge: %d elements",
                         THIS->timerqueue.getLength());
#endif // debug
}

/*!
  Trigger all delay queue entries in priority order.

  The \a isidle flag indicates whether or not the processing happens
  because the application is idle or because the delay queue timeout
  was reached.

  A delay queue sensor with priority > 0 can only be triggered once
  during a call to this function. If a delay sensor is rescheduled
  during processDelayQueue(), it is not processed until the next time
  this function is called. This is done to avoid an infinite loop
  while processing the sensors.

  A delay queue sensor with priority 0 is called an immediate sensor.

  \sa SoDB::setDelaySensorTimeout() 
  \sa SoSensorManager::processImmediateQueue() 
*/
void
SoSensorManager::processDelayQueue(SbBool isidle)
{
  this->processImmediateQueue();

  if (THIS->processingdelayqueue || THIS->delayqueue.getLength() == 0)
    return;

#if DEBUG_DELAY_SENSORHANDLING // debug
  SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                         "start: %d elements", THIS->delayqueue.getLength());
#endif // debug

  THIS->processingdelayqueue = TRUE;

  // triggerlist is used to store sensors that has already been
  // triggered.  a sensor should only be triggered once during a call
  // to processDelayQueue(), otherwise we might risk never returning
  // from this function. E.g. SoSceneManager::scheduleRedraw()
  // triggers a delay sensor, which again triggers a redraw. During
  // the redraw, SoSceneManager::scheduleRedraw() might be called
  // again, etc...
  THIS->triggerlist.truncate(0);

  // Sensors with higher priorities are triggered first.
  while (THIS->delayqueue.getLength()) {
#if DEBUG_DELAY_SENSORHANDLING // debug
    SoDebugError::postInfo("SoSensorManager::processDelayQueue",
                           "treat element with pri %d",
                           THIS->delayqueue[0]->getPriority());
#endif // debug

    SoDelayQueueSensor * sensor = THIS->delayqueue[0];
    THIS->delayqueue.remove(0);
    if (!isidle && sensor->isIdleOnly()) {
      // move sensor to another temporary list. It will be reinserted
      // at the end of this function. We do this to be able to always
      // remove the first list element. We avoid searching for the
      // first non-idle sensor.
      THIS->reinsertlist.append(sensor);
    }
    else {
      // only trigger sensor once per processing loop
      if (THIS->triggerlist.find(sensor) < 0) {
        sensor->trigger();
        THIS->triggerlist.append(sensor);
      }
      else {
        // Reuse the "reinsert" list to store the sensor. It will be
        // reinserted at the end of this function.
        if (THIS->reinsertlist.find(sensor) < 0) {
          THIS->reinsertlist.append(sensor);
        }
      }
    }
  }
  // reinsert sensors that couldn't be triggered, either because it
  // was an idle sensor, or because the sensor had already been
  // triggered
  for (int i = 0; i < THIS->reinsertlist.getLength(); i++) {
    this->insertDelaySensor(THIS->reinsertlist[i]);
  }
  THIS->reinsertlist.truncate(0);
  THIS->triggerlist.truncate(0);
  THIS->processingdelayqueue = FALSE;
}

/*!
  Process all immediate sensors (delay sensors with priority 0).

  Be aware that you might risk an infinite loop using immediate
  sensors. Unlike delay queue sensors, immediate sensors can be
  rescheduled and triggered multiple times during immediate queue
  processing.
  
  \sa SoDelayQueueSensor::setPriority() */
void
SoSensorManager::processImmediateQueue(void)
{
  if (THIS->processingimmediatequeue) return;

#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
  SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                         "start: %d elements in full immediate queue",
                         THIS->immediatequeue.getLength());
#endif // debug

  THIS->processingimmediatequeue = TRUE;

  int triggercnt = 0;

  while (THIS->immediatequeue.getLength()) {
#if DEBUG_DELAY_SENSORHANDLING || 0 // debug
    SoDebugError::postInfo("SoSensorManager::processImmediateQueue",
                           "trigger element");
#endif // debug
    SoSensor * sensor = THIS->immediatequeue[0];
    THIS->immediatequeue.remove(0);
    sensor->trigger();
    triggercnt++;
    if (triggercnt > 10000) break;
  }
  if (THIS->immediatequeue.getLength()) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoSensorManager::processImmediateQueue",
                              "Infinite loop detected. Breaking out.");
#endif // COIN_DEBUG
  }
  THIS->processingimmediatequeue = FALSE;
}

/*!
  FIXME: write doc
*/
void
SoSensorManager::rescheduleTimer(SoTimerSensor * s)
{
  THIS->reschedulelist.append(s);
}

/*!
  FIXME: write doc
*/
void
SoSensorManager::removeRescheduledTimer(SoTimerQueueSensor * s)
{
  int idx = THIS->reschedulelist.find((SoTimerSensor*)s);
  if (idx >= 0) {
    THIS->reschedulelist.remove(idx);
  }
  else {
    this->removeTimerSensor(s);
  }
}

/*!
  FIXME: write doc
*/
SbBool
SoSensorManager::isDelaySensorPending(void)
{
  return (THIS->delayqueue.getLength() || 
          THIS->immediatequeue.getLength()) ? TRUE : FALSE;
}

/*!
  Returns \c TRUE if at least one timer sensor is present in the
  queue, otherwise \c FALSE.

  If sensors are pending, the time interval until the next one should
  be triggered will be put in the \a tm variable.
*/
SbBool
SoSensorManager::isTimerSensorPending(SbTime & tm)
{
  if (THIS->timerqueue.getLength() > 0) {
    tm = THIS->timerqueue[0]->getTriggerTime();
    return TRUE;
  }

  return FALSE;
}

/*!
  Delay sensors are usually triggered only when the system is
  idle. But when there are continuous updates to the scene graph,
  there's a possibility that the delay queue will starve and sensors
  are never triggered. To make sure this won't happen, this is a
  timeout value for the delay queue. When this timeout has been reached,
  the sensors in the delay queue gets processed before other sensors and
  events. This method will let the user set this timeout value.

  The default value is 1/12 of a second.

  \sa getDelaySensorTimeout(), SoDelayQueueSensor
*/
void
SoSensorManager::setDelaySensorTimeout(const SbTime & t)
{
#if COIN_DEBUG
  if(t < SbTime(0.0)) {
    SoDebugError::postWarning("SoDB::setDelaySensorTimeout",
                              "Tried to set negative interval.");
    return;
  }
#endif // COIN_DEBUG

  THIS->delaysensortimeout = t;
}

/*!
  Returns the timeout value for sensors in the delay queue.

  \sa setDelaySensorTimeout(), SoDelayQueueSensor
 */
const SbTime &
SoSensorManager::getDelaySensorTimeout(void)
{
  return THIS->delaysensortimeout;
}

/*!
  FIXME: write doc
*/
void
SoSensorManager::setChangedCallback(void (*func)(void *), void * data)
{
  THIS->queueChangedCB = func;
  THIS->queueChangedCBData = data;
}

void
SoSensorManager::notifyChanged(void)
{
  if (THIS->queueChangedCB &&
      !THIS->processingtimerqueue &&
      !THIS->processingdelayqueue &&
      !THIS->processingimmediatequeue) {
    THIS->queueChangedCB(THIS->queueChangedCBData);
  }
}

/*!
  This is a wrapper around the standard select(2) call, which will
  make sure the sensor queues are updated while waiting for any action
  to happen on the given file descriptors.

  The void* arguments must be valid pointers to fd_set
  structures. We've changed this from the original SGI Inventor API to
  avoid messing up the header file with system-specific includes.
*/
int
SoSensorManager::doSelect(int nfds, void * readfds, void * writefds,
                          void * exceptfds, struct timeval * usertimeout)
{
  // FIXME: implement. See SoDB::doSelect() (which should probably only
  // be a wrapper around this call). 19990425 mortene.
  COIN_STUB();
  return 0;
}

int 
SoSensorManager::mergeTimerQueues(void)
{
  assert(0 && "obsoleted");
}

int 
SoSensorManager::mergeDelayQueues(void)
{
  assert(0 && "obsoleted");
}


#undef THIS
