#! /usr/bin/perl -w

=pod
File: doc_queries_generator.pl

Copyright (c) 2013 pgRouting developers

Function contributors:
  	Celia Virginia Vergara Castillo
  	Stephen Woodbridge
  	Vadim Zhukov
	Nagase Ko

Mail:

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

use strict;
use lib './';
use File::Find ();
use File::Basename;
use Data::Dumper;
use Time::HiRes qw(gettimeofday tv_interval);
$Data::Dumper::Sortkeys = 1;

# for the convenience of &wanted calls, including -eval statements:
use vars qw/*name *dir *prune/;
*name   = *File::Find::name;
*dir    = *File::Find::dir;
*prune  = *File::Find::prune;

my $POSGRESQL_MIN_VERSION = '12';
my $DOCUMENTATION = 0;
my $DATA = 0;
my $VERBOSE = 0;
my $LEVEL = "NOTICE";
my $FORCE = 0;

my $DBNAME = "pgr_test__db__test";
my $DBUSER;
my $DBHOST;
my $DBPORT;

sub Usage {
    die "Usage: doc_queries_generator.pl -pgver vpg -pgisver vpgis -psql /path/to/psql\n" .
    "       -alg 'dir'          - directory to select which algorithm subdirs to test\n" .
    "       -pgver version      - postgresql version\n" .
    "       -pghost host        - postgresql host or socket directory to use\n" .
    "       -pgport port        - postgresql port to use\n" .
    "       -pguser username    - postgresql user role to use\n" .
    "       -dbname name        - Database name defaults to $DBNAME\n" .
    "       -pgrver version     - pgrouting version. (Not all compares will pass)\n" .
    "       -psql /path/to/psql - optional path to psql\n" .
    "       -data               - only install the sampledata.\n" .
    "       -l(evel)  NOTICE    - client_min_messages value. Defaults to $LEVEL. other values can be WARNING, DEBUG3, etc\n" .
    "       -c(lean)            - dropdb before running.\n" .
    "       -doc(umentation)    - ONLY generate documentation examples. LEVEL is set to NOTICE\n" .
    "       -v(erbose)          - verbose messages of the execution\n" .
    "       -h(elp)             - help\n";
}

print "RUNNING: doc_queries_generator.pl " . join(" ", @ARGV) . "\n";

my ($vpg, $vpgr, $psql);
my $alg = '';
my @testpath = ("docqueries/");
my @test_direcotry = ();
my $clean;


while (my $a = shift @ARGV) {
    if ( $a eq '-pgver') {
        $vpg   = shift @ARGV || Usage();
    }
    elsif ($a eq '-pghost') {
        $DBHOST = shift @ARGV || Usage();
    }
    elsif ($a eq '-pgport') {
        $DBPORT = shift @ARGV || Usage();
    }
    elsif ($a eq '-pguser') {
        $DBUSER = shift @ARGV || Usage();
    }
    elsif ($a eq '-pgrver') {
        $vpgr = shift @ARGV || Usage();
    }
    elsif ($a eq '-alg') {
        $alg = shift @ARGV || Usage();
        @testpath = ("$alg");
    }
    elsif ($a eq '-dbname') {
        $DBNAME = shift @ARGV || Usage();
    }
    elsif ($a eq '-psql') {
        $psql = shift @ARGV || Usage();
        die "'$psql' is not executable!\n"
        unless -x $psql;
    }
    elsif ($a eq '-data') {
        $DATA = 1;
    }
    elsif ($a =~ /^-h/) {
        Usage();
    }
    elsif ($a =~ /^-c/i) {
        $clean = 1;
    }
    elsif ($a =~ /^-l(evel)?/i) {
        $LEVEL = shift @ARGV || Usage();
        print "The level $LEVEL\n";
    }
    elsif ($a =~ /^-v/i) {
        $VERBOSE = 1;
    }
    elsif ($a =~ /^-force/i) {
        $FORCE = 1;
    }
    elsif ($a =~ /^-doc(umentation)?/i) {
        $DOCUMENTATION = 1;
    }
    else {
        warn "Error: unknown option '$a'\n";
        Usage();
    }
}

# documentation gets NOTICE
$LEVEL = "NOTICE" if $DOCUMENTATION;

my $connopts = "";
$connopts .= " -U $DBUSER" if defined $DBUSER;
$connopts .= " -h $DBHOST" if defined $DBHOST;
$connopts .= " -p $DBPORT" if defined $DBPORT;
print "connoptions '$connopts'\n" if $VERBOSE;

%main::tests = ();
my @cfgs = ();
my %stats = (z_pass=>0, z_fail=>0, z_crash=>0,RunTimeTotal=>0);
my $TMP = "/tmp/pgr-test-runner-$$";
my $TMP2 = "/tmp/pgr-test-runner-$$-2";
my $TMP3 = "/tmp/pgr-test-runner-$$-3";

if (! $psql) {
    $psql = findPsql() || die "ERROR: can not find psql, specify it on the command line.\n";
}

my $OS = "$^O";
if (length($psql)) {
    if ($OS =~ /msys/
        || $OS =~ /MSWin/) {
        $psql = "\"$psql\"";
    }
}
print "Operative system found: $OS\n";


createTestDB($DBNAME);

# Load the sample data & any other relevant data files
mysystem("$psql $connopts -A -t -q -f tools/testers/sampledata.sql $DBNAME >> $TMP2 2>\&1 ");
mysystem("$psql $connopts -A -t -q -f tools/testers/lc101.pg $DBNAME >> $TMP2 2>\&1 ");

if ($DATA) {exit 0;};

# Traverse desired filesystems
File::Find::find({wanted => \&want_tests}, @testpath);

die "Error: no queries files found. Run this command from the top path of pgRouting repository!\n" unless @cfgs;

$vpg = '' if ! $vpg;

# cfgs = SET of configuration file names
# c  one file in cfgs
# print join("\n",@cfgs),"\n";
for my $c (@cfgs) {
    my $found = 0;

    print "test.conf = $c\n" if $VERBOSE;

    # load the config file for the tests
    require $c;

    print Data::Dumper->Dump([\%main::tests],['test']) if $VERBOSE;

    run_test($c, $main::tests{any});
    $found++;

    if (! $found) {
        $stats{$c} = "No files found for '$c'!";
    }
}

print Data::Dumper->Dump([\%stats], ['stats']);

unlink $TMP;
unlink $TMP2;
unlink $TMP3;

if ($stats{z_crash} > 0 || $stats{z_fail} > 0) {
    exit 1;  # signal we had failures
}

exit 0;      # signal we passed all the tests


# file  one file in cfgs
# t  contents of array that has keys comment, data and test
sub run_test {
    my $confFile = shift;
    my $t = shift;

    my $dir = dirname($confFile);

    # There is data to load relative to the directory
    for my $datafile (@{$t->{data}}) {
        mysystem("$psql $connopts -A -t -q -f '$dir/$datafile' $DBNAME >> $TMP2 2>\&1 ");
    }

    for my $file (@{$t->{files}}) {
        process_single_test($file, $dir, $DBNAME);
    }

    # Just in case but its not used
    if ($OS =~/msys/ || $OS=~/MSW/ || $OS =~/cygwin/) {
        for my $x (@{$t->{windows}}) {
            process_single_test($x, $dir, $DBNAME)
        }
    } elsif ($OS=~/Mac/ ||  $OS=~/dar/) {
        for my $x (@{$t->{macos}}) {
            process_single_test($x, $dir, $DBNAME)
        }
    } else {
        for my $x (@{$t->{linux}}) {
            process_single_test($x, $dir, $DBNAME)
        }
    }
}

# file: file to be processed. Example: johnson.pg
# dir: apth of the file. Example: docqueries/allpairs/
# database: the database name: Example: pgr_test__db__test
# each tests will use clean data

sub process_single_test{
    my $file = shift;
    my $dir = shift;
    my $database = shift;

    (my $filename = $file) =~ s/((\.[^.\s]+)+)$//;
    my $inputFile = "$dir/$file";
    my $resultsFile = "$dir/$filename.result";

    print "Processing $inputFile";
    my $t0 = [gettimeofday];

    # Load the sample data & any other relevant data files
    mysystem("$psql $connopts -A -t -q -f tools/testers/sampledata.sql $DBNAME >> $TMP2 2>\&1 ");
    mysystem("$psql $connopts -A -t -q -f tools/testers/lc101.pg $DBNAME >> $TMP2 2>\&1 ");

    # TIN = test input file
    open(TIN, "$inputFile") || do {
        print "\tFAILED: could not open '$inputFile \n";
        $stats{"$inputFile"} = "FAILED: could not open '$inputFile' : $!";
        $stats{z_fail}++;
        return;
    };


    # Processing is handled kinda like a file
    # Where the commands are stored on PSQL file
    # When the PSQL is closed is when everything gets executed

    # Connect to the database with PSQL
    if ($DOCUMENTATION) {
        # For rewriting the results files
        # Do the rewrite or store FAILURE
        open(PSQL, "|$psql $connopts --set='VERBOSITY terse' -e $database > $resultsFile 2>\&1 ") || do {
            $stats{"$inputFile"} = "FAILED: could not open connection to db : $!";
            $stats{z_fail}++;
            next;
        };
    } else {
        # For comparing the result
        # Create temp file with current results
        open(PSQL, "|$psql $connopts  --set='VERBOSITY terse' -e $database > $TMP 2>\&1 ") || do {
            $stats{"$inputFile"} = "FAILED: could not open connection to db : $!";
            $stats{z_fail}++;
            next;
        };
    }

    # Read the whole (input) file into the array @d
    my @queries = ();
    @queries = <TIN>;

    print PSQL "BEGIN;\n";
    print PSQL "SET client_min_messages TO $LEVEL;\n";
    # prints the whole file stored in @queries
    print PSQL @queries;
    print PSQL "\nROLLBACK;";

    # executes everything
    close(PSQL);

    #closes the input file  /TIN = test input
    close(TIN);

    my $runTime = tv_interval($t0, [gettimeofday]);
    print "\tRun time: $runTime";
    $stats{RunTimeTotal} += $runTime;

    if ($DOCUMENTATION) {
        # convert tabs to spaces
        print "\n";
        my $cmd = q(perl -pi -e 's/[ \t]+$//');
        $cmd .= " $resultsFile";
        mysystem( $cmd );
        return;
    }

    if (! -f "$resultsFile") {
        $stats{"$inputFile"} = "\nFAILED: '$resultsFile` file missing : $!";
        $stats{z_fail}++;
        next;
    }

    # diff ignore white spaces when comparing
    my $originalDiff = `diff -w '$resultsFile' '$TMP' `;

    #looks for removing leading blanks and trailing blanks
    $originalDiff =~ s/^\s*|\s*$//g;
    if ($originalDiff =~ /connection to server was lost/) {
        # when the server crashed
        $stats{"$inputFile"} = "CRASHED SERVER: $originalDiff";
        $stats{z_crash}++;
        # allow the server some time to recover from the crash
        warn "CRASHED SERVER: '$inputFile', sleeping 5 ...\n";
        sleep 20;
    } elsif (length($originalDiff)) {
        # Things changed print the diff
        $stats{"$inputFile"} = "FAILED: $originalDiff";
        $stats{z_fail}++ unless $LEVEL ne "NOTICE";
        print "\t FAIL\n";
    } else {
        $stats{z_pass}++;
        print "\t PASS\n";
    }
}

sub createTestDB {
    print "-> createTestDB\n" if $VERBOSE;
    my $databaseName = shift;
    dropTestDB() if $clean && dbExists($databaseName);

    my $template;

    my $dbver = getServerVersion();
    my $dbshare = getSharePath($dbver);

    if ($VERBOSE) {
        print "\tDBVERSION: $dbver\n";
        print "\tDBSHARE: $dbshare\n";
    }

    die "
    Unsupported postgreSQL version $dbver
    Minimum requirement is $POSGRESQL_MIN_VERSION version
    Use -force to force the tests\n"
    unless version_greater_eq($dbver, $POSGRESQL_MIN_VERSION);

    # Create a database with postgis installed in it
    mysystem("createdb $connopts $databaseName") if !dbExists($databaseName);
    die "ERROR: Failed to create database '$databaseName'!\n" unless dbExists($databaseName);
    my $encoding = '';
    if ($OS =~ /msys/
        || $OS =~ /MSWin/) {
        $encoding = "SET client_encoding TO 'UTF8';";
    }

    # Install pgrouting into the database
    my $myver = '';
    if ($vpgr) {
        $myver = " VERSION '$vpgr'";
    }


    mysystem("$psql $connopts -c \"DROP EXTENSION IF EXISTS pgrouting $myver\" $databaseName ");

    print "Installing pgrouting extension $myver\n" if $VERBOSE;
    mysystem("$psql $connopts -c \"CREATE EXTENSION pgrouting $myver CASCADE\" $databaseName");

    # Verify pgrouting was installed

    my $pgrv = `$psql $connopts -c "select pgr_version()" $databaseName`;
    die "ERROR: failed to install pgrouting into the database!\n" unless $pgrv;

    print `$psql $connopts -c "select version();" $databaseName `, "\n";
    print `$psql $connopts -c "select postgis_full_version();" $databaseName `, "\n";
    print `$psql $connopts -c "select pgr_full_version();" $databaseName `, "\n";
}

sub dropTestDB {
    mysystem("dropdb $connopts $DBNAME");
}

sub version_greater_eq {
    my ($a, $b) = @_;

    return 0 if !$a || !$b;

    my @a = split(/\./, $a);
    my @b = split(/\./, $b);

    my $va = 0;
    my $vb = 0;

    while (@a || @b) {
        $a = shift @a || 0;
        $b = shift @b || 0;
        $va = $va*1000+$a;
        $vb = $vb*1000+$b;
    }

    return 0 if $va < $vb;
    return 1;
}


sub getServerVersion {
    print "-> getServerVersion\n" if $VERBOSE;
    my $v = `$psql $connopts -q -t -c "select version()" postgres`;
    print "\t$psql $connopts -q -t -c \"select version()\" postgres\n    # RETURNED: $v\n" if $VERBOSE;
    if ($v =~ m/PostgreSQL (\d+(\.\d+)?)/) {
        my $version = $1 + 0;
        print "\tGot: $version\n" if $VERBOSE;
        $version = int($version) if $version >= 10;
        print "\tGot: $version\n" if $VERBOSE;
        return $version;
    }
    return undef;
}

sub dbExists {
    my $dbname = shift;

    my $isdb = `$psql $connopts -l | grep $dbname`;
    $isdb =~ s/^\s*|\s*$//g;
    return length($isdb);
}

sub findPsql {
    my $psql = `which psql`;
    $psql =~ s/^\s*|\s*$//g;
    print "which psql = $psql\n" if $VERBOSE;
    return length($psql)?$psql:undef;
}

# getSharePath is complicated by the fact that on Debian we can have multiple
# versions installed in a cluster. So we get the DB version by connection,
# to the port for the server we want. Then we get the share path for the
# newest version od pg installed on the cluster. And finally we change the
# in the path to the version of the server.

sub getSharePath {
    my $pg = shift;

    my $share;
    my $isdebian = -e "/etc/debian_version";
    my $pg_config = `which pg_config`;
    $pg_config =~ s/^\s*|\s*$//g;
    print "which pg_config = $pg_config\n" if $VERBOSE;
    if (length($pg_config)) {
        $share = `"$pg_config" --sharedir`;
        $share =~ s/^\s*|\s*$//g;
        if ($isdebian) {
            $share =~ s/(\d+(\.\d+)?)$/$pg/;
            if (length($share) && -d $share) {
                return $share;
            }
        } else {
            return "$share"
        }
    }
    die "Could not determine the postgresql version" unless $pg;
    $pg =~ s/^(\d+(\.\d+)).*$/$1/;
    $share = "/usr/share/postgresql/$pg";
    return $share if -d $share;
    $share = "/usr/local/share/postgresql/$pg";
    return $share if -d $share;
    die "Could not determine the postgresql share dir for ($pg)!\n";
}

sub mysystem {
    my $cmd = shift;
    print "$cmd\n" if $VERBOSE;
    system($cmd);
}

sub want_tests {
    /^test\.conf\z/s &&
    push @cfgs, $name;
}


