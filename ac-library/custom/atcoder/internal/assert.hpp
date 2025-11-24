#ifndef ATCODER_INTERNAL_ASSERT_HPP
#define ATCODER_INTERNAL_ASSERT_HPP

#include <cstdlib>
#include <iostream>
#include <source_location>

#ifdef LOCAL_DEBUG
namespace atcoder::internal {
struct location {
    std::source_location loc_;
    consteval location(std::source_location loc) : loc_(loc) {
        const char NG[] = "atcoder/";
        const char* filename = loc.file_name();
        for (int i = 0; filename[i] != '\0'; ++i) {
            bool match = true;
            for (int j = 0; j < 8; ++j) {
                if (filename[i + j] == '\0' || filename[i + j] != NG[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                throw std::logic_error("invalid location initialize");
            }
        }
    }
};
void output_location(std::source_location loc) {
    std::cerr << loc.function_name() << ": " << loc.line() << "\n";
}
[[noreturn]] inline void assertion_fail(const char* expr,
                                        std::source_location error_location,
                                        location from_location) {
    std::cerr << "assertion failed: \"" << expr << "\"\n";
    std::cerr << "where: ";
    output_location(error_location);
    std::cerr << " from: ";
    output_location(from_location.loc_);
    std::cerr << std::flush;
    std::abort();
}
}  // namespace atcoder::internal
#define ACL_ASSERT(expr)                      \
    (static_cast<bool>(expr)                  \
         ? void(0)                            \
         : atcoder::internal::assertion_fail( \
               #expr, std::source_location::current(), _from))
#define ACL_FROM_LOCATION                                \
    [[maybe_unused]] atcoder::internal::location _from = \
        std::source_location::current()
#else
#define ACL_ASSERT(expr) assert(expr)
#define ACL_FROM_LOCATION [[maybe_unused]] std::nullptr_t _from = nullptr
#endif

#endif
