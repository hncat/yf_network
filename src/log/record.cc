#include "record.h"

namespace yf {
namespace log {
static const char *level_name[] = {
#define XXX(lv) [lv] = #lv
    XXX(NONE),  XXX(DEBUG), XXX(INFO), XXX(WARNING),
    XXX(ERROR), XXX(FAILED)
#undef XXX
};

const char *get_level_name(level lv) { return level_name[lv]; }
}  // namespace log
}  // namespace yf