#!/usr/bin/perl
use File::Copy;

my $simulator = "/home/rlong/git/simulator/Simulator";
my $seed_location = "/home/rlong/git/simulator/support/seeds/random_xy_seeds_";
my $storage_location = "/home/rlong/git/simulator/data";
my $output_dir = "/home/rlong/git/simulator";
my $stdout_file = " > " . $output_dir . "/stdout.out";
my $trials = 30;

#save_data(1);

my @formation_types = (0,4,6,9);

my @nums_of_robots = (10,50,100);

my @push_or_insertion = ('push','insertion');

my $total_runs = $trials * scalar(@formation_types) * scalar(@num_robots);

print "Beginning a run of $total_runs trials.\n";

for my $seed (1 .. $trials){
    for my $formation_type (@formation_types){
        for my $num_robots (@nums_of_robots){
            for my $auction_type (@push_or_insertion){
                my $type = '';
                if ($auction_type eq 'insertion'){
                    $type = "-i -e 99999";
                }
                my $sim_call = $simulator . " -t 1 -s " . $seed_location . $seed . ".txt -f ". $formation_type . " -n " . $num_robots . " " . $type . $stdout_file;
                print "Calling sim on trial $seed \n";
                print "using: $sim_call\n";
                unless(system($sim_call)==0){
                    print "sim_call = " . $sim_call . "\n";
                    die "sim did not exit properly.";
                }
                save_data($seed,$formation_type,$num_robots,$auction_type);
                #exit(0);
            }
        }
    }
}

sub save_data {
    my @stuff = @_;
    $seed = $stuff[0];
    $formation_type = $stuff[1];
    $num_robots = $stuff[2];
    $auction_type = $stuff[3];
    print "Storing data for trial " .$seed . ".\n";
    my $storage_dir = $storage_location . "/".$auction_type."_run_" . $seed . "_formation-type_" . $formation_type . "_num-robots_". $num_robots;
    unless(not -d $storage_dir){
        die "storage_dir already exists at " . $storage_dir;
    }
    unless(mkdir($storage_dir)){
        die "mkdir failed... trying " . $storage_dir;
    }
    my $mv = "mv *.out " . $storage_dir;
    unless(system($mv)==0){
        print "copy command was ".$mv."\n";
        die "data failed to copy for trial ".$seed;
    }
}

