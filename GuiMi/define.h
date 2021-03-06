
#ifndef DEFINE_H
#define DEFINE_H

#include "common/datastructure/vector2d.h"
#include "common/datastructure/gamevector.h"

typedef unsigned int			UInt32;
typedef unsigned short			Coord;		// ��������
typedef Vector2D<Coord>			Posi;
typedef int						ObjID;
typedef int						RoleID;
typedef int						SceneID;
typedef int						MapID;

static const ObjID	INVALID_OBJ_ID = ObjID(-1);

struct SceneRet
{
	game::Vector<ObjID> leave_other;
	game::Vector<ObjID> enter_other;
	game::Vector<ObjID> other_leave;
	game::Vector<ObjID> other_enter;
};
#endif // !DEFINE_H