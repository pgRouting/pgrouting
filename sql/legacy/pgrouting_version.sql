/*PGR-MIT*****************************************************************

FILE: pgrouting_version.sql

Copyright (c) 2013 pgRouting developers
Mail: project@pgrouting.org
Author: Stephen Woodbridge <woodbri@imaptools.com>

------
MIT/X license

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:


The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.


THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

********************************************************************PGR-MIT*/

--v3.0
CREATE FUNCTION pgr_version()
RETURNS TABLE(
        "version" varchar,
        tag varchar,
        hash varchar,
        branch varchar,
        boost varchar
    ) AS
$BODY$
    SELECT '${PROJECT_VERSION}'::varchar AS version,
        '${PROJECT_FULL_VERSION}'::varchar AS tag,
        '${PROJECT_GIT_HASH}'::varchar AS hash,
        '${PROJECT_GIT_BRANCH}'::varchar AS branch,
        '${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}.${Boost_SUBMINOR_VERSION}'::varchar AS boost;
$BODY$
LANGUAGE sql IMMUTABLE;

COMMENT ON FUNCTION pgr_version() IS
'pgr_version
- Documentation
  - ${PROJECT_DOC_LINK}/pgr_version.html
';
