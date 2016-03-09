#!/usr/bin/perl -w
eval 'exec /usr/bin/perl -S $0 ${1+"$@"}'
    if 0; #$running_under_some_shell

# -------------------------------------------------------------
# Usage: build-extension-update-files <version> [<pgrouting-src-dir>]
#
# Author: Stephen Woodbridge
# Date: 2015-07-25
# License: MIT-X
# -------------------------------------------------------------
# Description
#
# This script read a file from lib/pgrouting--<version>.sig
# that should be the .sig for the current version of the extension.
# lib/pgrouting--<version>.sig
#
# It then looks in tools/sigs/pgrouting--<old_version>.sig and
# creates an update script for each version found. The update scripts
# are written to:
#   build/lib/pgrouting--<old_version>--<current_version>.sql
# and "sudo make install" will install them as part of the extension.
#
# Algorithm
# 1. check if lib/pgrouting--<version>.sig exists and load it
# 2. get a list of version signatures from tools/sigs/*
# 3. foreach old version sig file
# 4.   generate an update file in lib/
#
# This is part of the automated pgRouting extension update
# system that PostgreSQL require a collection of files that define
# how to update an old version of pgRouting to the current version
# using "ALTER EXTENSION pgrouting UPDATE TO <version>;"
# The lib/pgrouting--<current_version>.sig and the update scripts
# are automatically generated when when cmake is run. You don't
# need to run this script manually.
#
# See also tools/mk-signature-file
# ------------------------------------------------------------

use strict;
use Data::Dumper;
use File::Find ();

my $DEBUG = 0;

use vars qw/*name *dir *prune/;
*name   = *File::Find::name;
*dir    = *File::Find::dir;
*prune  = *File::Find::prune;


sub Usage {
    die "Usage: build-extension-update-files <version> [<pgrouting-src-dir>]\n";
}

# get the commandline options
# these are typically set by cmake
my $version = shift @ARGV || Usage();
my $src_dir = shift @ARGV || '.';

# define the input file names and verify they exist.
my $SIG = "lib/pgrouting--$version.sig";
my $SQL = "lib/pgrouting--$version.sql";
die "ERROR: Failed to find '$SIG'\n" unless -f $SIG;
die "ERROR: Failed to find '$SQL'\n" unless -f $SQL;

# collect a list of the old version .sig files
my @old_files = ();

# read and parse the current version .sig file
my $new_hash = read_sig_file( $SIG );

# assume we are in the build dir, but we might be up one level
# if we are somewhere else we will fail to find the files
my $sig_dir = 'tools/sigs/';
$sig_dir = '../tools/sigs/' if  -d '../tools/sigs';
$sig_dir = $src_dir . '/tools/sigs/' if  -d $src_dir . '/tools/sigs';

# search for the old version .sig files in $sig_dir
# and save the /path/file.sig into @old_files
File::Find::find({wanted => \&wanted}, $sig_dir);

# foreach old files
for my $old_file ( sort @old_files ) {
    # read and parse the .sig
    my $old_hash = read_sig_file( $old_file );
    # and generate and write the update script file
    generate_upgrade_script( $new_hash, $old_hash);
}

exit 0;


sub wanted {
    /^.*\.sig\z/s &&
    ! /^pgrouting--$version.sig\z/s &&
    push @old_files, $name;
}

# read and parse the .sig file and store the results into hash
sub read_sig_file {
    my $file = shift;

    my %hash = ();

    open(IN, $file) || die "ERROR: Failed to open '$file'\n";

    my $state = 0;
    while (my $line = <IN>) {
        if ($line =~ /^#VERSION pgrouting (\d+\.\d+\.\d+)\s*$/i) {
            $hash{VERSION} = $1;
        }
        elsif ($line =~ /^#TYPES\s*/i) {
            $state = 1;
            next;
        }
        elsif ($line =~ /^#FUNCTIONS\s*/i) {
            $state = 2;
            next;
        }
        elsif ($line =~ /^#/) {
            next;
        }
        $line =~ s/\s*$//;
        if ($state == 1) {
            push @{$hash{types}}, $line;
        }
        elsif ($state == 2) {
            push @{$hash{funcs}}, $line;
        }
    }
    close(IN);

    return \%hash;
}

