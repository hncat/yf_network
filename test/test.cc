#include <execinfo.h>
#include <malloc.h>

int main() {
    int size = 12;
    void **buf = new void*[size];
    int nptrs = backtrace(buf, size);
    auto strings = backtrace_symbols(buf, nptrs);
    delete []buf;
    // for (size_t i = 0; i < count; i++)
    // {
    //     /* code */
    // }
    
    free(strings);
    return 0;
}