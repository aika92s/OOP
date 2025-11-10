#include <gtest/gtest.h>
#include "BitArray.h"
#include <string>
#include <vector>

class BitArrayTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST(BitArrayTest, DefaultConstructor) {
    BitArray ba;
    EXPECT_EQ(ba.size(), 0);
    EXPECT_TRUE(ba.empty());
}

TEST(BitArrayTest, ConstructorWithSize) {
    BitArray ba(8);
    EXPECT_EQ(ba.size(), 8);
    EXPECT_FALSE(ba.empty());
    EXPECT_EQ(ba.to_string(), "00000000");
}

TEST(BitArrayTest, ConstructorWithSizeAndValue) {
    BitArray ba(8, 0xFF);
    EXPECT_EQ(ba.size(), 8);
    EXPECT_EQ(ba.to_string(), "11111111");
}

TEST(BitArrayTest, CopyConstructor) {
    BitArray original(8);
    original.set(2, true).set(5, true);

    BitArray copy(original);
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.to_string(), original.to_string());
    EXPECT_EQ(copy[2], true);
    EXPECT_EQ(copy[5], true);
}

TEST(BitArrayTest, AssignmentOperator) {
    BitArray original(8);
    original.set(3, true);

    BitArray assigned;
    assigned = original;

    EXPECT_EQ(assigned.size(), original.size());
    EXPECT_EQ(assigned.to_string(), original.to_string());
    EXPECT_EQ(assigned[3], true);
}

// Test set and reset operations
TEST(BitArrayTest, SetAndResetIndividualBits) {
    BitArray ba(8);

    ba.set(3, true);
    EXPECT_EQ(ba[3], true);
    EXPECT_EQ(ba.to_string(), "00010000");

    ba.set(3, false);
    EXPECT_EQ(ba[3], false);
    EXPECT_EQ(ba.to_string(), "00000000");

    ba.set(3);
    EXPECT_EQ(ba[3], true);
    EXPECT_EQ(ba.to_string(), "00010000");
}

TEST(BitArrayTest, SetAllAndResetAll) {
    BitArray ba(8);

    ba.set();
    EXPECT_EQ(ba.to_string(), "11111111");

    ba.reset();
    EXPECT_EQ(ba.to_string(), "00000000");
}

TEST(BitArrayTest, ResetIndividualBit) {
    BitArray ba(8);
    ba.set();

    ba.reset(4);
    EXPECT_EQ(ba.to_string(), "11101111");
}

// Test bitwise operations
TEST(BitArrayTest, BitwiseAndAssign) {
    BitArray ba1(8);
    BitArray ba2(8);

    ba1.set(1, true).set(3, true).set(5, true);
    ba2.set(1, true).set(2, true).set(5, true);

    ba1 &= ba2;
    EXPECT_EQ(ba1.to_string(), "00100010");  // Only bits 1 and 5 are set
}

TEST(BitArrayTest, BitwiseOrAssign) {
    BitArray ba1(8);
    BitArray ba2(8);

    ba1.set(1, true).set(3, true);
    ba2.set(2, true).set(5, true);

    ba1 |= ba2;
    EXPECT_EQ(ba1.to_string(), "00101110");  // Bits 1, 2, 3, 5 are set
}

TEST(BitArrayTest, BitwiseXorAssign) {
    BitArray ba1(8);
    BitArray ba2(8);

    ba1.set(1, true).set(3, true).set(5, true);
    ba2.set(1, true).set(2, true).set(3, true);

    ba1 ^= ba2;
    EXPECT_EQ(ba1.to_string(), "00010010");  // Only bit 2 and 5 are set
}

// Test bitwise operations with different sizes
TEST(BitArrayTest, BitwiseOperationsDifferentSizes) {
    BitArray ba1(4);
    BitArray ba2(8);

    ba1.set(1, true).set(3, true);  // "1010"
    ba2.set(0, true).set(2, true).set(4, true);  // "00010101"

    BitArray result = ba1 & ba2;
    EXPECT_EQ(result.size(), 8);
    EXPECT_EQ(result.to_string(), "00000000");  // Only common bits: "0000" & "0101" = "0000"

    result = ba1 | ba2;
    EXPECT_EQ(result.to_string(), "00011111");  // "00001010" | "00010101" = "00011111"

    result = ba1 ^ ba2;
    EXPECT_EQ(result.to_string(), "00011111");  // Different sizes behavior
}

// Test equality and inequality
TEST(BitArrayTest, EqualityOperators) {
    BitArray ba1(8);
    BitArray ba2(8);

    ba1.set(3, true);
    ba2.set(3, true);

    EXPECT_TRUE(ba1 == ba2);
    EXPECT_FALSE(ba1 != ba2);

    ba2.set(4, true);
    EXPECT_FALSE(ba1 == ba2);
    EXPECT_TRUE(ba1 != ba2);

    BitArray ba3(4);
    EXPECT_FALSE(ba1 == ba3);
    EXPECT_TRUE(ba1 != ba3);
}

// Test shift operations
TEST(BitArrayTest, LeftShift) {
    BitArray ba(8);
    ba.set(0, true).set(1, true).set(2, true);  // "00000111"

    ba <<= 2;
    EXPECT_EQ(ba.to_string(), "00011100");
}

