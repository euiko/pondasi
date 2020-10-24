#include <utility>
#ifndef __PONDASI_UTILITY_RESULT_HPP__

namespace pondasi {

namespace detail {

template <typename ValueType, int Id = 0>
struct ValueWrapperWithId {
  inline explicit ValueWrapperWithId(const ValueType& value) : val_(value) {}
  inline explicit ValueWrapperWithId(ValueType&& value) : val_(value) {}

  ValueType val_;
};

template <int Id>
struct ValueWrapperWithId<void, Id> {};

template <typename T>
using Ok = detail::ValueWrapperWithId<T, 1>;
template <typename E>
using Err = detail::ValueWrapperWithId<E, 0>;
}  // namespace detail

template <typename T>
inline detail::Ok<T> Ok(T&& t) {
  return detail::Ok<T>(std::forward<T>(t));
}
inline detail::Ok<void> Ok() {
  return detail::Ok<void>{};
}

template <typename E>
inline detail::Err<E> Err(E&& e) {
  return detail::Err<E>(std::forward<E>(e));
}
inline detail::Err<void> Err() {
  return detail::Err<void>{};
}

template <typename T, typename E>
class Result {
 public:
  Result() = default;
  using Ok = detail::Ok<T>;
  using Err = detail::Err<E>;

  Result(const Result<T, E>& other);
  Result(Ok&& t); // NOLINT
  Result(Err&& e); // NOLINT
  ~Result();

  void copy(const Result& other);

  Result& operator=(const Result& rhs);
  explicit operator bool();
  // check whether is contain Ok value
  bool isOk();
  // check whether is contain E value
  bool isErr();
  // check whether is contain any of Ok or Err
  bool valid();
  // reset the value to uninitialized
  void reset();
  // forward constructor of Ok value
  template <typename... Args>
  void setOk(Args&&... args);
  // forward constructor of Err value
  template <typename... Args>
  void setErr(Args&&... args);
  // set the forwarded and moved of Ok value
  void set(Ok&& t);
  // set the forwarded and moved of Err value
  void set(Err&& e);

 private:
  // representation of underlying value type on runtime
  enum class Type {
    kTypeOk,
    kTypeErr,
    kTypeNone,
  };

  // to hold storage of T and E
  union Storage {
    Storage(){};
    ~Storage(){};
    Ok t;
    Err e;
  } value_;
  Type type_ = Type::kTypeNone;
};

template <typename T, typename E>
inline Result<T, E>::~Result() {
  reset();
}

template <typename T, typename E>
inline Result<T, E>::Result(const Result& other) {
  copy(other);
}
template <typename T, typename E>
inline Result<T, E>::Result(Ok&& t) {
  this->set(std::forward<Ok>(t));
}

template <typename T, typename E>
inline Result<T, E>::Result(Err&& e) {
  this->set(std::forward<Err>(e));
}

template <typename T, typename E>
inline void Result<T, E>::copy(const Result& other) {
  switch(other.type_) {
  case Type::kTypeOk:
    setOk(other.value_.t);
    break;
  case Type::kTypeErr:
    setErr(other.value_.e);
    break;
  case Type::kTypeNone:
    type_ = Type::kTypeNone;
    break;
  }
}

template <typename T, typename E>
inline Result<T, E>::operator bool() {
  return isOk();
}

template <typename T, typename E>
inline Result<T, E>& Result<T, E>::operator=(const Result& rhs) {
  copy(rhs);
  return *this;
}

template <typename T, typename E>
inline bool Result<T, E>::isOk() {
  return type_ == Type::kTypeOk;
}

template <typename T, typename E>
inline bool Result<T, E>::isErr() {
  return type_ == Type::kTypeErr;
}

template <typename T, typename E>
inline bool Result<T, E>::valid() {
  return type_ == Type::kTypeNone;
}

template <typename T, typename E>
inline void Result<T, E>::reset() {
  if (!valid()) return;

  if (isOk()) {
    value_.t.~Ok();
  } else if (isErr()) {
    value_.e.~Err();
  }

  type_ = Type::kTypeNone;
}

template <typename T, typename E>
template <typename... Args>
inline void Result<T, E>::setOk(Args&&... args) {
  // reset if value has been set
  reset();
  // instantiate the lifetime of object T using forwarded construction
  new (&value_.t) Ok(std::forward<Args>(args)...);
  type_ = Type::kTypeOk;
}

template <typename T, typename E>
template <typename... Args>
inline void Result<T, E>::setErr(Args&&... args) {
  // reset if value has been set
  reset();
  // instantiate the lifetime of object E using forwarded construction
  new (&value_.e) Err(std::forward<Args>(args)...);
  type_ = Type::kTypeErr;
}

template <typename T, typename E>
inline void Result<T, E>::set(Ok&& t) {
  // reset if value has been set
  reset();
  // instantiate the lifetime of object T using forwarded object
  new (&value_.t) Ok{std::forward<Ok>(t)};
  type_ = Type::kTypeOk;
}

template <typename T, typename E>
inline void Result<T, E>::set(Err&& e) {
  // reset if value has been set
  reset();
  // instantiate the lifetime of object E using forwarded object
  new (&value_.e) Err{std::forward<Err>(e)};
  type_ = Type::kTypeErr;
}

}  // namespace pondasi

#endif  // !__PONDASI_UTILITY_RESULT_HPP__