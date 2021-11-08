// Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_MIN_HEAP_HPP_
#define INCLUDE_MIN_HEAP_HPP_

#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>
template <typename Key, typename Value, typename Cmp = std::less<Key>>
class Heap {
 public:
  using key_type = Key;
  using value_type = Value;

  size_t insert(const key_type& key, const value_type& value) {
    return _insert(key, value);
  }

  size_t insert(key_type&& key, value_type&& value) {
    return _insert(std::move(key), std::move(value));
  }

  inline const auto& min() const { return heap[0]; }

  auto& min() {
    if (heap.empty()) {
      throw std::out_of_range("Heap is empty");
    }
    return heap[0];
  }

  std::pair<const std::pair<Key, Value>&, size_t> max() const {
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

  auto& at(const Key& key) { return heap[index.at(key)].second; }

  const auto& at(const Key& key) const { return heap[index.at(key)].second; }

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

  auto erase(const key_type& key) {
    auto place = index.at(key);
    std::swap(heap[place], heap.back());
    index[heap[place].first] = place;
    index.erase(key);
    heap.pop_back();
    heapify(place);
  }

  inline bool empty() const { return heap.empty(); }
  inline bool size() const { return heap.size(); }

  std::pair<std::pair<Key, Value>, size_t> find(const key_type& key) const {
    auto i = index.at(key);
    return {heap[i], i};
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

  void heapify(size_t i) {
    auto l = left(i);
    auto r = right(i);
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
    if (i > 0 && cmp(heap[i].first, heap[parent(i)].first)) {
      std::swap(index[heap[i].first], index[heap[parent(i)].first]);
      std::swap(heap[i], heap[parent(i)]);
      heapify(parent(i));
    }
  }

  std::vector<std::pair<Key, Value>> heap;
  std::unordered_map<Key, size_t> index;
  Cmp cmp = Cmp();

  template <typename Key_, typename Value_, typename Cmp_>
  friend std::ostream& operator<<(std::ostream&,
                                  const Heap<Key_, Value_, Cmp_>&);
};

template <typename Key, typename Value, typename Cmp = std::less<Key>>
std::ostream& operator<<(std::ostream& out, const Heap<Key, Value, Cmp>& heap) {
  if (heap.empty()) {
    out << "_";
    return out;
  }
  // Print root
  out << "[" << heap.heap.front().first << " " << heap.heap.front().second
      << "]";
  // Print non-root part of heap
  size_t i = 1;
  size_t layer_size = 2;
  while (layer_size <= heap.heap.size()) {
    out << "\n";

    for (size_t j = 0; j < layer_size; ++j, ++i) {
      if (j != 0) {
        out << " ";
      }
      if (i < heap.heap.size()) {
        out << "[" << heap.heap[i].first << " " << heap.heap[i].second << " "
            << heap.heap[heap.parent(i)].first << "]";
      } else {
        out << "_";
      }
    }
    layer_size <<= 1u;
  }

  return out;
}

#endif  // INCLUDE_MIN_HEAP_HPP_
