#!/usr/bin/perl -w

use Time::HiRes qw(gettimeofday);

#
print "Deleting old labeling storage...";
unlink "tcentry.dat";
unlink "tc_meta.dat";
unlink "crosstcdb.dat";
unlink ".hops";
print "Done!\n";
print "Deleting obselete update temporary files...";
unlink ".relbldagedge";
unlink "delnodes.log";
unlink "addnodes.log";
unlink ".updated2hop";
while (<./update/*>) {
	unlink $_;
}
print "Done!\n";
system("copy .\\approMaxCard.exe .\\update \n");




$t0 = gettimeofday;

#
print "\ngenTC.exe -r -t d\n";
system("genTC.exe -r -t d\n");
print "\napproMaxCard.exe -t d\n";
system("approMaxCard.exe  -t d\n");







$t1 = gettimeofday;
$elapsed = $t1-$t0;

END:
printf "Elapsed time: %.6f seconds\n", $elapsed;
print "***********************************************\n";
print "\nEnd of Program\n";



__END__