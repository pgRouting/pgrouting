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

* Signature Changes within micros do no change
* Signature Changes within minors:
  * The higher the number the more SQL functions it has
  * The common functions signatures do not change
* Changes within Majors
  * Anything can happen

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

my $version_2_0 = qr/(2.0.[\d+])/;
my $version_2_1 = qr/(2.1.[\d+])/;
my $version_2_2 = qr/(2.2.[\d+])/;
my $version_2_3 = qr/(2.3.[\d+])/;
my $version_2_4 = qr/(2.4.[\d+])/;
my $version_2_5 = qr/(2.5.[\d+])/;
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
        for my $old_function (sort @{$old_signatures}) {
            # Skip signatures from the old version that exist in the new version
            next if $function_map{$old_function};

            # Enforcing: When its the same mayor, the signatures should exist in the new version
            die "$old_function should exist in $new_minor" if $new_mayor == $old_mayor;


            # Remove from the extension
            push @commands, "-- $old_function does not exists on $new_version\n" if $DEBUG;
            push @commands, "ALTER EXTENSION pgrouting DROP FUNCTION $old_function;\n";
            push @commands, "DROP FUNCTION IF EXISTS $old_function;\n\n";
        }

        # updating to 3.5+
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
    }

    #------------------------------------
    # Special cases
    #------------------------------------

    if ($old_mayor == 2) {
        push @commands, update_from_version_2();
    }

=pod
    if ("$old_version" eq "3.0.0") {
        push @commands, update_from_version_3_0_0();
    }
=cut

    # UGH! someone change the definition of the TYPE or reused an existing
    # TYPE name which is VERY BAD because other people might be dependent
    # on the old TYPE so we can DROP TYPE <type> CASCADE; or we might drop
    # a user's function. So just DIE and maybe someone can resolve this
    die "ERROR: pgrouting TYPE changed! Cannot continue!\n" if $err;


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
    #my ($types) = @_;

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

=pod
*****************************************************************
functions to fix signatures
*****************************************************************
=cut

sub drop_special_case_function {
    my ($function) = @_;
    my @commands = ();

    push @commands,  "-- $old_version  **WARN: DROP $function (something changed for $new_version)\n" if $DEBUG;
    push @commands, "ALTER EXTENSION pgrouting DROP FUNCTION $function;\n";
    push @commands, "DROP FUNCTION IF EXISTS $function;\n";
    return @commands;
}

sub update_pg_proc_short {
    my ($func_name, $new_sig) = @_;
    return "
UPDATE pg_proc SET
proargnames = '{$new_sig}'
WHERE proname = '$func_name';
    ";
}

sub update_pg_proc {
    my ($func_name, $old_sig, $new_sig) = @_;
    return "
UPDATE pg_proc SET
proargnames = '{$new_sig}'
WHERE proname = '$func_name'
AND proargnames = '{$old_sig}';
    ";
}

sub update_proallargtypes{
    my ($func_name, $old_sig, $new_sig) = @_;
    return "
UPDATE pg_proc SET
proallargtypes = '{$new_sig}'
WHERE proname = '$func_name'
AND proallargtypes = '{$old_sig}';
    ";
}


=pod
*****************************************************************
code beyond this point is v3.0.0 specific
*****************************************************************
=cut

sub update_from_version_3_0_0 {
    my @commands = ();
    push @commands, "

---------------------------------------------
-- Updating from version 3.0.0
---------------------------------------------
" if $DEBUG;

    push @commands, tsp();
    push @commands, withpoints();
    push @commands, components();
    push @commands, flow();
    push @commands, others_3_0_0();
    return @commands;
}

sub others_3_0_0 {
    my @commands = ();
    push @commands, update_pg_proc(
        'pgr_topologicalsort',
        'edges_sql,seq,sorted_v',
        '"",seq,sorted_v');
    push @commands, update_pg_proc(
        'pgr_transitiveclosure',
        'edges_sql,seq,vid,target_array',
        '"",seq,vid,target_array');
    return @commands;
}

=pod
*****************************************************************
code beyond this point is v2.6 specific
*****************************************************************
=cut