TEST(BitArrayTest, RightShift) {
    BitArray ba(8);
    ba.set(5, true).set(6, true).set(7, true);  // "11100000"

    ba >>= 2;
    EXPECT_EQ(ba.to_string(), "00111000");
}

TEST(BitArrayTest, ShiftBySizeOrMore) {
    BitArray ba(8);
    ba.set(0, true).set(1, true).set(2, true);

    ba <<= 8;  // Shift by size
    EXPECT_EQ(ba.to_string(), "00000000");

    ba.set(0, true).set(1, true).set(2, true);
    ba <<= 10;  // Shift by more than size
    EXPECT_EQ(ba.to_string(), "00000000");
}

// Test bitwise negation
TEST(BitArrayTest, BitwiseNegation) {
    BitArray ba(8);
    ba.set(0, true).set(2, true).set(4, true).set(6, true);  // "01010101"

    BitArray negated = ~ba;
    EXPECT_EQ(negated.to_string(), "10101010");
}

// Test any and none
TEST(BitArrayTest, AnyAndNone) {
    BitArray ba(8);
    EXPECT_FALSE(ba.any());
    EXPECT_TRUE(ba.none());

    ba.set(3, true);
    EXPECT_TRUE(ba.any());
    EXPECT_FALSE(ba.none());

    ba.reset();
    EXPECT_FALSE(ba.any());
    EXPECT_TRUE(ba.none());

    BitArray empty;
    EXPECT_FALSE(empty.any());
    EXPECT_TRUE(empty.none());
}

// Test count
TEST(BitArrayTest, Count) {
    BitArray ba(8);
    EXPECT_EQ(ba.count(), 0);

    ba.set(1, true).set(3, true).set(5, true);
    EXPECT_EQ(ba.count(), 3);

    ba.set();
    EXPECT_EQ(ba.count(), 8);

    BitArray empty;
    EXPECT_EQ(empty.count(), 0);
}

// Test to_string
TEST(BitArrayTest, ToString) {
    BitArray ba(8);
    EXPECT_EQ(ba.to_string(), "00000000");

    ba.set(0, true).set(7, true);
    EXPECT_EQ(ba.to_string(), "10000001");

    ba.set(3, true).set(4, true);
    EXPECT_EQ(ba.to_string(), "10011001");

    BitArray empty;
    EXPECT_EQ(empty.to_string(), "");
}

// Test subscript operator
TEST(BitArrayTest, SubscriptOperator) {
    BitArray ba(8);
    ba.set(2, true).set(5, true);

    EXPECT_EQ(ba[0], false);
    EXPECT_EQ(ba[2], true);
    EXPECT_EQ(ba[5], true);
    EXPECT_EQ(ba[7], false);
}

// Test size and empty
TEST(BitArrayTest, SizeAndEmpty) {
    BitArray ba(10);
    EXPECT_EQ(ba.size(), 10);
    EXPECT_FALSE(ba.empty());

    BitArray empty;
    EXPECT_EQ(empty.size(), 0);
    EXPECT_TRUE(empty.empty());
}

TEST(BitArrayTest, PartialLastBlock) {
    BitArray ba(12);
    ba.set(11, true);

    EXPECT_EQ(ba[11], true);
    EXPECT_EQ(ba.to_string(), "100000000000");
    EXPECT_EQ(ba.count(), 1);

    ba.set();
    EXPECT_EQ(ba.count(), 12);
    EXPECT_EQ(ba.to_string().substr(0, 12), "111111111111");
}

// Test resize and push_back functionality (if implemented)
TEST(BitArrayTest, Resize) {
    BitArray ba(4);
    ba.set(1, true).set(3, true);

    ba.resize(8);
    EXPECT_EQ(ba.size(), 8);
    EXPECT_EQ(ba[1], true);
    EXPECT_EQ(ba[3], true);
    EXPECT_EQ(ba.to_string(), "00001010");

    ba.resize(2);
    EXPECT_EQ(ba.size(), 2);
    EXPECT_EQ(ba.to_string(), "10");
}

TEST(BitArrayTest, BitwiseOperationsDifferentSizesException) {
    BitArray ba1(8);
    BitArray ba2(4);

    EXPECT_THROW(ba1 &= ba2, std::invalid_argument);
    EXPECT_THROW(ba1 |= ba2, std::invalid_argument);
    EXPECT_THROW(ba1 ^= ba2, std::invalid_argument);
}

// Test large BitArray
TEST(BitArrayTest, LargeBitArray) {
    BitArray ba(100);
    EXPECT_EQ(ba.size(), 100);

    // Set some bits
    ba.set(0, true).set(50, true).set(99, true);
    EXPECT_EQ(ba[0], true);
    EXPECT_EQ(ba[50], true);
    EXPECT_EQ(ba[99], true);

    EXPECT_EQ(ba.count(), 3);

    // Test shifting
    ba <<= 10;
    EXPECT_EQ(ba[10], true);
    EXPECT_EQ(ba[60], true);
    EXPECT_EQ(ba[99], false);
}
