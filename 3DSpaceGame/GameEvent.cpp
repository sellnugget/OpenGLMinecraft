#include "GameEvent.h"


void GameEvent::Call()
{
	for (int i = 0; i < UpdateEvents.size(); i++) {
		UpdateEvents[i].func();
	}
}

void GameEvent::Sub(std::function<void()> function, int* EventID)
{
	UpdateEvents.push_back({ subCount,function });
	*EventID = subCount;
	subCount++;
}

void GameEvent::Unsub(int Instance)
{
	for (int i = 0; i < UpdateEvents.size(); i++) {
		if (UpdateEvents[i].Instance == Instance) {
			UpdateEvents.erase(UpdateEvents.begin() + i);
			break;
		}
	}
}

GameEvent GameEvents::Update;