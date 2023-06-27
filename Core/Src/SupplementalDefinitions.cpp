extern "C" {
void _exit() {}
void _kill() {}
void _getpid() {}
void _sbrk() {}
void _close() {}
void _fstat() {}
void _isatty() {}
void _lseek() {}
void _read() {}
void _write() {}

/**
 * The special functions _init and _fini are some historic left-overs to control
 * constructors and destructors. However, they are obsolete, and their use can
 * lead to unpredictable results. No modern library should make use of these
 * anymore, and make use of the GCC function attributes constructor and
 * destructor instead, which add methods to those tables inside .preinit_array,
 * .init_array and .fini_array sections.
 */
void _init() {}
void _fini() {}
}