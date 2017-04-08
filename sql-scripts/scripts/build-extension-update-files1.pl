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

my $version_2_0 = qr/(2.0.[\d+])/;
my $version_2_1 = qr/(2.1.[\d+])/;
my $version_2_2 = qr/(2.2.[\d+])/;
my $version_2_3 = qr/(2.3.[\d+])/;
my $version_2_4 = qr/(2.4.[\d+])/;
my $version_2_5 = qr/(2.5.[\d+])/;
my $version_2 = qr/(2.[\d+].[\d+])/;


sub Usage {
    die "Usage:\nFrom the cmake:
    build-extension-update-files";
}

# Get the commandline options
# these are typically set by cmake
my $src_dir = shift @ARGV || '.';

my $version = "@PGROUTING_VERSION@";

my $input_directory = '@PGROUTING_SOURCE_DIR@/tools/sigs';
my $output_directory = '@PGROUTING_SOURCE_DIR@/sql-scripts';

# my $output_directory = 'sql-scripts';
# my $input_directory = 'tools/sigs';
# my $output_directory = 'tools/sql-update-scripts';
# my $output_directory = 'sql-scripts';

die "ERROR: Failed to find input directory: $input_directory\n" unless -d $input_directory;
die "ERROR: Failed to find output directory: $output_directory\n" unless -d $output_directory;

my $sig_dir = "$input_directory/" if -d $input_directory;

print "Building the updating files\n";

# Verify current $version files exist.
my $curr_signature_file_name = "$input_directory/pgrouting--$version.sig";
my $curr_sql_file_name = "$output_directory/pgrouting--$version.sql.in";


die "ERROR: Failed to find '$curr_signature_file_name'\n" unless -f $curr_signature_file_name;
die "ERROR: Failed to find '$curr_sql_file_name'\n" unless -f $curr_sql_file_name;

# Read and parse the current version signature file
my $curr_signature = read_signature_file($curr_signature_file_name);

# collect a list of the old version .sig files
my @old_signatures_file_names = ();


# search for the old version .sig files in $sig_dir
# and save the /path/file.sig into old_signatures_file_names
File::Find::find({wanted => \&wanted}, $sig_dir);

# Generate the upgrade SQL script needed for all signatures
for my $old_file (sort @old_signatures_file_names) {
    print "\ngenerating $old_file upgrade file\n" if $DEBUG;

    my $old_signatures = read_signature_file($old_file);

    generate_upgrade_script($curr_signature, $old_signatures);

    #last;
}

exit 0;


# Processing files with .sig extension
# But not including the current signature file
sub wanted {
    /^.*\.sig\z/s &&
    ! /^pgrouting--$version.sig\z/s &&

    push @old_signatures_file_names, $name;
}

