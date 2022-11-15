#!/usr/bin/perl -w
=pod
 /*PGR-GNU*****************************************************************
 File: notes2news.pl
 Copyright (c) 2017 pgRouting developers
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
=cut
use strict;

sub Usage {
  die "Usage: notes2news.pl (from the root of the repository or pre-commit hook)\n";
}


my $in_file = "doc/src/release_notes.rst";
my $out_file = "NEWS";

my $ofh;
open($ofh, ">$out_file") || die "ERROR: failed to open '$out_file' for write! : $!\n";

my $ifh;
open($ifh, "$in_file") || die "ERROR: failed to open '$in_file' for read! : $!\n";

my $skipping = 1;
while (my $line = <$ifh>) {
  next if $skipping and $line !~ /^pgRouting/;
  $skipping = 0;

  # convert urls to markdown
  $line =~ s/`([^<]+?)\s*<([^>]+)>`__/\[$1\]($2)/g;
  $line =~ s/`([^<]+?)\s*<([^>]+)>`_/\[$1\]($2)/g;

  # convert rubric to bold
  $line =~ s/^\.\. rubric::\s*(.+)$/**$1**/;

  next if $line =~ /^\.\. _changelog/;

  print $ofh $line;
}

close($ifh);
close($ofh);

