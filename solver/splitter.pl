#!/usr/bin/perl

use IO::File;

my $file = "0_formation_10_cell.txt";

my $fh = new IO::File;

$fh->open($file);

my @lines;

while(<$fh>){
    push @lines, $_;
}
$fh->close;

my $ffh = new IO::File;

$ffh->open("> 0_formation_10_cell.data");

for my $stuff (@lines) {
    for(split(",",$stuff)){
        print $ffh $_ . "\n";
    }
}

$ffh->close;
