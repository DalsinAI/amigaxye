/* A Shell gives 4 KB of stack; libnix's startup sets this much up instead.
 * The AmiCradle Team. zlib licence, like Xye. */
unsigned long __stack = 512 * 1024;
static const char version[] __attribute__((used)) = "$VER: Xye 0.12.2 Amiga (06.11.2026)";
