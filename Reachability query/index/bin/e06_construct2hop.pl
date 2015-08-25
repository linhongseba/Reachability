#!/usr/bin/perl -w

use Time::HiRes qw(gettimeofday);

#
print "Deleting old labeling storage...";
unlink "intvlvcb.dat";
unlink "intervaldb.dat";
unlink "meta.dat";
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
system("copy .\\approGeo.exe .\\update \n");




$t0 = gettimeofday;

#
print "\ngenIntLabeling.exe -r -t d\n";
system("genIntLabeling.exe -r -t d\n");
print "\napproGeo.exe -t d\n";
system("approGeo.exe  -t d\n");







$t1 = gettimeofday;
$elapsed = $t1-$t0;

END:
printf "Elapsed time: %.6f seconds\n", $elapsed;
print "***********************************************\n";
print "\nEnd of Program\n";



__END__