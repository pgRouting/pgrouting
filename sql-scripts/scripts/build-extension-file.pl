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

my $DEBUG="@PgRouting_DEBUG@";

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
    build-extension-file.pl version \n";
}


my $version = "@PGROUTING_VERSION@";
my $working_directory = "@CMAKE_CURRENT_BINARY_DIR@/..";
my $PgRouting_SQL_FILES =  shift @ARGV || 0;

print "debug status= $DEBUG\n" if $DEBUG;
print "working_directory $working_directory\n" if $DEBUG;
print "PgRouting_SQL_FILES $PgRouting_SQL_FILES\n" if $DEBUG;

my @sql_file = split(/ /, $PgRouting_SQL_FILES);



#my $out_file_name = "sql-scripts/pgrouting--$version.sql";
my $out_file_name = "@CMAKE_CURRENT_BINARY_DIR@/../pgrouting--$version.sql";
open(OUT, ">", "$out_file_name")
    || die "pgrouting--$version.sql ERROR: failed to create '$out_file_name' : $!\n";

print "Generating $out_file_name\n" if $DEBUG;


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
    print "trying ro open $file\n";
    die "ERROR: Failed to find: $file'\n" unless -f $file;
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

    $contents =~ s/\$\{PGROUTING_LIBRARY_NAME\}/libpgrouting-2.5/g;
    $contents =~ s/\$\{PGROUTING_VERSION\}/$version/g;
    $contents =~ s/\$\{PGROUTING_FULL_VERSION\}/v$version-dev/g;
    $contents =~ s/\$\{PGROUTING_GIT_HASH\}//g;
    $contents =~ s/\$\{PGROUTING_GIT_BRANCH\}//g;
    $contents =~ s/\$\{Boost_MAJOR_VERSION\}.\$\{Boost_MINOR_VERSION\}.\$\{Boost_SUBMINOR_VERSION\}/1.54.0/g;
    return $contents;
}

