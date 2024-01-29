#if __has_include(<base64/base64.hpp>)
# include <base64/base64.hpp>
#else
# error "Error: Can't find 'base64.hpp'?"
#endif

/** @brief the 'njwt::base64' namespace. */
namespace njwt::base64 {
  using namespace base64;
} // namespace njwt::base64
