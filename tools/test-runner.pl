#! /usr/bin/perl -w
    eval 'exec /usr/bin/perl -S $0 ${1+"$@"}'
        if 0; #$running_under_some_shell

use strict;
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

my $VERBOSE = 0;
my $DRYRUN = 0;
my $DEBUG = 0;

my $DBNAME = "pgr_test__db__test";
my $DBUSER;
my $DBHOST;
my $DBPORT;

sub Usage {
    die "Usage: test-runner.pl -pgver vpg -pgisver vpgis -psql /path/to/psql\n" .
        "       -pgver vpg          - postgresql version\n" .
        "       -pghost host        - postgresql host or socket directory to use\n" .
        "       -pgport port        - postgresql port to use\n" .
        "       -pguser username    - postgresql user role to use\n" .
        "       -pgisver vpgis      - postgis version\n" .
        "       -pgrver vpgr        - pgrouting version\n" .
        "       -psql /path/to/psql - optional path to psql\n" .
        "       -v                  - verbose messages for debuging(enter twice for more)\n" .
        "       -clean              - dropdb pgr_test__db__test\n" .
        "       -ignorenotice       - ignore NOTICE statements when reporting failures\n" .
        "       -alg 'dir'          - directory to select which algorithm subdirs to test\n" .
        "       -h                  - help\n";
}

print "RUNNING: test-runner.pl " . join(" ", @ARGV) . "\n";

