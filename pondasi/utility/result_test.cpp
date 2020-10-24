#include "result.hpp"
#include "error.hpp"
#include <string>
#include <cassert>

using pondasi::Result;
using pondasi::Ok;
using pondasi::Err;

enum class MyCustomError {
  kTimeout,
  kInvalid,
  kTooLong,
};

Result<void, pondasi::Error<MyCustomError>> connectDb(const std::string& connection, int max_wait) {
  if(max_wait > 10) {
    return Err(pondasi::Error<MyCustomError>{MyCustomError::kTooLong});
  }

  if(connection.length() > 10) {
    return Err(pondasi::Error<MyCustomError>{MyCustomError::kTimeout, "timeout"});
  }

  return Ok();
}


int main() {

  Result<std::string, int> res1;
  res1.setOk("halohalo");
  assert(res1);
  res1.setErr(1);
  assert(!res1);

  Result<std::string, std::string> res2 = Ok<std::string>("halohalo");
  assert(res2);
  Result<std::string, std::string> res3 = Err<std::string>("halohalo");
  assert(!res3);

  Result<void, std::string> res4 = Ok();
  assert(res4);
  res4.setErr("halohalobandung");
  assert(!res4);

  Result<std::string, void> res5 = Err();
  assert(!res5);
  res5 = Ok<std::string>("halohalobandung");
  assert(res5);

  auto res = connectDb("db://local", 5);
  assert(res);
  
  res = connectDb("db://timedout", 5);
  assert(!res);

  res = connectDb("db://toolong", 11);
  assert(!res);

  return 0;
}