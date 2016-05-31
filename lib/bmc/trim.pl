#!/usr/bin/perl

use strict;
use warnings;

my $file = '<shuffled.csv';
my $out = '>trimmed_all.csv';
open my $infile, $file or die "Could not open $file: $!";
open my $outfile, $out or die "Could not open $out: $!";
while( my $line = <$infile>)  {
    print $outfile $line;
    last if $. == 42600;
}

close $infile;
close $outfile;
