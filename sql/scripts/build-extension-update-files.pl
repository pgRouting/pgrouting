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

=pod

* Signature within micros do not change
* Signature changes within minors:
  * The higher the number the more SQL functions it can have
  * Need a higher number when the function parameters names/types change
* Changes within Majors
  * Anything can happen

For example:
File: pgrouting--3.4.sig works for 3.4.0, 3.4.1 .... 3.4.x
File: pgrouting--3.5.sig
* Has all the signatures of pgrouting--3.4.sig and maybe some more
* When IN parameters names or OUT parameters name, type change
  * use drop_special_case_function


=cut

eval 'exec /usr/bin/perl -S $0 ${1+"$@"}'
    if 0; #$running_under_some_shell


use strict;
use Data::Dumper;
use File::Find ();
use version 0.77;


use vars qw/*name *dir *prune/;
*name   = *File::Find::name;
*dir    = *File::Find::dir;
*prune  = *File::Find::prune;

my $version_2_6 = qr/(2.6.[\d+])/;
my $version_3_0 = qr/(3.0.[\d+])/;
my $version_3_1 = qr/(3.1.[\d+])/;
my $version_3_2 = qr/(3.2.[\d+])/;
my $version_3_3 = qr/(3.3.[\d+])/;
my $version_3_4 = qr/(3.4.[\d+])/;
my $version_3_5 = qr/(3.5.[\d+])/;
my $version_3_6 = qr/(3.6.[\d+])/;
my $version_3_7 = qr/(3.7.[\d+])/;
my $version_3_8 = qr/(3.8.[\d+])/;
my $version_4_0 = qr/(4.0.[\d+])/;
# add minor here

my $version_2 = qr/(2.[\d+].[\d+])/;
my $version_3 = qr/(3.[\d+].[\d+])/;
my $version_format = qr/([\d+].[\d+].[\d+])/;
my $minor_format   = qr/([\d+].[\d+]).[\d+]/;
my $mayor_format   = qr/([\d+]).[\d+].[\d+]/;


my $current = $version_4_0;


sub Usage {
    die "Usage:\nFrom the cmake:
    build-extension-update-files <new_version> <old_version> <signature_dir> <output_dir> <debug>";
}

# Get information from command line
my $new_version = shift @ARGV || die "Missing: new version to convert";
my $old_version = shift @ARGV || die "Missing: old version to convert";
my $signature_dir = shift @ARGV || die "Missing: signature directory";
my $output_directory = shift @ARGV || die "Missing: output directory";
my $DEBUG = shift @ARGV;


print "DEBUG is on\n" if $DEBUG;
print "build-extension-update-file $new_version $old_version $signature_dir $output_directory $DEBUG\n" if $DEBUG;

die "ERROR: Wrong format for new version expected n.n.n got '$new_version'" unless $new_version =~ $version_format;
die "ERROR: Wrong format for old version expected n.n.n got '$old_version'" unless $old_version =~ $version_format;
die "ERROR: '$new_version' must be greater than '$old_version'" unless version->parse($new_version) > version->parse($old_version);
die "ERROR: Can not update from '$old_version' must be at least '2.6.0'" if version->parse($old_version) < version->parse("2.6.0");

my ($new_mayor) = $new_version  =~ $mayor_format;
my ($old_mayor) = $old_version  =~ $mayor_format;

die "ERROR: This script does not upgrade from $old_version to $new_version $new_mayor\n" if $new_mayor == 2;

my ($new_minor) = $new_version  =~ $minor_format;
my ($old_minor) = $old_version  =~ $minor_format;

my $curr_signature_file_name = "$signature_dir/pgrouting--$new_minor.sig";
my $old_signature_file_name = "$signature_dir/pgrouting--$old_minor.sig";
my $curr_sql_file_name = "$output_directory/pgrouting--$new_version.sql";
my $output_file_name = "$output_directory/pgrouting--$old_version--$new_version.sql";


# Verify directories exist
die "ERROR: Failed to find: input directory: '$signature_dir'\n" unless -d $signature_dir;
die "ERROR: Failed to find: output directory: '$output_directory'\n" unless -d $output_directory;

# Verify needed files exist.
die "ERROR: Failed to find: old signature file: '$old_signature_file_name'\n" unless -f $old_signature_file_name;
die "ERROR: Failed to find: current signature file: '$curr_signature_file_name'\n" unless -f $curr_signature_file_name;
die "ERROR: Failed to find: current sql file: '$curr_sql_file_name'\n" unless -f $curr_sql_file_name;


######################################################
# process section
######################################################

if ($new_minor == $old_minor) {
    print "Same minor: The signatures didn't change\n" if $DEBUG;
    my $curr_signature = read_and_parse_signature_file($curr_signature_file_name);
    generate_upgrade_script($curr_signature, $curr_signature);
    exit 0;
};

print "Building the updating files\n" if $DEBUG;

my $curr_signature = read_and_parse_signature_file($curr_signature_file_name);
my $old_signatures = read_and_parse_signature_file($old_signature_file_name);

print "\nGenerating $old_version upgrade file to $new_version\n" if $DEBUG;
generate_upgrade_script($curr_signature, $old_signatures);

exit 0;


######################################################
# subroutines
######################################################

# read_and_parse_signature_file
# generate_upgrade_script
#
# Special case functions

# read and parse the .sig file and store the results in a hash
sub read_and_parse_signature_file {
    my $file = shift;

    my %hash = ();

    open(IN, $file) || die "ERROR: Failed to open '$file'\n";

    # state:
    # 0 - starting process
    # 1 - processing types
    # 2 - processing functions
    # Other kinds of postgres objctes are not used in 2.x version of pgRouting
    my $state = 2;
    while (my $line = <IN>) {
        # Remove spaces from the line
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



# analyze the old signatures compared to the new signatures hash
# and create an update script file for its version.
sub generate_upgrade_script {
    my ($new, $old) = @_;

    my $err = 0;
    my @types2remove = ();
    my @commands = ();

    #------------------------------------
    # analyze function signatures
    #------------------------------------

    my $new_signatures = $new->{funcs};
    my $old_signatures = $old->{funcs};

    # create an unique hash map of the new signatures
    # to quickly determine if an old signature exists or not
    # in the new signatures.
    my %function_map = map { $_ => 1 } @{$new_signatures};

    if ($new_minor != $old_minor) {
        if ($old_mayor == 2) {
            push @commands, "-- Removing functions from $old_mayor\n" if $DEBUG;
        } else {
            push @commands, "-- Removing functions that does not exists on $new_version\n" if $DEBUG;
        }

        for my $old_function (sort @{$old_signatures}) {
            if ($old_mayor == 2) {
                push @commands, drop_special_case_function($old_function);
                next;
            }

            # Skip signatures from the old version that exist in the new version
            next if $function_map{$old_function};

            # Enforcing: When its the same mayor, the signatures should exist in the new version
            die "$old_function should exist in $new_minor" if $new_mayor == $old_mayor;

            # Remove from the extension
            push @commands, drop_special_case_function($old_function);
        }

        if ($old_mayor == 3) {
            # standardized output columns
            if ($old_minor < 3.5) {
                push @commands, drop_special_case_function("pgr_dijkstra(text,anyarray,bigint,boolean)");
                push @commands, drop_special_case_function("pgr_dijkstra(text,bigint,anyarray,boolean)");
                push @commands, drop_special_case_function("pgr_dijkstra(text,bigint,bigint,boolean)");
            }

            # updating to 3.6+
            if ($old_minor < 3.6) {
                push @commands, drop_special_case_function("pgr_astar(text,anyarray,bigint,boolean,integer,double precision,double precision)");
                push @commands, drop_special_case_function("pgr_astar(text,bigint,anyarray,boolean,integer,double precision,double precision)");
                push @commands, drop_special_case_function("pgr_astar(text,bigint,bigint,boolean,integer,double precision,double precision)");
                push @commands, drop_special_case_function("pgr_ksp(text,bigint,bigint,integer,boolean,boolean)");
                push @commands, drop_special_case_function("pgr_bdastar(text,bigint,bigint,boolean,integer,numeric,numeric)");
                push @commands, drop_special_case_function("pgr_bdastar(text,bigint,anyarray,boolean,integer,numeric,numeric)");
                push @commands, drop_special_case_function("pgr_bdastar(text,anyarray,bigint,boolean,integer,numeric,numeric)");
                push @commands, drop_special_case_function("pgr_drivingdistance(text,anyarray,double precision,boolean,boolean)");
                push @commands, drop_special_case_function("pgr_drivingdistance(text,bigint,double precision,boolean)");
            }

            # updating to 3.7+
            if ($old_mayor >= 3.0 && $old_minor < 3.7) {
                push @commands, drop_special_case_function("pgr_primbfs(text,anyarray,bigint)");
                push @commands, drop_special_case_function("pgr_primbfs(text,bigint,bigint)");
                push @commands, drop_special_case_function("pgr_primdfs(text,anyarray,bigint)");
                push @commands, drop_special_case_function("pgr_primdfs(text,bigint,bigint)");
                push @commands, drop_special_case_function("pgr_primdd(text,bigint,numeric)");
                push @commands, drop_special_case_function("pgr_primdd(text,bigint,double precision)");
                push @commands, drop_special_case_function("pgr_primdd(text,anyarray,numeric)");
                push @commands, drop_special_case_function("pgr_primdd(text,anyarray,double precision)");
                push @commands, drop_special_case_function("pgr_kruskalbfs(text,anyarray,bigint)");
                push @commands, drop_special_case_function("pgr_kruskalbfs(text,bigint,bigint)");
                push @commands, drop_special_case_function("pgr_kruskaldfs(text,anyarray,bigint)");
                push @commands, drop_special_case_function("pgr_kruskaldfs(text,bigint,bigint)");
                push @commands, drop_special_case_function("pgr_kruskaldd(text,bigint,numeric)");
                push @commands, drop_special_case_function("pgr_kruskaldd(text,bigint,double precision)");
                push @commands, drop_special_case_function("pgr_kruskaldd(text,anyarray,numeric)");
                push @commands, drop_special_case_function("pgr_kruskaldd(text,anyarray,double precision)");
            }

            if ($old_minor >= "3.2") {
                # Out parameters changed names on v4.0.0
                push @commands, drop_special_case_function("pgr_dagshortestpath(text,text)");
                push @commands, drop_special_case_function("pgr_sequentialvertexcoloring(text)");
                push @commands, drop_special_case_function("pgr_bipartite(text)");

                push @commands, drop_special_case_function("_pgr_depthfirstsearch(text,anyarray,boolean,bigint)");
                push @commands, drop_special_case_function("pgr_depthfirstsearch(text,anyarray,boolean,bigint)");
                push @commands, drop_special_case_function("pgr_depthfirstsearch(text,bigint,boolean,bigint)");
            }

            if ($old_minor >= "3.3") {
                push @commands, drop_special_case_function("pgr_edgecoloring(text)");
            }

            push @commands, drop_special_case_function("_pgr_breadthfirstsearch(text,anyarray,bigint,boolean)");

            # Row type defined by OUT parameters is different.
            # Out parameters changed names on v4.0.0

            push @commands, drop_special_case_function("pgr_turnrestrictedpath(text,text,bigint,bigint,integer,boolean,boolean,boolean,boolean)");

            push @commands, drop_special_case_function("pgr_bellmanford(text,bigint,bigint,boolean)");
            push @commands, drop_special_case_function("pgr_bellmanford(text,anyarray,bigint,boolean)");
            push @commands, drop_special_case_function("pgr_bellmanford(text,bigint,anyarray,boolean)");

            push @commands, drop_special_case_function("pgr_binarybreadthfirstsearch(text,bigint,bigint,boolean)");
            push @commands, drop_special_case_function("pgr_binarybreadthfirstsearch(text,anyarray,bigint,boolean)");
            push @commands, drop_special_case_function("pgr_binarybreadthfirstsearch(text,bigint,anyarray,boolean)");

            push @commands, drop_special_case_function("pgr_topologicalsort(text)");
            push @commands, drop_special_case_function("pgr_transitiveclosure(text)");

            push @commands, drop_special_case_function("pgr_dagshortestpath(text,bigint,bigint)");
            push @commands, drop_special_case_function("pgr_dagshortestpath(text,bigint,anyarray)");
            push @commands, drop_special_case_function("pgr_dagshortestpath(text,anyarray,bigint)");
            push @commands, drop_special_case_function("pgr_dagshortestpath(text,anyarray,anyarray)");

            push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,bigint,bigint,boolean)");
            push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,anyarray,bigint,boolean)");
            push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,bigint,anyarray,boolean)");

            push @commands, drop_special_case_function("pgr_edwardmoore(text,bigint,bigint,boolean)");
            push @commands, drop_special_case_function("pgr_edwardmoore(text,anyarray,bigint,boolean)");
            push @commands, drop_special_case_function("pgr_edwardmoore(text,bigint,anyarray,boolean)");

            # Official functions
            push @commands, drop_special_case_function("pgr_bddijkstra(text,bigint,bigint,boolean)");
            push @commands, drop_special_case_function("pgr_bddijkstra(text,anyarray,bigint,boolean)");
            push @commands, drop_special_case_function("pgr_bddijkstra(text,bigint,anyarray,boolean)");
        }
    }

    if ($old_mayor == 2) {
        push @commands, "ALTER EXTENSION pgrouting DROP TYPE pgr_costresult;  DROP TYPE pgr_costresult;\n";
        push @commands, "ALTER EXTENSION pgrouting DROP TYPE pgr_costresult3; DROP TYPE pgr_costresult3;\n";
        push @commands, "ALTER EXTENSION pgrouting DROP TYPE pgr_geomresult;  DROP TYPE pgr_geomresult;\n";
    }

    write_script(join('', @commands));
}



sub write_script {
    my ($commands) = @_;

    my $contents = get_current_sql();

    print "Creating '$output_file_name'\n" if $DEBUG;

    open(OUT, ">$output_file_name")
    || die "ERROR: failed to create '$output_directory/pgrouting-pgrouting--$old_version--$new_version.sql' : $!\n";

    my $header = "-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- pgRouting extension upgrade from $old_version to $new_version
-- generated by tools/build-extension-update-file
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
";
    my $echo = "\\echo Use \"ALTER extension pgrouting update to '$new_version'\" to load this file. \\quit";

    my $delete_msg ="";
    $delete_msg = "
-------------------------------------
-- remove functions no longer in the $new_version extension
-------------------------------------" unless $new_mayor == $old_mayor;


    # write out the header and the commands to clean up the old extension
    print OUT <<EOF;
$header
$echo
$delete_msg
$commands
$contents

EOF

    close(OUT);
    print "  -- Created $output_file_name\n" if $DEBUG;
}


sub get_current_sql {

    open(IN, $curr_sql_file_name) ||
    die "ERROR: Failed to find '$curr_sql_file_name' : $!\n";
    my @file = <IN>;
    close(IN);
    my $contents = join('', @file);

    # remove the header
    $contents  =~ s/\\echo Use "CREATE EXTENSION pgrouting" to load this file. \\quit//;

    my %seen = ();
    while ($contents =~ /--v([\d+].[\d+])/g) {  # scalar context
        $seen{$1}++;
    }
    my @uniq = keys %seen;

    foreach my $minor (@uniq) {
        if  ($old_minor >= $minor) {
            $contents =~ s{
                --v$minor
                .*?
                FUNCTION
            }[CREATE OR REPLACE FUNCTION]gsxi;
        }
    }


    return $contents
}

sub drop_special_case_function {
    my ($function) = @_;
    my @commands = ();

    push @commands,  "-- $old_version  **WARN: DROP $function (something changed for $new_version)\n" if $DEBUG;
    push @commands, "ALTER EXTENSION pgrouting DROP FUNCTION $function;\n";
    push @commands, "DROP FUNCTION IF EXISTS $function;\n";
    return @commands;
}
