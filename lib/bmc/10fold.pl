use strict;
use warnings;

my $infile = '../trimmed_all.csv';

for (my $i= 0; $i < 10; ++$i) {

    my $start = ($i * 4260);
    my $end = $start + 4260;

    my $existingdir = "./test$i";
    mkdir $existingdir unless -d $existingdir; # Check if dir exists. If not create it.

    my $trainfile = "train.csv";
    my $testfile = "test.csv";
    my $trainwidfile = "trainwid.csv";
    my $testwidfile = "testwid.csv";

    my $testpbsfile = "test.pbs";
    my $trainpbsfile = "train.pbs";

    open my $in, '<', $infile or die "Cannot open $infile: $!";
    open my $train, '>', "$existingdir/$trainfile" or die "Cannot open $trainfile: $!";
    open my $test, '>', "$existingdir/$testfile" or die "Cannot open $testfile: $!";
    open my $trainwid, '>', "$existingdir/$trainwidfile" or die "Cannot open $trainfile: $!";
    open my $testwid, '>', "$existingdir/$testwidfile" or die "Cannot open $testfile: $!";
    open my $testpbs, '>', "$existingdir/$testpbsfile" or die "Cannot open $testpbsfile: $!";
    open my $trainpbs, '>', "$existingdir/$trainpbsfile" or die "Cannot open $trainpbsfile: $!";

    while( my $line = <$in>)  {
    	$line =~ /^(\d*),(.*)$/;
      if ($. > $start && $. <= $end) {
    	    print $testwid $line;
          print $test $2 . "\n";
    	} else {
    	    print $trainwid $line;
          print $train $2 . "\n";
    	}
    }

    print $testpbs "#!/bin/bash\n";
    print $testpbs "#PBS -l nodes=52:ppn=32\n";
    print $testpbs "#PBS -l walltime=00:30:00\n";
    print $testpbs "cd \$PBS_O_WORKDIR\n\n";

    print $testpbs "time aprun -n 52 -d 32 ./k2.out -t 64 -d test.csv -p 32 -m 5 -o test_out$i.csv\n";

    print $trainpbs "#!/bin/bash\n";
    print $trainpbs "#PBS -l nodes=52:ppn=32\n";
    print $trainpbs "#PBS -l walltime=00:30:00\n";
    print $trainpbs "cd \$PBS_O_WORKDIR\n\n";

    print $trainpbs "time aprun -n 52 -d 32 ./k2.out -t 64 -d train.csv -p 32 -m 5 -o train_out$i.csv\n";

    close $in;
    close $train;
    close $test;
    close $trainwid;
    close $testwid;
    close $testpbs;
    close $trainpbs;


}
