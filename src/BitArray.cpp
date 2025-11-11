#include "BitArray.h"
#include <algorithm>

BitArray::BitArray() {
	num_bits_ = 0;
}

BitArray::BitArray(size_t num_bits, BlockType value) {
    num_bits_ = num_bits;

    size_t num_of_blocks = (num_bits_ + (BlockSize - 1)) / BlockSize;
    blocks_ = std::vector<BlockType>(num_of_blocks);

    if (num_of_blocks > 0) {
        blocks_[0] = value;
		clean_last_block();
    }
};

BitArray::BitArray(const BitArray& b) {
    num_bits_ = b.num_bits_;
    blocks_ = b.blocks_;
};

void BitArray::swap(BitArray& b) {
    size_t num_bits_temporary = num_bits_;
    num_bits_ = b.num_bits_;
    b.num_bits_ = num_bits_temporary;

    std::vector<BlockType> blocks_temporary = blocks_;
    blocks_ = b.blocks_;
    b.blocks_ = blocks_temporary;
};

void BitArray::resize(size_t num_bits, bool value) {
    if (num_bits == num_bits_) return;

	const size_t old_num_bits = num_bits_;
	num_bits_ = num_bits;

    size_t num_of_new_blocks = (num_bits + (BlockSize - 1)) / BlockSize;

    if (num_bits < old_num_bits) {
        blocks_.resize(num_of_new_blocks);
    } else {
		blocks_.resize(num_of_new_blocks, value ? ~BlockType(0) : BlockType(0));

        if (value && old_num_bits > 0) {
            size_t old_bits_in_end = old_num_bits % BlockSize;

            if (old_bits_in_end != 0) {
                size_t last_block_idx = (old_num_bits - 1) / BlockSize;
                BlockType mask = ((BlockType(1) << (BlockSize - old_bits_in_end)) - BlockType(1)) << old_bits_in_end;
                blocks_[last_block_idx] |= mask;
            }
        }
	}
	clean_last_block();
};

void BitArray::clear() {
    num_bits_ = 0;
    blocks_.clear();
};

void BitArray::push_back(bool bit) {
    size_t last_block_index = 0;
    if (num_bits_ > 0) {
        last_block_index = ((num_bits_ + (BlockSize - 1)) / BlockSize) - 1;
    }

    if (num_bits_ % BlockSize == 0) {
        blocks_.push_back(BlockType(0));
        last_block_index++;
    }

    num_bits_++;
    if (!bit) return;
    blocks_[last_block_index] |= BlockType(1) << ((num_bits_ - 1) % BlockSize);
};

BitArray& BitArray::operator=(const BitArray& b) {
    if (this == &b) return *this;

    blocks_ = b.blocks_;
    num_bits_ = b.num_bits_;
    return *this;
};

BitArray& BitArray::operator&=(const BitArray& b) {
    if (num_bits_ != b.num_bits_) {
        throw std::invalid_argument("BitArray::operator&=: sizes do not match");
    }

    for (size_t i = 0; i < blocks_.size(); i++) {
        this->blocks_[i] &= b.blocks_[i];
    }
    return *this;
};

BitArray& BitArray::operator|=(const BitArray& b) {
    if (num_bits_ != b.num_bits_) {
        throw std::invalid_argument("BitArray::operator|=: sizes do not match");
    }

    for (size_t i = 0; i < blocks_.size(); i++) {
        this->blocks_[i] |= b.blocks_[i];
    }
    return *this;
};

BitArray& BitArray::operator^=(const BitArray& b) {
    if (num_bits_ != b.num_bits_) {
        throw std::invalid_argument("BitArray::operator^=: sizes do not match");
    }

    for (size_t i = 0; i < blocks_.size(); i++) {
        this->blocks_[i] ^= b.blocks_[i];
    }
    return *this;
};

BitArray& BitArray::set(size_t n, bool val) {
    size_t block_index = n / BlockSize;

    if (val) {
        blocks_[block_index] |= (BlockType(1) << (n % BlockSize));
        return *this;
    }
    blocks_[block_index] &= ~(BlockType(1)  << (n % BlockSize));
    return *this;
};

BitArray& BitArray::reset(size_t n) {
    size_t block_index = n / BlockSize;

    blocks_[block_index] &= ~(BlockType(1)  << (n % BlockSize));
    return *this;
};

