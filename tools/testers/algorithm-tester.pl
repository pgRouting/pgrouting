#! /usr/bin/perl -w

=pod
File: aplgorithm testes

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

my $POSGRESQL_MIN_VERSION = '9.2';
my $DOCUMENTATION = 0;
my $INTERNAL_TESTS = 0;
my $VERBOSE = 0;
my $DRYRUN = 0;
my $DEBUG = 0;
my $DEBUG1 = 0;
my $FORCE = 0;

my $DBNAME = "pgr_test__db__test";
my $DBUSER;
my $DBHOST;
my $DBPORT;

sub Usage {
    die "Usage: algorithm-tester.pl -pgver vpg -pgisver vpgis -psql /path/to/psql\n" .
    "       -pgver vpg          - postgresql version\n" .
    "       -pghost host        - postgresql host or socket directory to use\n" .
    "       -pgport port        - postgresql port to use\n" .
    "       -pguser username    - postgresql user role to use\n" .
    "       -pgisver vpgis      - postgis version\n" .
    "       -pgrver vpgr        - pgrouting version\n" .
    "       -psql /path/to/psql - optional path to psql\n" .
    "       -v                  - verbose messages for small debuging\n" .
    "       -dbg                - use when CMAKE_BUILD_TYPE = DEBUG\n" .
    "       -debug              - verbose messages for debuging(enter twice for more)\n" .
    "       -debug1             - DEBUG1 messages (for timing reports)\n" .
    "       -clean              - dropdb pgr_test__db__test\n" .
    "       -ignorenotice       - ignore NOTICE statements when reporting failures\n" .
    "       -alg 'dir'          - directory to select which algorithm subdirs to test\n" .
    "       -documentation      - ONLY generate documentation examples\n" .
    "       -force              - Force tests for unsupported versions >= 9.1 of postgreSQL \n" .
    "       -h                  - help\n";
}

print "RUNNING: algorithm-tester.pl " . join(" ", @ARGV) . "\n";

my ($vpg, $postgis_ver, $vpgr, $psql);
my $alg = '';
my @testpath = ("test/");
my @test_direcotry = ();
my $clean;
my $ignore;

$postgis_ver = '';

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
    elsif ($a eq '-pgisver') {
        $postgis_ver = shift @ARGV || Usage();
        $postgis_ver = " VERSION '$postgis_ver'";
    }
    elsif ($a eq '-pgrver') {
        $vpgr = shift @ARGV || Usage();
    }
    elsif ($a eq '-alg') {
        $alg = shift @ARGV || Usage();
        @testpath = ("test/$alg");
    }
    elsif ($a eq '-psql') {
        $psql = shift @ARGV || Usage();
        die "'$psql' is not executable!\n"
        unless -x $psql;
    }
    elsif ($a =~ /^-h/) {
        Usage();
    }
    elsif ($a =~ /^-clean/) {
        $clean = 1;;
    }
    elsif ($a =~ /^-ignoren/i) {
        $ignore = 1;;
    }
    elsif ($a =~ /^-debug1$/i) {
        $DEBUG1 = 1 unless $DOCUMENTATION;
    }
    elsif ($a =~ /^-debug$/i) {
        $DEBUG++;
        $VERBOSE = 1;
    }
    elsif ($a =~ /^-v/i) {
        $VERBOSE = 1;
    }
    elsif ($a =~ /^-force/i) {
        $FORCE = 1;
    }
    elsif ($a =~ /^-doc(umentation)?/i) {
        $DOCUMENTATION = 1;
        $DEBUG1 = 0; # disbale timing reports during documentation generation
    }
    elsif ($a =~ /^-dbg/i) {
        $INTERNAL_TESTS = 1; #directory internalQueryTests is also tested
    }
    else {
        warn "Error: unknown option '$a'\n";
        Usage();
    }
}

my $connopts = "";
$connopts .= " -U $DBUSER" if defined $DBUSER;
$connopts .= " -h $DBHOST" if defined $DBHOST;
$connopts .= " -p $DBPORT" if defined $DBPORT;

