#ifndef OOP_BITARRAY_H
#define OOP_BITARRAY_H
#include <vector>
#include <string>
#include <climits>
#include <stdexcept>

using BlockType = unsigned long long;

class BitArray {
    private:
        size_t num_bits_;
        std::vector<BlockType> blocks_;
		static const size_t BlockSize = sizeof(BlockType) * 8;

		void clean_last_block() {
        	if (blocks_.empty()) return;

        	size_t bits_in_last_block = num_bits_ % BlockSize;
        	if (bits_in_last_block != 0) {
            	BlockType mask = (BlockType(1) << bits_in_last_block) - BlockType(1);
            	blocks_.back() &= mask;
        	}
    	}

    public:
        BitArray();

        explicit BitArray(size_t num_bits, BlockType value = 0);
        BitArray(const BitArray& b);

        void swap(BitArray& b);

        void resize(size_t num_bits, bool value = false);
        void clear();
        void push_back(bool bit);
        BitArray& operator=(const BitArray& b);

        BitArray& operator&=(const BitArray& b);
        BitArray& operator|=(const BitArray& b);
        BitArray& operator^=(const BitArray& b);

        BitArray& operator<<=(size_t n);
        BitArray& operator>>=(size_t n);
        BitArray operator<<(size_t n) const;
        BitArray operator>>(size_t n) const;

        BitArray& set(size_t n, bool val = true);
        BitArray& reset(size_t n);
        BitArray& set();
        BitArray& reset();

        bool any() const;
        bool none() const;
        BitArray operator~() const;
        size_t count() const;

        bool operator[](size_t i) const;

        size_t size() const;
        bool empty() const;

        std::string to_string() const;

        bool operator==(const BitArray & other) const;
        bool operator!=(const BitArray & other) const;

        BitArray operator&(const BitArray & other) const;
        BitArray operator|(const BitArray & other) const;
        BitArray operator^(const BitArray & other) const;
};

#endif //OOP_BITARRAY_H