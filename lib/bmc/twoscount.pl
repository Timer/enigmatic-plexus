#!/usr/bin/perl

use strict;
use warnings;


my @matrix=map [(0)x30], 0..29;


my $file = '< data.csv';
my $out = '> pair_counts.csv';
my $mfile = '> pairs.csv';
open my $infile, $file or die "Could not open $file: $!";
open my $outfile, $out or die "Could not open $out: $!";
open my $matfile, $mfile or die "Could not open $mfile: $!";
my $count = 0;
while( my $line = <$infile>)  {
  chomp($line);
  for (my $i = 0; $i < 30; ++$i) {
    for (my $j = $i + 1; $j < 30; ++$j) {
      check_pair($i, $j, $line);
    }
  }
  ++$count;
}
for (my $i = 0; $i < 30; ++$i) {
  for (my $j = $i + 1; $j < 30; ++$j) {
    print $outfile "$i,$j,$matrix[$i][$j]\n";
  }
}


for (my $i = 0; $i < 30; ++$i) {
  for (my $j = 0; $j < 30; ++$j) {
    print $matfile "$matrix[$i][$j],";
  }
  print $matfile "\n";
}

close $infile;
close $outfile;
close $matfile;
print "$count lines checked\n";
sub check_pair{
    my ($a, $b, $line) = @_;
    my $c = ($b - $a) - 1;
    $matrix[$a][$b] += 1 if $line =~ /^(\d,){$a}2,(\d,){$c}2.*$/;
}
