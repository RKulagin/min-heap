// Copyright 2021 Your Name <your_email>

#include <gtest/gtest.h>

#include <min_heap.hpp>
#include <stdexcept>

TEST(Heap, RValue) {
  Heap<int64_t, std::string> heap;
  heap.insert(10, "10");
  auto correct = std::make_pair<int64_t, std::string>(10, "10");
  EXPECT_EQ(heap.min().first.first, correct.first);
  EXPECT_EQ(heap.min().first.second, correct.second);
  heap.insert(-1, "-1");
  heap.insert(1, "1");
  EXPECT_EQ(heap.min().first.first, -1);
  EXPECT_EQ(heap.min().first.second, "-1");
  EXPECT_EQ(heap.at(1), "1");
  heap.at(1) = "1000";
  EXPECT_EQ(heap.at(1), "1000");
  auto top = heap.extract_min();
  EXPECT_EQ(top.first, -1);
  EXPECT_EQ(heap.min().first.first, 1);
}

TEST(Heap, LValue) {
  Heap<int64_t, std::string> heap;
  auto correct = std::make_pair<int64_t, std::string>(10, "10");
  heap.insert(correct.first, correct.second);
  EXPECT_EQ(heap.min().first.first, correct.first);
  EXPECT_EQ(heap.min().first.second, correct.second);
}


TEST(Heap, Print){ Heap<int64_t , std::string> heap;
  heap.insert(8, "10");
  heap.insert(4, "14");
  heap.insert(7, "15");
  heap.insert(9, "11");
  heap.insert(3, "13");
  heap.insert(5, "16");
  heap.insert(88, "1");
  heap.insert(11, "2");
  heap.insert(6, "18");
  heap.insert(1, "22");
  std::stringstream stream;
  stream<<heap;
  EXPECT_EQ(stream.str(), "[1 22]\n"
            "[3 13 1] [5 16 1]\n"
            "[6 18 3] [4 14 3] [7 15 5] [88 1 5]\n"
            "[11 2 6] [9 11 6] [8 10 4] _ _ _ _ _");

}