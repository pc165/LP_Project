#pragma once
class Matrix {
	Matrix():matrix_(nullptr), row_(0), col_(0) {}

private:
	int *matrix_;
	int row_, col_;
};

