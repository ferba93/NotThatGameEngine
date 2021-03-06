#include "ManagerEvent.h"


Event::Event() {

	id = EVENT_ENUM::NULL_EVENT;
	idTrigger = EVENT_ENUM::NULL_EVENT;

}

Event::Event(EVENT_ENUM idValue, EVENT_ENUM idTriggerValue) {

	id = idValue;
	idTrigger = idTriggerValue;

}

Event::~Event() {}

ManagerEvent::ManagerEvent(Application* app, bool start_enabled) : Module(app, start_enabled) {}


ManagerEvent::~ManagerEvent() {}


bool ManagerEvent::CleanUp() {

	CleanListenerMap();

	eventVector.clear();
	variablesVector.clear();

	return true;

}

void ManagerEvent::GenerateEvent(EVENT_ENUM eventId, EVENT_ENUM eventTriggerId, void* var) {

	if (eventId != EVENT_ENUM::NULL_EVENT) {

		Event newEvent(eventId, eventTriggerId);
		EVENT_ENUM eventCheck = CheckEventTrigger(eventId);

		if (eventCheck != EVENT_ENUM::NULL_EVENT) { FireEvent(eventCheck); }
		if (eventTriggerId == EVENT_ENUM::NULL_EVENT) { FireEvent(eventId); }
		else { eventVector.push_back(newEvent); }

	}

	if (var != nullptr) { variablesVector.push_back(VariableStorage(eventId, var)); }

}

void ManagerEvent::FireEvent(EVENT_ENUM eventId) const {

	if (eventId != EVENT_ENUM::NULL_EVENT) {

		if (eventListenersMap.size() == 0) { return; }

		if (eventListenersMap.count(eventId) == 1) {

			std::vector<Module*> listeners = eventListenersMap.at(eventId);
			int numElem = listeners.size();
			for (int i = 0; i < numElem; i++) { listeners[i]->AddEvent(eventId); }
			listeners.clear();

		}
	}

}

EVENT_ENUM ManagerEvent::CheckEventTrigger(EVENT_ENUM eventTrigger) {

	EVENT_ENUM ret = EVENT_ENUM::NULL_EVENT;

	int numElem = eventVector.size();

	for (int i = 0; i < numElem; i++) {

		if (eventVector[i].idTrigger == eventTrigger) {

			ret = eventVector[i].id;
			eventVector.erase(eventVector.begin() + i);
			break;

		}
	}

	return ret;

}

//returns true if the event has been registered or if the new listener has been added, else returns false
bool ManagerEvent::EventRegister(EVENT_ENUM event, Module* mod)
{
	bool ret = false;

	//if event isn't on the map create a new entry on the map
	if (!eventListenersMap.count(event))
	{
		CreateEventOnMap(event);
		eventListenersMap[event].push_back(mod);
		ret = true;
	}
	//if listener has already been added don't add
	//else add a new listener
	else if (FindListener(event, mod) == -1)
	{
		eventListenersMap[event].push_back(mod);
		ret = true;
	}


	return ret;
}


//returns true if an element has been erased from the listeners map
bool ManagerEvent::EventUnRegister(EVENT_ENUM event, Module* mod)
{
	bool ret = false;

	//if event does exist
	if (eventListenersMap.count(event))
	{
		//then if listener does exist erase the element
		int i = FindListener(event, mod);
		if (i != -1)
		{
			EraseListener(event, mod, i);
			ret = true;
		}

	}


	return ret;
}


void ManagerEvent::CreateEventOnMap(EVENT_ENUM event)
{
	std::vector<Module*> vec;
	eventListenersMap[event] = vec;

	vec.clear();
}


//returns a positive number if a listener is already in the vector else returns -1
int ManagerEvent::FindListener(EVENT_ENUM event, Module* mod)
{
	int ret = -1;

	for (int i = 0; i < eventListenersMap[event].size(); i++)
	{

		if (eventListenersMap[event].at(i) == mod)
		{
			ret = i;
			break;
		}

	}

	return ret;
}


std::vector<Module*>::iterator ManagerEvent::EraseListener(EVENT_ENUM event, Module* mod, int vecId)
{
	std::vector<Module*>::iterator iter = eventListenersMap[event].begin();

	//generate the iterator for the given ID
	for (int i = 0; i < vecId; i++)
	{
		iter++;
	}

	//delete the listener
	eventListenersMap[event].erase(iter);

	//if there are no more listeners delete the map event entry
	if (eventListenersMap[event].size() <= 0)
	{
		eventListenersMap.erase(event);
	}

	return iter;
}


void  ManagerEvent::CleanListenerMap() { eventListenersMap.clear(); }


void ManagerEvent::CleanVariable(EVENT_ENUM event) {

	for (int i = 0; i < variablesVector.size(); i++) { if (variablesVector[i].id == event) { variablesVector.erase(variablesVector.begin() + i); } }

}


VariableStorage::VariableStorage(EVENT_ENUM _id, void* _var) : id(_id), varBuffer(_var) {}


VariableStorage::~VariableStorage() { varBuffer = nullptr; }

