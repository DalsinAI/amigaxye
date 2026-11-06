/* Included before every C++ file (-include): the C++ streams first, since
 * the toolchain's pthread.h wraps read(), write() and close() in
 * cancellation macros once <unistd.h> is in, which breaks <istream> and
 * <fstream> read after it; and <ctime> and <cstdlib>, which glibc's
 * headers bring in on the way for src/gen.cpp and the others.
 * The AmiCradle Team. zlib licence, like Xye. */
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
