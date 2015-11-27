#PQ-Trees

This fork is for my ammendments of the original repository in order to compile and run under Windows using cygwin and withouth CPPUnit library.

My aim is to be able to use the PQ-Tree algorithm implementation for testing it for consecutive-ones property of binary matrices. I hope someone else also may be interested in that and this my version will become handy.

##Changes made on top of the original version

- Eliminated CPPUnit
- changed "auto" keywords to respective data types
- added a custom to_string() method since std::to_string()
- some minor modifications in the original makefile (i.e. removed clang++, -stdlib=libc++, and -lcppunit -ldl -lPQTree library line).

##Notes:

1. This software is distributed without any warranty or guarantee and is currently in development.
2. The compiler I was using was GCC 4.8.2 in cygwin.
