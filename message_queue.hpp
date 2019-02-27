#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>
using std::string;
class MessageQueue
{
public:
	void push(string const& message);
	bool empty() const;
	bool try_pop(string& popped_value);
	void wait_and_pop(string& popped_value);
	

private:
	std::queue<string> the_queue;
	mutable std::mutex the_mutex;
	std::condition_variable the_condition_variable;

};
#endif // MESSAGE_QUEUE_H