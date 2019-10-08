// libmemory_freelist can't find this symbol, add here as a hack to allow linking
void __assert_fail(const char* expr, const char* file,
        unsigned int line, const char* function)
{ return; }
