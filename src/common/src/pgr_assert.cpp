/*PGR-MIT******************************************************************
 *
 * file pgr_assert.cpp
 *
 * Copyright 2014 Stephen Woodbridge <woodbri@imaptools.com>
 * Copyright 2014 Vicky Vergara <vicky_vergara@hotmail.com>
 *
 * This is free software; you can redistribute and/or modify it under
 * the terms of the MIT License. Please file MIT-LICENSE for details.
 *
 *****************************************************************PGR-MIT*/
#include "./pgr_assert.h"
#include <exception>

#ifdef assert
#undef assert
#endif

#ifndef __STRING
#define __STRING(x) #x
#endif

#define __TOSTRING(x) __STRING(x)


const char *AssertFailedException::what() const throw() {
  return str;
}

AssertFailedException::AssertFailedException(const char *_str) :
    str(_str) {}


