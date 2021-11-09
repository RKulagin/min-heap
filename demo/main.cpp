#include <min_heap.hpp>

int main() {
  Heap<int64_t, std::string> heap;
  std::string command;
  int64_t key;
  std::string value;
  while (std::cin >> command) {
    try {
      if (command == "add") {
        std::cin >> key >> value;
        heap.insert(key, value);
      } else if (command == "set") {
        std::cin >> key >> value;
        heap.at(key) = value;
      } else if (command == "delete") {
        std::cin >> key;
        heap.erase(key);
      } else if (command == "search") {
        std::cin >> key;
        try {
          auto [node, index] = heap.find(key);
          std::cout << "1 " << index << " " << node.second << std::endl;
        } catch (std::out_of_range&) {
          std::cout << "0" << std::endl;
        }
      } else if (command == "min") {
        auto [node, idx] = heap.min();
        std::cout << node.first << " " << idx << " " << node.second
                  << std::endl;
      } else if (command == "max") {
        auto [node, idx] = heap.max();
        std::cout << node.first << " " << idx << " " << node.second
                  << std::endl;
      } else if (command == "print") {
        std::cout << heap << std::endl;
      } else if (command == "extract") {
        auto node = heap.extract_min();
        std::cout << node.first << " " << node.second << std::endl;
      }
    } catch (std::exception& e) {
      std::cout << "error" << std::endl;
    }
  }
  return 0;
}