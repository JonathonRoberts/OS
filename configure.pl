#!/usr/bin/perl

use strict;
use warnings;

my $OS = `uname`;
my @programs = qw(
	clang
	dd
	mkisofs
	cu
	make
	nasm
	ld
	objcopy
	ls
);

my @failedprograms;
sub checkprograms;

if($OS =~ /linux/i){
	push( @programs,"yasm");
	unless(&checkprograms){exit 1;}
	system("cp src/linux/kernel/Makefile.linux kernel/Makefile");
	system("cp src/linux/bootloader/Makefile.linux bootloader/Makefile");
	exit 0;
}
elsif($OS =~ /openbsd/i){
	unless(&checkprograms){exit 1;}
	system("cp src/openbsd/kernel/Makefile.openbsd kernel/Makefile");
	system("cp src/openbsd/bootloader/Makefile.openbsd bootloader/Makefile");
	exit 0;
}
else{
	print "EspressOS has not previously been compiled on your Operating system\n";
	return 0;
}

sub checkprograms{
	foreach(@programs){
		unless( `whereis $_`){
			push (@failedprograms, $_);
		}
	}

	if($#failedprograms>=0){
		print "EspressOS installation failed!\n";
		print "Missing the following dependancies:\n";
		foreach(@failedprograms){
			print "$_\n";
		}
		return 0;
	}
	return 1;

}
