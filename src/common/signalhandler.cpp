/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

------

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 ********************************************************************PGR-GNU*/
/*PGR-MIT******************************************************************
 *
 * file signalhandler.cpp
 *
 * Copyright 2014 Stephen Woodbridge <woodbri@imaptools.com>
 * Copyright 2014 Vicky Vergara <vicky_vergara@hotmail.com>
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the MIT License. Please file MIT-LICENSE for details.
 *
 *****************************************************************PGR-MIT*/

#include "cpp_common/signalhandler.h"

SignalHandler *
SignalHandler::instance() {
  if (!instance_) instance_ = new SignalHandler;

  // assert(instance_ != NULL);
  return instance_;
}


EventHandler *
SignalHandler::registerHandler(int signum, EventHandler *eh) {
  // Copy the <old_eh> from the <signum> slot in
  // the <signalHandlers_> table.
  EventHandler *old_eh = signalHandlers_[signum];

  // Store <eh> into the <signum> slot in the
  // <signalHandlers_> table.
  SignalHandler::signalHandlers_[signum] = eh;

  // Register the <dispatcher> to handle this
  // <signum>.
  struct sigaction sa;
  sa.sa_handler = SignalHandler::dispatcher;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(signum, &sa, 0);

  return old_eh;
}

void SignalHandler::removeHandler(int signum) {
  // EventHandler *old_eh = signalHandlers_[signum];
  signalHandlers_[signum] = static_cast<EventHandler *>(0);
}


void SignalHandler::dispatcher(int signum) {
  // Perform a sanity check...
  if (SignalHandler::signalHandlers_[signum] != 0)
    // Dispatch the handler's hook method.
    SignalHandler::signalHandlers_[signum]->handleSignal(signum);
}

// these allocate actual storage
SignalHandler *SignalHandler::instance_ = NULL;
EventHandler *SignalHandler::signalHandlers_[NSIG];

