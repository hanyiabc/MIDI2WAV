// Define your shared double buffer here
#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <algorithm>
using std::vector;

class DoubleBuffer
{
public:
	DoubleBuffer();
	void push_back(const double & data);
	double pop();
	void reserve(size_t num);
	void erase(size_t pos);
	size_t size();
	size_t capacity();
	void clear();
	bool isFull();
private:
	vector<double> write;
	vector<double> read;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;
};

