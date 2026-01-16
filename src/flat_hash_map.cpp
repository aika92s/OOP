#include "flat_hash_map.h"

#include <stdexcept>
#include <immintrin.h>

static inline int count_trailing_zeros(int mask) {
#if defined(_MSC_VER)
    unsigned long index;
    _BitScanForward(&index, mask);
    return (int)index;
#else
    return __builtin_ctz(mask);
#endif
}

template<class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::iterator::advance_to_next_occupied() {
    auto cap = map_->m_capacity_;
    while (index_ < cap && !map_->is_occupied(index_)) {
        ++index_;
    }
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::iterator & flat_hash_map<Key, T, Hash, KeyEqual>::iterator::operator
++() {
    ++index_;
    advance_to_next_occupied();

    return *this;
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::iterator flat_hash_map<Key, T, Hash, KeyEqual>::iterator::operator
++(int) {
    iterator temp = *this;
    ++(*this);

    return temp;
}

template<class Key, class T, class Hash, class KeyEqual>
bool flat_hash_map<Key, T, Hash, KeyEqual>::iterator::operator==(const const_iterator &other) const {
    return map_ == other.map_ && index_ == other.index_;
}

template<class Key, class T, class Hash, class KeyEqual>
bool flat_hash_map<Key, T, Hash, KeyEqual>::iterator::operator!=(const const_iterator &other) const {
    return !(map_ == other.map_ && index_ == other.index_);
}

template<class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator::advance_to_next_occupied() {
    auto cap = map_->m_capacity_;
    while (index_ < cap && !map_->is_occupied(index_)) {
        ++index_;
    }
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator & flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator::
operator++() {
    ++index_;
    advance_to_next_occupied();

    return *this;
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator::
operator++(int) {
    const_iterator temp = *this;
    ++(*this);

    return temp;
}

template<class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map() : m_capacity_(0), m_size_(0), m_hash_(), m_equal_(),
    m_data_(0), m_metadata_(0) {}

template<class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map(size_type bucket_count, const Hash &hash, const KeyEqual &equal) :
    m_capacity_(next_power_of_two(bucket_count)), m_size_(0), m_hash_(hash), m_equal_(equal),
    m_data_(m_capacity_),  m_metadata_(m_capacity_) {}

template<class Key, class T, class Hash, class KeyEqual>
template<class InputIt>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map(InputIt first, InputIt last, size_type bucket_count,
    const Hash &hash, const KeyEqual &equal) : flat_hash_map(bucket_count, hash, equal)
{
    this->insert(first, last);
}
template<class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map(std::initializer_list<value_type> init, size_type bucket_count,
    const Hash &hash, const KeyEqual &equal)
    : flat_hash_map(init.begin(), init.end(), bucket_count, hash, equal) {
}

template<class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map(const flat_hash_map &other) : m_capacity_(other.m_capacity_),
    m_size_(other.m_size_), m_hash_(other.m_hash_), m_equal_(other.m_equal_),
    m_data_(other.m_data_), m_metadata_(other.m_metadata_) {}

template<class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual>::flat_hash_map(flat_hash_map &&other) noexcept : m_capacity_(other.m_capacity_),
    m_size_(other.m_size_), m_equal_(std::move(other.m_equal_)),
    m_data_(std::move(other.m_data_)), m_metadata_(std::move(other.m_metadata_)) {

    other.m_capacity_ = 0;
    other.m_size_ = 0;

}

template<class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual> & flat_hash_map<Key, T, Hash, KeyEqual>::operator=(const flat_hash_map &other) {

    if (this == &other) return *this;
    flat_hash_map temp(other);
    swap(temp);

    return *this;
}

template<class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual> & flat_hash_map<Key, T, Hash, KeyEqual>::operator
=(flat_hash_map &&other) noexcept {

    if (this == &other) return *this;

    m_capacity_ = other.m_capacity_;
    other.m_capacity_ = 0;

    m_size_ = other.m_size_;
    other.m_size_ = 0;

    m_hash_ = std::move(other.m_hash_);
    m_equal_ = std::move(other.m_equal_);
    m_data_ = std::move(other.m_data_);
    m_metadata_ = std::move(other.m_metadata_);

    return *this;
}

template<class Key, class T, class Hash, class KeyEqual>
flat_hash_map<Key, T, Hash, KeyEqual> & flat_hash_map<Key, T, Hash, KeyEqual>::operator=(
    std::initializer_list<value_type> init) {

    this->clear();

    for (const auto& item : init) this->insert(item);

    return *this;
}

template<class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::swap(flat_hash_map &other) noexcept {
    std::swap(m_capacity_, other.m_capacity_);
    std::swap(m_size_, other.m_size_);
    std::swap(m_hash_, other.m_hash_);
    std::swap(m_equal_, other.m_equal_);
    std::swap(m_data_, other.m_data_);
    std::swap(m_metadata_, other.m_metadata_);
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::iterator flat_hash_map<Key, T, Hash, KeyEqual>::begin() noexcept {
    return iterator(this, 0);
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator flat_hash_map<Key, T, Hash, KeyEqual>::
begin() const noexcept {
    return const_iterator(this, 0);
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator flat_hash_map<Key, T, Hash, KeyEqual>::
cbegin() const noexcept {
    return begin();
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::iterator flat_hash_map<Key, T, Hash, KeyEqual>::end() noexcept {
    return iterator(this, m_capacity_);
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator flat_hash_map<Key, T, Hash, KeyEqual>::
end() const noexcept {
    return const_iterator(this, m_capacity_);
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator flat_hash_map<Key, T, Hash, KeyEqual>::
cend() const noexcept {
    return end();
}

template<class Key, class T, class Hash, class KeyEqual>
bool flat_hash_map<Key, T, Hash, KeyEqual>::empty() const noexcept {
    if (m_size_ == 0) return true;

    return false;
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::size_type flat_hash_map<Key, T, Hash, KeyEqual>::size() const noexcept {
    return m_size_;
}

template<class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::clear() noexcept {

    m_size_ = 0;
    m_capacity_ = 0;
    m_data_.clear();
    m_metadata_.clear();
}

template<class Key, class T, class Hash, class KeyEqual>
std::pair<typename flat_hash_map<Key, T, Hash, KeyEqual>::iterator, bool> flat_hash_map<Key, T, Hash, KeyEqual>::insert(
    const value_type &value) {

    if (m_capacity_ == 0) {
        reserve(1);
    }
    if (static_cast<float>(m_size_ + 1) > max_load_factor_ * static_cast<float>(m_capacity_)) {
        reserve(next_power_of_two(m_capacity_ * 2));
    }

    const size_t hash_value = m_hash_(value.first);
    size_type index = hash_value & (m_capacity_ - 1);
    size_type first_deleted = m_capacity_;

    const __m128i vec_empty = _mm_set1_epi8(EMPTY);
    const __m128i vec_deleted = _mm_set1_epi8(DELETED);
    const __m128i vec_full = _mm_set1_epi8(FULL);

    while (true) {
        if (index + 16 > m_capacity_) {
            while (index < m_capacity_) {
                if (m_metadata_[index] == EMPTY) {

                    size_type insert_index = (first_deleted != m_capacity_) ? first_deleted : index;
                    new (&m_data_[insert_index]) value_type(value);
                    m_metadata_[insert_index] = FULL;
                    m_size_++;

                    return { iterator(this, insert_index), true };
                }
                if (m_metadata_[index] == DELETED) {
                    if (first_deleted == m_capacity_) first_deleted = index;
                }
                if (m_metadata_[index] == FULL && m_equal_(m_data_[index].first, value.first))
                    return { iterator(this, index), false };
                ++index;
            }
            index = 0;
        }
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&m_metadata_[index]));

        int mask_full = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vec_full));
        while (mask_full != 0) {
            int bit = count_trailing_zeros(mask_full);
            if (m_equal_(m_data_[index + bit].first, value.first)) {
                return { iterator(this, index + bit), false };
            }
            mask_full &= ~(1 << bit); 
        }

        int mask_deleted = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vec_deleted));
        if (mask_deleted != 0 && first_deleted == m_capacity_) {
            first_deleted = index + count_trailing_zeros(mask_deleted);
        }

        int mask_empty = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vec_empty));
        if (mask_empty != 0) {
            int bit = count_trailing_zeros(mask_empty);
            size_type found_empty_index = index + bit;

            size_type insert_index = (first_deleted != m_capacity_) ? first_deleted : found_empty_index;
            new (&m_data_[insert_index]) value_type(value);
            m_metadata_[insert_index] = FULL;
            m_size_++;
            return { iterator(this, insert_index), true };
        }
        
        index += 16;
        if (index >= m_capacity_) index = 0;
    }
}

template<class Key, class T, class Hash, class KeyEqual>
std::pair<typename flat_hash_map<Key, T, Hash, KeyEqual>::iterator, bool> flat_hash_map<Key, T, Hash, KeyEqual>::
insert(value_type &&value) {
    if (m_capacity_ == 0) {
        reserve(1);
    }
    if (static_cast<float>(m_size_ + 1) > max_load_factor_ * static_cast<float>(m_capacity_)) {
        reserve(next_power_of_two(m_capacity_ * 2));
    }

    const size_t hash_value = m_hash_(value.first);
    size_type index = hash_value & (m_capacity_ - 1);
    size_type first_deleted = m_capacity_;

    const __m128i vec_empty = _mm_set1_epi8(EMPTY);
    const __m128i vec_deleted = _mm_set1_epi8(DELETED);
    const __m128i vec_full = _mm_set1_epi8(FULL);

    while (true) {
        if (index + 16 > m_capacity_) {
            while (index < m_capacity_) {
                if (m_metadata_[index] == EMPTY) {
                    size_type insert_index = (first_deleted != m_capacity_) ? first_deleted : index;
                    new (&m_data_[insert_index]) value_type(std::move(value));
                    m_metadata_[insert_index] = FULL;
                    m_size_++;
                    return { iterator(this, insert_index), true };
                }
                if (m_metadata_[index] == DELETED) {
                    if (first_deleted == m_capacity_) first_deleted = index;
                }
                if (m_metadata_[index] == FULL) {
                    if (m_equal_(m_data_[index].first, value.first)) return { iterator(this, index), false };
                }
                index++;
            }
            index = 0;
            continue;
        }
        
        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&m_metadata_[index]));

        int mask_full = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vec_full));
        while (mask_full != 0) {
            int bit = count_trailing_zeros(mask_full);
            if (m_equal_(m_data_[index + bit].first, value.first)) {
                return { iterator(this, index + bit), false };
            }
            mask_full &= ~(1 << bit);
        }

        int mask_deleted = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vec_deleted));
        if (mask_deleted != 0 && first_deleted == m_capacity_) {
            first_deleted = index + count_trailing_zeros(mask_deleted);
        }

        int mask_empty = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vec_empty));
        if (mask_empty != 0) {
            int bit = count_trailing_zeros(mask_empty);
            size_type found_empty_index = index + bit;

            size_type insert_index = (first_deleted != m_capacity_) ? first_deleted : found_empty_index;
            new (&m_data_[insert_index]) value_type(std::move(value));
            m_metadata_[insert_index] = FULL;
            m_size_++;
            return { iterator(this, insert_index), true };
        }

        index += 16;
        if (index >= m_capacity_) index = 0;
    }
}

