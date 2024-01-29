#if __has_include(<cmrc/cmrc.hpp>)
# include <cmrc/cmrc.hpp>
#else
# error "Error: Can't find 'cmrc.hpp'?"
#endif

/** @brief the 'njwt::cmrc' namespace. */
namespace njwt::cmrc {
  using namespace cmrc;
} // namespace njwt::cmrc
