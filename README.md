c42
===

Common C Code Collection -- ver. 2

This is a freestanding library implementing various utility functions commonly
used in most programs (for example: string manipulation).

The library also defines various interfaces along with some support functions
for basic services such as memory allocation, file I/O, file system access, 
etc. It does not provide real implementations of those interfaces as they would
be platform dependent.

All platform dependent functions are part of a separate library - c42svc which
provides implementations for all (or most) interfaces defined in c42.

For command line interface apps, there is c42clia library which depends on c42
and c42svc and it provides a platform dependent implementation of main() that
initialises services and calls user's c42_main(). This allows writing C programs
trully portable with the same code base for all platforms.

This model allows writing portable programs quite easily, the core of the 
program being isolated from all platform-dependent functions.

Since c42 is freestanding (no libs imported - except in some cases for what
the compiler enforces) the goal is to have it compiled for all environments
(all Unix-like systems, MS Windows etc. as user-mode and kernel-mode library).

The library here is the next iteration after c41.

Compiling
====
For generating character width tables (ucw8.c) one must download the Unicode
description files UnicodeData and EastAsianWith. At the time of writing this,
they can be found at:
ftp://ftp.unicode.org/Public/UNIDATA/UnicodeData.txt
ftp://ftp.unicode.org/Public/UNIDATA/EastAsianWidth.txt

To generate ucw8.c run:
  ruby cw.rb UnicodeData.txt EastAsianWidth.txt 8 > ucw8.c

License: BSD 2-clause
