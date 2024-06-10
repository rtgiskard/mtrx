#ifndef INCLUDE_LOG_H
#define INCLUDE_LOG_H

// turn on/off logging statements at compile time
// make sure this is defined before include spdlog.h
#define SPGLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#include <spdlog/spdlog.h>

#include "config.h"

bool init_log(SView name, const LogSettings & setting);

#endif
