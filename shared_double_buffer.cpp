#include "shared_double_buffer.hpp"
#include <chrono>
// Implement your shared double buffer here
DoubleBuffer::DoubleBuffer()
{
}

void DoubleBuffer::push_back(const double & data)
{
	std::lock_guard<std::mutex> lock(the_mutex);
	write.push_back(data);
}


double DoubleBuffer::pop()
{
	while (read.empty())
	{
		std::swap(write, read);
	}
	auto data = *read.begin();
	read.erase(read.begin());
	return data;
}

void DoubleBuffer::reserve(size_t num)
{
	std::lock_guard<std::mutex> lock(the_mutex);
	write.reserve(num);
	read.reserve(num);
}

void DoubleBuffer::erase(size_t pos)
{
	std::lock_guard<std::mutex> lock(the_mutex);
	read.erase(read.begin(), read.begin() + pos);
}



size_t DoubleBuffer::size()
{
	std::lock_guard<std::mutex> lock(the_mutex);
	return read.size();
}

size_t DoubleBuffer::capacity()
{
	std::lock_guard<std::mutex> lock(the_mutex);
	return read.capacity();
}

void DoubleBuffer::clear()
{
	std::lock_guard<std::mutex> lock(the_mutex);
	read.clear();
	write.clear();
}

bool DoubleBuffer::isFull()
{
	std::lock_guard<std::mutex> lock(the_mutex);
	return (write.size() == write.capacity());
}

