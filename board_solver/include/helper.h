#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <type_traits>
#include <utility>

namespace DataStructures::Hash
{
  template <typename Key, typename Val>
  // requires std::is_trivially_constructible_v<T> && std::is_trivially_destructible_v<T>
  class FastHashTable
  {
  private:
    constexpr static size_t DEFAULT_SIZE = 256;
    size_t _size_, _size_half_;
    size_t _utilized_ = 0;
    Val *array;
    bool *used;

  private:
    template <typename K>
    static uint64_t hash(const K &&in)
      requires std::is_integral_v<K> || std::is_floating_point_v<K> || std::is_convertible_v<K, std::string_view>
    {
      if constexpr(std::is_integral_v<K> || std::is_floating_point_v<K>)
      {
        return static_cast<const uint64_t>(in);
      }
      else if constexpr(std::is_same_v<K, std::string> || std::is_same_v<K, std::string_view>)
      {
        uint64_t hash = 0, cumm = 0;
        constexpr uint64_t SIZEOFCHAR = sizeof(char);
        constexpr uint64_t SIZEOFUINT64 = sizeof(uint64_t);
        for(const auto &i : in)
        {
          hash += i << (cumm++ * SIZEOFCHAR);
          if(cumm > SIZEOFUINT64)
          {
            cumm = 0;
          }
        }

        return hash;
      }
      else if constexpr(std::is_same_v<K, std::string> || std::is_same_v<K, std::string_view>)
      {
        uint64_t hash = 0, cumm = 0;
        constexpr uint64_t SIZE = std::strlen(in);
        constexpr uint64_t SIZEOFCHAR = sizeof(char);
        constexpr uint64_t SIZEOFUINT64 = sizeof(uint64_t);
        for(size_t i = 0; i < SIZE; i++)
        {
          hash += in[i] << (cumm++ * SIZEOFCHAR);
          if(cumm > SIZEOFUINT64)
          {
            cumm = 0;
          }
        }

        return hash;
      }
      else
      {
        throw "Type of inputted hash is not suppported";
      }
    }

    inline size_t mod(const uint64_t &hash)
    {
      return hash % _size_;
    }

  public:
    void Insert(const Key && /*key*/, const Val & /*val*/) noexcept;
    void Remove(const Key && /*key*/) noexcept;
    [[nodiscard]] Val *get() noexcept;
    [[nodiscard]] size_t size() noexcept;

  public:
    Val &operator[](const Key &&in)
    {
      const auto hsh = hash(std::forward<const Key>(in));
      const auto loc = mod(hsh);
      return array[loc];
    }

  public:
    FastHashTable() : _size_(DEFAULT_SIZE), _size_half_(DEFAULT_SIZE / 2), array(new Val[DEFAULT_SIZE]), used(new bool[DEFAULT_SIZE])
    {
      std::memset(used, false, sizeof(bool) * _size_);
    }

    explicit FastHashTable(const size_t &&_starting_size)
        : _size_(_starting_size), _size_half_(_starting_size / 2), array(new Val[_starting_size]), used(new bool[_starting_size])
    {
      std::memset(used, false, sizeof(bool) * _size_);
    }

    FastHashTable(const FastHashTable &other) noexcept
    {
      delete[] array;

      _size_ = other._size_;           /* NOLINT */
      _size_half_ = other._size_half_; /* NOLINT */
      _utilized_ = other._utilized_;   /* NOLINT */
      array = new Val[_size_];         /* NOLINT */
      used = new bool[_size_];         /* NOLINT */

      std::memcpy(array, other.array, sizeof(Val) * _size_);
      std::memcpy(used, other.used, sizeof(bool) * _size_);
    }

    FastHashTable &operator=(const FastHashTable &other) noexcept /* NOLINT */
    {
      delete[] array;

      _size_ = other._size_;
      _size_half_ = other._size_half_;
      _utilized_ = other._utilized_;
      array = new Val[_size_];
      used = new bool[_size_];

      std::memcpy(array, other.array, sizeof(Val) * _size_);
      std::memcpy(used, other.used, sizeof(bool) * _size_);
    }

    FastHashTable(FastHashTable &&other) noexcept
    {
      delete[] array;

      _size_ = other._size_;           /* NOLINT */
      _size_half_ = other._size_half_; /* NOLINT */
      _utilized_ = other._utilized_;   /* NOLINT */
      array = new Val[_size_];         /* NOLINT */
      used = new bool[_size_];

      std::memmove(array, other.array, sizeof(Val) * _size_);
      std::memmove(used, other.used, sizeof(bool) * _size_);

      delete[] other.array;
      delete[] other.used;
      other.array = other.used = nullptr;
      other._size_ = other._size_half_ = other._utilized_ = 0;
    }

    FastHashTable &operator=(FastHashTable &&other) noexcept
    {
      delete[] array;

      _size_ = other._size_;
      _size_half_ = other._size_half_;
      _utilized_ = other._utilized_;
      array = new Val[_size_];
      used = new bool[_size_];

      std::memmove(array, other.array, sizeof(Val) * _size_);
      std::memmove(used, other.used, sizeof(bool) * _size_);

      delete[] other.array;
      delete[] other.used;
      other.array = other.used = nullptr;
      other._size_ = other._size_half_ = other._utilized_ = 0;
    }

    virtual ~FastHashTable()
    {
      delete[] array;
    }
  };

} // namespace DataStructures::Hash

#include "helper.ipp"
