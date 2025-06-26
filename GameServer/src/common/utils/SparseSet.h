#pragma once

/*========================
        SparseSet
========================*/

#include <vector>
#include <cstdint>
#include <limits>
#include <utility> 

#undef max

template<typename Key, typename Value>
class SparseSet
{
public:
    using key_type = Key;
    using value_type = Value;
    using size_type = typename std::vector<value_type>::size_type;
    using iterator = typename std::vector<value_type>::iterator;
    using const_iterator = typename std::vector<value_type>::const_iterator;

    SparseSet() = default;
    ~SparseSet() = default;

public:
    void add(key_type key, const value_type& value);
    void add(key_type key, value_type&& value);
    bool remove(key_type key);
    bool contains(key_type key) const noexcept;
    bool empty() const noexcept { return _denseValues.empty(); }
    size_type size() const noexcept { return _denseValues.size(); }
    void clear() noexcept;

public:
    iterator begin() noexcept { return _denseValues.begin(); }
    iterator end() noexcept { return _denseValues.end(); }
    const_iterator begin() const noexcept { return _denseValues.cbegin(); }
    const_iterator end() const noexcept { return _denseValues.cend(); }
    const_iterator cbegin() const noexcept { return _denseValues.cbegin(); }
    const_iterator cend() const noexcept { return _denseValues.cend(); }

private:
    std::vector<value_type> _denseValues;
    std::vector<key_type>   _denseKeys;
    std::vector<size_type>  _sparse;

    static constexpr size_type _invalidIndex{ std::numeric_limits<size_type>::max() };
};

template<typename K, typename V>
void SparseSet<K, V>::add(key_type key, const value_type& value)
{
    if (contains(key))
    {
        return;
    }

    if (key >= _sparse.size())
    {
        _sparse.resize(key + 1, _invalidIndex);
    }

    _sparse[key] = _denseValues.size();
    _denseValues.push_back(value);
    _denseKeys.push_back(key);
}

template<typename K, typename V>
void SparseSet<K, V>::add(key_type key, value_type&& value)
{
    if (contains(key))
    {
        return;
    }

    if (key >= _sparse.size())
    {
        _sparse.resize(key + 1, _invalidIndex);
    }

    _sparse[key] = _denseValues.size();
    _denseValues.push_back(std::move(value));
    _denseKeys.push_back(key);
}

template<typename K, typename V>
bool SparseSet<K, V>::remove(key_type key)
{
    if (contains(key) == false)
    {
        return false;
    }

    const size_type denseIndexToRemove{ _sparse[key] };

    _denseValues[denseIndexToRemove] = std::move(_denseValues.back());
    _denseKeys[denseIndexToRemove] = _denseKeys.back();

    const key_type movedElementKey{ _denseKeys[denseIndexToRemove] };
    _sparse[movedElementKey] = denseIndexToRemove;

    _denseValues.pop_back();
    _denseKeys.pop_back();

    _sparse[key] = _invalidIndex;

    return true;
}

template<typename K, typename V>
bool SparseSet<K, V>::contains(key_type key) const noexcept
{
    return key < _sparse.size() && _sparse[key] != _invalidIndex;
}

template<typename K, typename V>
void SparseSet<K, V>::clear() noexcept
{
    _denseValues.clear();
    _denseKeys.clear();
    _sparse.clear();
}