# read and parse the .sig file and store the results in a hash
sub read_signature_file {
    my $file = shift;

    my %hash = ();

    open(IN, $file) || die "ERROR: Failed to open '$file'\n";

    # state:
    # 0 - starting proces
    # 1 - processing types
    # 2 - processing functions
    # Other kinds of postgresobjcts are not used in 2.x version of pgRouting
    my $state = 0;
    while (my $line = <IN>) {
        if ($line =~ /^#VERSION pgrouting (\d+\.\d+\.\d+)\s*$/i) {
            my $content_version = $1;

            $hash{VERSION} = $content_version;

            #check that the version is the same as the file
            die "ERROR file $file has the version $1\n" unless $file =~ m/$content_version/
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

    # fetch the VERSION numbers
    my $new_version = $new->{VERSION};
    my $old_version = $old->{VERSION};

    die "ERROR: can not upgrade from $old_version to n_ver" unless $new_version !~ $old_version;

    #------------------------------------
    # analyze function signatures
    #------------------------------------

    my $new_signatures = $new->{funcs};
    my $old_signatures = $old->{funcs};

    # create an unique hash map of the new signatures
    # to quickly determine if an old signature exists or not
    # in the new signatures.
    my %function_map = map { $_ => 1 } @{$new_signatures};

    for my $old_function (@{$old_signatures}) {

        # check if the old signature is in the new signature map
        my $exists = $function_map{$old_function} || '0';
        print "$exists\t$old_function\n" if $DEBUG;

        # Nothing to do, the signature is valid on the current version
        # Might not be the case because signature changes include
        #  - parameter names
        #  - different types of out parameters
        #  but the signature files do not catch that information (yet)
        next if $function_map{$old_function};

        # The function does NOT exist in the current version
        # -- Remove from the extension
        push @commands, "\n\nALTER EXTENSION pgrouting DROP FUNCTION $old_function;\n";
        push @commands, "DROP FUNCTION IF EXISTS $old_function;\n";
    }

    #------------------------------------
    # Special cases
    #------------------------------------

    push @commands, underscored($old_version, $new_version);
    push @commands, deprecated_on_2_1($old_version, $new_version);
    push @commands, deprecated_on_2_2($old_version, $new_version);
    push @commands, pgr_version($old_version, $new_version);
    push @commands, pgr_trsp($old_version, $new_version);
    push @commands, pgr_bddijkstra($old_version, $new_version);
    push @commands, pgr_gsoc_vrppdtw($old_version, $new_version);
    push @commands, pgr_astar($old_version, $new_version);
    push @commands, pgr_ksp($old_version, $new_version);
    push @commands, pgr_drivingdistance($old_version, $new_version);
    push @commands, pgr_edgedisjointpaths($old_version, $new_version);


    # analyze types

    my $ntype = $new->{types};
    my $otype = $old->{types};

    # create a hash like <name> => <column_list> for new types
    my %ntype_h = ();
    for my $x (@{$ntype}) {
        #$x =~ m/(\w+)(\([^\)]+\))$/;
        $ntype_h{lc($x)} = lc($x);
    }

    # check if old type exists with different column types
    for my $x (@{$otype}) {
        my $name = lc($x);
        if (!exists $ntype_h{$name}) {
            #types no longer used are droped form the extension
            push @commands, "ALTER EXTENSION pgrouting DROP TYPE $name;\n";
            push @commands, "DROP TYPE $name;\n";
        } else {
            push @types2remove, $name;
        }
    }

    # UGH! someone change the definition of the TYPE or reused an existing
    # TYPE name which is VERY BAD because other poeple might be dependent
    # on the old TYPE so we can DROP TYPE <type> CASCADE; or we might drop
    # a user's function. So juse DIE and maybe someone can resolve this
    die "ERROR: pgrouting TYPE changed! Cannot continue!\n" if $err;

    write_script($old_version, $new_version, \@types2remove, join('', @commands));
}


sub deprecated_on_2_1 {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    # This deprecated functions from so long ago should not being used by now
    # Dropping

    # file tested for 2.4 & 2.5
    if ($old_version =~ /$version_2_0/
            and $new_version !~ /$version_2_0/
            and $new_version =~ /$version_2/) {
        push @commands,  "\n\n ------------------------------------------\n";
        push @commands,  "--    New functions:  2.0\n";
        push @commands,  "-- Signature change:  2.1\n";
        push @commands,  "--       Deprecated:  2.1\n";
        push @commands,  "------------------------------------------\n";

        push @commands, "-- pgr_dijkstra:\n";
        push @commands, "-- $old_version: {      sql, source_id, target_id, directed, has_reverse_cost}\n";
        push @commands, "-- $new_version: {edges_sql, start_vid,   end_vid, directed, has_rcost}\n";
        push @commands, drop_special_case_function("pgr_dijkstra(text,integer,integer,boolean,boolean)",  "");
    }


    if ($old_version =~ /$version_2_0|$version_2_1/
            and $new_version !~ /$version_2_0|$version_2_1/
            and $new_version =~ /$version_2/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.0\n";
        push @commands,  "--       Deprecated:  2.1\n";
        push @commands,  "--       Deprecated:  2.1 & 2.2\n";
        push @commands,  "------------------------------------------\n";

        push @commands, "-- pgr_ksp\n";
        push @commands, "-- $old_version: {      sql, source_id, target_id, no_paths,has_reverse_cost}\n" if $old_version  =~ /$version_2_0/;
        push @commands, "-- $old_version: {      sql, start_vid,   end_vid, k,       has_rcost}\n"        if $old_version  =~ /$version_2_1/;
        push @commands, "-- $new_version: {edges_sql, start_vid,   end_vid, k,       has_rcost}\n";
        push @commands, drop_special_case_function("pgr_ksp(text,integer,integer,integer,boolean)",  "");
    }

    if ($old_version =~ /$version_2_0/
            and $new_version !~ /$version_2_0|$version_2_1|$version_2_4/
            and $new_version =~ /$version_2/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.0\n";
        push @commands,  "-- Signature change:  2.1\n";
        push @commands,  "--       Deprecated:  2.1 & 2.2\n";
        push @commands,  "------------------------------------------\n";

        push @commands, "-- pgr_pgr_drivingdistance\n";
        push @commands, "-- $old_version: {      sql, source_id, distance, has_reverse_cost}   \n" if $old_version =~ /$version_2_0/;
        push @commands, "-- $old_version: {      sql, source, distance, has_rcost}   \n"           if $old_version =~ /$version_2_1/;
        push @commands, "-- $new_version: {edges_sql,  source,    distance, directed, has_rcost}\n"; 
        push @commands, drop_special_case_function("pgr_drivingdistance(text,integer,double precision,boolean,boolean)",  "");
    }
    push @commands, drop_special_case_function("pgr_drivingdistance(text,bigint,double precision,boolean,boolean)",  "") if $old_version =~ /$version_2_1/ and $new_version =~ /$version_2_4/;
    return @commands;
}

sub deprecated_on_2_2 {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    # This deprecated functions from so long ago should not being used by now
    # Dropping

    if ($old_version =~ /$version_2_0|$version_2_1/
            and $new_version !~ /$version_2_0|$version_2_1/
            and $new_version =~ /$version_2/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.0\n";
        push @commands,  "-- Signature change:  2.2\n";
        push @commands,  "--       Deprecated:  2.2\n";
        push @commands,  "------------------------------------------\n";

        push @commands, "-- pgr_apspjohnson\n";
        push @commands, "-- $old_version: {      sql}\n";
        push @commands, "-- $new_version: {edges_sql}\n";
        push @commands, drop_special_case_function("pgr_apspjohnson(text)", "");

        push @commands, "-- pgr_apspwarshall\n";
        push @commands, "-- $old_version: {      sql, directed, has_reverse_cost}\n";
        push @commands, "-- $new_version: {edges_sql, directed, has_rcost}\n";
        push @commands, drop_special_case_function("pgr_apspwarshall(text,boolean,boolean)",  "");

        push @commands, "-- pgr_kdijkstrapath\n";
        push @commands, "-- $old_version: {sql,source_vid, target_vid, directed, has_reverse_cost}\n";
        push @commands, "-- $new_version: {sql,    source,    targets, directed, has_rcost} \n";
        push @commands, drop_special_case_function("pgr_kdijkstrapath(text,integer,integer[],boolean,boolean)",  "");

        push @commands, "-- pgr_kdijkstracost\n";
        push @commands, "-- $old_version: {sql,source_vid, target_vid, directed, has_reverse_cost}\n";
        push @commands, "-- $new_version: {sql,    source,    targets, directed, has_rcost} \n";
        push @commands, drop_special_case_function("pgr_kdijkstracost(text,integer,integer[],boolean,boolean)",  "");
    }


    return @commands;
}



sub pgr_version {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    # Out parameter changes:
    # Dropping

    if ($old_version =~ /$version_2_0|$version_2_1/
            and $new_version !~ /$version_2_0|$version_2_1/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.0\n";
        push @commands,  "-- Signature change:  2.2\n";
        push @commands,  "------------------------------------------\n";

        push @commands, "-- pgr_version\n";
        push @commands, "-- $old_version:  {version,tag,build,hash,branch,boost}\n";
        push @commands, "-- $new_version:  {version,tag,hash,branch,boost}\n";
        push @commands, drop_special_case_function("pgr_version()",  "");
    }

    return @commands;
}


sub pgr_trsp {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    # changes were so long ago and trsp is going to be deprecated eventually

    if ($old_version =~ /$version_2_0|$version_2_1/
            and $new_version !~ /$version_2_0|$version_2_1/
            and $new_version =~ /$version_2/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.0\n";
        push @commands,  "-- Signature change:  2.2\n";
        push @commands,  "------------------------------------------\n";

        push @commands, "-- pgr_trsp\n";
        push @commands, "-- $old_version:  {      sql, source_vid, target_vid, directed, has_reverse_cost, turn_restrict_sql}\n";
        push @commands, "-- $new_version:  {edges_sql,  start_vid,    end_vid, directed, has_rcost,        restrictions_sql}\n";
        my $update_command = "
UPDATE pg_proc SET
proargnames = '{\"edges_sql\",\"start_vid\",\"end_vid\",\"directed\",\"has_rcost\",\"restrictions_sql\"}'
WHERE proname = 'pgr_trsp'
    AND proargnames = '{\"sql\",\"source_vid\",\"target_vid\",\"directed\",\"has_reverse_cost\",\"turn_restrict_sql\"}';
";

        push @commands, $update_command;
        #push @commands, drop_special_case_function("pgr_trsp(text,integer,integer,boolean,boolean,text)",  "");
    }

    if ($old_version =~ /$version_2_1/
            and $new_version !~ /$version_2_1/
            and $new_version =~ /$version_2/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.0\n";
        push @commands,  "-- Signature (types) change:  2.2\n";
        push @commands,  "------------------------------------------\n";

        push @commands, "-- pgr_trspviaedges\n";
        push @commands, "-- $old_version:  {sql, eids, pcts, directed, has_reverse_cost,turn_restrict_sql} \n";
        push @commands, "-- $new_version:  {sql, eids, pcts, directed, has_rcost,       turn_restrict_sql}\n"; 
        push @commands, drop_special_case_function("pgr_trspviaedges(text,integer[],double precision[],boolean,boolean,text)", "");
    }

    return @commands;
}


sub pgr_bddijkstra {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    # too recent, updating

    if ($old_version =~ /$version_2_0|$version_2_1|$version_2_2|$version_2_3/
            and $new_version !~ /$version_2_0|$version_2_1|$version_2_2|$version_2_3/
            and $new_version =~ /$version_2/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.0\n";
        push @commands,  "-- Signature change:  2.4\n";
        push @commands,  "--       Deprecated:  2.4\n";
        push @commands,  "------------------------------------------\n";

        push @commands, "-- pgr_bddijkstra\n";
        push @commands, "-- $old_version: {      sql, source_vid, target_vid, directed, has_reverse_cost}   \n";
        push @commands, "-- $new_version: {edges_sql,  start_vid,    end_vid, directed, has_rcost}\n"; 
        my $update_command = "
UPDATE pg_proc SET
proargnames = '{\"edges_sql\",\"start_vid\",\"end_vid\",\"directed\",\"has_rcost\"}'
WHERE proname = 'pgr_bddijkstra'
    AND proargnames = '{\"sql\",\"source_vid\",\"target_vid\",\"directed\",\"has_reverse_cost\"}';
";

        push @commands, $update_command;
        #push @commands, drop_special_case_function("pgr_bddijkstra(text,integer,integer,boolean,boolean)",  "cannot change name of input parameter sql");
    }

    return @commands;
}



sub pgr_ksp {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_1/ and $new_version !~ /$version_2_1/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.1\n";
        push @commands,  "-- Signature change:  2.2\n";
        push @commands,  "------------------------------------------\n";

        push @commands, "-- pgr_ksp\n";
        push @commands, "-- $old_version:  {      sql, start_vid, end_vid, k, directed, heap_paths, seq, path_id, path_seq, node,edge, cost, agg_cost}\n";
        push @commands, "-- $new_version:  {edges_sql, start_vid, end_vid, k, directed, heap_paths, seq, path_id, path_seq, node,edge, cost, agg_cost}\n"; 
        push @commands, drop_special_case_function("pgr_ksp(text,bigint,bigint,integer,boolean,boolean)",  "");
    }

    return @commands;
}


sub underscored {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    # underscored are dropped

    if ($old_version =~ /$version_2_1/ and $new_version !~ /$version_2_1/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.1\n";
        push @commands,  "-- Signature change:  2.2\n";
        push @commands,  "------------------------------------------\n";

        push @commands, drop_special_case_function("_pgr_ksp(text,bigint,bigint,integer,boolean,boolean)",  "cannot change name of input parameter sql");
    }

    if ($old_version =~ /$version_2_1/ and $new_version =~ /$version_2_4/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.1\n";
        push @commands,  "-- Signature change:  2.4\n";
        push @commands,  "------------------------------------------\n";
        push @commands, drop_special_case_function("_pgr_dijkstra(text,bigint,bigint,boolean,boolean)",    "cannot change name of input parameter has_rcost");
        push @commands, drop_special_case_function("_pgr_dijkstra(text,bigint,anyarray,boolean,boolean)",  "cannot change name of input parameter has_rcost");
        push @commands, drop_special_case_function("_pgr_dijkstra(text,anyarray,bigint,boolean,boolean)",  "cannot change name of input parameter end_vids");
        push @commands, drop_special_case_function("_pgr_dijkstra(text,anyarray,anyarray,boolean,boolean)","cannot change name of input parameter has_rcost");
    }


    return @commands;
}


sub pgr_gsoc_vrppdtw {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    # too long ago
    # dropping

    if ($old_version =~ /$version_2_1|$version_2_2/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "-- New functions on 2.1\n";
        push @commands,  "-- Signature change on 2.3\n";
        push @commands,  "------------------------------------------\n";

        push @commands, drop_special_case_function("pgr_gsoc_vrppdtw(text,integer,integer)", "cannot change return type of existing function name");
    }


    return @commands;
}

sub pgr_astar {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_0|$version_2_1|$version_2_2/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "-- New functions on 2.0\n";
        push @commands,  "-- Signature change on 2.3\n";
        push @commands,  "-- Deprecated on 2.4\n";
        push @commands,  "------------------------------------------\n";

        push @commands, drop_special_case_function("pgr_astar(text,integer,integer,boolean,boolean)",  "cannot change name of input parameter sql");
    }

    return @commands;
}