sub update_from_version_2 {
    my @commands = ();
    push @commands, "
---------------------------------------------
-- Updating from version 2.6
---------------------------------------------
" if $DEBUG;
    push @commands, "ALTER EXTENSION pgrouting DROP TYPE pgr_costresult;  DROP TYPE pgr_costresult CASCADE;\n";
    push @commands, "ALTER EXTENSION pgrouting DROP TYPE pgr_costresult3; DROP TYPE pgr_costresult3 CASCADE;\n";
    push @commands, "ALTER EXTENSION pgrouting DROP TYPE pgr_geomresult;  DROP TYPE pgr_geomresult CASCADE;\n";
    push @commands, dijkstra();
    push @commands, allpairs();
    push @commands, astar();
    push @commands, withpoints();
    push @commands, drivingDistance();
    push @commands, ksp();
    push @commands, bddijkstra();
    push @commands, bdastar();
    push @commands, flow();
    push @commands, tsp();
    push @commands, components();
    push @commands, vrp();
    push @commands, topology();
    push @commands, version();
    return @commands;
}

sub version {
    my @commands = ();
    push @commands, drop_special_case_function("pgr_version()");
    return @commands;
}

sub bddijkstra {
    my @commands = ();

    push @commands, drop_special_case_function("_pgr_bddijkstra(text,anyarray,anyarray,boolean,boolean)");
    push @commands, drop_special_case_function("pgr_bddijkstra(text,bigint,anyarray,boolean)");

    push @commands, update_pg_proc(
        'pgr_bddijkstra',
        'edges_sql,start_vid,end_vid,directed,seq,path_seq,node,edge,cost,agg_cost',
        '"","","",directed,seq,path_seq,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_bddijkstra',
        'edges_sql,start_vid,end_vids,directed,seq,path_seq,end_vid,node,edge,cost,agg_cost',
        '"","","",directed,seq,path_seq,node,end_vid,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_bddijkstra',
        'edges_sql,start_vids,end_vid,directed,seq,path_seq,start_vid,node,edge,cost,agg_cost',
        '"","","",directed,seq,path_seq,start_vid,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_bddijkstra',
        'edges_sql,start_vids,end_vids,directed,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost',
        '"","","",directed,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost');

    # pgr_BDdijkstraCost
    push @commands, update_pg_proc_short(
        'pgr_bddijkstracost',
        '"","","",directed,start_vid,end_vid,agg_cost');

    # pgr_BDdijkstraCostMatrix
    push @commands, update_pg_proc(
        'pgr_bddijkstracostmatrix',
        'edges_sql,vids,directed,start_vid,end_vid,agg_cost',
        '"","",directed,start_vid,end_vid,agg_cost');

    return @commands;
}


sub withpoints {
    my @commands = ();

    push @commands, update_pg_proc(
        'pgr_withpoints',
        'edges_sql,points_sql,start_pid,end_pid,directed,driving_side,details,seq,path_seq,node,edge,cost,agg_cost',
        '"","","","",directed,driving_side,details,seq,path_seq,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_withpoints',
        'edges_sql,points_sql,start_pid,end_pids,directed,driving_side,details,seq,path_seq,end_pid,node,edge,cost,agg_cost',
        '"","","","",directed,driving_side,details,seq,path_seq,end_pid,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_withpoints',
        'edges_sql,points_sql,start_pids,end_pid,directed,driving_side,details,seq,path_seq,start_pid,node,edge,cost,agg_cost',
        '"","","","",directed,driving_side,details,seq,path_seq,start_pid,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_withpoints',
        'edges_sql,points_sql,start_pids,end_pids,directed,driving_side,details,seq,path_seq,start_pid,end_pid,node,edge,cost,agg_cost',
        '"","","","",directed,driving_side,details,seq,path_seq,start_pid,end_pid,node,edge,cost,agg_cost');

    # pgr_withPointsCost
    push @commands, update_pg_proc_short(
        'pgr_withpointscost',
        '"","","","",directed,driving_side,start_pid,end_pid,agg_cost');

    #pgr_withPointsCostMatrix
    push @commands, update_pg_proc(
        'pgr_withpointscostmatrix',
        'edges_sql,points_sql,pids,directed,driving_side,start_vid,end_vid,agg_cost',
        '"","","",directed,driving_side,start_vid,end_vid,agg_cost');

    # pgr_withPointsDD
    push @commands, update_pg_proc(
        'pgr_withpointsdd',
        'edges_sql,points_sql,start_pid,distance,directed,driving_side,details,equicost,seq,start_vid,node,edge,cost,agg_cost',
        '"","","","",directed,driving_side,details,equicost,seq,start_vid,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_withpointsdd',
        'edges_sql,points_sql,start_pid,distance,directed,driving_side,details,seq,node,edge,cost,agg_cost',
        '"","","","",directed,driving_side,details,seq,node,edge,cost,agg_cost');

    return @commands;
}


