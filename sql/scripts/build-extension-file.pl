#!/usr/bin/perl -w
=pod

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

=cut

eval 'exec /usr/bin/perl -S $0 ${1+"$@"}'
    if 0; #$running_under_some_shell

# Extension files almost do not change
# Almost files in the src/*/sql are part of the extension
# The files indicated on the CMakelists of the subdirectory:
# Are going to be included (when not commented out)
# Used on cmake

use strict;
use Data::Dumper;
use File::Find();

my $DEBUG="@PROJECT_DEBUG@";

use vars qw/*name *dir *prune/;
*name   = *File::Find::name;
*dir    = *File::Find::dir;
*prune  = *File::Find::prune;


sub Usage {
    die "Usage:\nFrom the root of the repository:
    build-extension-file.pl version \n";
}


my $version = "@PROJECT_VERSION@";
my $working_directory = "@CMAKE_CURRENT_BINARY_DIR@/..";
my $PROJECT_SQL_FILES =  "@PROJECT_SQL_FILES@";
my @sql_file = split(/;/, $PROJECT_SQL_FILES);
my $out_file_name = "$working_directory/@PROJECT_EXTENSION_FILE@";

print "Working directory $working_directory\n"      if $DEBUG;
print "Output file name $out_file_name\n"      if $DEBUG;

open(OUT, ">", "$out_file_name")
    || die "@PROJECT_EXTENSION_FILE@ ERROR: failed to create: '$out_file_name' : $!\n";

foreach my $f (@sql_file) {
    print "--  $f\n" if $DEBUG;
    my $contents = get_contents($f);
    $contents = eliminate_license($contents);
    print OUT "$contents";
}

close (OUT);

exit 0;



sub get_contents {
    my ($file) = @_;
    local $/=undef;
    die "ERROR: Failed to find: $file\n" unless -f $file;
    open(IN, $file) || die "pgrouting--$version.sql ERROR: Failed to open $file\n";
    my @contents = <IN>;
    close(IN);
    my $contents = join('', @contents);

    return $contents;
}

sub eliminate_license {
    my ($contents) = @_;

    $contents =~ s{
        /\*PGR-(GNU|MIT)
        .*?
        PGR-(GNU|MIT)\*/
    }[]gsxi;

    $contents =~ s{
        /\*FOR
        .*?
        USER\*/
    }[]gsxi;

    $contents =~ s{
        /\*
        .*?
        \*/
    }[]gsxi;



    return $contents;
}

