use strict;
use warnings;

my $infile = 'olddata.csv';
my $outfile = 'data.csv';

open my $in, '<', $infile or die "Cannot open $infile: $!";
open my $out, '>', $outfile or die "Cannot open $outfile: $!";

while( my $line = <$in>)  {
    print $out $line;
    print $out $line;
}

close $in;

