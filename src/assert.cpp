#include "competitive/utility/assert.hpp"

namespace common::debug::internal {
#ifdef LOCAL_DEBUG
namespace {
void output_location(std::source_location loc) {
  std::cerr << loc.function_name() << ": " << loc.line() << "\n";
}
thread_local std::source_location FROM = std::source_location::current();
thread_local int depth = 0;
} // namespace
[[noreturn]] void fail(const char* expr, std::source_location error_location) {
  std::cerr << "assertion failed: \"" << expr << "\"\n";
  std::cerr << "where: ";
  output_location(error_location);
  std::cerr << " from: ";
  output_location(FROM);
  std::cerr << std::flush;
  std::abort();
}
location::location(std::source_location loc) {
  if (depth++ == 0) {
    FROM = loc;
  }
}
location::~location() {
  --depth;
}
#endif
} // namespace common::debug::internal
