#pragma once 

#include <deque>
#include <glm/mat4x4.hpp>

class MatrixStack;

class MatrixStackRef {
private:
	MatrixStack* stack;
	glm::mat4x4& myPtr;

public:
	MatrixStackRef(MatrixStack* st, glm::mat4x4& top) : stack(st), myPtr(top){};
	~MatrixStackRef();

	auto& operator->() {
		return myPtr;
	}
	auto& operator*() {
		return myPtr;
	}
};

class MatrixStack {
private:
	std::deque<glm::mat4x4> stack;
	bool isDirty;

public:
	MatrixStackRef push() {
		this->isDirty = 1;
		const auto latestAndGreatest = this->stack.back();
		this->stack.push_back(latestAndGreatest);
		return MatrixStackRef(this, this->stack.back());
	}

	void pop() {
		this->isDirty = 1;
		this->stack.pop_back();
	}

	glm::mat4x4& top() {
		return this->stack.back();
	}
};