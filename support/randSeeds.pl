#!/usr/bin/perl

my $randGen = '/home/rlong/git/simulator/support/randGenerator';

for (1 .. 30){
    print "on loop # ".$_."\n";
    my $syscall = $randGen . " 1000 random_xy_seeds_" . $_ . ".txt";
    unless(system($syscall)==0){
        die "randGen failed to return properly";
    }
}
