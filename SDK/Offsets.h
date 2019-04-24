#pragma once

#include <vector>

struct _Offsets {
	std::initializer_list<std::uintptr_t> EntityListOffsets;
	std::initializer_list<std::uintptr_t> InGameOffsets;
	std::initializer_list<std::uintptr_t> GmodeOffsets;

	_Offsets() {
		EntityListOffsets = { 0x0, 0x60,0x10,0x40,0x0 };
		InGameOffsets = { 0x0, 0x48,0xA8,0x150 };
		GmodeOffsets = { 0x240,0x18,0x8B8};
	}
};