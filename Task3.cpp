#include "pch.h"
#include <iostream>



template <typename T>
class SmartPtr {
private:
	T* obj;
	int refCount;

public:
	SmartPtr(T* obj) {
		this->obj = obj;
		refCount = 1;
	}

	T* get() {
		return obj;
	}

	void set(T* other) {
		*this->obj = *other;
	}

	void release() {
		refCount--;
		if (refCount == 0)
			delete obj;
	}

	SmartPtr& operator =(SmartPtr& other) {
		if (refCount > 0) {
			release();
		}
		if (this != &other) {
			set(other.get());
			other.refCount++;
		}
		return *this;
	}

	T* operator ->(){
		return get();
	}

	T& operator *() {
		return *obj;
	}
};

int main()
{
	SmartPtr<int> ref1 = new int(1);
	SmartPtr<int> ref2 = new int(2);
	std::cout << "ref1 = " << *ref1 << "\n";
	std::cout << "ref2 = " << *ref2 << "\n";

	ref1 = ref2;
	std::cout << "ref1 = " << *ref1 << "\n";

	ref1.set(new int(3));
	std::cout << "ref1 = " << *ref1 << "\n";
	ref2.release();
}