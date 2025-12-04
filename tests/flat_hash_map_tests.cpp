#include <gtest/gtest.h>
#include "flat_hash_map.h"
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <utility>

// --- Тесты конструкторов ---

TEST(FlatHashMapTest, DefaultConstructor) {
    flat_hash_map<int, int> map;
    EXPECT_TRUE(map.empty());
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(map.capacity(), 0);
    EXPECT_EQ(map.begin(), map.end());
}

TEST(FlatHashMapTest, BucketCountConstructor) {
    flat_hash_map<int, int> map(10);
    EXPECT_TRUE(map.empty());
    // Емкость должна быть >= 10 (скорее всего 16, так как next_power_of_two)
    EXPECT_GE(map.capacity(), 10);
}

TEST(FlatHashMapTest, InitializerListConstructor) {
    flat_hash_map<int, std::string> map = { {1, "one"}, {2, "two"}, {3, "three"} };
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map.at(1), "one");
    EXPECT_TRUE(map.contains(2));
}

TEST(FlatHashMapTest, RangeConstructor) {
    std::vector<std::pair<const int, int>> data = { {1, 10}, {2, 20}, {3, 30} };
    flat_hash_map<int, int> map(data.begin(), data.end());
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map[2], 20);
}

// --- Тесты вставки ---

TEST(FlatHashMapTest, InsertUnique) {
    flat_hash_map<std::string, int> map;
    auto result = map.insert({ "key", 100 });

    EXPECT_TRUE(result.second); // Вставка прошла успешно
    EXPECT_EQ(result.first->first, "key");
    EXPECT_EQ(result.first->second, 100);
    EXPECT_EQ(map.size(), 1);
}

TEST(FlatHashMapTest, InsertDuplicate) {
    flat_hash_map<int, int> map;
    map.insert({ 1, 10 });
    auto result = map.insert({ 1, 20 });

    EXPECT_FALSE(result.second); // Вставка не прошла (дубликат)
    EXPECT_EQ(result.first->second, 10); // Значение осталось старым
    EXPECT_EQ(map.size(), 1);
}

TEST(FlatHashMapTest, Emplace) {
    flat_hash_map<std::string, std::string> map;
    // Emplace конструирует элемент на месте
    auto result = map.emplace("hello", "world");

    EXPECT_TRUE(result.second);
    EXPECT_EQ(map.at("hello"), "world");
}

// --- Тесты доступа к элементам ---

TEST(FlatHashMapTest, OperatorBrackets) {
    flat_hash_map<int, int> map;

    map[1] = 10; // Вставка
    EXPECT_EQ(map.size(), 1);
    EXPECT_EQ(map.at(1), 10);

    map[1] = 20; // Обновление
    EXPECT_EQ(map.at(1), 20);
    EXPECT_EQ(map.size(), 1);
}

TEST(FlatHashMapTest, AtMethod) {
    flat_hash_map<int, int> map;
    map.insert({ 1, 100 });

    EXPECT_EQ(map.at(1), 100);

    // Проверка на выброс исключения при отсутствии ключа
    EXPECT_THROW(map.at(999), std::out_of_range);
}

// --- Тесты поиска и проверки наличия ---

TEST(FlatHashMapTest, FindAndContains) {
    flat_hash_map<int, int> map = { {1, 10}, {2, 20} };

    auto it = map.find(1);
    ASSERT_NE(it, map.end());
    EXPECT_EQ(it->second, 10);

    auto it_missing = map.find(3);
    EXPECT_EQ(it_missing, map.end());

    EXPECT_TRUE(map.contains(2));
    EXPECT_FALSE(map.contains(3));

    EXPECT_EQ(map.count(1), 1);
    EXPECT_EQ(map.count(5), 0);
}

// --- Тесты удаления ---

TEST(FlatHashMapTest, EraseByKey) {
    flat_hash_map<int, int> map = { {1, 10}, {2, 20}, {3, 30} };

    size_t count = map.erase(2);
    EXPECT_EQ(count, 1);
    EXPECT_EQ(map.size(), 2);
    EXPECT_FALSE(map.contains(2));

    size_t count_missing = map.erase(99);
    EXPECT_EQ(count_missing, 0);
}

