/*PGR-GNU*****************************************************************

Copyright (c) 2015 pgRouting developers
Mail: project@pgrouting.org

Copyright (c) 2020 Mohamed Bakli, Esteban Zimányi, Mahmoud Sakr
mohamed_bakli@ulb.ac.be, estebanzimanyi@gmail.com, m_attia_sakr@yahoo.com

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

#ifndef INCLUDE_CPP_COMMON_INTERRUPTION_H_
#define INCLUDE_CPP_COMMON_INTERRUPTION_H_

#ifdef _MSC_VER
#define __PGR_PRETTY_FUNCTION__ __FUNCSIG__
#else
#define __PGR_PRETTY_FUNCTION__ __PRETTY_FUNCTION__
#endif

extern "C" {

#ifdef _MSC_VER

#include <postgres.h>
#include <miscadmin.h>

#else
    /*
     * Instead of including all the files
     * copy/paste what is used
     * IMPORTANT: this copy/paste might depend on the postgreSQL version
     */
#include <signal.h>

    /*
     * https://doxygen.postgresql.org/c_8h.html#a166c1d950e659804f0e3247aad99a81f
     */
#define PGDLLIMPORT

    /*
     * https://doxygen.postgresql.org/miscadmin_8h_source.html
     */
    extern PGDLLIMPORT volatile sig_atomic_t InterruptPending;
    extern void ProcessInterrupts(void);

#if __GNUC__ >= 3
#define unlikely(x) __builtin_expect((x) != 0, 0)
#else
#define unlikely(x) ((x) != 0)
#endif

    /* Test whether an interrupt is pending */
#if !defined(WIN32) || defined(__MINGW64_VERSION_MAJOR)
#define INTERRUPTS_PENDING_CONDITION() \
    (unlikely(InterruptPending))
#else
    extern void pgwin32_dispatch_queued_signals(void);
#define INTERRUPTS_PENDING_CONDITION() \
    (unlikely(UNBLOCKED_SIGNAL_QUEUE()) ? pgwin32_dispatch_queued_signals() : 0, \
     unlikely(InterruptPending))
#endif

    /* Service interrupt, if one is pending and it's safe to service it now */
#define CHECK_FOR_INTERRUPTS() \
    do { \
        if (INTERRUPTS_PENDING_CONDITION()) \
        ProcessInterrupts(); \
    } while (0)
#endif

}


#endif  // INCLUDE_CPP_COMMON_INTERRUPTION_H_