BitArray& BitArray::set() {
    if (num_bits_ == 0) return *this;

    for (size_t i = 0; i < blocks_.size(); i++) {
        blocks_[i] = ~BlockType(0);
    }
    clean_last_block();
    return *this;
};


BitArray& BitArray::reset() {
    if (num_bits_ == 0) return *this;

    for (size_t i = 0; i < blocks_.size(); i++) {
        blocks_[i] = BlockType(0);
    }
    return *this;
};

bool BitArray::any() const {
    if (num_bits_ == 0) return false;

    for (size_t i = 0; i < blocks_.size(); i++) {
        if (blocks_[i] != BlockType(0)) return true;
    }
    return false;
};

bool BitArray::none() const {
    if (num_bits_ == 0) return true;

    for (size_t i = 0; i < blocks_.size(); i++) {
        if (blocks_[i] != BlockType(0)) return false;
    }
    return true;
};

BitArray BitArray::operator~() const {
    if (num_bits_ == 0) return BitArray(0);

    BitArray new_array(*this);

    for (size_t i = 0; i < new_array.blocks_.size(); i++) {
        new_array.blocks_[i] = ~(new_array.blocks_[i]);
    }

    new_array.clean_last_block();
    return new_array;
};

size_t BitArray::count() const {
	if (num_bits_ == 0) return 0;
	size_t total = 0;

	for (size_t i = 0; i < blocks_.size() - 1; i++) {
       for (size_t n = 0; n < BlockSize; n++) {
          if (blocks_[i] & (BlockType(1) << n)) total += 1;
       }
    }

	size_t bits_in_last_block = num_bits_ % BlockSize;
    if (bits_in_last_block == 0 && !blocks_.empty()) {
        bits_in_last_block = BlockSize;
    }

    if (!blocks_.empty()) {
        for (size_t n = 0; n < bits_in_last_block; n++) {
           if (blocks_.back() & (BlockType(1) << n)) total += 1;
        }
    }
	return total;
};

size_t BitArray::size() const {
    return num_bits_;
};

bool BitArray::empty() const {
    return num_bits_ == 0;
};

std::string BitArray::to_string() const {
	std::string string_bit_array;

	for (size_t i = 0; i < num_bits_; i++) {
		bool bit = blocks_[(i / BlockSize)] & (BlockType(1) << (i % BlockSize));
		if (bit) {
			string_bit_array += '1';
		} else {
			string_bit_array += '0';
		}
	}
	return string_bit_array;
};

BitArray& BitArray::operator<<=(size_t n) {
	if (n <= 0) return *this;

	if (n >= num_bits_) {
		for (size_t i = 0; i < blocks_.size(); i++) {
			blocks_[i] = BlockType(0);
		}
		return *this;
	}

	size_t shift_bits = n % BlockSize;
	size_t shift_blocks = (n / BlockSize);

	if (shift_blocks != 0) {
		for (size_t i = blocks_.size() - 1; i >= shift_blocks; i--) {
			blocks_[i] = blocks_[i  - shift_blocks];
		}
		for (size_t i = 0; i < shift_blocks; i++) {
			blocks_[i] = BlockType(0);
		}
	}

	if (shift_bits != 0) {
		for (size_t i = blocks_.size()- 1; i > 0; i--) {
            blocks_[i] = (blocks_[i - 1] >> (BlockSize - shift_bits)) |
                         (blocks_[i] << shift_bits);
        }
        blocks_[0] <<= shift_bits;
	}
	clean_last_block();
	return *this;
};

BitArray& BitArray::operator>>=(size_t n) {
	if (n <= 0) return *this;

	if (n >= num_bits_) {
		for (size_t i = 0; i < blocks_.size(); i++) {
			blocks_[i] = BlockType(0);
		}
		return *this;
	}

	size_t shift_bits = n % BlockSize;
	size_t shift_blocks = (n / BlockSize);

	if (shift_blocks != 0) {
		for (size_t i = 0; i < blocks_.size() - shift_blocks; i--) {
			blocks_[i] = blocks_[i + shift_blocks];
		}
		for (size_t i = blocks_.size() - shift_blocks; i < blocks_.size(); i++) {
			blocks_[i] = BlockType(0);
		}
	}

	if (shift_bits != 0) {
		for (size_t i = 0; i < blocks_.size() - 1; i++) {
            blocks_[i] = (blocks_[i + 1] << (BlockSize - shift_bits)) |
                         (blocks_[i] >> shift_bits);
        }
        blocks_[blocks_.size() - 1] >>= shift_bits;
	}
	return *this;
};

