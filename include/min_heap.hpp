// Copyright 2021 RKulagin

#ifndef INCLUDE_MIN_HEAP_HPP_
#define INCLUDE_MIN_HEAP_HPP_

#include <functional>
#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>
/// \brief A data structure consisting of (key, value) pairs that can be
/// obtained based on the key in constant time.
/// \tparam Key Type of key objects.
/// \tparam Value Type of mapped objects.
/// \tparam Cmp Comparison function object type, defaults to less<_Key>.
/// Using standard comparison heap is min-heap.
template <typename Key, typename Value, typename Cmp = std::less<Key>>
class Heap {
 public:
  using key_type = Key;
  using value_type = Value;
  using node = std::pair<key_type, value_type>;
  /// Add (key, value) pair to the heap
  /// \param key Key of pair to be stored
  /// \param value Value to be stored
  /// \return Index of pair in heap
  size_t insert(const key_type& key, const value_type& value) {
    return _insert(key, value);
  }

  size_t insert(key_type&& key, value_type&& value) {
    return _insert(std::move(key), std::move(value));
  }

  /// Returns a tuple containing the element with the minimal key.
  /// \throw std::out_of_range If heap is empty.
  /// \return A tuple of constant key reference, value reference, and element
  /// index on the heap.
  std::pair<const node&, size_t> min() {
    if (heap.empty()) {
      throw std::out_of_range("Heap is empty");
    }
    return {heap[0], 0};
  }

  /// Returns a tuple containing the element with the maximum key.
  /// \throw std::out_of_range If heap is empty.
  /// \return A tuple of constant key reference, value reference, and element
  /// index on the heap.
  std::pair<const node&, size_t> max() const {
    if (heap.empty()) {
      throw std::out_of_range("Heap is empty");
    }
    auto max_index = heap.size() - 1;
    auto stop = parent(max_index);
    for (size_t i = max_index; i > stop; --i) {
      if (cmp(heap[max_index].first, heap[i].first)) {
        max_index = i;
      }
    }
    return {heap[max_index], max_index};
  }

  /// \brief Access to %heap data.
  /// \param key The key for which data should be retrieved.
  /// \throw  std::out_of_range If no such data is present.
  /// \return A reference to the data whose key is equivalent to \a key, if
  /// such a data is present in the %heap
  ///
  /// Provides access to the data contained in heap under key.
  /// The key is checked that heap contains data with such key.
  /// The function throws out_of_range if the check fails.
  auto& at(const Key& key) { return heap[index.at(key)].second; }

  const auto& at(const Key& key) const { return heap[index.at(key)].second; }

  /// \brief Erases an element with minimal key from a %heap.
  /// \return Pair of removed key and value.
  auto extract_min() {
    if (heap.empty()) {
      throw std::out_of_range("Heap is empty");
    }
    auto extracted = std::move(heap[0]);
    index.erase(extracted.first);
    std::swap(heap.front(), heap.back());
    index[heap[0].first] = 0;
    heap.pop_back();
    heapify(0);
    return extracted;
  }

  /// \brief Erases an element from a %heap according to the provided key
  /// \param key Key of element to be erased
  /// \throw std::out_of_range If no such element with \a key in %heap
  void erase(const key_type& key) {
    auto place = index.at(key);
    std::swap(heap[place], heap.back());
    index[heap[place].first] = place;
    index.erase(key);
    heap.pop_back();
    heapify(place);
  }

  /// Returns true if the %heap is empty.
  inline bool empty() const { return heap.empty(); }

  /// Returns the size of the %heap
  inline size_t size() const { return heap.size(); }

  /// \brief Tries to locate an element in a %heap
  /// \param key Key of (key, value) pair to be located
  /// \throw std::out_of_range If no such element with \a key in %heap
  /// \return Pair of located (key, value) pair and index of this pair in
  /// storage
  std::pair<node, size_t> find(const key_type& key) const {
    auto i = index.at(key);
    return {heap[i], i};
  }

  /// \brief Checks if the heap contains a (key, value) pair with the given key
  /// \param key Key of (key, value) pair to be checked
  inline bool isin(const key_type& key) const{
    return index.count(key) == 1;
  }

 private:
  static constexpr inline size_t left(size_t i) { return (i << 1u) + 1; }
  static constexpr inline size_t right(size_t i) { return (i << 1u) + 2; }
  static constexpr inline size_t parent(size_t i) { return (i - 1) >> 1u; }

  template <typename Key_, typename Value_>
  size_t _insert(Key_&& key, Value_&& value) {
    if (index.count(key) != 0) {
      throw std::runtime_error("Can't insert value two times");
    }
    size_t i = heap.size();
    heap.push_back({});
    while (i > 0 && cmp(key, heap[parent(i)].first)) {
      heap[i] = std::move(heap[parent(i)]);
      index[heap[i].first] = i;
      i = parent(i);
    }
    heap[i] = {std::forward<Key_>(key), std::forward<Value_>(value)};
    index[heap[i].first] = i;
    return i;
  }

  /// Rearrange a heap to maintain the heap property.
  /// \param i Index to start heapify.
  void heapify(size_t i) {
    auto l = left(i);
    auto r = right(i);
    // Heapify down
    auto smallest = i;
    if (l < heap.size() && cmp(heap[l].first, heap[smallest].first)) {
      smallest = l;
    }
    if (r < heap.size() && cmp(heap[r].first, heap[smallest].first)) {
      smallest = r;
    }
    if (smallest != i) {
      std::swap(index[heap[i].first], index[heap[smallest].first]);
      std::swap(heap[i], heap[smallest]);
      heapify(smallest);
    }
    // Heapify up
    if (i > 0 && cmp(heap[i].first, heap[parent(i)].first)) {
      std::swap(index[heap[i].first], index[heap[parent(i)].first]);
      std::swap(heap[i], heap[parent(i)]);
      heapify(parent(i));
    }
  }

  std::vector<node> heap;
  std::unordered_map<key_type, size_t> index;
  Cmp cmp = Cmp();

  template <typename Key_, typename Value_, typename Cmp_>
  friend std::ostream& operator<<(std::ostream&,
                                  const Heap<Key_, Value_, Cmp_>&);
};

/// Outputs the heap layer by layer to the output stream
template <typename Key, typename Value, typename Cmp = std::less<Key>>
std::ostream& operator<<(std::ostream& out,
                         const Heap<Key, Value, Cmp>& min_heap) {
  if (min_heap.empty()) {
    out << "_";
    return out;
  }
  // Print root
  out << "[" << min_heap.heap.front().first << " "
      << min_heap.heap.front().second << "]";
  // Print non-root part of heap
  size_t i = 1;
  size_t layer_size = 2;
  while (layer_size <= min_heap.size()) {
    out << "\n";

    for (size_t j = 0; j < layer_size; ++j, ++i) {
      if (j != 0) {
        out << " ";
      }
      if (i < min_heap.size()) {
        out << "[" << min_heap.heap[i].first << " " << min_heap.heap[i].second
            << " " << min_heap.heap[min_heap.parent(i)].first << "]";
      } else {
        out << "_";
      }
    }
    layer_size <<= 1u;
  }

  return out;
}

#endif  // INCLUDE_MIN_HEAP_HPP_
