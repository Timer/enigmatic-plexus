use strict;
use warnings;

my $infile = 'shuffled.csv';

for (my $i= 0; $i < 10; ++$i) {

    my $start = ($i * 4455);
    my $end = $start + 4455;

    my $existingdir = "./test$i";
    mkdir $existingdir unless -d $existingdir; # Check if dir exists. If not create it.

    my $trainfile = "train.csv";
    my $testfile = "test.csv";
    my $testpbsfile = "test.pbs";
    my $trainpbsfile = "train.pbs";

    open my $in, '<', $infile or die "Cannot open $infile: $!";
    open my $train, '>', "$existingdir/$trainfile" or die "Cannot open $trainfile: $!";
    open my $test, '>', "$existingdir/$testfile" or die "Cannot open $testfile: $!";
    open my $testpbs, '>', "$existingdir/$testpbsfile" or die "Cannot open $testpbsfile: $!";
    open my $trainpbs, '>', "$existingdir/$trainpbsfile" or die "Cannot open $trainpbsfile: $!";
    

    while( my $line = <$in>)  {
	if ($. > $start && $. <= $end) {
	    print $test $line;    
	} else {
	    print $train $line;    
	}
    }

    print $testpbs "#!/bin/bash\n";
    print $testpbs "#PBS -l nodes=52:ppn=32\n";
    print $testpbs "#PBS -l walltime=00:30:00\n";
    print $testpbs "cd \$PBS_O_WORKDIR\n\n";

    print $testpbs "time aprun -n 52 -d 32 ./k2.out -t 64 -d test.csv -p 32 -m 5 -o test_out.csv\n";

    print $trainpbs "#!/bin/bash\n";
    print $trainpbs "#PBS -l nodes=52:ppn=32\n";
    print $trainpbs "#PBS -l walltime=00:30:00\n";
    print $trainpbs "cd \$PBS_O_WORKDIR\n\n";

    print $trainpbs "time aprun -n 52 -d 32 ./k2.out -t 64 -d train.csv -p 32 -m 5 -o train_out.csv\n";
    
    close $in;
    close $train;
    close $test;
    close $testpbs;
    close $trainpbs;
}
