#pragma once
#include "Arena.h"

template <typename T, size_t N>
struct MyAllocator {
public:
	using value_type = T;
	using arena_type = Arena<N>;

	MyAllocator() {
		arena = new arena_type;
	};
	MyAllocator(const MyAllocator&) = default;
	MyAllocator& operator= (const MyAllocator&) = default;

	MyAllocator(arena_type& arena) : arena{ arena } {}

	template <typename U>
	MyAllocator(const MyAllocator<U, N>& other) : arena{ other.arena } {}

	template <typename U> struct rebind {
		using other = MyAllocator<U, N>;
	};

	auto allocate(size_t n) -> T* {
		return reinterpret_cast<T*>(arena->allocate(n * sizeof(T)));
	}

	auto deallocate(T* p, size_t n) noexcept -> void {
		arena->deallocate(reinterpret_cast<std::byte*>(p), n * sizeof(T));
	}

	template <typename U, size_t M>
	auto operator== (const MyAllocator<U, M> other) const noexcept {
		return N == M && arena == other.arena;
	}

	template <typename U, size_t M>
	auto operator!= (const MyAllocator<U, M> other) const noexcept {
		return !(*this == other);
	}

	template <typename U, size_t M>
	friend struct MyAllocator;

private:
	arena_type* arena;
};