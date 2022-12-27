#pragma once
#include <cstddef>
#include <iostream>

template <size_t N>
class Arena {
	static constexpr int alignment = alignof(std::max_align_t);
public:
	Arena() noexcept : ptr{ buffer } {}
	Arena(const Arena&) = delete;
	Arena& operator=(const Arena&) = delete;
	auto reset() noexcept { ptr = buffer; }
	static constexpr auto size() noexcept {
		return N;
	}
	auto used() const noexcept {
		return static_cast<size_t>(ptr - buffer);
	}
	auto allocate(size_t n) -> std::byte* {
		static int counter = 0;
		const auto aligned_n = align_up(n);
		const auto available_bytes = static_cast<decltype(aligned_n)>(buffer + N - ptr);
		if (available_bytes >= aligned_n) {
			auto* r = ptr;
			ptr += aligned_n;
			return r;
		}
		return static_cast<std::byte*>(::operator new(n));
	}
	auto deallocate(std::byte* p, size_t n) noexcept -> void {
		static int counter = 0;
		if (pointer_in_buffer(p)) {
			n = align_up(n);
			if (p + n == ptr) {
				ptr = p;
			}
		}
		else {
			::operator delete(p);
		}
	}
private:
	static auto align_up(size_t n) noexcept -> size_t {
		return (n + (alignment - 1)) & ~(alignment - 1);
	}
	auto pointer_in_buffer(const std::byte* p) const noexcept -> bool {
		return std::uintptr_t(p) >= std::uintptr_t(buffer) && std::uintptr_t(p) < std::uintptr_t(buffer) + N;
	}
	alignas(alignment) std::byte buffer[N];
	std::byte* ptr{};
};