use strict;
use warnings;

my $infile = '../shuffled.csv';

for (my $i= 0; $i < 25; ++$i) {

    my $existingdir = "./test$i";
    mkdir $existingdir unless -d $existingdir; # Check if dir exists. If not create it.


    my $outfile = "data.csv";
    my $pbsfile = "test.pbs";

    open my $in, '<', $infile or die "Cannot open $infile: $!";
    open my $out, '>', "$existingdir/$outfile" or die "Cannot open $outfile: $!";
    open my $testpbs, '>', "$existingdir/$pbsfile" or die "Cannot open $pbsfile: $!";

    while( my $line = <$in>)  {
      $line =~ /^(\d*),(.*)$/;
      print $out $2 . "\n";
    }

    print $testpbs "#!/bin/bash\n";
    print $testpbs "#PBS -l nodes=52:ppn=32\n";
    print $testpbs "#PBS -l walltime=00:30:00\n";
    print $testpbs "cd \$PBS_O_WORKDIR\n\n";

    print $testpbs "time aprun -n 52 -d 32 ./k2.out -t " . (($i + 1) * 4) . " -d data.csv -p 32 -m 5 -o top$i.csv\n";

    close $in;
    close $out;
    close $testpbs;
}
