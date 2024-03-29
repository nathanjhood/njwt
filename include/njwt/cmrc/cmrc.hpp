/**
 * @file cmrc.hpp
 * @author vector-of-bool <vectorofbool@gmail.com>
 * @brief THIS FILE IS AUTO-GENERATED BY 'CMakeRC.cmake'
 * @version 2.0.1
 * @date 2018-09-08
 *
 * @copyright Copyright (c) 2017 vector-of-bool <vectorofbool@gmail.com>
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef CMRC_CMRC_HPP_INCLUDED
#define CMRC_CMRC_HPP_INCLUDED

#include <cassert>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <mutex>
#include <string>
#include <system_error>
#include <type_traits>

#if !(defined(__EXCEPTIONS) \
  || defined(__cpp_exceptions) \
  || defined(_CPPUNWIND) \
  || defined(CMRC_NO_EXCEPTIONS))
 #define CMRC_NO_EXCEPTIONS 1
#endif

/**
 * @brief The ```cmrc``` namespace.
 *
 */
namespace cmrc
{

/**
 * @brief The ```cmrc::detail``` namespace.
 *
 */
namespace detail
{

struct dummy;

} // namespace detail

} // namespace cmrc

#define CMRC_DECLARE(libid) \
namespace cmrc { \
namespace detail { \
struct dummy; \
static_assert(std::is_same<dummy, ::cmrc::detail::dummy>::value, \
"CMRC_DECLARE() must only appear at the global namespace"); \
} \
} \
namespace cmrc { \
namespace libid { \
cmrc::embedded_filesystem get_filesystem(); \
} \
} \
static_assert(true, "")

namespace cmrc
{
/** @addtogroup cmrc
 *  @{
 */

/**
 * @brief The ```cmrc::file``` class.
 *
 * @link https://github.com/vector-of-bool/cmrc?tab=readme-ov-file#members-of-cmrcfile
 *
 */
class file
{
public:
  using iterator = const char*;
  using const_iterator = iterator;

  /**
   * @brief Return an ```iterator``` to the beginning of the resource.
   *
   * @return ```iterator```
   */
  iterator begin() const noexcept { return _begin; }

  /**
   * @brief Return an ```iterator``` to the beginning of the resource.
   *
   * @return ```iterator```
   */
  iterator cbegin() const noexcept { return _begin; }

  /**
   * @brief Return an ```iterator``` past the end of the resource.
   *
   * @return ```iterator```
   */
  iterator end() const noexcept { return _end; }

  /**
   * @brief Return an ```iterator``` past the end of the resource.
   *
   * @return ```iterator```
   */
  iterator cend() const noexcept { return _end; }

  /**
   * @brief
   *
   * @return ```std::size_t```
   */
  std::size_t size() const
  {
    return static_cast<std::size_t>(std::distance(begin(), end()));
  }

  /**
   * @brief Default constructor, refers to no resource.
   *
   */
  file() = default;

  /**
   * @brief Construct a new ```cmrc::file``` object.
   *
   * @param beg
   * @param end
   */
  file(iterator beg, iterator end) noexcept
   : _begin(beg)
   , _end(end)
  {}

private:
  const char* _begin = nullptr;
  const char* _end = nullptr;
};

class directory_entry;

namespace detail
{
/** @addtogroup detail
 *  @{
 */

class directory;
class file_data;

/**
 * @brief The ```cmrc::detail::file_or_directory``` class.
 *
 */
class file_or_directory
{
public:
  /**
   * @brief Construct a new ```cmrc::detail::file_or_directory``` object.
   *
   * @param f
   */
  explicit file_or_directory(file_data& f)
  {
    _data.file_data = &f;
  }

  /**
   * @brief Construct a new ```cmrc::detail::file_or_directory``` object.
   *
   * @param d
   */
  explicit file_or_directory(directory& d)
  {
    _data.directory = &d;
    _is_file = false;
  }

  /**
   * @brief
   *
   * @return ```true```
   * @return ```false```
   */
  bool is_file() const noexcept
  {
    return _is_file;
  }

