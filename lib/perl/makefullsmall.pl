use strict;
use warnings;

my $infile = 'shuffled.csv';

for (my $i= 0; $i < 1; ++$i) {

    my $existingdir = "./fullsmall$i";
    mkdir $existingdir unless -d $existingdir; # Check if dir exists. If not create it.


    my $outfile = "data.csv";
    my $of2 = "data_ids.csv";
    my $pbsfile = "test.pbs";

    open my $in, '<', $infile or die "Cannot open $infile: $!";
    open my $out, '>', "$existingdir/$outfile" or die "Cannot open $outfile: $!";
    open my $out2, '>', "$existingdir/$of2" or die "Cannot open $outfile: $!";
    open my $testpbs, '>', "$existingdir/$pbsfile" or die "Cannot open $pbsfile: $!";

    while( my $line = <$in>)  {

      my $tmp = $line;
      $tmp =~ /^(\d*,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,.,).,(.,.,.,.,.,.,.,.,.,.,.)/;
      $tmp = $1 . $2;
      $tmp =~ /^(\d*),(.*)$/;
      my $id = $1;
      my $nums = $2;
      if ($nums =~ /.*,2.*,2/) {
        print $out2 $id . "," . $nums . "\n";
        print $out $nums . "\n";
      }
    }

    print $testpbs "#!/bin/bash\n";
    print $testpbs "#PBS -l nodes=52:ppn=32\n";
    print $testpbs "#PBS -l walltime=00:30:00\n";
    print $testpbs "cd \$PBS_O_WORKDIR\n\n";

    print $testpbs "time aprun -n 52 -d 32 ./k2.out -t 100 -d data.csv -p 32 -m 5 -o fullsmall$i.csv\n";

    close $in;
    close $out;
    close $testpbs;
}
