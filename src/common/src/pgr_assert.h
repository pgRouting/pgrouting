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

#ifndef  SRC_COMMON_SRC_PGR_ASSERT_H_
#define  SRC_COMMON_SRC_PGR_ASSERT_H_
#ifdef __MINGW32__
#include <winsock2.h>
#include <windows.h>
#endif


#include <string>
#include <exception>

#ifdef pgassert
#undef pgassert
#endif


/*! \file vrp_assert.h
 * **Do not crash the backend server.**
 *
 * \brief An assert functionality that uses C++ throw().
 *
 * This file provides an alternative to assert functionality that will
 * convert all assert() into C++ throw using an AssertFailedException class.
 * This allows catching errors and do appropriate clean up 
 * re-throw if needed to catch errors in the postgresql environment
 *
 * **Do not crash the backend server.**
 */

#ifndef __STRING
#define __STRING(x) #x
#endif

#define __TOSTRING(x) __STRING(x)

/*! \def pgassert(expr)
 * \brief Uses the standard assert syntax.
 *
 * When an assertion fails it will throw \ref AssertFailedException and what()
 * will return a string like "AssertFailedException(2+2 == 5) at t.cpp:11"
 *
 * Here is an example of using it:
 * \code
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
\endcode
 */
#ifdef NDEBUG
#define pgassert(expr) ((void)0)
#else
#define pgassert(expr) \
    ((expr) \
     ? static_cast<void>(0) \
     : throw AssertFailedException("AssertFailedException: " __STRING(expr) " at " __FILE__ ":" __TOSTRING(__LINE__) + get_backtrace() ) )  
#endif

/*! @brief returns the execution path of the trace
 
  Does not work for windows, please read:
  http://stackoverflow.com/questions/27639931/can-not-find-execinfo-h-when-setup-malpack
 
  In case of a failed exception the backtrace cann be shoun in the error message
 */
std::string get_backtrace();

/*! \class AssertFailedException
 * \brief Extends std::exception and is the exception that we throw if an assert fails.
 */
class AssertFailedException : public std::exception {
    private:
        const std::string str;   ///< str Holds what() we got as message from the #define

    public:
        virtual const char *what() const throw();
        explicit AssertFailedException(std::string msg);
        virtual ~AssertFailedException() throw() {};
};

#endif  //  SRC_COMMON_SRC_PGR_ASSERT_H_