sub drivingDistance {
    my @commands = ();

    push @commands, drop_special_case_function("pgr_drivingdistance(text,bigint,double precision,boolean)");
    push @commands, update_pg_proc(
        'pgr_drivingdistance',
        'edges_sql,start_vids,distance,directed,equicost,seq,from_v,node,edge,cost,agg_cost',
        '"","","",directed,equicost,seq,from_v,node,edge,cost,agg_cost');

    return @commands;
}


sub topology {
    my @commands = ();

    push @commands, update_pg_proc(
        'pgr_nodenetwork',
        'edge_table,tolerance,id,the_geom,table_ending,rows_where,outall',
        '"","",id,the_geom,table_ending,rows_where,outall');
    return @commands;
}


sub allpairs {
    my @commands = ();

    push @commands, update_pg_proc(
        'pgr_johnson',
        'edges_sql,directed,start_vid,end_vid,agg_cost',
        '"",directed,start_vid,end_vid,agg_cost');
    push @commands, update_pg_proc(
        'pgr_floydwarshall',
        'edges_sql,directed,start_vid,end_vid,agg_cost',
        '"",directed,start_vid,end_vid,agg_cost');
    return @commands;
}



sub components {
    my @commands = ();
    if ("$old_version" eq "3.0.0") {
        push @commands, update_proallargtypes(
            '_pgr_strongcomponents',
            '25,23,20,20',
            '25,20,20,20');
        push @commands, update_proallargtypes(
            'pgr_strongcomponents',
            '25,23,20,20',
            '25,20,20,20');
        push @commands, update_proallargtypes(
            '_pgr_biconnectedcomponents',
            '25,23,20,20',
            '25,20,20,20');
        push @commands, update_proallargtypes(
            'pgr_biconnectedcomponents',
            '25,23,20,20',
            '25,20,20,20');
    }

    if ($old_mayor == 2) {
        push @commands, drop_special_case_function("pgr_connectedcomponents(text)");
        push @commands, drop_special_case_function("pgr_strongcomponents(text)");
        push @commands, drop_special_case_function("pgr_biconnectedcomponents(text)");
        push @commands, drop_special_case_function("pgr_articulationpoints(text)");
        push @commands, drop_special_case_function("pgr_bridges(text)");
    }

    return @commands;
}


sub tsp {
    my @commands = ();

    push @commands, update_pg_proc(
        'pgr_tsp',
        'matrix_row_sql,start_id,end_id,max_processing_time,tries_per_temperature,max_changes_per_temperature,max_consecutive_non_changes,initial_temperature,final_temperature,cooling_factor,randomize,seq,node,cost,agg_cost',
        '"",start_id,end_id,max_processing_time,tries_per_temperature,max_changes_per_temperature,max_consecutive_non_changes,initial_temperature,final_temperature,cooling_factor,randomize,seq,node,cost,agg_cost');

    return @commands;
}

sub dijkstra {
    my @commands = ();

    push @commands, drop_special_case_function("pgr_dijkstra(text,bigint,anyarray,boolean)");

    push @commands, update_pg_proc(
        'pgr_dijkstra',
        'edges_sql,start_vid,end_vid,directed,seq,path_seq,node,edge,cost,agg_cost',
        '"","","",directed,seq,path_seq,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_dijkstra',
        'edges_sql,start_vid,end_vids,directed,seq,path_seq,end_vid,node,edge,cost,agg_cost',
        '"","","",directed,seq,path_seq,node,end_vid,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_dijkstra',
        'edges_sql,start_vids,end_vid,directed,seq,path_seq,start_vid,node,edge,cost,agg_cost',
        '"","","",directed,seq,path_seq,start_vid,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_dijkstra',
        'edges_sql,start_vids,end_vids,directed,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost',
        '"","","",directed,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost');

    # pgr_dijkstraCost
    push @commands, update_pg_proc_short(
        'pgr_dijkstracost',
        '"","","",directed,start_vid,end_vid,agg_cost');

    # pgr_dijkstraCostMatrix
    push @commands, update_pg_proc(
        'pgr_dijkstracostmatrix',
        'edges_sql,vids,directed,start_vid,end_vid,agg_cost',
        '"","",directed,start_vid,end_vid,agg_cost');

    push @commands, update_pg_proc(
        'pgr_dijkstravia',
        'edges_sql,via_vertices,directed,strict,u_turn_on_edge,seq,path_id,path_seq,start_vid,end_vid,node,edge,cost,agg_cost,route_agg_cost',
        '"","",directed,strict,u_turn_on_edge,seq,path_id,path_seq,start_vid,end_vid,node,edge,cost,agg_cost,route_agg_cost');


    return @commands;
}



