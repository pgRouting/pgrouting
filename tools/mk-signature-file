#!/usr/bin/perl -w
eval 'exec /usr/bin/perl -S $0 ${1+"$@"}'
    if 0; #$running_under_some_shell

# -------------------------------------------------------------
# Usage: mk-signature-file <version>
#
# Author: Stephen Woodbridge
# Date: 2015-07-25
# License: MIT-X
# -------------------------------------------------------------
# Description
#
# This script read a file from lib/pgrouting--<version>.sql
# that should be the sql for the postgresql extension.
# It parses this file and extracts the signatures for the TYPES and
# FUNCTIONS and writes them to the output file
# lib/pgrouting--<version>.sig
#
# This is part of the automated pgRouting extension update
# system that PostgreSQL require a collection of files that define
# how to update an old version of pgRouting to the current version
# using "ALTER EXTENSION pgrouting UPDATE TO <version>;"
# The lib/pgrouting--<current_version>.sig and the update scripts
# are automatically generated when when cmake is run. You don't
# need to run this script manually.
#
# See also tools/build-extension-update-files
# ------------------------------------------------------------

use strict;
use warnings;

use Data::Dumper;
use Cwd qw(cwd chdir);

my $DEBUG = 0;

sub Usage {
    die "Usage: mk-signature-file <version>\n";
}

# get the version from the commandline
# cmake provides the version when it is run
my $version = shift @ARGV || Usage();

# generate the input and output file names
my $sql = "lib/pgrouting--$version.sql";
my $sig = "lib/pgrouting--$version.sig";

die "ERROR: File '$sql' does not exist!\n"
    unless -f $sql;

# read the inpput file and parse it into signatures
# for both types and functions
my ($types, $funcs) = parse_signatures( $sql );

# create and write the output file
open(SIG, ">$sig") || die "ERROR: Failed to create '$sig' : $!\n";
print SIG "#VERSION pgrouting $version\n";
print SIG "#TYPES\n";
print SIG join("\n", @{$types}), "\n";
print SIG "#FUNCTIONS\n";
print SIG join("\n", @{$funcs}), "\n";
close(SIG);

exit 0;

sub writeout {
    my ($file, $data) = @_;
    open(TMPOUT, ">$file") || die "ERROR: Failed to create '$file' : $!\n";
    print TMPOUT $data;
    close(TMPOUT);
}

# open the input file
# manipulate it with regular expression to extract what we need
sub parse_signatures {
    my $file = shift;

    # open and read the file into an array and convert it to one big string
    my %sigs = ();
    open(IN, $file) || die "ERROR: '$file' does not exist!\n";
    my @data = <IN>;
    close(IN);
    my $data = join('', @data);

    # manipulate the data with regular expressions

    # delete function bodies
    $data =~ s{
        AS\s*\$(BODY)?\$
        .*?
        END;?\s*\$(BODY)?\$
    } []gsxi;
    writeout('a1', $data) if $DEBUG;
    # delete C style comments
    $data =~ s{
        /\*
        .*?
        \*/
    } []gsx;
    # delete sql comments
    writeout('aa', $data) if $DEBUG;
    $data =~ s/^--[^\n]*\n//mgs;
    writeout('aa1', $data) if $DEBUG;
    $data =~ s/^ +//mgs;
    writeout('bb', $data) if $DEBUG;
    # remove all text enclosed in single quotes(')
    $data =~ s/'[^\']*'/''/gs;
    # remove the default clauses
    {
        # temporarily disable warnings for the next command
        # because either $2 OR $3 will always be uninitialized
        no warnings 'uninitialized';
        $data =~ s/\bdefault\s*[^,)]+((,)|\)\s*(\)))/$2$3/igs;
    }
    # and now clean up the other cases
    $data =~ s/\bdefault[^)]+\)/\)/igs;
    writeout('cc', $data) if $DEBUG;
    $data =~ s/--[^\n]*\n//gs;
    writeout('dd', $data) if $DEBUG;

    # extract TYPEs

    # extract all "create type <name> as (<column_definition>, ...)"
    # and sort them
    my @types = $data =~ /create\s+type\s+(\w+\s+as\s*\([^\)]+\))/igs;
    @types = sort @types;

    # clean up types
    for (my $i=0; $i<@types; $i++) {
        # remove the "AS " phrase
        $types[$i] =~ s/\bas\s*\(/\(/is;
        # compress multiple white spaces into a single <space>
        $types[$i] =~ s/\s+/ /gs;
        # remove <space> before and after characters ",()"
        $types[$i] =~ s/\s*([,\(\)])\s*/$1/gs;
        # search for "<name> <type>[,)] and remove <name>
        $types[$i] =~ s/\b(\w+)\s([^,\)]+)([,\)])/$2$3/gs;
    }

    # extract FUNCTIONs

    # extract all "create or replace function <name>(<args>) returns"
    # and sort them
    my @funcs = $data =~ /create\s+or\s+replace\s+function\s+(\w+\s*\([^\)]*\))/igs;
    @funcs = sort @funcs;

    # clean up functions
    for (my $i=0; $i<@funcs; $i++) {
        # compress multiple white spaces into a single <space>
        $funcs[$i] =~ s/\s+/ /gs;
        # remove argument names and keep IN|OUT and type name
        $funcs[$i] =~ s/(IN|OUT)\s(\w+)\s([^,\)]+)/$1 $3/ig;
        # remove <space> following "("
        $funcs[$i] =~ s/\(\s+/(/g;
        # remove <space> following ","
        $funcs[$i] =~ s/,\s+/,/g;
        # handle cleanup of more complex arguments
        $funcs[$i] = parse_function_args($funcs[$i]);
    }

    print Data::Dumper->Dump([\@types, \@funcs], ['types', 'funcs']) if $DEBUG;

    return (\@types, \@funcs);
}


sub parse_function_args {
    my $str = shift;

    # convert it to lowercase
    $str = lc($str);

    # split arguments based ","
    my @parts = split /,/, $str;
    foreach my $p (@parts) {
        # if we handled it above skip over it
        next if  $p =~ /\b(in|out)\s(\w+)/;
        # if we still have <name> <type> then remove <name>
        $p =~ s/\b(\w+)\s(\w+)/$2/;
    }

    # put all the parts back into a string
    return join(',', @parts);
}

