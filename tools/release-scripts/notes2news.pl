#!/usr/bin/perl -w
=pod
 /*PGR-GNU*****************************************************************
 File: notes2news.pl
 Copyright (c) 2017 pgRouting developers
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
 ********************************************************************PGR-GNU*/
=cut
use strict;
use warnings;
use File::Find;
use Data::Dumper;

sub Usage {
  die "Usage: notes2news.pl (from the root of the repository or pre-commit hook)\n";
}

my $DEBUG = '';
my $in_file = "doc/src/release_notes.rst";
my $out_file = "NEWS.md";

my $ofh;
open($ofh, ">$out_file") || die "ERROR: failed to open '$out_file' for write! : $!\n";

my $ifh;
open($ifh, "$in_file") || die "ERROR: failed to open '$in_file' for read! : $!\n";

my %conversions = get_substitutions();
my $check = join '|', keys %conversions;
print Dumper(\%conversions) if $DEBUG;


my $skipping = 1;
my $file = '';
my $start = '';
my $end = '';
while (my $line = <$ifh>) {
  next if $skipping and $line !~ /^pgRouting/;
  $skipping = 0;

  next if $line =~ /contents|:local:|:depth:|\*\*\*\*\*\*\*|\=\=\=\=\=\=\=|\-\-\-\-\-\-\-|\+\+\+\+\+\+\+\+/;

  $line =~ s/[\|]+//g;
  $line =~ s/($check)/$conversions{$1}/go;

  # Handling the headers
  if ($line =~ m/^pgRouting [0-9]$/i) {
      print $ofh "# $line";
      next;
  };
  if ($line =~ m/^pgRouting [0-9].[0-9]$/i) {
      print $ofh "## $line";
      next;
  };
  if ($line =~ m/^pgRouting [0-9].[0-9].[0-9] Release Notes$/i) {
      print $ofh "### $line";
      next;
  };

  # get include filename
  if ($line =~ /include/) {
      $line =~ s/^.*include\:\: (.*)/$1/;
      chomp $line;
      $line =~ s/^\s+//;
      $file = $line;
      my @wanted_files;
      find(
          sub{
              -f $_ && $_ =~ /$file/
              && push @wanted_files,$File::Find::name
          }, "doc"
      );
      foreach(@wanted_files){
          print "wanted: $_\n" if $DEBUG;
      }
      $file = $wanted_files[0];
      print "rewanted: $file\n" if $DEBUG;
      next;
  };

  if ($line =~ /start\-after/) {
      $line =~ s/start\-after\:\ (.*)/$1/;
      $line =~ tr/://d;
      chomp $line;
      $line =~ s/^\s+//;
      $start = $line;
      next;
  };

  if ($line =~ /end\-before/) {
      $line =~ s/end\-before\:\ (.*)/$1/;
      $line =~ tr/://d;
      chomp $line;
      $line =~ s/^\s+//;
      $end = $line;
      print "--->      $file from $start to $end \n" if $DEBUG;
      find_stuff($file, $start, $end);
      next;
  }



  # convert urls to markdown
  $line =~ s/`([^<]+?)\s*<([^>]+)>`_*/\[$1\]($2)/g;

  $line =~ s/`(Git closed)/\[$1/g;
  $line =~ s/<([^>]+)>`_*/\]($1)/g;

  # convert rubric to bold
  $line =~ s/^\.\. rubric::\s*(.+)$/**$1**/;

  next if $line =~ /^\.\. _changelog/;

  print $ofh $line;
}


close($ifh);
close($ofh);

sub find_stuff {
    my ($file, $mstart, $mend) = @_;
    print "find_stuff $file from $mstart to $mend \n" if $DEBUG;
    my $fh;
    open($fh, "$file") || die "ERROR: failed to open '$file' for read! : $!\n";

    my $skipping = 1;
    while (my $line = <$fh>) {
        next if $skipping and $line !~ /$mstart/;
        $skipping = 0;
        next if $line =~ /$mstart/;
        $line =~ s/[\|]+//g;
        $line =~ s/($check)/$conversions{$1}/go;
        print $ofh "  $line" if $line !~ /$mend/ && $line ne "\n";
        last if $line =~ /$mend/;
    }
    close($fh);
}

sub get_substitutions {
    my $file = "doc/conf.py.in";
    my $mstart = "rst_epilog";
    my $mend = "epilog_end";
    print "get_substitutions $file from $mstart to $mend \n" if $DEBUG;
    my $fh;
    open($fh, "$file") || die "ERROR: failed to open '$file' for read! : $!\n";
    my %data;

    my $skipping = 1;
    while (my $line = <$fh>) {
        next if $skipping and $line !~ /$mstart/;
        $skipping = 0;
        next if $line =~ /$mstart/;
        last if $line =~ /$mend/;
        my ($key) = substr($line, 4, index(substr($line, 4), "|"));
        my ($value) = substr($line, index($line,"`"));
        $value =~ s/\R//g;
        $data{$key} = $value;
        print "$key $data{$key} \n" if $DEBUG;
    }
    close($fh);
    return %data;
}
