#pragma once
#include <array>
#include <memory>
#include <vector>
#include <optional>
#include <algorithm>



constexpr auto PAGE_SIZE = 8;

template<typename T>
class PagedSparseSet
{
public:
	// Initialized with zero pages.
	explicit PagedSparseSet(T defaultValue = T());

	// Updates defaultValue to another value
	void setDefaultValue(T& defaultValue);
	
	// Gets value at index.
	// if there isn't any, a default
	// initialized value is created, stored
	// and returned.
	T& operator[](const size_t& index);
	

	// Returns value at index.
	// If that value haven't been initialized,
	// returns nullptr.
	T* get(const size_t& index);
	
private:
	inline const size_t getPage(const size_t& index);
	inline const size_t getIndexInPage(const size_t& index);
	void initialize_at(const size_t& index);


private:
	T m_defaultValue;
	std::vector<std::unique_ptr<std::array<T,1<<PAGE_SIZE>>> m_map;

};

template<typename T>
inline PagedSparseSet<T>::PagedSparseSet(T defaultValue)
	:
	m_defaultValue(defaultValue)
{}

template<typename T>
inline void PagedSparseSet<T>::setDefaultValue(T& defaultValue)
{
	m_defaultValue = defaultValue;
}

template<typename T>
inline T& PagedSparseSet<T>::operator[](const size_t& index)
{
	const auto pageIndex = getPage(index);
	// Case: If index is not allocated or out of bounds
	if (pageIndex >= m_map.size() || m_map[pageIndex] == nullptr) {
		initialize_at(index);
	}
	// Fallback: index is allocated
	return (*m_map[pageIndex])[getIndexInPage(index)];

}

template<typename T>
inline T* PagedSparseSet<T>::get(const size_t& index)
{
	const auto pageIndex = getPage(index);
	// Case: If index is not allocated or out of bounds
	if (pageIndex >= m_map.size() || m_map[pageIndex] == nullptr) {
		return nullptr;
	}
	// Fallback: index is allocated
	return &(*m_map[pageIndex])[getIndexInPage(index)];
}

template<typename T>
inline const size_t PagedSparseSet<T>::getPage(const size_t& index)
{
	return index >> PAGE_SIZE;
}

template<typename T>
inline const size_t PagedSparseSet<T>::getIndexInPage(const size_t& index)
{
	return index & ((1 << PAGE_SIZE) - 1);
}

template<typename T>
inline void PagedSparseSet<T>::initialize_at(const size_t& index)
{
	// Resizes
	m_map.resize(max(getPage(index) + 1, m_map.size()));
	// Initializes
	using PageArrayType = std::array<T, 1 << PAGE_SIZE>;
	m_map[getPage(index)] = std::make_unique<PageArrayType>();
	m_map[getPage(index)]->fill(m_defaultValue);
}
