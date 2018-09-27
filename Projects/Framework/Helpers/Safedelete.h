#pragma once

template<typename T> void safe_delete(T*& a) {
	delete a;
	a = nullptr;
}