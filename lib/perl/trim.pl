#!/usr/bin/perl

use strict;
use warnings;

my $file = '<total_data.csv';
my $out = '>trimmed_data.csv';
open my $infile, $file or die "Could not open $file: $!";
open my $outfile, $out or die "Could not open $out: $!";
while( my $line = <$infile>)  {
    print $outfile $line;
    last if $. == 55800;
}

close $infile;
close $outfile;
