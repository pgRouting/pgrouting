#! /usr/bin/perl -w
    eval 'exec /usr/bin/perl -S $0 ${1+"$@"}'
        if 0; #$running_under_some_shell

use strict;
use File::Find ();
use File::Basename;
use Data::Dumper;
$Data::Dumper::Sortkeys = 1;

# for the convenience of &wanted calls, including -eval statements:
use vars qw/*name *dir *prune/;
*name   = *File::Find::name;
*dir    = *File::Find::dir;
*prune  = *File::Find::prune;

my $VERBOSE = 0;
my $DRYRUN = 0;

my $DBNAME = "pgr_test__db__test";
my $DBUSER = 'postgres';
my $DBHOST = 'localhost';

sub Usage {
    die "Usage: test-runner.pl -pgver vpg -pgisver vpgis -psql /path/to/psql\n" .
        "       -pgver vpg          - postgresql version\n" .
        "       -pgisver vpgis      - postgis version\n" .
        "       -pgrver vpgr        - pgrouting version\n" .
        "       -psql /path/to/psql - optional path to psql\n" .
        "       -h                  - help\n";
}

print "RUNNING: test-runner.pl " . join(" ", @ARGV) . "\n";

my ($vpg, $vpgis, $vpgr, $psql);

while (my $a = shift @ARGV) {
    if ( $a eq '-pgver') {
        $vpg   = shift @ARGV || Usage();
    }
    elsif ($a eq '-pgisver') {
        $vpgis = shift @ARGV || Usage();
    }
    elsif ($a eq '-pgrver') {
        $vpgr = shift @ARGV || Usage();
    }
    elsif ($a eq '-psql') {
        $psql = shift @ARGV || Usage();
        die "'$psql' is not executable!\n"
            unless -x $psql;
    }
    elsif ($a =~ /^-h/) {
        Usage();
    }
    else {
        warn "Error: unknown option '$a'\n";
        Usage();
    }
}

%main::tests = ();
my @cfgs = ();
my %stats = (z_pass=>0, z_fail=>0, z_crash=>0);
my $TMP = "/tmp/pgr-test-runner-$$";
my $TMP2 = "/tmp/pgr-test-runner-$$-2";

if (! $psql) {
    $psql = findPsql() || die "ERROR: can not find psql, specify it on the command line.\n";
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
File::Find::find({wanted => \&want_tests}, 'src');

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
        mysystem("$psql -U $DBUSER -h $DBHOST -A -t -q -f '$dir/$x' $DBNAME >> $TMP2 2>\&1 ");
    }

    for my $x (@{$t->{tests}}) {
        mysystem("$psql -U $DBUSER -h $DBHOST -A -t -q -f '$dir/$x.test' $DBNAME > $TMP 2>\&1 ");
        # use diff -w to ignore white space differences like \r vs \r\n
        my $r = `diff -w '$dir/$x.rest' $TMP`;
        $r =~ s/^\s*|\s*$//g;
        if ($r =~ /connection to server was lost/) {
            $res{"$dir/$x.test"} = "CRASHED SERVER: $r";
            $stats{z_crash}++;
            # allow the server some time to recover from the crash
            warn "CRASHED SERVER: '$dir/$x.test', sleeping 5 ...\n";
            sleep 5;
        }
        elsif (length($r)) {
            $res{"$dir/$x.test"} = "FAILED: $r";
            $stats{z_fail}++;
        }
        else {
            $res{"$dir/$x.test"} = "Passed";
            $stats{z_pass}++;
        }
    }

    return \%res;
}

sub createTestDB {
    die "ERROR: test database '$DBNAME' exists, you must drop or rename it!\n"
        if dbExists($DBNAME);

    my $template;
    
    my $dbver = getServerVersion();
    my $dbshare = getSharePath($dbver);

    # first create a database with postgis installed in it
    if (version_greater_eq($dbver, '9.1') &&
            -f "$dbshare/extension/postgis.control") {
        mysystem("createdb -U $DBUSER -h $DBHOST $DBNAME");
        die "ERROR: Failed to create database '$DBNAME'!\n"
            unless dbExists($DBNAME);
        my $myver = '';
        if ($vpgis) {
            $myver = " VERSION '$vpgis'";
        }
        mysystem("$psql -U $DBUSER -h $DBHOST -c \"create extension postgis $myver\" $DBNAME");
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
        mysystem("createdb -U $DBUSER -h $DBHOST -T $template $DBNAME");
        die "ERROR: Failed to create database '$DBNAME'!\n"
            if ! dbExists($DBNAME);
    }

    # next we install pgrouting into the new database
    if (version_greater_eq($dbver, '9.1') &&
            -f "$dbshare/extension/postgis.control") {
        my $myver = '';
        if ($vpgr) {
            $myver = " VERSION '$vpgr'";
        }
        mysystem("$psql -U $DBUSER -h $DBHOST -c \"create extension pgrouting $myver\" $DBNAME");
    }
    elsif ($vpgr && -f "$dbshare/extension/pgrouting--$vpgr.sql") {
        mysystem("$psql -U $DBUSER -h $DBHOST -f '$dbshare/extension/pgrouting--$vpgr.sql'");
    }
    else {
        die "ERROR: failed to install pgrouting into the database!\n";
    }
    my $pgrv = `$psql -U $DBUSER -h $DBHOST -c \"select pgr_version()\" $DBNAME`;
    die "ERROR: failed to install pgrouting into the database!\n"
        unless $pgrv;

}

sub dropTestDB {
    mysystem("dropdb -U $DBUSER -h $DBHOST $DBNAME");
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
    my $v = `$psql -U $DBUSER -h $DBHOST -q -t -c "select version()" postgres`;
    print "$psql -U $DBUSER -h $DBHOST -q -t -c \"select version()\" postgres\n    # RETURNED: $v\n" if $VERBOSE;
    if ($v =~ m/PostgreSQL (\d+(\.\d+)?(\.\d+)?)/) {
        print "    # Got ($1)\n" if $VERBOSE;
        return $1;
    }
    return undef;
}

sub dbExists {
    my $dbname = shift;

    my $isdb = `$psql -U $DBUSER -h $DBHOST -l | grep $dbname`;
    $isdb =~ s/^\s*|\s*$//g;
    return length($isdb);
}

sub findPsql {
    my $psql = `which psql`;
    $psql =~ s/^\s*|\s*$//g;
    print "which psql = $psql\n" if $VERBOSE;
    return length($psql)?$psql:undef;
}

sub getSharePath {
    my $pg = shift;

    my $share;
    my $pg_config = `which pg_config`;
    $pg_config =~ s/^\s*|\s*$//g;
    print "which pg_config = $pg_config\n" if $VERBOSE;
    if (length($pg_config)) {
        $share = `$pg_config --sharedir`;
        $share  =~ s/^\s*|\s*$//g;
        if (length($share) && -d $share) {
            return $share;
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