  /**
   * @brief
   *
   * @return ```true```
   * @return ```false```
   */
  bool is_directory() const noexcept
  {
    return !is_file();
  }

  /**
   * @brief
   *
   * @return ```const cmrc::detail::directory&```
   */
  const directory& as_directory() const noexcept
  {
    assert(!is_file());
    return *_data.directory;
  }

  /**
   * @brief
   *
   * @return ```const cmrc::detail::file_data&```
   */
  const file_data& as_file() const noexcept
  {
    assert(is_file());
    return *_data.file_data;
  }

private:

  union _data_t
  {
    class file_data* file_data;
    class directory* directory;
  } _data;

  bool _is_file = true;
};

/**
 * @brief the ```cmrc::detail::file_data``` class.
 *
 */
class file_data
{
public:
  const char* begin_ptr;
  const char* end_ptr;

  /**
   * @brief Default-construct a new ```cmrc::detail::file_data``` object.
   *
   */
  file_data(const file_data&) = delete;

  /**
   * @brief Copy-construct a new ```cmrc::detail::file_data``` object.
   *
   * @param b
   * @param e
   */
  file_data(const char* b, const char* e)
   : begin_ptr(b)
   , end_ptr(e)
  {}

};

/**
 * @brief
 *
 * @param path
 * @return ```std::pair<std::string, std::string>```
 */
inline std::pair<std::string, std::string> split_path(const std::string& path)
{
  auto first_sep = path.find("/");

  if (first_sep == path.npos) {
    return std::make_pair(path, "");
  } else {
    return std::make_pair(path.substr(0, first_sep), path.substr(first_sep + 1));
  }
}

struct created_subdirectory
{
  class directory& directory;
  class file_or_directory& index_entry;
};

/**
 * @brief The ```cmrc::detail::directory``` class.
 *
 */
class directory
{
private:
  std::list<file_data> _files;
  std::list<directory> _dirs;
  std::map<std::string, file_or_directory> _index;

  using base_iterator = std::map<std::string, file_or_directory>::const_iterator;

public:

  /**
   * @brief Default-construct a new ```cmrc::detail::directory``` object.
   *
   */
  directory() = default;

  /**
   * @brief Construct a new ```cmrc::detail::directory``` object.
   *
   */
  directory(const directory&) = delete;

  /**
   * @brief
   *
   * @param name
   * @return ```cmrc::detail::created_subdirectory```
   */
  created_subdirectory add_subdir(std::string name) &
  {
    _dirs.emplace_back();
    auto& back = _dirs.back();
    auto& fod = _index.emplace(name, file_or_directory{back}).first->second;
    return created_subdirectory{back, fod};
  }

  /**
   * @brief
   *
   * @param name
   * @param begin
   * @param end
   * @return ```cmrc::detail::file_or_directory*```
   */
  file_or_directory* add_file(std::string name, const char* begin, const char* end) &
  {
    assert(_index.find(name) == _index.end());
    _files.emplace_back(begin, end);
    return &_index.emplace(name, file_or_directory{_files.back()}).first->second;
  }

  /**
   * @brief
   *
   * @param path
   * @return ```const cmrc::detail::file_or_directory*```
   */
  const file_or_directory* get(const std::string& path) const
  {
    auto pair = split_path(path);
    auto child = _index.find(pair.first);
    if (child == _index.end()) {
      return nullptr;
    }
    auto& entry  = child->second;
    if (pair.second.empty()) {
      // We're at the end of the path
      return &entry;
    }

    if (entry.is_file()) {
      // We can't traverse into a file. Stop.
      return nullptr;
    }
    // Keep going down
    return entry.as_directory().get(pair.second);
  }

  /**
   * @brief The ```directory::iterator``` class.
   *
   * @link https://github.com/vector-of-bool/cmrc?tab=readme-ov-file#members-of-cmrcdirectory_iterator
   *
   */
  class iterator
  {
  private:
    base_iterator _base_iter;
    base_iterator _end_iter;
  public:
    using value_type = directory_entry;
    using difference_type = std::ptrdiff_t;
    using pointer = const value_type*;
    using reference = const value_type&;
    using iterator_category = std::input_iterator_tag;

