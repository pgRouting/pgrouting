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
my $version_3_0 = qr/(3.0.[\d+])/;
my $version_3_1 = qr/(3.1.[\d+])/;
my $version_2 = qr/(2.[\d+].[\d+])/;
my $version_3 = qr/(3.[\d+].[\d+])/;

my $current = $version_3_1;


sub Usage {
    die "Usage:\nFrom the cmake:
    build-extension-update-files1 <old_version>";
}

my $old_version = shift @ARGV || die "Missing: old version to convert";
my $DEBUG = "@PGROUTING_DEBUG@";
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
die "ERROR: 'build-extension-update-files1.pl' expected old version: 2.0.x ~~ 2.6.x\nHINT: Please check:\n  file: sql/CMakeLists.txt\n  Section: 'TARGET: update_files'"
    unless $old_version =~ /$version_2_0|$version_2_1|$version_2_2|$version_2_3|$version_2_4|$version_2_5|$version_2_6|$version_3_0|$version_3_1/;

die "ERROR: 'build-extension-update-files1.pl' expected version: 3.0.x\nHINT: ERROR on PGROUTING_VERSION variable\nor File sql/scripts/build-extension-update-files1.pl needs to be reviewed"
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

    push @commands, dijkstra($old_version, $new_version);
    push @commands, bddijkstra($old_version, $new_version);
    push @commands, allpairs($old_version, $new_version);
    push @commands, astar($old_version, $new_version);
    push @commands, bdastar($old_version, $new_version);
    push @commands, withpoints($old_version, $new_version);
    push @commands, ksp($old_version, $new_version);
    push @commands, flow($old_version, $new_version);
    push @commands, components($old_version, $new_version);
    push @commands, version($old_version, $new_version);
    push @commands, drivingDistance($old_version, $new_version);
    push @commands, vrp($old_version, $new_version);
    push @commands, topology($old_version, $new_version);
    push @commands, tsp($old_version, $new_version);

    #------------------------------------
    # analyze types
    #------------------------------------

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
            push @commands, "DROP TYPE $name CASCADE;\n";
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


sub version {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4|$version_2_3|$version_2_2/ and $new_version  =~ /$version_3/) {
        push @commands, drop_special_case_function("pgr_version()",  $old_version, $new_version);
    }
    return @commands;
}

sub bddijkstra {
    my ($old_version, $new_version) = @_;
    my @commands = ();


    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4/ and $new_version  =~ /$version_3/) {
        if ($old_version =~ /$version_2_6|$version_2_5/ and $new_version  =~ /$version_3/) {
            push @commands, drop_special_case_function("_pgr_bddijkstra(text,anyarray,anyarray,boolean,boolean)",  $old_version, $new_version);
            push @commands, drop_special_case_function("pgr_bddijkstra(text,bigint,anyarray,boolean)",  $old_version, $new_version);
        }
        my $update_command =  update_pg_proc(
            'pgr_bddijkstra',
            'edges_sql,start_vid,end_vid,directed,seq,path_seq,node,edge,cost,agg_cost',
            '"","","",directed,seq,path_seq,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_bddijkstra',
            'edges_sql,start_vid,end_vids,directed,seq,path_seq,end_vid,node,edge,cost,agg_cost',
            '"","","",directed,seq,path_seq,node,end_vid,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_bddijkstra',
            'edges_sql,start_vids,end_vid,directed,seq,path_seq,start_vid,node,edge,cost,agg_cost',
            '"","","",directed,seq,path_seq,start_vid,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_bddijkstra',
            'edges_sql,start_vids,end_vids,directed,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost',
            '"","","",directed,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost');
        push @commands, $update_command;

        # pgr_BDdijkstraCost
        $update_command =  update_pg_proc_short(
            'pgr_bddijkstracost',
             '"","","",directed,start_vid,end_vid,agg_cost');
        push @commands, $update_command;

        # pgr_BDdijkstraCostMatrix
        $update_command =  update_pg_proc(
            'pgr_bddijkstracostmatrix',
             'edges_sql,vids,directed,start_vid,end_vid,agg_cost',
             '"","",directed,start_vid,end_vid,agg_cost');
        push @commands, $update_command;
    }

    return @commands;
}

sub update_pg_proc_short {
    my ($func_name, $new_sig) = @_;
    my $update_command = "
UPDATE pg_proc SET
proargnames = '{$new_sig}'
WHERE proname = '$func_name';

    ";
    return $update_command;
}