# analyze the old .sig hash as compared to the new .sig hash
# and create an update script file for its version.
sub generate_upgrade_script {
    my ($new, $old) = @_;

    my $err = 0;
    my @types2remove = ();
    my @commands = ();

    # fetch the VERSION numbers
    my $n_ver = $new->{VERSION};
    my $o_ver = $old->{VERSION};

    # analyze types

    my $ntype = $new->{types};
    my $otype = $old->{types};

    # create a hash like <name> => <column_list> for new types
    my %ntype_h = ();
    for my $x (@{$ntype}) {
        $x =~ m/(\w+)(\([^\)]+\))$/;
        $ntype_h{lc($1)} = lc($2);
    }

    # check if old type exists with different column types
    for my $x (@{$otype}) {
        $x =~ m/(\w+)(\([^\)]+\))$/;
        my $name = lc($1);
        my $cols = lc($2);
        if ($ntype_h{$name}) {
            if ($ntype_h{$name} ne $cols) {
                warn "WARNING: old type '$name$cols' changed to '$name$ntype_h{$name}' !\n";
                $err = 1;
            }
            else {
                push @types2remove, $name;
            }
        }
    }


    # analyze function sigs

    my $nsig = $new->{funcs};
    my $osig = $old->{funcs};

    # create an unique hash map of the new signatures
    # to quickly determine if an old signature exists or not
    # in the new signatures.
    my %fmap = map { $_ => 1 } @{$nsig};

    for my $x (@{$osig}) {

        # see if the old signature is in the new signature map
        my $exists = $fmap{$x} || '0';
        print "$exists\t$x\n" if $DEBUG;

        # if the function does NOT exist in the current version
        # then it needs to be removed from the extension
        if (! $exists) {
            print "ALTER EXTENSION pgrouting DROP FUNCTION $x;\n" if $DEBUG;
            push @commands, "ALTER EXTENSION pgrouting DROP FUNCTION $x;\n";
        }

        # otherwise we only need to drop in case an argument name change
        # so it will not fail on create or replace function
        print "DROP FUNCTION IF EXISTS $x;\n" if $DEBUG;
        push @commands, "DROP FUNCTION IF EXISTS $x;\n";
    }

    # UGH! someone change the definition of the TYPE or reused an existing
    # TYPE name which is VERY BAD because other poeple might be dependent
    # on the old TYPE so we can DROP TYPE <type> CASCADE; or we might drop
    # a user's function. So juse DIE and maybe someone can resolve this
    die "ERROR: pgrouting TYPE changed! Cannot continue!\n" if $err;

    write_script($o_ver, $n_ver, \@types2remove, join('', @commands));
}


# we have all the data we need, so write the extension update script
sub write_script {
    my ($o_ver, $n_ver, $types, $cmds) = @_;

    # open the extension update script or die if we can't
    open(OUT, ">lib/pgrouting--$o_ver--$n_ver.sql")
        || die "ERROR: failed to create 'lib/pgrouting-pgrouting--$o_ver--$n_ver.sql' : $!\n";

    # write out the header and the commands to clean up the old extension
    print OUT <<EOF;
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- pgRouting extension upgrade from $o_ver to $n_ver
-- generated by tools/build-extension-update-files
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- remove functions no longer in the $n_ver extension
$cmds

-- now install the new extension

EOF

    # open the new extension.sql file
    # and load it into an array
    open(IN, $SQL) ||
        die "ERROR: Failed to find '$SQL' : $!\n";
    my @file = <IN>;
    close(IN);

    # comment out the TYPEs that the new extension defines
    # that already existed in the old extension
    # so they will not abort the script
    remove_types( \@file, $types );

    # append the new extension SQL to the update script
    print OUT @file;
    close(OUT);
}


sub remove_types {
    my ($data, $types) = @_;

    for my $type (@{$types}) {
        my $state = 1;
        for my $x (@{$data}) {
            if ($state == 1) {
                next unless $x =~ m/create\s+type\s+$type\b/i;
                $x = "-- $x";
                $state = 2
                    unless $x =~ m/create\s+type\s+$type\s+as\s*\([^\)]+\)/i;
            }
            elsif ($state == 2) {
                $x = "-- $x";
                last if $x =~ /\)\s*;/;
            }
        }
    }
}

