#!/usr/bin/perl

use strict;
use warnings;

my $file = '< trimmed_data.csv';
my $out = '> trimmed_noid.csv';
open my $infile, $file or die "Could not open $file: $!";
open my $outfile, $out or die "Could not open $out: $!";
while( my $line = <$infile>)  {
    $line =~ /(\d*,)(.*)/;
    print $outfile $2."\n";
}

close $infile;
close $outfile;