sub update_pg_proc {
    my ($func_name, $old_sig, $new_sig) = @_;
    my $update_command = "
UPDATE pg_proc SET
proargnames = '{$new_sig}'
WHERE proname = '$func_name'
AND proargnames = '{$old_sig}';

    ";
    return $update_command;
}

sub withpoints {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4|$version_2_3|$version_2_2/ and $new_version  =~ /$version_3/) {
        my $update_command =  update_pg_proc(
            'pgr_withpoints',
             'edges_sql,points_sql,start_pid,end_pid,directed,driving_side,details,seq,path_seq,node,edge,cost,agg_cost',
             '"","","","",directed,driving_side,details,seq,path_seq,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_withpoints',
             'edges_sql,points_sql,start_pid,end_pids,directed,driving_side,details,seq,path_seq,end_pid,node,edge,cost,agg_cost',
             '"","","","",directed,driving_side,details,seq,path_seq,end_pid,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_withpoints',
             'edges_sql,points_sql,start_pids,end_pid,directed,driving_side,details,seq,path_seq,start_pid,node,edge,cost,agg_cost',
             '"","","","",directed,driving_side,details,seq,path_seq,start_pid,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_withpoints',
             'edges_sql,points_sql,start_pids,end_pids,directed,driving_side,details,seq,path_seq,start_pid,end_pid,node,edge,cost,agg_cost',
             '"","","","",directed,driving_side,details,seq,path_seq,start_pid,end_pid,node,edge,cost,agg_cost');
        push @commands, $update_command;

        # pgr_withPointsCost
        $update_command =  update_pg_proc_short(
            'pgr_withpointscost',
             '"","","","",directed,driving_side,start_pid,end_pid,agg_cost');
        push @commands, $update_command;

        #pgr_withPointsCostMatrix
        $update_command =  update_pg_proc(
            'pgr_withpointscostmatrix',
             'edges_sql,points_sql,pids,directed,driving_side,start_vid,end_vid,agg_cost',
             '"","","",directed,driving_side,start_vid,end_vid,agg_cost');
        push @commands, $update_command;

        # pgr_withPointsDD
        $update_command =  update_pg_proc(
            'pgr_withpointsdd',
             'edges_sql,points_sql,start_pid,distance,directed,driving_side,details,equicost,seq,start_vid,node,edge,cost,agg_cost',
             '"","","","",directed,driving_side,details,equicost,seq,start_vid,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_withpointsdd',
             'edges_sql,points_sql,start_pid,distance,directed,driving_side,details,seq,node,edge,cost,agg_cost',
             '"","","","",directed,driving_side,details,seq,node,edge,cost,agg_cost');
        push @commands, $update_command;
    }
    return @commands;
}


sub drivingDistance {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_3|$version_2_2/ and $new_version  =~ /$version_3/) {
        my $update_command =  update_pg_proc(
            '_pgr_drivingdistance',
             'edges_sql,start_vids,distance,directed,equicost,seq,start_v,node,edge,cost,agg_cost',
             'edges_sql,start_vids,distance,directed,equicost,seq,from_v,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_drivingdistance',
             'edges_sql,start_v,distance,directed,seq,node,edge,cost,agg_cost',
             '"","","",directed,seq,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_drivingdistance',
             'sql,start_v,distance,directed,equicost,seq,from_v,node,edge,cost,agg_cost',
             '"","","",directed,equicost,seq,from_v,node,edge,cost,agg_cost');
        push @commands, $update_command;
    }

    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4/ and $new_version  =~ /$version_3/) {

        push @commands, drop_special_case_function("pgr_drivingdistance(text,anyarray,double precision,boolean,boolean)",  $old_version, $new_version);
        push @commands, drop_special_case_function("pgr_drivingdistance(text,bigint,double precision,boolean)",  $old_version, $new_version);
    }
    return @commands;
}


sub topology {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4|$version_2_3|$version_2_2/ and $new_version  =~ /$version_3/) {
        my $update_command =  update_pg_proc(
            'pgr_nodenetwork',
             'edge_table,tolerance,id,the_geom,table_ending,rows_where,outall',
             '"","",id,the_geom,table_ending,rows_where,outall');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_createverticestable',
             'edge_table,the_geom,source,target,rows_where',
             '"",the_geom,source,target,rows_where');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_createtopology',
             'edge_table,tolerance,the_geom,id,source,target,rows_where,clean',
             '"","",the_geom,id,source,target,rows_where,clean');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_analyzegraph',
             'edge_table,tolerance,the_geom,id,source,target,rows_where',
             '"","",the_geom,id,source,target,rows_where');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_analyzeoneway',
             'edge_table,s_in_rules,s_out_rules,t_in_rules,t_out_rules,two_way_if_null,oneway,source,target',
             '"","","","","",two_way_if_null,oneway,source,target');
        push @commands, $update_command;
    }
    return @commands;
}