    /**
     * @brief Default-construct a new ```iterator``` object.
     *
     */
    iterator() = default;

    /**
     * @brief Initialize-construct a new ```iterator``` object.
     *
     * @param iter
     * @param end
     */
    explicit iterator(base_iterator iter, base_iterator end)
     : _base_iter(iter)
     , _end_iter(end)
    {}

    /**
     * @brief Returns ```*this```.
     *
     * @return iterator
     */
    iterator begin() const noexcept
    {
      return *this;
    }

    /**
     * @brief Returns a past-the-end iterator corresponding to this iterator.
     *
     * @return iterator
     */
    iterator end() const noexcept
    {
      return iterator(_end_iter, _end_iter);
    }

    /**
     * @brief Returns the ```directory_entry``` for which the iterator
     * corresponds.
     *
     * @return value_type
     */
    inline value_type operator*() const noexcept;

    /**
     * @brief Implement iterator semantics.
     *
     * @param rhs
     * @return true
     * @return false
     */
    bool operator==(const iterator& rhs) const noexcept
    {
      return _base_iter == rhs._base_iter;
    }

    /**
     * @brief Implement iterator semantics.
     *
     * @param rhs
     * @return true
     * @return false
     */
    bool operator!=(const iterator& rhs) const noexcept
    {
      return !(*this == rhs);
    }

    /**
     * @brief Implement iterator semantics.
     *
     * @return iterator&
     */
    iterator& operator++() noexcept
    {
      ++_base_iter;
      return *this;
    }

    /**
     * @brief Implement iterator semantics.
     *
     * @return iterator
     */
    iterator operator++(int) noexcept
    {
      auto cp = *this;
      ++_base_iter;
      return cp;
    }
  };

  using const_iterator = iterator;

  /**
   * @brief
   *
   * @return iterator
   */
  iterator begin() const noexcept
  {
    return iterator(_index.begin(), _index.end());
  }

  /**
   * @brief
   *
   * @return iterator
   */
  iterator end() const noexcept
  {
    return iterator();
  }
};

/**
 * @brief
 *
 * @param path
 * @return std::string
 */
inline std::string normalize_path(std::string path)
{
  while (path.find("/") == 0) {
    path.erase(path.begin());
  }
  while (!path.empty() && (path.rfind("/") == path.size() - 1)) {
    path.pop_back();
  }
  auto off = path.npos;
  while ((off = path.find("//")) != path.npos) {
    path.erase(path.begin() + static_cast<std::string::difference_type>(off));
  }
  return path;
}

using index_type = std::map<std::string, const cmrc::detail::file_or_directory*>;

  /// @} group detail
} // namespace detail

/**
 * @brief The ```cmrc::directory_entry``` class.
 *
 */
class directory_entry
{
public:
  /**
   * @brief Default-construct a new ```cmrc::directory_entry``` object.
   *
   */
  directory_entry() = delete;

  /**
   * @brief Copy-construct a new ```cmrc::directory_entry``` object.
   *
   * @param filename
   * @param item
   */
  explicit directory_entry(std::string filename, const detail::file_or_directory& item)
    : _fname(filename)
    , _item(&item)
  {}

  /**
   * @brief The filename of the entry.
   *
   * @return ```const std::string&```
   */
  const std::string& filename() const &
  {
    return _fname;
  }

  /**
   * @brief The filename of the entry.
   *
   * @return ```std::string```
   */
  std::string filename() const &&
  {
    return std::move(_fname);
  }

  /**
   * @brief Returns ```true``` is the entry is a file; else, returns
   * ```false```.
   *
   * @return ```true```
   * @return ```false```
   */
  bool is_file() const
  {
    return _item->is_file();
  }