sub pgr_drivingdistance {
    my ($old_version, $new_version) = @_;
    my @commands = ();


    if ($old_version =~ /$version_2_1|$version_2_2|$version_2_3/
            and $new_version !~ /$version_2_1|$version_2_2|$version_2_3/
            and $new_version =~ /$version_2/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.1\n";
        push @commands,  "-- Signature change:  2.4\n";
        push @commands,  "------------------------------------------\n";

        push @commands, "-- pgr_drivingdistance\n";
        push @commands, "-- $old_version:  {sql,start_v,distance,directed,equicost,seq,from_v,node,edge,cost,agg_cost}\n";
        push @commands, "-- $new_version:  {edges_sql,start_vids,distance,directed,equicost,seq,from_v,node,edge,cost,agg_cost}\n"; 

        push @commands, drop_special_case_function("pgr_drivingdistance(text,anyarray,double precision,boolean,boolean)",  "");
    }



    if ($old_version =~ /$version_2_1|$version_2_2|$version_2_3/
            and $new_version !~ /$version_2_1|$version_2_2|$version_2_3/
            and $new_version =~ /$version_2/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--     New function:  2.1\n";
        push @commands,  "-- Signature change:  2.2\n";
        push @commands,  "------------------------------------------\n";


        push @commands, "-- pgr_drivingdistance\n";
        push @commands, "-- $old_version:  {      sql, start_v,   distance, directed, seq, node, edge, cost, agg_cost}\n" if $old_version =~ /$version_2_1/;
        push @commands, "-- $old_version:  {edges_sql, start_v,   distance, directed, seq, node, edge, cost, agg_cost}\n" if $old_version =~ /$version_2_2|$version_2_3/;
        push @commands, "-- $new_version:  {edges_sql, start_vid, distance, directed, seq, node, edge, cost, agg_cost}\n"; 

        push @commands, drop_special_case_function("pgr_drivingdistance(text,bigint,double precision,boolean)",  "");
    }

    return @commands;
}




