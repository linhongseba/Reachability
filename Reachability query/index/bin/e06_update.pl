#!/usr/bin/perl -w

use Time::HiRes qw(gettimeofday);

#

print "Deleting obselete update temporary files...";
unlink ".relbldagedge";
unlink ".updated2hop";
unlink ".\\update\\.hops";
while (<./update/*.dat>) {
    unlink $_;
}
print "Done!\n";





$t0 = gettimeofday;

#
print "genRelblGInt.exe -n ".$ARGV[0]."\n";
system("genRelblGInt.exe -n ".$ARGV[0]);
chdir ".\\update";
$doupdate=-e "intervaldb.dat";
if ($doupdate) 
{
	print "\napproGeo.exe -t d\n";
	system("approGeo.exe  -t d\n");
	chdir ".\\..";
	#print "relabelSubGGeo.exe\n";
	system("relabelSubGGeo.exe");
}
else
{
	print "\nduplicate nodes for deletion!\n";
	chdir ".\\..";
}







$t1 = gettimeofday;
$elapsed = $t1-$t0;

END:
printf "Elapsed time: %.6f seconds\n", $elapsed;
print "***********************************************\n";
if( $doupdate )
{
	system("copy .updated2hop .hops");
}
print "\nEnd of Program\n";



__END__