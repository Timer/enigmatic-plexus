#!/usr/bin/perl

use strict;
use warnings;

my $file = '<all_data.csv';
my $out = '>twentyseven.csv';
open my $infile, $file or die "Could not open $file: $!";
open my $outfile, $out or die "Could not open $out: $!";
while( my $line = <$infile>)  {
  $line =~ /((?:\d*),)((?:\d,){16})\d,((?:\d,){10})(\d),\d,\d/;
  print $outfile "$1$2$3$4\n";
}

close $infile;
close $outfile;
