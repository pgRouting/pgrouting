/*PGR-GNU*****************************************************************

Copyright (c) 2019 pgRouting developers
Mail: project@pgrouting.org

Developer:
Copyright (c) 2019 Celia Virginia Vergara Castillo

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

CREATE OR REPLACE FUNCTION _pgr_boost_version()
RETURNS TEXT AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_boost_version() IS
'pgRouting internal function';

---

CREATE OR REPLACE FUNCTION _pgr_build_type()
RETURNS TEXT AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_build_type() IS
'pgRouting internal function';

---

CREATE OR REPLACE FUNCTION _pgr_compilation_date()
RETURNS TEXT AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_compilation_date() IS
'pgRouting internal function';

---

CREATE OR REPLACE FUNCTION _pgr_compiler_version()
RETURNS TEXT AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_compiler_version() IS
'pgRouting internal function';

---

CREATE OR REPLACE FUNCTION _pgr_git_hash()
RETURNS TEXT AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_compilation_date() IS
'pgRouting internal function';

---

CREATE OR REPLACE FUNCTION _pgr_lib_version()
RETURNS TEXT AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_lib_version() IS
'pgRouting internal function';


---

CREATE OR REPLACE FUNCTION _pgr_operating_system()
RETURNS TEXT AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_operating_system() IS
'pgRouting internal function';
---

CREATE OR REPLACE FUNCTION _pgr_pgsql_version()
RETURNS TEXT AS
'MODULE_PATHNAME'
LANGUAGE C VOLATILE STRICT;

COMMENT ON FUNCTION _pgr_pgsql_version() IS
'pgRouting internal function';

