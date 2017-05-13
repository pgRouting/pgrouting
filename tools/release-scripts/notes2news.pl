#!/usr/bin/perl -w

use strict;

sub Usage {
  die "Usage: notes2news.pl (from the root of the repository or pre-commit hook)\n";
}


my $in_file = "doc/src/release_notes.rst";
my $out_file = "NEWS";

my $ofh;
open($ofh, ">$out_file") || die "ERROR: failed to open '$out_file' for write! : $!\n";

my $ifh;
open($ifh, "$in_file") || die "ERROR: failed to open '$in_file' for read! : $!\n";

my $skipping = 1;
while (my $line = <$ifh>) {
  if ($skipping) {
    if ($line =~ /^\.\. _changelog_/) {
      $skipping = 0;
    }
    next;
  }

  # convert urls to markdown
  $line =~ s/`([^<]+)<([^>]+)>`_/\[$1\]($2)/g;

  # convert rubric to bold
  $line =~ s/^\.\. rubric::\s*(.+)$/*$1*/;

  next if $line =~ /^\.\. _changelog/;

  print $ofh $line;
}

close($ifh);
close($ofh);