mysystem("dropdb $connopts $DBNAME") if $clean;

%main::tests = ();
my @cfgs = ();
my %stats = (z_pass=>0, z_fail=>0, z_crash=>0);
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


# Traverse desired filesystems
File::Find::find({wanted => \&want_tests}, @testpath);

die "Error: no test files found. Run this command from the top level pgRouting directory!\n" unless @cfgs;

createTestDB($DBNAME);

$vpg = '' if ! $vpg;
$postgis_ver = '' if ! $postgis_ver;

# cfgs = SET of configuration file names
# c  one file in cfgs
# print join("\n",@cfgs),"\n";
for my $c (@cfgs) {
    my $found = 0;

    print "test.conf = $c\n" if $VERBOSE;

    # load the config file for the tests
    require $c;

    print Data::Dumper->Dump([\%main::tests],['test']) if $VERBOSE;

    if ($main::tests{any} && !$DOCUMENTATION) {
        push @{$stats{$c}}, run_test($c, $main::tests{any});
        $found++;
    }
    elsif ($main::tests{any}{documentation} && $DOCUMENTATION) {
        push @{$stats{$c}}, run_test($c, $main::tests{any});
        $found++;
    }

    if (! $found) {
        $stats{$c} = "No tests were found for '$vpg-$postgis_ver'!";
    }
}

dropTestDB();

print Data::Dumper->Dump([\%stats], ['stats']);

unlink $TMP;
unlink $TMP2;
unlink $TMP3;

if ($stats{z_crash} > 0 || $stats{z_fail} > 0) {
    exit 1;  # signal we had failures
}

exit 0;      # signal we passed all the tests


# c  one file in cfgs
# t  contents of array that has keys comment, data and test
sub run_test {
    my $c = shift;
    my $t = shift;
    my %res = ();

    my $dir = dirname($c);

    $res{comment} = $t->{comment} if $t->{comment};
    #t->{data}  referencing the key data of the data files

    my $singleDB = "____pgr___single_test___";
    for my $testName (@{$t->{singleTest}}) {
        createTestDB($singleDB);
        mysystem("$psql $connopts -A -t -q -f tools/testers/sampledata.sql' $singleDB >> $TMP2 2>\&1 ");
        for my $x (@{$t->{data}}) {
            mysystem("$psql $connopts -A -t -q -f '$dir/$x' $singleDB >> $TMP2 2>\&1 ");
        }
        process_single_test($testName, $dir, $singleDB,\%res);
        mysystem("dropdb $connopts $singleDB");
    }

    mysystem("$psql $connopts -A -t -q -f tools/testers/sampledata.sql $DBNAME >> $TMP2 2>\&1 ");
    for my $x (@{$t->{data}}) {
        mysystem("$psql $connopts -A -t -q -f '$dir/$x' $DBNAME >> $TMP2 2>\&1 ");
    }

    if ($INTERNAL_TESTS) {
        for my $x (@{$t->{debugtests}}) {
            process_single_test($x, $dir,, $DBNAME, \%res)
        }
    }
    if ($DOCUMENTATION) {
        for my $x (@{$t->{documentation}}) {
            process_single_test($x, $dir,, $DBNAME, \%res);
            my $cmd = q(perl -pi -e 's/[ \t]+$//');
            $cmd .= " doc/queries/$x.queries";
            mysystem( $cmd );
        }
    }
    else {
        for my $x (@{$t->{tests}}) {
            process_single_test($x, $dir,, $DBNAME, \%res)
        }
        if ($OS =~/msys/ || $OS=~/MSW/ || $OS =~/cygwin/) {
            for my $x (@{$t->{windows}}) {
                process_single_test($x, $dir,, $DBNAME, \%res)
            }
        } elsif ($OS=~/Mac/ ||  $OS=~/dar/) {
            for my $x (@{$t->{macos}}) {
                process_single_test($x, $dir,, $DBNAME, \%res)
            }
        } else {
            for my $x (@{$t->{linux}}) {
                process_single_test($x, $dir,, $DBNAME, \%res)
            }
        }
    }

    return \%res;
}