sub pgr_edgedisjointpaths {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_3|$version_2_4/
            and $new_version !~ /$version_2_3|$version_2_4/) {

        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--       New functions:  2.3\n";
        push @commands,  "--    Signature change:  2.5\n";
        push @commands,  "-- Inner query changed:  2.5\n";
        push @commands,  "------------------------------------------\n";

        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,bigint,bigint,boolean)",     "Row type defined by OUT parameters is different");
        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,bigint,anyarray,boolean)",   "Row type defined by OUT parameters is different");
        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,anyarray,bigint,boolean)",   "Row type defined by OUT parameters is different");
        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,anyarray,anyarray,boolean)", "Row type defined by OUT parameters is different");
    }

    return @commands;
}



sub drop_special_case_function {
    my ($function, $reason) = @_;
    my @commands = ();
    push @commands, "\nALTER EXTENSION pgrouting DROP FUNCTION $function;\n";
    push @commands, "DROP FUNCTION IF EXISTS $function;\n\n\n";
    return @commands;
}


# we have all the data we need, so write the extension update script
sub write_script {
    my ($old_version, $new_version, $types, $cmds) = @_;

    # open the extension update script or die if we can't
    open(OUT, ">$output_directory/pgrouting--$old_version--$new_version.sql.in")
    || die "ERROR: failed to create '$output_directory/pgrouting-pgrouting--$old_version--$new_version.sql.in' : $!\n";

    # write out the header and the commands to clean up the old extension
    print OUT <<EOF;
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- pgRouting extension upgrade from $old_version to $new_version
-- generated by tools/build-extension-update-files
-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -



\\echo Use "ALTER extension pgrouting update to '$new_version'" to load this file. \\quit


-------------------------------------
-- remove functions no longer in the $new_version extension
-------------------------------------


-- $cmds

-- now install the new extension

EOF

# open the new extension.sql file
# and load it into an array
open(IN, $curr_sql_file_name) ||
die "ERROR: Failed to find '$curr_sql_file_name' : $!\n";
my @file = <IN>;
close(IN);

# comment out the TYPEs that the new extension defines
# that already existed in the old extension
# so they will not abort the script
remove_types(\@file, $types);

# append the new extension SQL to the update script
print OUT "-- @file";
close(OUT);
print "  -- Created lib/pgrouting--$old_version--$new_version.sql.in\n";
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


