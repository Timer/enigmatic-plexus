use strict;
use warnings;
use File::Copy;

my $infile = 'shuffled.csv';

for (my $i= 0; $i < 16; ++$i) {

    my $existingdir = "./test$i";
    mkdir $existingdir unless -d $existingdir; # Check if dir exists. If not create it.

    my $pbsfile = "data.pbs";
    open my $pbs, '>', "$existingdir/$pbsfile" or die "Cannot open $pbsfile: $!";

    print $pbs "#!/bin/bash\n";
    print $pbs "#PBS -l nodes=52:ppn=32\n";
    print $pbs "#PBS -l walltime=00:30:00\n";
    print $pbs "cd \$PBS_O_WORKDIR\n\n";

    print $pbs "time aprun -n 52 -d 32 ./k2.out -t " . ($i + 1) * 4 . " -d ../data.csv -p 32 -m 5 -o data_out.csv\n";
    close $pbs;
}