TEST(FlatHashMapTest, EraseByIterator) {
    flat_hash_map<int, int> map = { {1, 10}, {2, 20}, {3, 30} };
    auto it = map.find(1); // Ищем ключ 1

    auto next_it = map.erase(it);
    EXPECT_EQ(map.size(), 2);
    EXPECT_FALSE(map.contains(1));

    // Проверяем, что итератор валиден (указывает на другой элемент или end)
    if (next_it != map.end()) {
        EXPECT_TRUE(next_it->first == 2 || next_it->first == 3);
    }
}

TEST(FlatHashMapTest, Clear) {
    flat_hash_map<int, int> map = { {1, 1}, {2, 2} };

    map.clear();
    EXPECT_TRUE(map.empty());
    EXPECT_EQ(map.size(), 0);
    EXPECT_EQ(map.capacity(), 0);
}

// --- Тесты итераторов ---

TEST(FlatHashMapTest, IteratorTraversal) {
    flat_hash_map<int, int> map = { {1, 1}, {2, 2}, {3, 3}, {4, 4} };
    size_t counter = 0;

    for (auto it = map.begin(); it != map.end(); ++it) {
        counter++;
        EXPECT_TRUE(map.contains(it->first));
    }
    EXPECT_EQ(counter, 4);
}

TEST(FlatHashMapTest, ConstIterator) {
    const flat_hash_map<int, int> map = { {1, 10} };
    auto it = map.begin(); // Должен вернуться const_iterator
    EXPECT_EQ(it->second, 10);
}

// --- Тесты копирования и перемещения ---

TEST(FlatHashMapTest, CopyConstructor) {
    flat_hash_map<int, int> original = { {1, 1}, {2, 2} };
    flat_hash_map<int, int> copy(original);

    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy[1], 1);

    copy.erase(1);
    EXPECT_FALSE(copy.contains(1));
    EXPECT_TRUE(original.contains(1)); // Оригинал не должен измениться
}

TEST(FlatHashMapTest, MoveConstructor) {
    flat_hash_map<int, int> original = { {1, 1}, {2, 2} };
    flat_hash_map<int, int> moved(std::move(original));

    EXPECT_EQ(moved.size(), 2);
    EXPECT_TRUE(moved.contains(1));
    EXPECT_TRUE(original.empty()); // Оригинал должен стать пустым после move
}

TEST(FlatHashMapTest, Swap) {
    flat_hash_map<int, int> m1 = { {1, 1} };
    flat_hash_map<int, int> m2 = { {2, 2} };

    m1.swap(m2);

    EXPECT_TRUE(m1.contains(2));
    EXPECT_FALSE(m1.contains(1));
    EXPECT_TRUE(m2.contains(1));
}

// --- Специфичные тесты реализации ---

TEST(FlatHashMapTest, ResizeLogic) {
    flat_hash_map<int, int> map(2);
    size_t initial_cap = map.capacity();

    // Вставляем много элементов, чтобы вызвать ресайз
    for(int i = 0; i < 100; ++i) {
        map.insert({i, i});
    }

    EXPECT_GT(map.capacity(), initial_cap);
    EXPECT_EQ(map.size(), 100);

    EXPECT_EQ(map.at(0), 0);
    EXPECT_EQ(map.at(99), 99);
}

TEST(FlatHashMapTest, Reserve) {
    flat_hash_map<int, int> map;
    map.reserve(100);
    EXPECT_GE(map.capacity(), 100);
}

TEST(FlatHashMapTest, TombstonesReuse) {
    flat_hash_map<int, int> map;
    map.reserve(16);

    map.insert({1, 10});
    map.insert({2, 20});

    map.erase(1); // Создаем "могильный камень" (DELETED)

    map.insert({3, 30}); // Должен корректно вставиться, возможно переиспользовав место

    EXPECT_TRUE(map.contains(3));
    EXPECT_FALSE(map.contains(1));
    EXPECT_TRUE(map.contains(2));
}
