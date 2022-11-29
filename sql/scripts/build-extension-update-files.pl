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
    }

    #------------------------------------
    # Special cases
    #------------------------------------


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

    $contents  =~ s/\\echo Use "CREATE EXTENSION pgrouting" to load this file. \\quit//;

    my %seen = ();
    while ($contents =~ /--v([\d+].[\d+])/g) {  # scalar context
        $seen{$1}++;
    }
    my @uniq = keys %seen;

=pod
    For example:
    When the function was first created on version 3.3
    * the update is from version 3.2 <= 3.3
    The function use: CREATE

    * the update is from version 3.4 not <= 3.3
    otherwise the function use: CREATE OR REPLACE


    Change based on old_version from

    --v3.x
    CREATE FUNCTION

    to

    --v3.x
    CREATE OR REPLACE FUNCTION
=cut

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

=pod
    Example
    --v3.3
    CREATE FUNCTION pgr_maxcardinalitymatch(FOO text, BAR boolean)

    But on v4.0 the parameter name changed then
    --v4.0
    CREATE FUNCTION pgr_maxcardinalitymatch(text, BAR boolean)

    The function needs to be dropped and the version comment to be updated to when it was changed.

    if ($old_minor >= 3.3 and $old_minor < 4.0) {
        push @commands, drop_special_case_function("pgr_maxcardinalitymatch(text,boolean)");
    }
=cut
sub drop_special_case_function {
    my ($function) = @_;
    my @commands = ();

    push @commands,  "-- $old_version  **WARN: DROP $function (something changed for $new_version)\n" if $DEBUG;
    push @commands, "\nALTER EXTENSION pgrouting DROP FUNCTION $function;\n";
    push @commands, "DROP FUNCTION IF EXISTS $function;\n\n\n";
    return @commands;
}