sub process_single_test{
    my $x = shift;
    my $dir = shift;
    my $database = shift;
    my $res = shift;
    #each tests will use clean data

    print "Processing test: $dir/$x";
    my $t0 = [gettimeofday];
    #TIN = test_input_file
    open(TIN, "$dir/$x.test.sql") || do {
        $res->{"$dir/$x.test.sql"} = "FAILED: could not open '$dir/$x.test.sql' : $!";
        $stats{z_fail}++;
        next;
    };

    my $level = "NOTICE";
    $level = "WARNING" if $ignore;
    $level = "DEBUG3" if $DEBUG1;


    if ($DOCUMENTATION) {
        mysystem("mkdir -p 'doc/queries' "); # make sure the directory exists
        open(PSQL, "|$psql $connopts --set='VERBOSITY terse' -e $database > doc/queries/$x.queries 2>\&1 ") || do {
            $res->{"$dir/$x.test.sql"} = "FAILED: could not open connection to db : $!";
            next;
        };
    }
    else {
        #open(PSQL, "|$psql $connopts --set='VERBOSITY terse' -e $database > $dir/$x.result 2>\&1 ") || do {
        #    $res->{"$dir/$x.test.sql"} = "FAILED: could not open connection to db : $!";
        #    $stats{z_fail}++;
        #    next;
        #};

        open(PSQL, "|$psql $connopts  --set='VERBOSITY terse' -e $database > $TMP 2>\&1 ") || do {
            $res->{"$dir/$x.test.sql"} = "FAILED: could not open connection to db : $!";
            if (!$INTERNAL_TESTS) {
               $stats{z_fail}++;
            }
            next;
        };
    }


    my @d = ();
    @d = <TIN>; #reads the whole file into the array @d

    print PSQL "BEGIN;\n";
    print PSQL "SET client_min_messages TO $level;\n";
    #prints the whole fle stored in @d
    print PSQL @d;
    print PSQL "\nROLLBACK;";

    # executes everything
    close(PSQL);
    #closes the input file  /TIN = test input
    close(TIN);

    if ($DOCUMENTATION) {
        print "\n";
        return;
    }

    my $dfile;
    my $dfile2;
    if ($ignore) { #decide how to compare results, if ignoring or not ignoring
        $dfile2 = $TMP2;
        mysystem("grep -v NOTICE '$TMP' | grep -v '^CONTEXT:' | grep -v '^PL/pgSQL function' | grep -v '^COPY' > $dfile2");
        $dfile = $TMP3;
        mysystem("grep -v NOTICE '$dir/$x.result' | grep -v '^CONTEXT:' | grep -v '^PL/pgSQL function' | grep -v '^COPY' > $dfile");
    }
    elsif ($DEBUG1) { #to delete CONTEXT lines
        $dfile2 = $TMP2;
        mysystem("grep -v '^CONTEXT:' '$TMP' | grep -v '^PL/pgSQL function' | grep -v '^COPY' > $dfile2");
        $dfile = $TMP3;
        mysystem("grep -v '^CONTEXT:' '$dir/$x.result' | grep -v '^PL/pgSQL function' | grep -v '^COPY' > $dfile");
    }
    else {
        $dfile2 = $TMP2;
        mysystem("grep -v '^COPY' '$TMP' | grep -v 'psql:tools' > $dfile2");
        $dfile = $TMP3;
        mysystem("grep -v '^COPY' '$dir/$x.result' | grep -v 'psql:tools' > $dfile");
    }
    if (! -f "$dir/$x.result") {
        $res->{"$dir/$x.test.sql"} = "\nFAILED: result file missing : $!";
        $stats{z_fail}++;
        next;
    }

    # use diff -w to ignore white space differences like \r vs \r\n
    #ignore white spaces when comparing
    #dfile is expected results
    #dfile2 is the actual results
    my $r = `diff -w '$dfile' '$dfile2' `;
    #looks for removing leading blanks and trailing blanks
    $r =~ s/^\s*|\s*$//g;
    if ($r =~ /connection to server was lost/) {
        $res->{"$dir/$x.test.sql"} = "CRASHED SERVER: $r";
        $stats{z_crash}++;
        # allow the server some time to recover from the crash
        warn "CRASHED SERVER: '$dir/$x.test.sql', sleeping 5 ...\n";
        sleep 20;
    }
    #if the diff has 0 length then everything was the same, so here we detect changes
    elsif (length($r)) {
        $res->{"$dir/$x.test.sql"} = "FAILED: $r";
        $stats{z_fail}++ unless $DEBUG1;
        print "\t FAIL\n";
    }
    else {
        $res->{"$dir/$x.test.sql"} = "Passed";
        $stats{z_pass}++;
        print "\t PASS\n";
    }
    print "    test run time: " . tv_interval($t0, [gettimeofday]) . "\n";
}