  /**
   * @brief Returns ```true``` is the entry is a directory; else, returns
   * ```false```.
   *
   * @return ```true ```
   * @return ```false```
   */
  bool is_directory() const
  {
    return _item->is_directory();
  }

private:
  std::string _fname;
  const detail::file_or_directory* _item;
};

directory_entry detail::directory::iterator::operator*() const noexcept {
  assert(begin() != end());
  return directory_entry(_base_iter->first, _base_iter->second);
}

using directory_iterator = detail::directory::iterator;

/**
 * @brief The ```embedded_filesystem``` class.
 *
 * @link https://github.com/vector-of-bool/cmrc?tab=readme-ov-file#methods-on-cmrcembedded_filesystem
 *
 */
class embedded_filesystem
{
  // Never-null:
  const cmrc::detail::index_type* _index;

  const detail::file_or_directory* _get(std::string path) const
  {
    path = detail::normalize_path(path);
    auto found = _index->find(path);
    if (found == _index->end()) {
      return nullptr;
    } else {
      return found->second;
    }
  }

public:

  /**
   * @brief Construct a new ```embedded_filesystem``` object.
   *
   * @param index
   */
  explicit embedded_filesystem(const detail::index_type& index)
    : _index(&index)
  {}

  /**
   * @brief Opens and returns a non-directory ```file``` object at ```path```,
   * or throws ```std::system_error()``` on error.
   *
   * @param path
   * @return ```cmrc::file```
   */
  file open(const std::string& path) const
  {
    auto entry_ptr = _get(path);
    if (!entry_ptr || !entry_ptr->is_file()) {
#ifdef CMRC_NO_EXCEPTIONS
      fprintf(stderr, "Error no such file or directory: %s\n", path.c_str());
      abort();
#else
      throw std::system_error(make_error_code(std::errc::no_such_file_or_directory), path);
#endif
    }
    auto& dat = entry_ptr->as_file();
    return file{dat.begin_ptr, dat.end_ptr};
  }

  /**
   * @brief Returns ```true``` if the given ```path``` names a regular file,
   * ```false``` otherwise.
   *
   * @param path
   * @return ```true```
   * @return ```false```
   */
  bool is_file(const std::string& path) const noexcept
  {
    auto entry_ptr = _get(path);
    return entry_ptr && entry_ptr->is_file();
  }

  /**
   * @brief Returns ```true``` if the given ```path``` names a directory,
   * ```false``` otherwise.
   *
   * @param path
   * @return ```true```
   * @return ```false```
   */
  bool is_directory(const std::string& path) const noexcept
  {
    auto entry_ptr = _get(path);
    return entry_ptr && entry_ptr->is_directory();
  }

  /**
   * @brief Returns ```true``` if the given ```path``` names an existing file
   * or directory, ```false``` otherwise.
   *
   * @param path
   * @return ```true```
   * @return ```false```
   */
  bool exists(const std::string& path) const noexcept
  {
    return !!_get(path);
  }

  /**
   * @brief Returns a ```directory_iterator``` for iterating the contents of a
   * directory. Throws if the given ```path``` does not identify a directory.
   *
   * @param path
   * @return ```cmrc::directory_iterator```
   */
  directory_iterator iterate_directory(const std::string& path) const
  {
    auto entry_ptr = _get(path);
    if (!entry_ptr) {
#ifdef CMRC_NO_EXCEPTIONS
      fprintf(stderr, "Error no such file or directory: %s\n", path.c_str());
      abort();
#else
      throw std::system_error(make_error_code(std::errc::no_such_file_or_directory), path);
#endif
    }
    if (!entry_ptr->is_directory()) {
#ifdef CMRC_NO_EXCEPTIONS
      fprintf(stderr, "Error not a directory: %s\n", path.c_str());
      abort();
#else
      throw std::system_error(make_error_code(std::errc::not_a_directory), path);
#endif
    }
    return entry_ptr->as_directory().begin();
  }
};

  /// @} group cmrc
} // namespace cmrc

#endif // CMRC_CMRC_HPP_INCLUDED
