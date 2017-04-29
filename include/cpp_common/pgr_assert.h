/*PGR-MIT******************************************************************
 *
 * file pgr_assert.h
 *
 * Copyright 2014 Stephen Woodbridge <woodbri@imaptools.com>
 * Copyright 2014 Vicky Vergara <vicky_vergara@hotmail.com>
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the MIT License. Please file MIT-LICENSE for details.
 *
 *****************************************************************PGR-MIT*/

#ifndef  INCLUDE_CPP_COMMON_PGR_ASSERT_H_
#define  INCLUDE_CPP_COMMON_PGR_ASSERT_H_
#pragma once

#include <string>
#include <exception>

#ifdef assert
#undef assert
#endif


/*! @file
 *
 * Assertions Handling
 *
 * @brief An assert functionality that uses C++ throw().
 *
 * This file provides an alternative to assert functionality that will
 * convert all pgassert() into C++ throw using an AssertFailedException class.
 *
 * This allows catching errors and do appropriate clean up
 * re-throw if needed to catch errors in the postgresql environment
 *
 * **Do not crash the backend server.**
 */


#ifndef __STRING
#define __STRING(x) #x
#endif

#define __TOSTRING(x) __STRING(x)


/*! @def pgassert(expr)
 * @brief Uses the standard assert syntax.
 *
 * When an assertion fails it will throw \ref AssertFailedException and what()
 * will return a string like "AssertFailedException(2+2 == 5) at t.cpp:11"
 *
 * Example:
 * @code
    #include <iostream>
    #include "pgr_assert.h"

    int main() {

        try {
            pgassert(2+2 == 4);
            pgassert(2+2 == 5);
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
    @endcode
 */
#ifdef NDEBUG
#define pgassert(expr) ((void)0)
#else
#define pgassert(expr) \
    ((expr) \
     ? static_cast<void>(0) \
     : throw AssertFailedException( \
         "AssertFailedException: " __STRING(expr) \
         " at " __FILE__ ":" __TOSTRING(__LINE__) + get_backtrace() ) )
#endif

/*! @def pgassertwm(expr, msg)
  @brief Adds a message to the assertion.

 Example:
 @code
    pgassert(2+2 == 5, "Expected a 4 as result");
    std::ostringstream log;
    log << "Expected a 4 as result"
    pgassert(2+2 == 5, log.str());
 @endcode
*/

#ifdef NDEBUG
#define pgassertwm(expr, msg) ((void)0)
#else
#define pgassertwm(expr, msg) \
    ((expr) \
     ? static_cast<void>(0) \
     : throw AssertFailedException( \
         "AssertFailedException: " __STRING(expr) \
         " at " __FILE__ ":" __TOSTRING(__LINE__) + get_backtrace(msg) ) )
#endif

/*! @brief returns the execution path of the trace

  In case of a failed exception the backtrace can be is shown in the error message.

  Does not work for windows, please read:
  http://stackoverflow.com/questions/27639931/can-not-find-execinfo-h-when-setup-malpack

 */
std::string get_backtrace();
std::string get_backtrace(const std::string &);

/*! @class AssertFailedException
 * @brief Extends std::exception and is the exception that we throw if an assert fails.
 */
class AssertFailedException : public std::exception {
 private:
     const std::string str;   ///< Holds what() we got as message

 public:
     virtual const char *what() const throw();
     explicit AssertFailedException(std::string msg);
     virtual ~AssertFailedException() throw() {}
};

#endif  //  INCLUDE_CPP_COMMON_PGR_ASSERT_H_
