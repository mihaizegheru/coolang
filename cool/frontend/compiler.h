#ifndef COOLANG_COOL_FRONTEND_COMPILER_H_
#define COOLANG_COOL_FRONTEND_COMPILER_H_

#include <string>
#include <vector>

namespace cool {

// Returns 0 for ordinary compilation, including reported source errors,
// with diagnostics for source errors. I/O/setup failures return nonzero.
int Compile(const std::vector<std::string>& files, bool tokens_only = false);

}  // namespace cool

#endif  // COOLANG_COOL_FRONTEND_COMPILER_H_
