#!/usr/bin/perl -w
eval 'exec /usr/bin/perl -S $0 ${1+"$@"}'
    if 0; #$running_under_some_shell


use strict;
use Data::Dumper;
use File::Find ();


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
my $version_2 = qr/(2.[\d+].[\d+])/;

my $current = $version_2_6;


sub Usage {
    die "Usage:\nFrom the cmake:
    build-extension-update-files1 <old_version>";
}

my $old_version = shift @ARGV || die "Missing: old version to convert";
my $DEBUG = "@PgRouting_DEBUG@";
my $version = "@PGROUTING_VERSION@";
my $signature_dir = '@CMAKE_CURRENT_BINARY_DIR@/../sigs';
my $output_directory = '@CMAKE_CURRENT_BINARY_DIR@/..';
my $curr_signature_file_name = "$signature_dir/pgrouting--$version.sig";
my $old_signature_file_name = "$signature_dir/pgrouting--$old_version.sig";
my $curr_sql_file_name = "$output_directory/pgrouting--$version.sql";



######################################################
#VERIFICATION SECTION
######################################################

# Verify Currently accepted old and new versions
die "ERROR: 'build-extension-update-files1.pl' expected old version: 2.0.x ~~ 2.5.x\nHINT: Please check:\n  file: sql/CMakeLists.txt\n  Section: 'TARGET: update_files'"
    unless $old_version =~ /$version_2_0|$version_2_1|$version_2_2|$version_2_3|$version_2_4|$version_2_5|$version_2_6/;

die "ERROR: 'build-extension-update-files1.pl' expected version: 2.5.x\nHINT: ERROR on PGROUTING_VERSION variable\nor File sql/scripts/build-extension-update-files1.pl needs to be reviewed"
    unless $version =~ /$current/;

die "ERROR: 'build-extension-update-files1.pl' can not upgrade from $old_version to $version\nHINT: Please check:\n  file: sql/CMakeLists.txt\n  Section: 'TARGET: update_files'" unless $version !~ $old_version;


# Verify directories exist
die "ERROR: Failed to find: input directory: '$signature_dir'\nHINT: 'sql/sigs' must exist in the repository" unless -d $signature_dir;
die "ERROR: Failed to find: output directory: '$output_directory'\nHINT: unexpected error, this should not happen" unless -d $output_directory;


# Verify current $version files exist.
die "ERROR: Failed to find: old signature file: '$old_signature_file_name'\n" unless -f $old_signature_file_name;
die "ERROR: Failed to find: current signature file: '$curr_signature_file_name'\n" unless -f $curr_signature_file_name;
die "ERROR: Failed to find: current sql file: '$curr_sql_file_name'\n" unless -f $curr_sql_file_name;

######################################################
# process section
######################################################
#
print "Building the updating files\n" if $DEBUG;

my $curr_signature = read_and_parse_signature_file($curr_signature_file_name);
my $old_signatures = read_and_parse_signature_file($old_signature_file_name);

print "\ngenerating $old_version upgrade file\n" if $DEBUG;
generate_upgrade_script($curr_signature, $old_signatures);

exit 0;


######################################################
# subroutines
######################################################

