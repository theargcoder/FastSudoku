#pragma once

#include "helper.h"

// requires std::is_trivially_constructible_v<T> && std::is_trivially_destructible_v<T>
template <typename Key, typename Val>
void DataStructures::Hash::FastHashTable<Key, Val>::Insert(const Key &&key, const Val &&val) noexcept
{
  const auto hsh = hash(std::forward<Key>(key));
  const auto loc = mod(hsh);

  if(used[loc])
  {
    array[loc].~Val();
  }
  array[loc] = val;
  used[loc] = true;
}

// requires std::is_trivially_constructible_v<T> && std::is_trivially_destructible_v<T>
template <typename Key, typename Val>
void DataStructures::Hash::FastHashTable<Key, Val>::Remove(const Key &&key) noexcept
{
  const auto hsh = hash(std::forward<Key>(key));
  const auto loc = mod(hsh);

  if(used[loc])
  {
    array[loc].~Val();
  }
  used[loc] = false;
}

template <typename Key, typename Val>
// requires std::is_trivially_constructible_v<T> && std::is_trivially_destructible_v<T>
[[nodiscard]] Val *DataStructures::Hash::FastHashTable<Key, Val>::get() noexcept
{
  return array;
}

template <typename Key, typename Val>
// requires std::is_trivially_constructible_v<T> && std::is_trivially_destructible_v<T>
[[nodiscard]] size_t DataStructures::Hash::FastHashTable<Key, Val>::size() noexcept
{
  return _size_;
}
