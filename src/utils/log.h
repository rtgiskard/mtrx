#ifndef INCLUDE_UTILS_LOG_H
#define INCLUDE_UTILS_LOG_H

#include <string>
#include <string_view>

namespace mtrx {
namespace utils {

struct LogSettings {
	std::string dir        = "logs";
	std::string level_term = "info";
	std::string level_file = "trace";
	std::string pattern    = "%Y%m%d_%H%M%S.%e %^%L%$ %n: %v";

	uint rotate_size    = 4 * 1024 * 1024;
	uint rotate_count   = 4;
	uint flush_interval = 1;
	bool sync           = true;
};

bool init_log(std::string_view name, const LogSettings & setting);

} // namespace utils
} // namespace mtrx

#endif
