#include "pch.h"
#include <vector>
#include <iostream>

using namespace std;

class Block{
public:
	size_t size;
	void* mem_point;
	bool isEmpty;

	Block(size_t size, void* point){
		this->size = size;
		mem_point = point;
		isEmpty = true;
	}
};

class Allocator {
private:
	size_t getPower(size_t size) {
		return pow(2, ceil(log2(size)));
	}

	int getBuddy(int i) {
		if (i == Blocks.size() - 1)
			return -1;
		int m = i + 1;
		while (Blocks[m].size <= Blocks[i].size && i != Blocks.size() - 1) {
			if (Blocks[m].size == Blocks[i].size && Blocks[m].isEmpty) {
				return m;
			}
			m++;
		}
		return -1;
	}

	void uniteBuddies(int i) {
		for (i; i < Blocks.size(); i += 2) {
			auto buddy_index = getBuddy(i);
			if (buddy_index == -1) {
				break;
			}

			i = i < buddy_index ? i : buddy_index;
			auto block = Block(Blocks[i].size * 2, Blocks[buddy_index].mem_point);
			Blocks.erase(i + Blocks.begin());
			Blocks.erase(i + Blocks.begin());
			Blocks.insert(i + Blocks.begin(), block);
		}
	}

	int getIndex(void* point) {
		for (auto i = 0; i < Blocks.size(); i++) {
			if (Blocks[i].mem_point == point) {
				return i;
			}
		}
		return -1;
	}

public:
	size_t mem_size;
	vector<Block> Blocks;

	Allocator(size_t size) {
		mem_size = getPower(size);
		auto point = malloc(size);
		Blocks.push_back(Block(mem_size, point));
	}

	void* alloc(size_t size) {
		for (auto i = 0; i < Blocks.size(); i++) {
			if (Blocks[i].isEmpty && Blocks[i].size >= size) {
				while (Blocks[i].isEmpty && Blocks[i].size / 2 >= size) {
					Block block = Block(Blocks[i].size / 2, Blocks[i].mem_point);
					Block twin = Block(Blocks[i].size / 2, (void*)((size_t)(Blocks[i].mem_point) + Blocks[i].size / 2));
					Blocks.erase(Blocks.begin() + i);
					Blocks.insert(Blocks.begin() + i, block);
					Blocks.insert(Blocks.begin() + i, twin);
				}
				Blocks[i].isEmpty = false;
				return Blocks[i].mem_point;
			}
		}
		return nullptr;
	}

	void dealloc(void* point) {
		auto index = getIndex(point);
		if (index != -1) {
			Blocks[index].isEmpty = true;
		}
		uniteBuddies(index);
	}

	void getInfo() {
		cout << "Blocks: \n";
		for (auto i = 0; i < Blocks.size(); i ++) {
			auto empty = "full";
			if (Blocks[i].isEmpty)
				empty = "empty";
			cout << Blocks[i].size << " " << Blocks[i].mem_point <<" " << empty << "\n";
		}
	}
};

int main() {
	Allocator* allocator = new Allocator((size_t)100);
	allocator->getInfo();
	void* point1 = allocator->alloc((size_t)60);
	allocator->getInfo();
	void* point2 = allocator->alloc((size_t)20);
	allocator->getInfo();
	void* point3 = allocator->alloc((size_t)16);
	allocator->getInfo();
	allocator->dealloc(point3);
	allocator->getInfo();
	return 0;
}