my ($vpg, $vpgis, $vpgr, $psql);
my $alg = '';
my @testpath = ("doc/", "src/");
my $clean;
my $ignore;

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
        $vpgis = shift @ARGV || Usage();
    }
    elsif ($a eq '-pgrver') {
        $vpgr = shift @ARGV || Usage();
    }
    elsif ($a eq '-alg') {
        $alg = shift @ARGV || Usage();
        if ($alg eq 'doc') {
            @testpath = ('doc');
        } elsif ($alg eq 'recipes') {
            @testpath = ("doc/src/recipes");
        } else {
            @testpath = ("src/$alg");
        }
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
    elsif ($a =~ /^-v/i) {
        $VERBOSE = 1 if $DEBUG;
        $DEBUG = 1;
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

# some unit tests
#my $server_ver = getServerVersion();
#my $server_ver = '9.2';
#print "PSQL=$psql\n";
#print "SERVER_VERSION=$server_ver\n";
#print "version_greater_eq('9.1')=" . version_greater_eq($server_ver, '9.1') . "\n";
#print "version_greater_eq('9.2')=" . version_greater_eq($server_ver, '9.2') . "\n";
#print "version_greater_eq('9.3')=" . version_greater_eq($server_ver, '9.3') . "\n";
#print "version_greater_eq('8.3')=" . version_greater_eq($server_ver, '8.3') . "\n";
#print "template_pgrouting=" . dbExists('template_pgrouting') . "\n";
#exit;

# Traverse desired filesystems
File::Find::find({wanted => \&want_tests}, @testpath);

die "Error: no test files found. Run this command from the top level pgRouting directory!\n" unless @cfgs;

createTestDB();

$vpg = '' if ! $vpg;
$vpgis = '' if ! $vpgis;

for my $c (@cfgs) {
    my $found = 0;

    # load the config file for the tests
    require $c;

    print "test.conf = $c\n" if $VERBOSE;
    print Data::Dumper->Dump([\%main::tests],['test']) if $VERBOSE;

    if ($main::tests{any}) {
        push @{$stats{$c}}, run_test($c, $main::tests{any});
        $found++;
    }
    if ($main::tests{"$vpg-$vpgis"}) {
        push @{$stats{$c}}, run_test($c, $main::tests{"$vpg-$vpgis"});
        $found++;
    }
    if (! $found) {
        $stats{$c} = "No tests were found for '$vpg-$vpgis'!";
    }
}

dropTestDB();

print Data::Dumper->Dump([\%stats], ['stats']);

unlink $TMP;
unlink $TMP2;
unlink $TMP3;

if ($stats{z_crash} || $stats{z_fail}) {
    exit 1;  # signal we had failures
}

exit 0;      # signal we passed all the tests


sub run_test {
    my $c = shift;
    my $t = shift;
    my %res = ();

    my $dir = dirname($c);

    $res{comment} = $t->{comment} if $t->{comment};
    for my $x (@{$t->{data}}) {
        mysystem("$psql $connopts -A -t -q -f '$dir/$x' $DBNAME >> $TMP2 2>\&1 ");
    }

    for my $x (@{$t->{tests}}) {
        print "Processing test: $x\n";
        my $t0 = [gettimeofday];
        open(TIN, "$dir/$x.test.sql") || do {
            $res{"$dir/$x.test.sql"} = "FAILED: could not open '$dir/$x.test.sql' : $!";
            $stats{z_fail}++;
            next;
        };
        open(PSQL, "|$psql $connopts -A -t -q $DBNAME > $TMP 2>\&1 ") || do {
            $res{"$dir/$x.test.sql"} = "FAILED: could not open connection to db : $!";
            $stats{z_fail}++;
            next;
        };
        print PSQL "set client_min_messages to WARNING;\n" if $ignore;
        my @d = ();
        @d = <TIN>;
        print PSQL @d;
        close(PSQL);
        close(TIN);

        my $dfile;
        my $dfile2;
        if ($ignore) {
            $dfile2 = $TMP2;
            mysystem("grep -v NOTICE '$TMP' | grep -v '^CONTEXT:' | grep -v '^PL/pgSQL function' > $dfile2");
            $dfile = $TMP3;
            mysystem("grep -v NOTICE '$dir/$x.result' | grep -v '^CONTEXT:' | grep -v '^PL/pgSQL function' > $dfile");
        }
        else {
            $dfile = "$dir/$x.result";
            $dfile2 = $TMP;
        }
        # use diff -w to ignore white space differences like \r vs \r\n
        my $r = `diff -w '$dfile' '$dfile2' `;
        $r =~ s/^\s*|\s*$//g;
        if ($r =~ /connection to server was lost/) {
            $res{"$dir/$x.test.sql"} = "CRASHED SERVER: $r";
            $stats{z_crash}++;
            # allow the server some time to recover from the crash
            warn "CRASHED SERVER: '$dir/$x.test.sql', sleeping 5 ...\n";
            sleep 5;
        }
        elsif (length($r)) {
            $res{"$dir/$x.test.sql"} = "FAILED: $r";
            $stats{z_fail}++;
        }
# TODO missing when the result file does not exist
        else {
            $res{"$dir/$x.test.sql"} = "Passed";
            $stats{z_pass}++;
        }
        print "    test run time: " . tv_interval($t0, [gettimeofday]) . "\n";
    }

    return \%res;
}

sub createTestDB {
    die "ERROR: test database '$DBNAME' exists, you must drop or rename it!\n"
        if dbExists($DBNAME);

    my $template;

    my $dbver = getServerVersion();
    my $dbshare = getSharePath($dbver);

    if ($DEBUG) {
        print "-- DBVERSION: $dbver\n";
        print "-- DBSHARE: $dbshare\n";
    }

    # first create a database with postgis installed in it
    if (version_greater_eq($dbver, '9.1') &&
            -f "$dbshare/extension/postgis.control") {
        mysystem("createdb $connopts $DBNAME");
        die "ERROR: Failed to create database '$DBNAME'!\n"
            unless dbExists($DBNAME);
        my $myver = '';
        if ($vpgis) {
            $myver = " VERSION '$vpgis'";
        }
        my $encoding = '';
        if ($OS =~ /msys/
            || $OS =~ /MSWin/) {
            $encoding = "SET client_encoding TO 'UTF8';";
        }
        print "-- Trying to install postgis extension $myver\n" if $DEBUG;
        mysystem("$psql $connopts -c \"$encoding create extension postgis $myver\" $DBNAME");
    }
    else {
        if ($vpgis && dbExists("template_postgis_$vpgis")) {
            $template = "template_postgis_$vpgis";
        }
        elsif (dbExists('template_postgis')) {
            $template = "template_postgis";
        }
        else {
            die "ERROR: Could not find an appropriate template_postgis database!\n";
        }
        print "-- Trying to install postgis from $template\n" if $DEBUG;
        mysystem("createdb $connopts -T $template $DBNAME");
        sleep(2);
        die "ERROR: Failed to create database '$DBNAME'!\n"
            if ! dbExists($DBNAME);
    }

    # next we install pgrouting into the new database
    if (version_greater_eq($dbver, '9.1') &&
            -f "$dbshare/extension/postgis.control") {
        my $myver = '';
        if ($vpgr) {
            $myver = " PGROUTING VERSION '$vpgr'";
        }
        print "-- Trying to install pgrouting extension $myver\n" if $DEBUG;
        mysystem("$psql $connopts -c \"create extension pgrouting $myver\" $DBNAME");
    }
    elsif ($vpgr && -f "$dbshare/extension/pgrouting--$vpgr.sql") {
        print "-- Trying to install pgrouting from '$dbshare/extension/pgrouting--$vpgr.sql'\n" if $DEBUG;
        mysystem("$psql $connopts -f '$dbshare/extension/pgrouting--$vpgr.sql' $DBNAME");
    }
    else {
        my $find = `find "$dbshare/contrib" -name pgrouting.sql | sort -r -n `;
        my @found = split(/\n/, $find);
        my $file = shift @found;
        if ($file && length($file)) {
            print "-- Trying to install pgrouting from '$file'\n" if $DEBUG;
            mysystem("$psql $connopts -f '$file' $DBNAME");
        }
        else {
            mysystem("ls -alR $dbshare") if $DEBUG;
            die "ERROR: failed to install pgrouting into the database!\n";
        }
    }

    # now verify that we have pgrouting installed

    my $pgrv = `$psql $connopts -c "select pgr_version()" $DBNAME`;
    die "ERROR: failed to install pgrouting into the database!\n"
        unless $pgrv;

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
    if ($v =~ m/PostgreSQL (\d+(\.\d+)?(\.\d+)?)/) {
        print "    # Got ($1)\n" if $VERBOSE;
        return $1;
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
}


