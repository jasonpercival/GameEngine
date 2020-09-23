#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

class EventListener
{
public:
	// disable copy/move constructors
	EventListener(const EventListener&) = delete;
	EventListener(EventListener&&) = delete;
	EventListener& operator=(const EventListener&) = delete;
	EventListener& operator=(EventListener&&) = delete;

	EventListener() = delete;
	~EventListener();
	
	static void Update();
};

#endif //!EVENTLISTENER_H

