/*VRP*********************************************************************
 *
 * vehicle routing problems
 *      A collection of C++ classes for developing VRP solutions
 *      and specific solutions developed using these classes.
 *
 * Copyright 2014 Stephen Woodbridge <woodbri@imaptools.com>
 * Copyright 2014 Vicky Vergara <vicky_vergara@hotmail.com>
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the MIT License. Please file LICENSE for details.
 *
 ********************************************************************VRP*/
/*! \file vrp_assert.h
 * \brief An assert functionality that uses C++ throw().
 *
 * This file provides an alternative to assert functionality that will
 * convert all assert() into C++ throw using an AssertFailedException class.
 * This allows us to catch these errors and do appropriate clean up and
 * re-throw if needed so we can catch errors in the postgresql environment
 * so we do not crash the backend server.
 */
#ifndef VRP_ASSERT_H
#define VRP_ASSERT_H

#include <exception>

#ifdef assert
#undef assert
#endif

#ifndef __STRING
#define __STRING(x) #x
#endif

#define __TOSTRING(x) __STRING(x)

/*! \def assert(expr)
 * \brief Uses the standard assert syntax.
 *
 * When an assertion fails it will throw \ref AssertFailedException and what()
 * will return a string like "AssertFailedException(2+2 == 5) at t.cpp:11"
 *
 * Here is an example of using it:
 * \code
    #include <iostream>
    #include "vrp_assert.h"

    int main() {

        try {
            assert(2+2 == 4);
            assert(2+2 == 5);
        }
        catch (AssertFailedException &e) {
            std::cout << e.what() << "\n";
        }
        catch (std::exception& e) {
            std::cout << e.what() << "\n";
        }
        catch(...) {
            std::cout << "Caught unknown exception!\n";
        }
        return 0;
    }
\endcode
 */
#define assert(expr) \
    ((expr) \
     ? static_cast<void>(0) \
     : throw AssertFailedException( "AssertFailedException: " __STRING(expr) " at " __FILE__ ":" __TOSTRING(__LINE__) ))


/*! \class AssertFailedException
 * \brief Extends std::exception and is the exception that we throw if an assert fails.
 */
class AssertFailedException : public std::exception
{
private:
  const char *str;    ///< str Holds the what() string for the exception.

public:
  virtual const char *what() const throw();
  AssertFailedException( const char *_str );
};

#endif
