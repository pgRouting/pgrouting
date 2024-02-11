/*PGR-GNU*****************************************************************

FILE: initials_code.h

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

/*! @file */

#ifndef INCLUDE_VRP_INITIALS_CODE_H_
#define INCLUDE_VRP_INITIALS_CODE_H_
#pragma once

namespace pgrouting {
namespace vrp {

/*! Different kinds to insert an order into the vehicle */
enum Initials_code {
    OneTruck,    /*! All orders in one truck */
    OnePerTruck, /*! One Order per truck */
    FrontTruck,  /*! Insetion at the front of the truck */
    BackTruck,   /*! Insetion at the back of the truck */
    BestInsert,  /*! Best place to insert Order */
    BestBack,    /*! Push back order that allows more orders to be inserted at the back */
    BestFront,   /*! Push front order that allows more orders to be inserted at the front */
    OneDepot     /*! Pick at front, drop at back, OneDepot for all vehicles */
};

}  // namespace vrp
}  // namespace pgrouting

#endif  // INCLUDE_VRP_INITIALS_CODE_H_