template<class Key, class T, class Hash, class KeyEqual>
template<class InputIt>
void flat_hash_map<Key, T, Hash, KeyEqual>::insert(InputIt first, InputIt last) {
    auto n = std::distance(first, last);
    if (n > 0) {
        reserve(m_size_ + n);
    }

    for (; first != last; ++first) {
        this->insert(*first);
    }
}

template<class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::insert(std::initializer_list<value_type> init) {
    insert(init.begin(), init.end());
}

template<class Key, class T, class Hash, class KeyEqual>
template<class ... Args>
std::pair<typename flat_hash_map<Key, T, Hash, KeyEqual>::iterator, bool> flat_hash_map<Key, T, Hash, KeyEqual>::
emplace(Args &&...args) {
    return insert(value_type(std::forward<Args>(args)...));
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::size_type flat_hash_map<Key, T, Hash, KeyEqual>::erase(
    const key_type &key) {

    auto pos = this->find(key);
    if (pos == this->end()) return 0;

    m_metadata_[pos.index_] = DELETED;
    m_size_--;

    return 1;
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::iterator flat_hash_map<Key, T, Hash, KeyEqual>::erase(iterator pos) {

    size_type index = pos.index_;

    m_metadata_[index] = DELETED;
    m_size_--;
    return iterator(this, index + 1);
}

template<class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::reserve(size_type new_capacity) {

    if (new_capacity <= m_capacity_) {
        return;
    }

    size_type old_capacity = m_capacity_;
    std::vector<value_type> old_data = std::move(m_data_);
    std::vector<uint8_t> old_metadata = std::move(m_metadata_);

    m_capacity_ = next_power_of_two(new_capacity);
    m_size_ = 0;
    m_data_.resize(m_capacity_);
    m_metadata_.assign(m_capacity_, EMPTY);

    for (size_type i = 0; i < old_capacity; i++) {
        if (old_metadata[i] == FULL) {
            this->insert(std::move(old_data[i]));
        }
    }
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::size_type flat_hash_map<Key, T, Hash, KeyEqual>::
capacity() const noexcept {
    return m_capacity_;
}

template<class Key, class T, class Hash, class KeyEqual>
float flat_hash_map<Key, T, Hash, KeyEqual>::load_factor() const noexcept {
    return static_cast<float>(m_size_) / m_capacity_;
}

template<class Key, class T, class Hash, class KeyEqual>
void flat_hash_map<Key, T, Hash, KeyEqual>::max_load_factor(float ml) {
    max_load_factor_ = ml;
}

template<class Key, class T, class Hash, class KeyEqual>
float flat_hash_map<Key, T, Hash, KeyEqual>::max_load_factor() const noexcept {
    return max_load_factor_;
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::mapped_type & flat_hash_map<Key, T, Hash, KeyEqual>::operator[](
    const key_type &key) {
    auto pos = this->find(key);

    if (pos != this->end()) {
        return pos->second;
    }
    auto insert_result = this->insert({std::move(key), mapped_type{}});
    return insert_result.first->second;
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::mapped_type & flat_hash_map<Key, T, Hash, KeyEqual>::operator[
](key_type &&key) {
    auto pos = this->find(key);

    if (pos != this->end()) {
        return pos->second;
    }

    auto insert_result = this->insert({std::move(key), mapped_type{}});
    return insert_result.first->second;
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::mapped_type & flat_hash_map<Key, T, Hash, KeyEqual>::at(
    const key_type &key) {

    auto it = this->find(key);

    if (it == this->end()) throw std::out_of_range("flat_hash_map::at: key not found");

    return it->second;
}

template<class Key, class T, class Hash, class KeyEqual>
const typename flat_hash_map<Key, T, Hash, KeyEqual>::mapped_type & flat_hash_map<Key, T, Hash, KeyEqual>::at(
    const key_type &key) const {

    auto it = this->find(key);
    if (it == this->end()) throw std::out_of_range("flat_hash_map::at: key not found");

    return it->second;
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::iterator flat_hash_map<Key, T, Hash, KeyEqual>::find(
    const key_type &key) {

    if (m_capacity_ == 0) return this->end();

    const size_t hash_value = m_hash_(key);
    size_type index = hash_value & (m_capacity_ - 1);

    const __m128i vec_empty = _mm_set1_epi8(EMPTY);
    const __m128i vec_full = _mm_set1_epi8(FULL);

    while (true) {
        if (index + 16 > m_capacity_) {
            while (index < m_capacity_) {
                if (m_metadata_[index] == EMPTY) return this->end();
                if (m_metadata_[index] == FULL && m_equal_(m_data_[index].first, key)) {
                    return iterator(this, index);
                }
                index++;
            }
            index = 0;
            continue;
        }

        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&m_metadata_[index]));

        int mask_full = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vec_full));
        while (mask_full != 0) {
            int bit = count_trailing_zeros(mask_full);
            if (m_equal_(m_data_[index + bit].first, key)) {
                return iterator(this, index + bit);
            }
            mask_full &= ~(1 << bit);
        }

        int mask_empty = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vec_empty));
        if (mask_empty != 0) {
            return this->end();
        }

        index += 16;
        if (index >= m_capacity_) index = 0;
    }
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::const_iterator flat_hash_map<Key, T, Hash, KeyEqual>::find(
    const key_type &key) const {

    if (m_capacity_ == 0) return this->end();

    const size_t hash_value = m_hash_(key);
    size_type index = hash_value & (m_capacity_ - 1);

    const __m128i vec_empty = _mm_set1_epi8(EMPTY);
    const __m128i vec_full = _mm_set1_epi8(FULL);

    while (true) {
        if (index + 16 > m_capacity_) {
            while (index < m_capacity_) {
                if (m_metadata_[index] == EMPTY) return this->end();
                if (m_metadata_[index] == FULL && m_equal_(m_data_[index].first, key)) {
                    return const_iterator(this, index);
                }
                index++;
            }
            index = 0;
            continue;
        }

        __m128i chunk = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&m_metadata_[index]));

        int mask_full = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vec_full));
        while (mask_full != 0) {
            int bit = count_trailing_zeros(mask_full);
            if (m_equal_(m_data_[index + bit].first, key)) {
                return const_iterator(this, index + bit);
            }
            mask_full &= ~(1 << bit);
        }

        int mask_empty = _mm_movemask_epi8(_mm_cmpeq_epi8(chunk, vec_empty));
        if (mask_empty != 0) {
            return this->end();
        }

        index += 16;
        if (index >= m_capacity_) index = 0;
    }
}

