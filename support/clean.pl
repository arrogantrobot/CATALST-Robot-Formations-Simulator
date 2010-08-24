#!/gsc/bin/perl

use IO::File;

my @files = glob('/gscuser/rlong/git/other/master/*.cpp');
push @files, glob('/gscuser/rlong/git/other/master/*.h');

for my $file (@files) {
    #print $file."\n";
    my $fh = new IO::File;
    $fh->open($file);
    for(<$fh>) {
        #print $_;
        if($_ =~ /\n/) {
            print "found ^M\n";
        }
    }
    $fh->close;
}