sub allpairs {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4|$version_2_3|$version_2_2/ and $new_version  =~ /$version_3/) {
        my $update_command =  update_pg_proc(
            'pgr_johnson',
             'edges_sql,directed,start_vid,end_vid,agg_cost',
             '"",directed,start_vid,end_vid,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_floydwarshall',
             'edges_sql,directed,start_vid,end_vid,agg_cost',
             '"",directed,start_vid,end_vid,agg_cost');
        push @commands, $update_command;
    }
    return @commands;
}



sub components {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_6|$version_2_5/ and $new_version  =~ /$version_3/) {
        push @commands, drop_special_case_function("pgr_connectedcomponents(text)",  $old_version, $new_version);
        push @commands, drop_special_case_function("pgr_strongcomponents(text)",  $old_version, $new_version);
        push @commands, drop_special_case_function("pgr_biconnectedcomponents(text)",  $old_version, $new_version);
        push @commands, drop_special_case_function("pgr_articulationpoints(text)",  $old_version, $new_version);
        push @commands, drop_special_case_function("pgr_bridges(text)",  $old_version, $new_version);
    }
    return @commands;
}


sub tsp {
    my ($old_version, $new_version) = @_;
    my @commands = ();


    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4|$version_2_3|$version_2_2/ and $new_version  =~ /$version_3/) {

        my $update_command =  update_pg_proc(
            'pgr_tsp',
            'matrix_row_sql,start_id,end_id,max_processing_time,tries_per_temperature,max_changes_per_temperature,max_consecutive_non_changes,initial_temperature,final_temperature,cooling_factor,randomize,seq,node,cost,agg_cost',
            '"",start_id,end_id,max_processing_time,tries_per_temperature,max_changes_per_temperature,max_consecutive_non_changes,initial_temperature,final_temperature,cooling_factor,randomize,seq,node,cost,agg_cost');
        push @commands, $update_command;
    }
    return @commands;
}

sub dijkstra {
    my ($old_version, $new_version) = @_;
    my @commands = ();


    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4|$version_2_3|$version_2_2/ and $new_version  =~ /$version_3/) {
        push @commands, drop_special_case_function("pgr_dijkstra(text,bigint,anyarray,boolean)",  $old_version, $new_version);

        my $update_command =  update_pg_proc(
            'pgr_dijkstra',
            'edges_sql,start_vid,end_vid,directed,seq,path_seq,node,edge,cost,agg_cost',
            '"","","",directed,seq,path_seq,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_dijkstra',
            'edges_sql,start_vid,end_vids,directed,seq,path_seq,end_vid,node,edge,cost,agg_cost',
            '"","","",directed,seq,path_seq,node,end_vid,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_dijkstra',
            'edges_sql,start_vids,end_vid,directed,seq,path_seq,start_vid,node,edge,cost,agg_cost',
            '"","","",directed,seq,path_seq,start_vid,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_dijkstra',
            'edges_sql,start_vids,end_vids,directed,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost',
            '"","","",directed,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost');
        push @commands, $update_command;

        # pgr_dijkstraCost
        $update_command =  update_pg_proc_short(
            'pgr_dijkstracost',
             '"","","",directed,start_vid,end_vid,agg_cost');
        push @commands, $update_command;

        # pgr_dijkstraCostMatrix
        $update_command =  update_pg_proc(
            'pgr_dijkstracostmatrix',
             'edges_sql,vids,directed,start_vid,end_vid,agg_cost',
             '"","",directed,start_vid,end_vid,agg_cost');
        push @commands, $update_command;

        # pgr_dijkstraVia
        $update_command =  update_pg_proc(
            'pgr_dijkstravia',
             'edges_sql,via_vertices,directed,strict,u_turn_on_edge,seq,path_id,path_seq,start_vid,end_vid,node,edge,cost,agg_cost,route_agg_cost',
             '"","",directed,strict,u_turn_on_edge,seq,path_id,path_seq,start_vid,end_vid,node,edge,cost,agg_cost,route_agg_cost');
        push @commands, $update_command;
    }
    return @commands;
}



