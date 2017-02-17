//
// Created by Arseny Tolmachev on 2017/02/15.
//

#ifndef JUMANPP_STATUS_HPP
#define JUMANPP_STATUS_HPP

#include <sstream>
#include <string>

#include "common.hpp"

namespace jumanpp {

enum class StatusCode : int {
  Ok = 0,
  InvalidParameter = 1,
  InvalidState = 2,
  MaxStatus
};

constexpr const char *const names[] = {"Ok", "InvalidParameter", "InvalidState",
                                       "MaxStatus"};

class StatusConstructor {
  StatusCode code_;
  std::stringstream msgbuilder_;

 public:
  StatusConstructor(StatusCode code) : code_(code) {}

  template <typename T>
  StatusConstructor &operator<<(const T &o) {
    msgbuilder_ << o;
    return *this;
  }

  StatusCode code() const { return code_; }

  std::string message() const { return msgbuilder_.str(); }
};

class Status {
 public:
 public:
  StatusCode code;
  std::string message;

 public:
  Status(StatusCode code_) : code(code_) {}

  Status(StatusCode code_, std::string message_)
      : code(code_), message(message_) {}

  Status(const StatusConstructor &ctr)
      : code{ctr.code()}, message{ctr.message()} {}

  bool isOk() const noexcept { return code == StatusCode::Ok; }

  explicit operator bool() const noexcept { return isOk(); }

  static Status Ok() { return Status(StatusCode::Ok); }

  static StatusConstructor InvalidParameter() {
    return StatusConstructor{StatusCode::InvalidParameter};
  }

  static StatusConstructor InvalidState() {
    return StatusConstructor{StatusCode::InvalidState};
  }
};

std::ostream &operator<<(std::ostream &str, const Status &st) {
  int idx = (int)st.code;
  auto *name = names[idx];
  str << idx << ":" << name;
  if (!st.message.empty()) {
    str << " " << st.message;
  }
  return str;
}

#define JPP_RETURN_IF_ERROR(expr)                            \
  {                                                          \
    Status __status__ = (expr);                              \
    if (JPP_UNLIKELY(!__status__.isOk())) return __status__; \
  }
}

#endif  // JUMANPP_STATUS_HPP