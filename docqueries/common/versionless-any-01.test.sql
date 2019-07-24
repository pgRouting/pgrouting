/*PGR-GNU*****************************************************************

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
-- these should all be true
select _pgr_versionless('2.1.0foobar23', '2.1');
select _pgr_versionless('2.1.0foobar23', '2.1-rc1');
select _pgr_versionless('2.1.0foobar23', '2.1-beta');
select _pgr_versionless('2.1.0foobar23', '2.1-alpha');
select _pgr_versionless('2.1.0foobar23', '2.1-dev');
select _pgr_versionless('2.1.0foobar23', '2.1rc1');
select _pgr_versionless('2.1.0foobar23', '2.1beta');
select _pgr_versionless('2.1.0foobar23', '2.1alpha');
select _pgr_versionless('2.1.0foobar23', '2.1dev');
select _pgr_versionless('2.1.0-dev', '2.1');
select _pgr_versionless('2.1.0-dev', '2.1-rc1');
select _pgr_versionless('2.1.0-dev', '2.1-beta');
select _pgr_versionless('2.1.0-dev', '2.1-alpha');
select _pgr_versionless('2.1.0-dev', '2.1rc1');
select _pgr_versionless('2.1.0-dev', '2.1beta');
select _pgr_versionless('2.1.0-dev', '2.1alpha');
select _pgr_versionless('2.1.0-alpha', '2.1');
select _pgr_versionless('2.1.0-alpha', '2.1-rc1');
select _pgr_versionless('2.1.0-alpha', '2.1-beta');
select _pgr_versionless('2.1.0-alpha', '2.1rc1');
select _pgr_versionless('2.1.0-alpha', '2.1beta');
select _pgr_versionless('2.1.0-beta', '2.1');
select _pgr_versionless('2.1.0-beta', '2.1-rc1');
select _pgr_versionless('2.1.0-beta', '2.1rc1');
select _pgr_versionless('2.0.0', '2.1');
select _pgr_versionless('2.1', '2.1.3');
select _pgr_versionless('1.05', '2.0.0');
select _pgr_versionless('1.05', '2.0');
select _pgr_versionless('1.05', '2');
-- these should all be false
select _pgr_versionless('2.1', '2.1.0foobar23');
select _pgr_versionless('2.1-rc1', '2.1.0foobar23');
select _pgr_versionless('2.1-beta', '2.1.0foobar23');
select _pgr_versionless('2.1-alpha', '2.1.0foobar23');
select _pgr_versionless('2.1-dev', '2.1.0foobar23');
select _pgr_versionless('2.1rc1', '2.1.0foobar23');
select _pgr_versionless('2.1beta', '2.1.0foobar23');
select _pgr_versionless('2.1alpha', '2.1.0foobar23');
select _pgr_versionless('2.1dev', '2.1.0foobar23');
select _pgr_versionless('2.1', '2.1.0-dev');
select _pgr_versionless('2.1-rc1', '2.1.0-dev');
select _pgr_versionless('2.1-beta', '2.1.0-dev');
select _pgr_versionless('2.1-alpha', '2.1.0-dev');
select _pgr_versionless('2.1rc1', '2.1.0-dev');
select _pgr_versionless('2.1beta', '2.1.0-dev');
select _pgr_versionless('2.1alpha', '2.1.0-dev');
select _pgr_versionless('2.1', '2.1.0-alpha');
select _pgr_versionless('2.1-rc1', '2.1.0-alpha');
select _pgr_versionless('2.1-beta', '2.1.0-alpha');
select _pgr_versionless('2.1rc1', '2.1.0-alpha');
select _pgr_versionless('2.1beta', '2.1.0-alpha');
select _pgr_versionless('2.1', '2.1.0-beta');
select _pgr_versionless('2.1-rc1', '2.1.0-beta');
select _pgr_versionless('2.1rc1', '2.1.0-beta');
select _pgr_versionless('2.1', '2.0.0');
select _pgr_versionless('2.1.3', '2.1');
select _pgr_versionless('2.0.0', '1.05');
select _pgr_versionless('2.0', '1.05');
select _pgr_versionless('2', '1.05');
select _pgr_versionless('2', '2.0.0foobar');
select _pgr_versionless('2.1', '2.1.0');
select _pgr_versionless('2.1.0beta3dev', '2.1.0beta1dev');
