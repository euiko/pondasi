#ifndef __PONDASI_UTILITY_ERROR_HPP__

#include <string>

namespace pondasi {

struct BasicError {
  std::string error;
};

template <typename Kind>
struct Error : public BasicError {
  explicit Error(Kind kind) : kind(kind) {}
  Error(Kind kind, std::string err)
      : BasicError({std::move(err)}), kind(kind) {}

  Kind kind;
};

}  // namespace pondasi

#endif  // !__PONDASI_UTILITY_ERROR_HPP__