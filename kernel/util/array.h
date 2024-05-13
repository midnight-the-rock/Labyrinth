/* Simpler, less sophisticated version of std::array */

#pragma once

#include <types.h>

namespace util {

  template <class T, u64 S>
  class array {
  public:
    template <class... U>
    constexpr array(U&&... items) : m_data(items...), m_size(S) {
      static_assert(sizeof...(U) <= S, "array can not be bigger than originally declared");
    };

    auto data() -> T* {
      return m_data;
    }

    auto size() const -> u64 {
      return m_size;
    }

    auto end() -> T* {
      return (m_data + m_size);
    }

    auto cend() const -> const T* {
      return (m_data + m_size);
    }

    auto begin() -> T* {
      return m_data;
    }

    auto cbegin() const -> const T* {
      return m_data;
    }

    constexpr auto at(u64 index) -> T& {
      return m_data[index];
    }

    constexpr auto operator[](u64 index) -> T& {
      return m_data[index];
    }

  private:
    u64 m_size;
    T   m_data[S];
  };
  
}
