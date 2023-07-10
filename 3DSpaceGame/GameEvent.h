#pragma once
#include <vector>
#include <functional>




class GameEvent
{
public:
	
	
	void Unsub(int Instance);

	void Call();

	//adds the function to list of stuff and returns a pointer so the event can be unsubscribed
	void Sub(std::function<void()> function, int* EventID);
private:

	//used for generating EventID's NOT FOR KEEPING TRACK OF HOW MANY SUBS THERE ARE
	int subCount;
	//this is used so that you can unsubscribe to events 
	struct EventData {
		int Instance;
		std::function<void()> func;
	};
	std::vector<EventData> UpdateEvents;
};


class GameEvents {
public:
	static GameEvent Update;
};