# read_and_parse_signature_file
# generate_upgrade_script
#
# Special case functions
# underscored($old_version, $new_version);
# deprecated_on_2_1($old_version, $new_version);
# deprecated_on_2_2($old_version, $new_version);
# pgr_version($old_version, $new_version);
# pgr_trsp($old_version, $new_version);
# pgr_bddijkstra($old_version, $new_version);
# pgr_gsoc_vrppdtw($old_version, $new_version);
# pgr_astar($old_version, $new_version);
# pgr_ksp($old_version, $new_version);
# pgr_drivingdistance($old_version, $new_version);
# pgr_edgedisjointpaths($old_version, $new_version);


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

    #------------------------------------
    # analyze function signatures
    #------------------------------------

    my $new_signatures = $new->{funcs};
    my $old_signatures = $old->{funcs};

    # create an unique hash map of the new signatures
    # to quickly determine if an old signature exists or not
    # in the new signatures.
    my %function_map = map { $_ => 1 } @{$new_signatures};


    for my $old_function (sort @{$old_signatures}) {

        # check if the old signature is in the new signature map
        my $exists = $function_map{$old_function} || '0';
        print "$old_version  **WARN: DROP $old_function (does not exists on $new_version)\n"
              if $DEBUG and not $exists and $old_function !~ /^_/;
        print "$old_version    INFO: DROP $old_function (does not exists on $new_version)\n"
              if $DEBUG and not $exists and $old_function =~ /^_/;

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
            #types no longer used are dropped form the extension
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
        push @commands, drop_special_case_function("pgr_dijkstra(text,integer,integer,boolean,boolean)",  $old_version, $new_version);
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
        push @commands, drop_special_case_function("pgr_ksp(text,integer,integer,integer,boolean)",  $old_version, $new_version);
    }

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
        push @commands, drop_special_case_function("pgr_apspjohnson(text)", $old_version, $new_version);

        push @commands, "-- pgr_apspwarshall\n";
        push @commands, "-- $old_version: {      sql, directed, has_reverse_cost}\n";
        push @commands, "-- $new_version: {edges_sql, directed, has_rcost}\n";
        push @commands, drop_special_case_function("pgr_apspwarshall(text,boolean,boolean)",  $old_version, $new_version);

        push @commands, "-- pgr_kdijkstrapath\n";
        push @commands, "-- $old_version: {sql,source_vid, target_vid, directed, has_reverse_cost}\n";
        push @commands, "-- $new_version: {sql,    source,    targets, directed, has_rcost} \n";
        push @commands, drop_special_case_function("pgr_kdijkstrapath(text,integer,integer[],boolean,boolean)",  $old_version, $new_version);

        push @commands, "-- pgr_kdijkstracost\n";
        push @commands, "-- $old_version: {sql,source_vid, target_vid, directed, has_reverse_cost}\n";
        push @commands, "-- $new_version: {sql,    source,    targets, directed, has_rcost} \n";
        push @commands, drop_special_case_function("pgr_kdijkstracost(text,integer,integer[],boolean,boolean)",  $old_version, $new_version);
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
        push @commands, drop_special_case_function("pgr_version()",  $old_version, $new_version);
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
        #push @commands, drop_special_case_function("pgr_trsp(text,integer,integer,boolean,boolean,text)",  $old_version, $new_version);
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
        push @commands, drop_special_case_function("pgr_trspviaedges(text,integer[],double precision[],boolean,boolean,text)", $old_version, $new_version);
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
        #push @commands, drop_special_case_function("pgr_bddijkstra(text,integer,integer,boolean,boolean)",  $old_version, $new_version);
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
        push @commands, drop_special_case_function("pgr_ksp(text,bigint,bigint,integer,boolean,boolean)",  $old_version, $new_version);
    }

    return @commands;
}


sub underscored {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    # underscored are dropped
    # Users should not be using this functions

    if ($old_version =~ /$version_2_1/ and $new_version !~ /$version_2_1/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.1\n";
        push @commands,  "-- Signature change:  2.2\n";
        push @commands,  "------------------------------------------\n";

        push @commands, drop_special_case_function("_pgr_ksp(text,bigint,bigint,integer,boolean,boolean)", $old_version, $new_version);
    }

    if ($old_version =~ /$version_2_1/ and $new_version =~ /$version_2_4/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--    New functions:  2.1\n";
        push @commands,  "-- Signature change:  2.4\n";
        push @commands,  "------------------------------------------\n";
        push @commands, drop_special_case_function("_pgr_dijkstra(text,bigint,bigint,boolean,boolean)",    $old_version, $new_version);
        push @commands, drop_special_case_function("_pgr_dijkstra(text,bigint,anyarray,boolean,boolean)",  $old_version, $new_version);
        push @commands, drop_special_case_function("_pgr_dijkstra(text,anyarray,bigint,boolean,boolean)",  $old_version, $new_version);
        push @commands, drop_special_case_function("_pgr_dijkstra(text,anyarray,anyarray,boolean,boolean)",$old_version, $new_version);
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
        push @commands,  "--   New function: 2.1\n";
        push @commands,  "-- (types) change: 2.3\n";
        push @commands,  "------------------------------------------\n";

        push @commands, drop_special_case_function("pgr_gsoc_vrppdtw(text,integer,integer)", $old_version, $new_version);
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

        push @commands, drop_special_case_function("pgr_astar(text,integer,integer,boolean,boolean)",  $old_version, $new_version);
    }

    return @commands;
}



