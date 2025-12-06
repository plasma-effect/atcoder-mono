#ifndef ATCODER_INTERNAL_ASSERT_HPP
#define ATCODER_INTERNAL_ASSERT_HPP

#include <cstdlib>
#include <iostream>
#include <source_location>

#ifdef LOCAL_DEBUG
namespace atcoder::internal {
struct location {
    inline static std::source_location FROM = std::source_location::current();
    inline static int depth = 0;
    location(std::source_location loc) {
        if (depth++ == 0) {
            FROM = loc;
        }
    }
    location(location const&) = delete;
    location(location&&) = delete;
    ~location() { --depth; }
};
inline void output_location(std::source_location loc) {
    std::cerr << loc.function_name() << ": " << loc.line() << "\n";
}
[[noreturn]] inline void fail(const char* expr,
                              std::source_location error_location) {
    std::cerr << "assertion failed: \"" << expr << "\"\n";
    std::cerr << "where: ";
    output_location(error_location);
    std::cerr << " from: ";
    output_location(location::FROM);
    std::cerr << std::flush;
    std::abort();
}
}  // namespace atcoder::internal
#define ACL_ASSERT(expr)     \
    (static_cast<bool>(expr) \
         ? void(0)           \
         : atcoder::internal::fail(#expr, std::source_location::current()))
#define ACL_FROM_LOCATION                                \
    [[maybe_unused]] atcoder::internal::location _from = \
        std::source_location::current()
#else
#define ACL_ASSERT(expr) assert(expr)
#define ACL_FROM_LOCATION [[maybe_unused]] std::nullptr_t _from = nullptr
#endif

#endif
