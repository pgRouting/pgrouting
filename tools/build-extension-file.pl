#!/usr/bin/perl -w
eval 'exec /usr/bin/perl -S $0 ${1+"$@"}'
    if 0; #$running_under_some_shell

# Extension files almost do not change
# Almost files in the src/*/sql are part of the extension
# The files indicated on the CMakelists of the subdirectory:
# Are going to be included (when not commented out)
# The generated file is going to be used to compare VS the generated file on cmake

use strict;
use Data::Dumper;
use File::Find();

my $DEBUG=0;

# only directories with sql directory
my @directories = qw(
    common

    dijkstra
    allpairs
    astar
    driving_distance
    ksp
    tsp


    alpha_shape
    bdAstar
    bdDijkstra
    trsp


    max_flow
    contraction

    pickDeliver

    vrp_basic
    withPoints


    topology
    label_graph
    costMatrix
    vrppdtw

    deprecated
    );


use vars qw/*name *dir *prune/;
*name   = *File::Find::name;
*dir    = *File::Find::dir;
*prune  = *File::Find::prune;


sub Usage {
    die "Usage:\nFrom the root of the repository:
    build-extension-file.pli version \n";
}


my $version = shift @ARGV || Usage();
$DEBUG = shift @ARGV || 0;


#my $out_file_name = "sql-scripts/pgrouting--$version.sql";
my $out_file_name = "tools/sql-update-scripts/pgrouting--$version.sql";
open(OUT, ">$out_file_name")
    || die "ERROR: failed to create '$out_file_name' : $!\n";


foreach  my $dir (@directories) {
    print "Processing $dir\n";
    my @sql_files = get_sql_files($dir);
    foreach my $file (@sql_files) {
        print "--  $file" if $DEBUG;
        my $contents = get_contents($file);
        $contents = eliminate_license($contents);
        print OUT "$contents";
    }
}

close (OUT);

exit 0;

sub get_sql_files {
    my ($directory) = @_;
    my $cmake_file = "src/$directory/sql/CMakeLists.txt";
    die "ERROR: Failed to find CMakeList.txt: $cmake_file\n" unless -e $cmake_file;

    # open the file
    open(IN, $cmake_file) || die "ERROR: Failed to open '$cmake_file'\n";

    my @sql_files = ();
    while (my $line = <IN>) {
        if ($line =~ /^\s*\${CMAKE_CURRENT_SOURCE_DIR}\//) {
            my $sql_filename = (split /\//, $line)[1];
            push @sql_files, "src/$directory/sql/$sql_filename";
        }
    }
    close(IN);
    return @sql_files;
}


sub get_contents {
    my ($file) = @_;
    local $/=undef;
    open(IN, $file) || die "ERROR: Failed to open '$file'\n";
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

    $contents =~ s/\${PGROUTING_LIBRARY_NAME}/libpgrouting-2.5/g;
    $contents =~ s/\${PGROUTING_VERSION}/$version/g;
    $contents =~ s/\${PGROUTING_FULL_VERSION}/v$version-dev/g;
    $contents =~ s/\${PGROUTING_GIT_HASH}//g;
    $contents =~ s/\${PGROUTING_GIT_BRANCH}//g;
    $contents =~ s/\${Boost_MAJOR_VERSION}.\${Boost_MINOR_VERSION}.\${Boost_SUBMINOR_VERSION}/1.54.0/g;
    return $contents;
}