sub pgr_drivingdistance {
    my ($old_version, $new_version) = @_;
    my @commands = ();


    if ($old_version =~ /$version_2_1/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--       New functions:  2.0\n";
        push @commands,  "--    Signature change:  2.1\n";
        push @commands,  "------------------------------------------\n";
        push @commands, "-- $old_version: {sql,source,distance,directed,has_rcost}\n";
        push @commands, "-- $new_version: {edges_sql,source,distance,directed,has_rcost}\n";

        my $update_command = "
UPDATE pg_proc SET
proargnames = '{\"edges_sql\",\"source\",\"distance\",\"directed\",\"has_rcost\"}'
WHERE proname = 'pgr_drivingdistance'
    AND proargnames = '{\"sql\",\"source\",\"distance\",\"directed\",\"has_rcost\"}';
";

        push @commands, $update_command;

        #push @commands, drop_special_case_function("pgr_drivingdistance(text,bigint,double precision,boolean,boolean)",  $old_version, $new_version);
    }


    if ($old_version =~ /$version_2_1|$version_2_2|$version_2_3/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--       New functions:  2.1\n";
        push @commands,  "--    Signature change:  2.4\n";
        push @commands,  "------------------------------------------\n";
        push @commands, "-- $old_version: {      sql,start_v,  distance,directed,seq,node,edge,cost,agg_cost}\n" if $old_version =~ /$version_2_1/;
        push @commands, "-- $old_version: {edges_sql,start_v,  distance,directed,seq,node,edge,cost,agg_cost}\n" if $old_version =~ /$version_2_2|$version_2_3/;
        push @commands, "-- $new_version: {edges_sql,start_vid,distance,directed,seq,node,edge,cost,agg_cost}\n";

        my $update_command = "
UPDATE pg_proc SET
proargnames = '{\"edges_sql\",\"start_vid\",\"distance\",\"directed\",\"seq\",\"node\",\"edge\",\"cost\",\"agg_cost\"}'
WHERE proname = 'pgr_drivingdistance'";
        push @commands, $update_command;

        $update_command = "
    AND proargnames = '{\"sql\",\"start_v\",\"distance\",\"directed\",\"seq\",\"node\",\"edge\",\"cost\",\"agg_cost\"}';
" if $old_version =~ /$version_2_1/;

        $update_command = "
    AND proargnames = '{\"edges_sql\",\"start_v\",\"distance\",\"directed\",\"seq\",\"node\",\"edge\",\"cost\",\"agg_cost\"}';
" if $old_version =~ /$version_2_2|$version_2_3/;

        push @commands, $update_command;

        #push @commands, drop_special_case_function("pgr_drivingdistance(text,bigint,double precision,boolean)",  $old_version, $new_version);
    }


    if ($old_version =~ /$version_2_1|$version_2_2|$version_2_3/) {
        push @commands,  "\n\n------------------------------------------\n";
        push @commands,  "--       New functions:  2.1\n";
        push @commands,  "--    Signature change:  2.4\n";
        push @commands,  "------------------------------------------\n";
        push @commands, "-- $old_version: {sql,      start_v,   distance,directed,equicost,seq,from_v,node,edge,cost,agg_cost}\n";
        push @commands, "-- $new_version: {edges_sql,start_vids,distance,directed,equicost,seq,from_v,node,edge,cost,agg_cost}\n";

        my $update_command = "
UPDATE pg_proc SET
proargnames = '{\"edges_sql\",\"start_vids\",\"distance\",\"directed\",\"equicost\",\"seq\",\"from_v\",\"node\",\"edge\",\"cost\",\"agg_cost\"}'
WHERE proname = 'pgr_drivingdistance'
    AND proargnames = '{\"sql\",\"start_v\",\"distance\",\"directed\",\"equicost\",\"seq\",\"from_v\",\"node\",\"edge\",\"cost\",\"agg_cost\"}';
";

        push @commands, $update_command;

        #push @commands, drop_special_case_function("pgr_drivingdistance(text,anyarray,double precision,boolean,boolean)",  $old_version, $new_version);
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

        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,bigint,bigint,boolean)",     $old_version, $new_version);
        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,bigint,anyarray,boolean)",   $old_version, $new_version);
        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,anyarray,bigint,boolean)",   $old_version, $new_version);
        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,anyarray,anyarray,boolean)", $old_version, $new_version);
    }

    return @commands;
}



sub drop_special_case_function {
    my ($function, $old_version, $new_version) = @_;
    my @commands = ();

    print "$old_version  **WARN: DROP $function (something changed for $new_version)\n"
              if $DEBUG;
    push @commands, "\nALTER EXTENSION pgrouting DROP FUNCTION $function;\n";
    push @commands, "DROP FUNCTION IF EXISTS $function;\n\n\n";
    return @commands;
}


sub write_script {
    my ($old_version, $new_version, $types, $commands) = @_;

    my $contents = get_current_sql($types);

    open(OUT, ">$output_directory/pgrouting--$old_version--$new_version.sql")
    || die "ERROR: failed to create '$output_directory/pgrouting-pgrouting--$old_version--$new_version.sql' : $!\n";


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


$commands

$contents

EOF

    close(OUT);
    print "  -- Created lib/pgrouting--$old_version--$new_version.sql.in\n" if $DEBUG;
}


sub get_current_sql {
    my ($types) = @_;

    open(IN, $curr_sql_file_name) ||
    die "ERROR: Failed to find '$curr_sql_file_name' : $!\n";
    my @file = <IN>;
    close(IN);
    my $contents = join('', @file);

    #remove existing types
    for my $type (@{$types}) {
        $contents =~ s{
            create\s+type\s+$type
            .*?
            \);
        }[]gsxi;
    }


    $contents =~ s/\\echo Use "CREATE EXTENSION pgrouting" to load this file. \\quit//;

    return $contents
}