sub ksp {
    my @commands = ();

    push @commands, update_pg_proc(
        'pgr_ksp',
        'edges_sql,start_vid,end_vid,k,directed,heap_paths,seq,path_id,path_seq,node,edge,cost,agg_cost',
        '"","","","",directed,heap_paths,seq,path_id,path_seq,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_withpointsksp',
        'edges_sql,points_sql,start_pid,end_pid,k,directed,heap_paths,driving_side,details,seq,path_id,path_seq,node,edge,cost,agg_cost',
        '"","","","","",directed,heap_paths,driving_side,details,seq,path_id,path_seq,node,edge,cost,agg_cost');

    return @commands;
}

sub vrp {
    my @commands = ();

    push @commands, update_pg_proc(
        'pgr_vrponedepot',
        'order_sql,vehicle_sql,cost_sql,depot_id,oid,opos,vid,tarrival,tdepart',
        '"","","","",oid,opos,vid,tarrival,tdepart');

    return @commands;
}

sub bdastar {
    my @commands = ();

    push @commands, update_pg_proc(
            'pgr_bdastarcostmatrix',
             'edges_sql,vids,directed,heuristic,factor,epsilon,start_vid,end_vid,agg_cost',
             '"","",directed,heuristic,factor,epsilon,start_vid,end_vid,agg_cost');

    return @commands;
}

sub astar {
    my @commands = ();

    push @commands, update_pg_proc(
        'pgr_astar',
        'edges_sql,start_vid,end_vid,directed,heuristic,factor,epsilon,seq,path_seq,node,edge,cost,agg_cost',
        '"","","",directed,heuristic,factor,epsilon,seq,path_seq,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_astar',
        'edges_sql,start_vid,end_vids,directed,heuristic,factor,epsilon,seq,path_seq,end_vid,node,edge,cost,agg_cost',
        '"","","",directed,heuristic,factor,epsilon,seq,path_seq,end_vid,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_astar',
        'edges_sql,start_vids,end_vid,directed,heuristic,factor,epsilon,seq,path_seq,start_vid,node,edge,cost,agg_cost',
        '"","","",directed,heuristic,factor,epsilon,seq,path_seq,start_vid,node,edge,cost,agg_cost');
    push @commands, update_pg_proc(
        'pgr_astar',
        'edges_sql,start_vids,end_vids,directed,heuristic,factor,epsilon,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost',
        '"","","",directed,heuristic,factor,epsilon,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost');

    #pgr_astarCost
    push @commands, update_pg_proc_short(
        'pgr_astarcost',
        '"","","",directed,heuristic,factor,epsilon,start_vid,end_vid,agg_cost');

    #pgr_astarCostMatrix
    push @commands, update_pg_proc(
        'pgr_astarcostmatrix',
        'edges_sql,vids,directed,heuristic,factor,epsilon,start_vid,end_vid,agg_cost',
        '"","",directed,heuristic,factor,epsilon,start_vid,end_vid,agg_cost');

    return @commands;
}




sub flow {
    my @commands = ();

    if ("$old_version" eq "3.0.0") {
        push @commands, update_pg_proc(
            'pgr_stoerwagner',
            'edges_sql,seq,edge,cost,mincut',
            '"",seq,edge,cost,mincut');
    }
    if ($old_mayor == 2) {
        push @commands, update_pg_proc(
            'pgr_maxcardinalitymatch',
            'edges_sql,directed,seq,edge,source,target',
            '"",directed,seq,edge,source,target');
        push @commands, update_pg_proc(
            'pgr_maxflow',
            'edges_sql,source_vertices,sink_vertices',
            '"","",""');
    }
    return @commands;
}



