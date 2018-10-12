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

my $DEBUG="@PGROUTING_DEBUG@";

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
my $PGROUTING_SQL_FILES =  "@PGROUTING_SQL_FILES@";
my @sql_file = split(/;/, $PGROUTING_SQL_FILES);
my $out_file_name = "$working_directory/@PGROUTING_CURRENT_SQL_FILE@";

print "Working_directory $working_directory\n"      if $DEBUG;
print "out_file_name $out_file_name\n"      if $DEBUG;

open(OUT, ">", "$out_file_name")
    || die "@PGROUTING_CURRENT_SQL_FILE@ ERROR: failed to create: '$out_file_name' : $!\n";

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

