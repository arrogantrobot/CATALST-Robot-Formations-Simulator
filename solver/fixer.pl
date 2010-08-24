#!/usr/bin/perl
use IO::File;
my $fh = new IO::File;

$fh->open("0_formation_10_cell.data");

my $out_file = new IO::File;
$out_file->open("> line_formation_10_cell.data");

while(<$fh>){
    print $out_file $_ . "\n";
}

$fh->close;
$out_file->close;
