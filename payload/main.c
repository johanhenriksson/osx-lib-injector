#include <syslog.h>

static void entry_point() __attribute__((constructor));

static void entry_point() {
    syslog(LOG_ERR, "hello world\n");
}
