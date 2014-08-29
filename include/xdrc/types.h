// -*- C++ -*-

/** \file xdrc.h Type definitions for xdrc compiler output. */

#ifndef _XDRC_H_HEADER_INCLUDED_
#define _XDRC_H_HEADER_INCLUDED_ 1

#include <array>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace xdr {

//! Generic class of XDR unmarshaling errors.
struct xdr_runtime_error : std::runtime_error {
  using std::runtime_error::runtime_error;
};

//! Attempt to exceed the bounds of a variable-length array or string.
struct xdr_overflow : xdr_runtime_error {
  using xdr_runtime_error::xdr_runtime_error;
};

//! Attempt to set invalid value for a union discriminant.
struct xdr_bad_value : xdr_runtime_error {
  using xdr_runtime_error::xdr_runtime_error;
};

//! Attempt to access wrong field of a union.
struct xdr_wrong_union : std::logic_error {
  using std::logic_error::logic_error;
};


constexpr std::uint32_t XDR_MAX_LEN = 0xffffffff;

//! A vector with a maximum size (returned by xvector::max_size()).
//! Note that you can exceed the size, but an error will happen when
//! marshaling or unmarshaling the data structure.
template<typename T, std::uint32_t N = XDR_MAX_LEN>
struct xvector : std::vector<T> {
  using vector = std::vector<T>;

  using vector::vector;

  //! Return the maximum size allowed by the type.
  static constexpr std::uint32_t max_size() { return N; }

  //! Check whether a size is in bounds
  static void check_size(size_t n) {
    if (n > max_size())
      throw xdr_overflow("xvector overflow");
  }
};

//! Variable-length opaque data is just a vector of std::uint32_t.
template<std::uint32_t N = XDR_MAX_LEN> using opaque = xvector<std::uint8_t, N>;

//! A string with a maximum length (returned by xstring::max_size()).
//! Note that you can exceed the size, but an error will happen when
//! marshaling or unmarshaling the data structure.
template<std::uint32_t N = XDR_MAX_LEN>
struct xstring : std::string {
  using string = std::string;

  //! Return the maximum size allowed by the type.
  static constexpr std::uint32_t max_size() { return N; }

  //! Check whether a size is in bounds
  static void check_size(size_t n) {
    if (n > max_size())
      throw xdr_overflow("xvector overflow");
  }

  //! Check that the string length is not greater than the maximum
  //! size.  \throws std::out_of_range and clears the contents of the
  //! string if it is too long.
  void validate() const { check_size(size()); }

  xstring() = default;
  xstring(const xstring &) = default;
  xstring(xstring &&) = default;
  xstring &operator=(const xstring &) = default;
  xstring &operator=(xstring &&) = default;

  template<typename...Args> xstring(Args&&...args)
    : string(std::forward<Args>(args)...) { validate(); }

  using string::data;
  char *data() { return &(*this)[0]; } // protobufs does this, so probably ok

#define ASSIGN_LIKE(method)					\
  template<typename...Args> xstring &method(Args&&...args) {	\
    string::method(std::forward<Args>(args)...);		\
    validate();							\
    return *this;						\
  }
  ASSIGN_LIKE(operator=)
  ASSIGN_LIKE(operator+=)
  ASSIGN_LIKE(append)
  ASSIGN_LIKE(push_back)
  ASSIGN_LIKE(assign)
  ASSIGN_LIKE(insert)
  ASSIGN_LIKE(replace)
  ASSIGN_LIKE(swap)
#undef ASSIGN_LIKE
};


struct _result_type_or_void_helper {
  template<typename T> static typename T::result_type sfinae(T *);
  static void sfinae(...);
};
//! \c result_type_or_void<T> is equivalent to the type \c
//! T::result_type unless \c T has no type named \c result_type, in
//! which case it is \c void.
template<typename T> using result_type_or_void =
  decltype(_result_type_or_void_helper::sfinae(static_cast<T*>(0)));

struct case_constructor_t {
  constexpr case_constructor_t() {}
  template<typename T> void operator()(T &t) const {
    new (static_cast<void *>(std::addressof(t))) T{};
  }
  void operator()() const {}
};
constexpr case_constructor_t case_constructor;

struct case_destroyer_t {
  constexpr case_destroyer_t() {}
  template<typename T> void operator()(T &t) const { t.~T(); }
  void operator()() const {}
};
constexpr case_destroyer_t case_destroyer;

}

#endif // !_XDRC_H_HEADER_INCLUDED_
