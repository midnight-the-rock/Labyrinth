//// an std::optional<T> rip off

// should take a look at cppreference every now and then
// to see if something can be added/removed
//
// https://en.cppreference.com/w/cpp/utility/optional

#pragma once

#include <util/panic.h>

namespace util {

  template <class T>
  class optional final {
  public:
    optional()
      : m_has_data(false), m_data() {};

    optional(const T& item)
      : m_has_data(true), m_data(item) {};

    auto value() -> T& {
      if (m_has_data) {
	return m_data;
      }

      util::panic("util::optional<T> tried to access a non existent value!");
    };
    
    template <class U>
    auto value_or(U&& default_value) -> T {
      if (m_has_data) {
	return m_data;
      }

      return default_value;
    }

    auto has_value() -> bool {
      return m_has_data;
    };

  private:
    T    m_data;
    bool m_has_data;
  };

}
