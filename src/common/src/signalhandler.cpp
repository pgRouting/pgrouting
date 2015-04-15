
#include "signalhandler.h"

SignalHandler *
SignalHandler::instance()
{
  if ( not instance_ ) instance_ = new SignalHandler;

  assert( instance_ != NULL );
  return instance_;
}


EventHandler *
SignalHandler::registerHandler ( int signum, EventHandler *eh )
{
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
  sigemptyset (&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction (signum, &sa, 0);

  return old_eh;
}

void SignalHandler::removeHandler( int signum )
{
  // EventHandler *old_eh = signalHandlers_[signum];
  signalHandlers_[signum] = static_cast<EventHandler *>(0);
}


void SignalHandler::dispatcher ( int signum )
{
  // Perform a sanity check...
  if (SignalHandler::signalHandlers_[signum] != 0)
    // Dispatch the handler's hook method.
    SignalHandler::signalHandlers_ [signum]->handleSignal (signum);
}

// these allocate actual storage
SignalHandler *SignalHandler::instance_ = NULL;
EventHandler *SignalHandler::signalHandlers_[NSIG];