sub createTestDB {
    my $databaseName = shift;
    dropTestDB() if dbExists($databaseName);

    my $template;

    my $dbver = getServerVersion();
    my $dbshare = getSharePath($dbver);

    if ($DEBUG) {
        print "-- DBVERSION: $dbver\n";
        print "-- DBSHARE: $dbshare\n";
    }

    die "
    Unsupported postgreSQL version $dbver
    Minimum requierment is $POSGRESQL_MIN_VERSION version
    Use -force to force the tests\n"
    unless version_greater_eq($dbver, $POSGRESQL_MIN_VERSION) or ($FORCE and version_greater_eq($dbver, '9.1'));

    die "postGIS extension $postgis_ver not found\n"
    unless -f "$dbshare/extension/postgis.control";


    # Create a database with postgis installed in it
    mysystem("createdb $connopts $databaseName");
    die "ERROR: Failed to create database '$databaseName'!\n"
    unless dbExists($databaseName);
    my $encoding = '';
    if ($OS =~ /msys/
        || $OS =~ /MSWin/) {
        $encoding = "SET client_encoding TO 'UTF8';";
    }
    print "-- Installing postgis extension $postgis_ver\n" if $DEBUG;
    mysystem("$psql $connopts -c \"$encoding CREATE EXTENSION postgis $postgis_ver \" $databaseName");

    # Install pgrouting into the database
    my $myver = '';
    if ($vpgr) {
        $myver = " VERSION '$vpgr'";
    }
    print "Installing pgrouting extension $myver\n" if $DEBUG;
    mysystem("$psql $connopts -c \"CREATE EXTENSION pgrouting $myver\" $databaseName");

    # Verify pgrouting was installed

    my $pgrv = `$psql $connopts -c "select pgr_version()" $databaseName`;
    die "ERROR: failed to install pgrouting into the database!\n"
    unless $pgrv;

    print `$psql $connopts -c "select version();" postgres `, "\n";
    print `$psql $connopts -c "select postgis_full_version();" $databaseName `, "\n";
    print `$psql $connopts -c "select pgr_version();" $databaseName `, "\n";
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
    my $v = `$psql $connopts -q -t -c "select version()" postgres`;
    print "$psql $connopts -q -t -c \"select version()\" postgres\n    # RETURNED: $v\n" if $VERBOSE;
    if ($v =~ m/PostgreSQL (\d+(\.\d+)?)/) {
        my $version = $1 + 0;
        print "    Got: $version\n" if $VERBOSE;
        $version = int($version) if $version >= 10;
        print "    Got: $version\n" if $VERBOSE;
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
# versions installed in a cluster. So we get the DB version by connectiong
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
    print "$cmd\n" if $VERBOSE || $DRYRUN;
    system($cmd) unless $DRYRUN;
}

sub want_tests {
#    /^bd_d.*\z/s &&
#    ($File::Find::prune = 1)
#    ||
    /^test\.conf\z/s &&
    push @cfgs, $name;
    #print join("\n",@cfgs),"\n";
}


