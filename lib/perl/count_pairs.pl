#!/usr/bin/perl

use strict;
use warnings;

my $file = '< mt.csv';
my $out = '> all_data.csv';
open my $infile, $file or die "Could not open $file: $!";
open my $outfile, $out or die "Could not open $out: $!";
while( my $line = <$infile>)  {
    print $outfile $line if $line =~ /.*,2.*,2/;
}

close $infile;
close $outfile;