sub ksp {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4|$version_2_3|$version_2_2/ and $new_version  =~ /$version_3/) {
        my $update_command =  update_pg_proc(
            'pgr_ksp',
             'edges_sql,start_vid,end_vid,k,directed,heap_paths,seq,path_id,path_seq,node,edge,cost,agg_cost',
             '"","","","",directed,heap_paths,seq,path_id,path_seq,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_withpointsksp',
             'edges_sql,points_sql,start_pid,end_pid,k,directed,heap_paths,driving_side,details,seq,path_id,path_seq,node,edge,cost,agg_cost',
             '"","","","","",directed,heap_paths,driving_side,details,seq,path_id,path_seq,node,edge,cost,agg_cost');
        push @commands, $update_command;
    }

    return @commands;
}

sub vrp {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4|$version_2_3|$version_2_2/ and $new_version  =~ /$version_3/) {
        my $update_command =  update_pg_proc(
            'pgr_vrponedepot',
             'order_sql,vehicle_sql,cost_sql,depot_id,oid,opos,vid,tarrival,tdepart',
             '"","","","",oid,opos,vid,tarrival,tdepart');
        push @commands, $update_command;
    }
    return @commands;
}

sub bdastar {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_6|$version_2_5/ and $new_version  =~ /$version_3/) {
        #pgr_BDaStarCostMatrix
        my $update_command =  update_pg_proc(
            'pgr_bdastarcostmatrix',
             'edges_sql,vids,directed,heuristic,factor,epsilon,start_vid,end_vid,agg_cost',
             '"","",directed,heuristic,factor,epsilon,start_vid,end_vid,agg_cost');
        push @commands, $update_command;
    }

    return @commands;
}

sub astar {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4|$version_2_3/ and $new_version  =~ /$version_3/) {
        my $update_command =  update_pg_proc(
            'pgr_astar',
             'edges_sql,start_vid,end_vid,directed,heuristic,factor,epsilon,seq,path_seq,node,edge,cost,agg_cost',
             '"","","",directed,heuristic,factor,epsilon,seq,path_seq,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_astar',
             'edges_sql,start_vid,end_vids,directed,heuristic,factor,epsilon,seq,path_seq,end_vid,node,edge,cost,agg_cost',
             '"","","",directed,heuristic,factor,epsilon,seq,path_seq,end_vid,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_astar',
             'edges_sql,start_vids,end_vid,directed,heuristic,factor,epsilon,seq,path_seq,start_vid,node,edge,cost,agg_cost',
             '"","","",directed,heuristic,factor,epsilon,seq,path_seq,start_vid,node,edge,cost,agg_cost');
        push @commands, $update_command;
        $update_command =  update_pg_proc(
            'pgr_astar',
             'edges_sql,start_vids,end_vids,directed,heuristic,factor,epsilon,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost',
             '"","","",directed,heuristic,factor,epsilon,seq,path_seq,start_vid,end_vid,node,edge,cost,agg_cost');
        push @commands, $update_command;

        #pgr_astarCost
        $update_command =  update_pg_proc_short(
            'pgr_astarcost',
             '"","","",directed,heuristic,factor,epsilon,start_vid,end_vid,agg_cost');
        push @commands, $update_command;

        #pgr_astarCostMatrix
        $update_command =  update_pg_proc(
            'pgr_astarcostmatrix',
             'edges_sql,vids,directed,heuristic,factor,epsilon,start_vid,end_vid,agg_cost',
             '"","",directed,heuristic,factor,epsilon,start_vid,end_vid,agg_cost');
        push @commands, $update_command;
    }

    return @commands;
}




sub flow {
    my ($old_version, $new_version) = @_;
    my @commands = ();

    if ($old_version =~ /$version_2_4|$version_2_3/ and $new_version  =~ /$version_3/) {
        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,bigint,bigint,boolean)",$old_version, $new_version);
        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,bigint,anyarray,boolean)",$old_version, $new_version);
        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,anyarray,bigint,boolean)",$old_version, $new_version);
        push @commands, drop_special_case_function("pgr_edgedisjointpaths(text,anyarray,anyarray,boolean)",$old_version, $new_version);
    }

    if ($old_version =~ /$version_2_6|$version_2_5/ and $new_version  =~ /$version_3/) {
        my $update_command =  update_pg_proc(
            'pgr_maxcardinalitymatch',
             'edges_sql,directed,seq,edge,source,target',
             '"",directed,seq,edge,source,target');
        push @commands, $update_command;
    }

    if ($old_version =~ /$version_2_6|$version_2_5|$version_2_4/ and $new_version  =~ /$version_3/) {
        my $update_command =  update_pg_proc(
            'pgr_maxflow',
             'edges_sql,source_vertices,sink_vertices',
             '"","",""');
        push @commands, $update_command;
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