template<class Key, class T, class Hash, class KeyEqual>
bool flat_hash_map<Key, T, Hash, KeyEqual>::contains(const key_type &key) const {

    auto it = this->find(key);
    if (it == this->end()) return false;

    return true;
}

template<class Key, class T, class Hash, class KeyEqual>
typename flat_hash_map<Key, T, Hash, KeyEqual>::size_type flat_hash_map<Key, T, Hash, KeyEqual>::count(
    const key_type &key) const {
    if (contains(key)) return 1;

    return 0;
}

template<class Key, class T, class Hash, class KeyEqual>
size_t flat_hash_map<Key, T, Hash, KeyEqual>::next_power_of_two(size_t bucket_size) {
        if (bucket_size <= 1) return 1;

        size_t result = 1;
        while (result <= bucket_size) {
            result *= 2;
        }

        return result;
}

template<class Key, class T, class Hash, class KeyEqual>
bool flat_hash_map<Key, T, Hash, KeyEqual>::is_occupied(size_type i) const {
        return m_metadata_[i] != EMPTY && m_metadata_[i] != DELETED;
}

// 1. flat_hash_map<int, int>
template class flat_hash_map<int, int>;

// 2. flat_hash_map<int, std::string>
template class flat_hash_map<int, std::string>;

// 3. flat_hash_map<std::string, int>
template class flat_hash_map<std::string, int>;

// 4. flat_hash_map<std::string, std::string>
template class flat_hash_map<std::string, std::string>;

template flat_hash_map<int, int>::flat_hash_map(
    typename std::vector<std::pair<const int, int>>::iterator,
    typename std::vector<std::pair<const int, int>>::iterator,
    typename flat_hash_map<int, int>::size_type,
    const typename flat_hash_map<int, int>::hasher&,
    const typename flat_hash_map<int, int>::key_equal&
);

template std::pair<flat_hash_map<std::string, std::string>::iterator, bool>
flat_hash_map<std::string, std::string>::emplace<char const (&)[6], char const (&)[6]>(
    char const (&)[6], char const (&)[6]
);