BitArray BitArray::operator<<(size_t n) const {
	BitArray shifted_array(*this);
	shifted_array <<= n;
	return shifted_array;
};
BitArray BitArray::operator>>(size_t n) const {
	BitArray shifted_array(*this);
	shifted_array >>= n;
	return shifted_array;
};

bool BitArray::operator==(const BitArray &b) const {
	if (num_bits_ != b.num_bits_) return false;

	for (size_t i = 0; i < blocks_.size(); i++) {
		if (blocks_[i] != b.blocks_[i]) return false;
	}
	return true;
};

bool BitArray::operator!=(const BitArray &b) const  {
	if (num_bits_ != b.num_bits_) return true;

	for (size_t i = 0; i < blocks_.size(); i++) {
		if (blocks_[i] != b.blocks_[i]) return true;
	}
	return false;
};

/* If some BitArray is larger, the member function expands (relative to the smallest) result array,
putting zeros in place of the expansion. */
BitArray BitArray::operator&(const BitArray &b) const {
	size_t result_num_bits = std::max(num_bits_, b.num_bits_);
	BitArray result(result_num_bits);

	size_t common_num_bits = std::min(num_bits_, b.num_bits_);
	size_t common_num_of_blocks = (common_num_bits + (BlockSize - 1)) / BlockSize;

	for (size_t i = 0; i < common_num_of_blocks; i++) {
		result.blocks_[i] = blocks_[i] & b.blocks_[i];
	}
	return result;
};

/* If some array is larger, the member function expands (relative to the smallest) the result array and puts the bits
from the larger array in place of the expansion. */
BitArray BitArray::operator|(const BitArray &b) const  {
	size_t result_num_bits = std::max(num_bits_, b.num_bits_);
	BitArray result(result_num_bits);

	size_t common_num_bits = std::min(num_bits_, b.num_bits_);
	size_t common_num_of_blocks = (common_num_bits + (BlockSize - 1)) / BlockSize;

	for (size_t i = 0; i < common_num_of_blocks; i++) {
		result.blocks_[i] = blocks_[i] | b.blocks_[i];
	}

	const BitArray& bigger_array = (blocks_.size() >= b.blocks_.size()) ? *this : b;

	if (blocks_.size() != b.blocks_.size()) {
        std::copy(
            bigger_array.blocks_.begin() + common_num_of_blocks,
            bigger_array.blocks_.end(),
            result.blocks_.begin() + common_num_of_blocks
        );
    }
	return result;
};

/* If some array is larger, the member function expands (relative to the smallest) the result array and puts the bits
from the larger array in place of the expansion. */
BitArray BitArray::operator^(const BitArray &b) const  {
	size_t result_num_bits = std::max(num_bits_, b.num_bits_);
	BitArray result(result_num_bits);

	size_t common_num_bits = std::min(num_bits_, b.num_bits_);
	size_t common_num_of_blocks = (common_num_bits + (BlockSize - 1)) / BlockSize;
	size_t common_residual_bits = common_num_bits % BlockSize;

	for (size_t i = 0; i < common_num_of_blocks; i++) {
		result.blocks_[i] = blocks_[i] ^ b.blocks_[i];
	}

	const BitArray& bigger_array = (num_bits_ >= b.num_bits_) ? *this : b;

	if (num_bits_ != b.num_bits_) {

		if (common_residual_bits > 0) {
			size_t last_block_index = common_num_of_blocks - 1;

			BlockType residual_mask = ~((BlockType(1) << common_residual_bits) - 1);

			BlockType bigger_residual_bits = bigger_array.blocks_[last_block_index] & residual_mask;

			result.blocks_[last_block_index] = (result.blocks_[last_block_index] & ~residual_mask) | bigger_residual_bits;
		}

		std::copy(
			bigger_array.blocks_.begin() + common_num_of_blocks,
			bigger_array.blocks_.end(),
			result.blocks_.begin() + common_num_of_blocks
		);
	}
	return result;
}
bool BitArray::operator[](size_t i) const {
	size_t block_index = i / BlockSize;
    size_t bit_position = i % BlockSize;
	return (blocks_[block_index] & (BlockType(1) << bit_position)) != BlockType(0);
};

