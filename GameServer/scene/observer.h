#ifndef OBSERVER_H
#define OBSERVER_H

#include "../global/globalvariable.h"
#include "../obj/obj.h"

class Observer
{
public:
	Observer():
	obj_id(Obj::INVALID_OBJ_ID), 
	pos(0, 0),
	be_observe_area_pos(-2,-2), 
	be_observer_index(-1)
	{

	};
	~Observer(){};

	ObjID	obj_id;
	Posi	pos;			// ����λ��

	Posi	be_observe_area_pos;	// ���۲�����
	UInt32	be_observer_index;	// ���ڱ��۲����������

	OVERLOAD_MEMORYPOOL();
private:
};

#endif // OBSERVER_H