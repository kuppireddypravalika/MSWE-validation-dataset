#pragma once
#include <algorithm>
#include <vector>

// Sequence container exposing checked and unchecked iterators

template <typename T>
class Sequence {
public:
    explicit Sequence(std::size_t n = 0) : data(n) {}

    using value_type = T;
    std::size_t size() const { return data.size(); }

    T& at(std::size_t i) { return data.at(i); }
    const T& at(std::size_t i) const { return data.at(i); }

    T& operator[](std::size_t i) { return data[i]; }
    const T& operator[](std::size_t i) const { return data[i]; }

    template <bool Checked>
    class Iter {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iter(Sequence* s, std::size_t idx) : seq(s), i(idx) {}

        reference operator*() const { return Checked ? seq->at(i) : (*seq)[i]; }
        pointer operator->() const { return &**this; }
        Iter& operator++() { ++i; return *this; }
        Iter& operator--() { --i; return *this; }
        Iter& operator+=(difference_type n) { i += n; return *this; }
        Iter& operator-=(difference_type n) { i -= n; return *this; }
        Iter operator+(difference_type n) const { return Iter(seq, i + n); }
        Iter operator-(difference_type n) const { return Iter(seq, i - n); }
        difference_type operator-(Iter other) const { return static_cast<difference_type>(i) - other.i; }
        reference operator[](difference_type n) const { return *(*this + n); }
        bool operator==(Iter other) const { return i == other.i; }
        bool operator!=(Iter other) const { return i != other.i; }
        bool operator<(Iter other) const { return i < other.i; }
        bool operator>(Iter other) const { return i > other.i; }
        bool operator<=(Iter other) const { return i <= other.i; }
        bool operator>=(Iter other) const { return i >= other.i; }

    private:
        Sequence* seq;
        std::size_t i;
    };

    using iterator = Iter<true>;
    using unchecked_iterator = Iter<false>;

    iterator begin() { return iterator(this, 0); }
    iterator end() { return iterator(this, size()); }
    unchecked_iterator unchecked_begin() { return unchecked_iterator(this, 0); }
    unchecked_iterator unchecked_end() { return unchecked_iterator(this, size()); }

    std::vector<T> data;
};

// Wrapper returned by unchecked()

template <typename Seq>
class UncheckedWrapper {
public:
    explicit UncheckedWrapper(Seq& s) : seq(s) {}
    auto begin() { return seq.unchecked_begin(); }
    auto end() { return seq.unchecked_end(); }
private:
    Seq& seq;
};

template <typename Seq>
UncheckedWrapper<Seq> unchecked(Seq& s) { return UncheckedWrapper<Seq>(s); }

// trivial pdqsort stand-in using std::sort

template <typename Seq, typename Cmp = std::less<>>
void pdqsort(Seq& seq, Cmp cmp = {}) {
    std::sort(seq.begin(), seq.end(), cmp);
}
