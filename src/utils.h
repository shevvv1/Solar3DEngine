#pragma once

#include <stdexcept>
#include <unordered_map>

namespace Utils {

template <typename K, typename V> class BiMap {
private:
  std::unordered_map<K, V> keyToValue;
  std::unordered_map<V, K> valueToKey;

public:
  // Insert a key-value pair
  bool insert(const K &key, const V &value) {
    // Check if either key or value already exists
    if (keyToValue.find(key) != keyToValue.end() ||
        valueToKey.find(value) != valueToKey.end()) {
      return false;
    }

    keyToValue[key] = value;
    valueToKey[value] = key;
    return true;
  }

  // Get value by key
  V getValue(const K &key) const {
    auto it = keyToValue.find(key);
    if (it == keyToValue.end()) {
      throw std::out_of_range("Key not found");
    }
    return it->second;
  }

  // Get key by value
  K getKey(const V &value) const {
    auto it = valueToKey.find(value);
    if (it == valueToKey.end()) {
      throw std::out_of_range("Value not found");
    }
    return it->second;
  }

  // Check if key exists
  bool hasKey(const K &key) const {
    return keyToValue.find(key) != keyToValue.end();
  }

  // Check if value exists
  bool hasValue(const V &value) const {
    return valueToKey.find(value) != valueToKey.end();
  }

  // Remove by key
  bool removeByKey(const K &key) {
    auto it = keyToValue.find(key);
    if (it != keyToValue.end()) {
      valueToKey.erase(it->second);
      keyToValue.erase(it);
      return true;
    }
    return false;
  }

  // Remove by value
  bool removeByValue(const V &value) {
    auto it = valueToKey.find(value);
    if (it != valueToKey.end()) {
      keyToValue.erase(it->second);
      valueToKey.erase(it);
      return true;
    }
    return false;
  }

  // Get size
  size_t size() const { return keyToValue.size(); }

  // Check if empty
  bool empty() const { return keyToValue.empty(); }

  // Clear all entries
  void clear() {
    keyToValue.clear();
    valueToKey.clear();
  }

  // Forward iterator access
  auto begin() const { return keyToValue.begin(); }
  auto end() const { return keyToValue.end(); }
};

} // namespace Utils
