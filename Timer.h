#ifndef TIMER_H
#define TIMER_H

#include <time.h>
#include <functional>
using std::function;
using std::bind;

namespace mynet {
class Timer {
public:
	//typedef void (*TimerCallback)(void *objArg);
	typedef function<void ()> TimerCallback; 
	Timer(struct timespec, struct timespec, TimerCallback);
	int compare(struct timespec);
	void doCallback();
	bool repeated();
	void updateExpire();
	struct timespec expire();
	static void addTime(struct timespec *, struct timespec);

	bool operator<(const Timer &);
	bool operator>(const Timer &);
private:
	struct timespec expire_;
	struct timespec interval_;
	TimerCallback callback_;
	void *objArg_;
};
}
#endif
