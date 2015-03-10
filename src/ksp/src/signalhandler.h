#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

#include <stddef.h>     // defines NULL
#include <csignal>
#include <exception>

#include "vrp_assert.h"

class UserQuitException : public std::exception
{
private:
  const char *str;    ///< str Holds the what() string for the exception.

public:
  virtual const char *what () const throw() {
    return str;
  };

  UserQuitException( const char *_str ) : str( _str ) {};

};


class EventHandler
{
public:
  // Hook method for the signal hook method.
  virtual void handleSignal ( int signum ) = 0;

  // ... other hook methods for other types of
  // events such as timers, I/O, and
  // synchronization objects.
};


class SignalHandler
{
public:
  // Entry point.
  static SignalHandler *instance( void );

  // Register an event handler <eh> for <signum>
  // and return a pointer to any existing <EventHandler>
  // that was previously registered to handle <signum>.
  EventHandler *registerHandler( int signum, EventHandler *eh );

  // Remove the <EventHandler> for <signum>
  // by setting the slot in the <signalHandlers_>
  // table to NULL.
  void removeHandler( int signum );

private:
  // Ensure we're a Singleton.
  SignalHandler(void) {};

  // Singleton pointer.
  static SignalHandler *instance_;

  // Entry point adapter installed into <sigaction>
  // (must be a static method or a stand-alone
  // extern "C" function).
  static void dispatcher( int signum );

  // Table of pointers to concrete <EventHandler>s
  // registered by applications.  NSIG is the number of
  // signals defined in </usr/include/sys/signal.h>.
  static EventHandler *signalHandlers_[NSIG];
};


// ---------------------------------------------------------
// -- some concrete signal handlers
// ---------------------------------------------------------

class SIGINT_Handler : public EventHandler
{
public:
  SIGINT_Handler (void) : graceful_quit_ (0) {}

  // Hook method.
  virtual void handleSignal (int signum) {
    if (signum == SIGINT) this->graceful_quit_ = 1;
  }

  // Accessor.
  sig_atomic_t gracefulQuit (void) { return this->graceful_quit_; }

private:
  sig_atomic_t graceful_quit_;
};


class SIGQUIT_Handler : public EventHandler
{
public:
  SIGQUIT_Handler (void) : abortive_quit_ (0) {}

  // Hook method.
  virtual void handleSignal (int signum) {
    if (signum == SIGQUIT) this->abortive_quit_ = 1;
  }

  // Accessor.
  sig_atomic_t abortiveQuit (void) { return this->abortive_quit_; }

private:
  sig_atomic_t abortive_quit_;
};


#define REG_SIGINT SIGINT_Handler sigint_handler; \
    SignalHandler::instance()->registerHandler (SIGINT, &sigint_handler);

#define REG_SIGQUIT SIGQUIT_Handler sigquit_handler; \
    SignalHandler::instance()->registerHandler (SIGQUIT, &sigquit_handler);

#define THROW_ON_SIGINT do { \
    if ( sigint_handler.gracefulQuit() == 1 ) \
        throw( UserQuitException( "Abort on User Request!" ) ); \
    } while (0);

#endif

