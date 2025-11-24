#ifndef ATCODER_SCC_HPP
#define ATCODER_SCC_HPP 1

#include <algorithm>
#include <cassert>
#include <vector>

#include "atcoder/internal/assert.hpp"
#include "atcoder/internal/scc.hpp"

namespace atcoder {

struct scc_graph {
  public:
    scc_graph() : internal(0) {}
    explicit scc_graph(int n) : internal(n) {}

    void add_edge(int from, int to, ACL_FROM_LOCATION) {
#ifndef NDEBUG
        int n = internal.num_vertices();
        ACL_ASSERT(0 <= from && from < n);
        ACL_ASSERT(0 <= to && to < n);
#endif
        internal.add_edge(from, to);
    }

    std::vector<std::vector<int>> scc() { return internal.scc(); }

  private:
    internal::scc_graph internal;
};

}  // namespace atcoder

#endif  // ATCODER_SCC_HPP
