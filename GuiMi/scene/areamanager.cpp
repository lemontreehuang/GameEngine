
#include "areamanager.h"
#include "obj/obj.h"
#include "obj/objmanager.h"
#include "observer.h"


AreaManager::~AreaManager()
{
	ReleaseAreaMatrix();
}

void AreaManager::Init( short unit_x,short unit_y,short max_x,short max_y )
{
	m_unit_x = unit_x;
	m_unit_y = unit_y;
	m_max_x  = max_x;
	m_max_y  = max_y;
}

void AreaManager::CreateAreaMatrix( AreaManager *am, short unit_x, short unit_y, short max_x, short max_y )
{
	am->m_unit_x = unit_x;
	am->m_unit_y = unit_y;
	am->m_max_x  = max_x;
	am->m_max_y  = max_y;

	am->m_x_num = max_x/unit_x + 1;
	am->m_y_num = max_y/unit_y + 1;
	
    am->m_area_matrix = (Area **)malloc(am->m_x_num * sizeof(Area *));
    memset(am->m_area_matrix, 0 ,am->m_x_num * sizeof(Area *));
    for (int i = 0; i < am->m_x_num; ++i)
	{
        am->m_area_matrix[i] = new Area[am->m_y_num];
        for (int j = 0; j < am->m_y_num; ++j)
		{
            am->m_area_matrix[i][j].Init( unit_x * i, unit_y * j, unit_x * (i + 1), unit_y * (j + 1));
		}
	}
}


void AreaManager::ReleaseAreaMatrix()
{
	for (int i = 0; i < m_x_num; ++i)
	{
		delete []m_area_matrix[i];
	}
	if (m_area_matrix != NULL)
	{
		free(m_area_matrix);
		m_area_matrix = NULL;
    }
}

//创建观察者也就是将obj加入到某个区域（area）,用于其它aoi需要通知这个area的所有obj。
UInt32 AreaManager::GreateObserver(ObjID obj_id, SceneRet &ret)
{
	Observer *obser = new Observer();
	obser->obj_id = obj_id;
	UInt32 obser_handle = m_observer_pool.Insert(obser);

	Obj *obj = m_obj_manager->GetObj(obj_id);
	obser->pos = obj->GetPos();

	Posi area_pos;
	if (!CheckArea(area_pos, obser->pos))
	{
		obj->SetPos(Posi(0, 0));
		area_pos.Init(0, 0);
		obser->pos.Init(0, 0);
	}
	obser->be_observe_area_pos = area_pos;
	obser->be_observer_index = m_area_matrix[area_pos.x][area_pos.y].AddObserver(obj_id);

	// 触发AOIEnter消息
	Area *area = &m_area_matrix[obser->be_observe_area_pos.x][obser->be_observe_area_pos.y];
	UInt32 aoi_size = area->GetAOISize();
	for (UInt32 i = 0; i < aoi_size; ++i)
	{
		AOI *aoi = &m_aoi_pool[area->GetAOIHandle(i)];
		if (aoi->IsIn(obser->pos) && aoi->obj_id != obser->obj_id)
		{
			//m_obj_manager->GetObj(aoi->obj_id)->OnEnterAOI(obser->obj_id);
			ret.enter_other.Push(aoi->obj_id);
		}
	}
	return obser_handle;
}

/* 
	CreateAOI和GreateObserver一般是同时调用，达到相互通知进入视野
	而例如掉落，它不需要CreateAOI，因为它不需要观察其它对象的状态，它只需要被其它对象观察
*/
UInt32 AreaManager::CreateAOI(ObjID obj_id, const Posi &center, Posi &radius, SceneRet &ret, bool is_circle)
{
    Posi offset_center(center.x < radius.x ? radius.x : center.x, center.y < radius.y ? radius.y : center.y);
    AOI aoi(obj_id, offset_center, radius, is_circle);
	UInt32 aoi_handle = m_aoi_pool.Insert(aoi);

    Posi bottom_left;
	Posi top_right;
    GetArea(bottom_left, offset_center.x - radius.x, offset_center.y - radius.y);  
    GetArea(top_right, offset_center.x + radius.x, offset_center.y + radius.y);

	Obj *obj = m_obj_manager->GetObj(obj_id);
	
	/* 
		前两个for循环是找到所有对obj所在的区域感兴趣的所有矩形区域
		之后的for循环是找到每个矩形区域的所有其它的obj
	*/
	Area *area = NULL;
	ObjID other_obj_id = 0;
	for (Coord i = bottom_left.x; i <= top_right.x; ++i)
    {
		for (Coord j = bottom_left.y; j <= top_right.y; ++j)
        {
            area = &m_area_matrix[i][j];
			area->AddAOIHandle(aoi_handle);
			Area::OBSERVER_LIST *observer_list = area->GetObserverList();
			for (Area::OBSERVER_LIST::iterator itr = observer_list->Begin(); itr != observer_list->End(); ++itr)
			{
				Obj *other_obj = m_obj_manager->GetObj(*itr);
				if (other_obj == NULL)
				{
					continue;
				}
				other_obj_id = other_obj->GetObjID();
				if (aoi.IsIn(obj->GetPos()) && obj_id != other_obj_id)
				{
					//obj->OnEnterAOI(other_obj->GetObjID());
					ret.other_enter.Push(other_obj_id);
				}
			}
        }
    }
	return aoi_handle;
}

UInt32 AreaManager::CreateCircleAOI(ObjID obj_id, const Posi &center, int radius, SceneRet &ret)
{
	return CreateAOI(obj_id, center, Posi(radius, radius), ret, true);
}

UInt32 AreaManager::CreateRectAOI(ObjID obj_id, const Posi &center, int aoi_x, int aoi_y, SceneRet &ret)
{
	return CreateAOI(obj_id, center, Posi(aoi_x, aoi_y), ret);
}


// 获得位置（x,y）所在的area(区域)
void AreaManager::GetArea(Posi &area, Coord x, Coord y)
{
    area.x = x/m_unit_x;
    area.y = y/m_unit_y;
	(area.x >= m_x_num) ? (area.x = m_x_num - 1) : 0;
	(area.y >= m_y_num) ? (area.y = m_y_num - 1) : 0;
}

// 判断位置p的位置是否合法，并获得将p所在的area
bool AreaManager::CheckArea( Posi &area, const Posi &p )
{
	area.x = p.x / m_unit_x;
	area.y = p.y / m_unit_y;
	return area.x < m_x_num && area.y < m_y_num;
}

void AreaManager::MoveObserver( UInt32 observer_handle )
{
	if (!m_observer_pool.Exist(observer_handle))
	{
		return ;
	}

	Observer *obser = m_observer_pool[observer_handle];
	ObjID objID = obser->obj_id;
	Posi old_area_pos = obser->be_observe_area_pos;
	Posi old_obser_pos = obser->pos;

	Obj *obj = m_obj_manager->GetObj(objID);
	obser->pos = obj->GetPos();

	Posi new_area_pos;
	bool is_same_area = false;
	if (CheckArea(new_area_pos, obser->pos))
	{
		if (new_area_pos != obser->be_observe_area_pos)
		{
			m_area_matrix[obser->be_observe_area_pos.x][obser->be_observe_area_pos.y].EraseObserver(obser->be_observer_index);

			obser->be_observe_area_pos = new_area_pos;
			obser->be_observer_index = m_area_matrix[new_area_pos.x][new_area_pos.y].AddObserver(objID);
		}
		else
		{
			is_same_area = true;
		}
	}

	Area *area = &m_area_matrix[old_area_pos.x][old_area_pos.y];
	int aoi_size = area->GetAOISize();
	if (is_same_area)
	{	
		for (int i = 0; i < aoi_size; ++i)
		{
			AOI *aoi = &m_aoi_pool[area->GetAOIHandle(i)];

			if (aoi->obj_id == objID)
			{
				continue;
			}

			if (aoi->IsIn(old_obser_pos))
			{
				if (aoi->IsOut(obser->pos))
				{
					m_obj_manager->GetObj(aoi->obj_id)->OnLeaveAOI(obser->obj_id);
				}
			}
			else
			{
				if (aoi->IsIn(obser->pos))
				{
					m_obj_manager->GetObj(aoi->obj_id)->OnEnterAOI(obser->obj_id);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < aoi_size; ++i)
		{
			AOI *aoi = &m_aoi_pool[area->GetAOIHandle(i)];

			if (aoi->obj_id == objID)
			{
				continue;
			}

			if (aoi->IsIn(old_obser_pos) && aoi->IsOut(obser->pos))
			{
				m_obj_manager->GetObj(aoi->obj_id)->OnLeaveAOI(obser->obj_id);
			}
		}

		area = &m_area_matrix[obser->be_observe_area_pos.x][obser->be_observe_area_pos.y];
		aoi_size = area->GetAOISize();
		for (int i = 0; i < aoi_size; ++i)
		{
			AOI *aoi = &m_aoi_pool[area->GetAOIHandle(i)];
			if (aoi->obj_id == objID)
			{
				continue;
			}
			if (aoi->IsOut(old_obser_pos) && aoi->IsIn(obser->pos))
			{
				m_obj_manager->GetObj(aoi->obj_id)->OnEnterAOI(obser->obj_id);
			}
		}
	}
}

void AreaManager::MoveAOI( UInt32 aoi_handle )
{
	if (!m_aoi_pool.Exist(aoi_handle))
	{
		return ;
	}

	AOI *aoi = &m_aoi_pool[aoi_handle];
	ObjID objID = aoi->obj_id;
	Obj *obj = m_obj_manager->GetObj(objID);
	Posi newCenter = obj->GetPos();
	Posi leaveRadius = aoi->radius;

	(newCenter.x < leaveRadius.x) ? (newCenter.x = leaveRadius.x) : 0;
	(newCenter.y < leaveRadius.y) ? (newCenter.y = leaveRadius.y) : 0;
	(newCenter.x > m_max_x - leaveRadius.x) ? (newCenter.x = m_max_x - leaveRadius.x) : 0;
	(newCenter.y > m_max_y - leaveRadius.y) ? (newCenter.y = m_max_y - leaveRadius.y) : 0;

	// AOI模块中对两次MoveAOI小于这个时不加处理
	static const int AOI_SENSITIVE_DISTANCE = 2;

	AOI aoi_old = *aoi;
	Posi oldCenter = aoi->center;

	Posi oldBottomLeft;
	Posi oldTopRight;
	GetArea(oldBottomLeft,	oldCenter.x - leaveRadius.x,		oldCenter.y - leaveRadius.y);
	GetArea(oldTopRight,	oldCenter.x + leaveRadius.x - 1,	oldCenter.y + leaveRadius.y - 1);

	aoi->center = newCenter;

	Posi newBottomLeft;
	Posi newTopRight;
	GetArea(newBottomLeft,	newCenter.x - leaveRadius.x,		newCenter.y - leaveRadius.y);
	GetArea(newTopRight,	newCenter.x + leaveRadius.x - 1,	newCenter.y + leaveRadius.y - 1);

	bool zone_is_the_same = (oldBottomLeft == newBottomLeft && oldTopRight == newTopRight);

	for (unsigned int x = oldBottomLeft.x; x <= oldTopRight.x; ++x)
	{
		for (unsigned int y = oldBottomLeft.y; y <= oldTopRight.y; ++y)
		{
			bool is_in = false;
			if (zone_is_the_same)
			{
				is_in = true;
			}
			else
			{
				for (unsigned int x_new = newBottomLeft.x; x_new <= newTopRight.x; ++x_new)
				{
					for (unsigned int y_new = newBottomLeft.y; y_new <= newTopRight.y; ++y_new)
					{
						if (x == x_new && y == y_new)
						{
							is_in = true;
							break;
						}
					}
					if (is_in)
					{
						break;
					}
				}
			}

			Area *area = &m_area_matrix[x][y];
			Area::OBSERVER_LIST *observer_list = area->GetObserverList();
			for (Area::OBSERVER_LIST::iterator itr = observer_list->Begin(); itr != observer_list->End(); ++itr)
			{
				ObjID id = *itr;
				if (objID == id)
				{
					continue;
				}
				Obj *obser = m_obj_manager->GetObj(id);
				if (aoi->IsOut(obser->GetPos()) && !aoi_old.IsOut(obser->GetPos()))
				{
					obj->OnLeaveAOI(id);
				}
			}

			if (!is_in)
			{
				area->EraseAOIHandle(aoi_handle);
			}
		}
	}

	for (unsigned int x = newBottomLeft.x; x <= newTopRight.x; ++x)
	{
		for (unsigned int y = newBottomLeft.y; y <= newTopRight.y; ++y)
		{
			bool is_in = false;
			if (zone_is_the_same)
			{
				is_in = true;
			}
			else
			{
				for (unsigned int x_old = oldBottomLeft.x; x_old <= oldTopRight.x; ++x_old)
				{
					for (unsigned int y_old = oldBottomLeft.y; y_old <= oldTopRight.y; ++y_old)
					{
						if (x == x_old && y == y_old)
						{
							is_in = true;
							break;
						}
					}
					if (is_in)
					{
						break;
					}
				}
			}

			Area *area = &m_area_matrix[x][y];
			Area::OBSERVER_LIST *observer_list = area->GetObserverList();
			for (Area::OBSERVER_LIST::iterator itr = observer_list->Begin(); itr != observer_list->End(); ++itr)
			{
				ObjID id = *itr;
				if (objID == id)
				{
					continue;
				}
				Obj *obser = m_obj_manager->GetObj(id);
				if (!aoi_old.IsIn(obser->GetPos()) && aoi->IsIn(obser->GetPos()))
				{
					obj->OnEnterAOI(id);
				}
			}
			
			if (!is_in)
			{
				area->AddAOIHandle(aoi_handle);
			}
		}
	}
}

void AreaManager::EraseObserver( UInt32 observer_handle )
{
	if (!m_observer_pool.Exist(observer_handle))
	{
		return;
	}
	Observer *obser = m_observer_pool[observer_handle];

	// 触发AOILeave消息
	Area *area = &m_area_matrix[obser->be_observe_area_pos.x][obser->be_observe_area_pos.y];
	int aoiSize = area->GetAOISize();
	for (int i = 0; i < aoiSize; ++i)
	{
		AOI *aoi = &m_aoi_pool[area->GetAOIHandle(i)];
		if (!aoi->IsOut(obser->pos))
		{
			m_obj_manager->GetObj(aoi->obj_id)->OnLeaveAOI(obser->obj_id);
		}
	}

	// 从对应zone的被观察者列表中移除
	m_area_matrix[obser->be_observe_area_pos.x][obser->be_observe_area_pos.y].EraseObserver(obser->be_observer_index);
	delete obser;
	m_observer_pool.Erase(observer_handle);
}

void AreaManager::EraseAOI( UInt32 aoi_handle )
{
	if (!m_aoi_pool.Exist(aoi_handle))
	{
		return ;
	}

	Posi center = m_aoi_pool[aoi_handle].center;
	Posi radius = m_aoi_pool[aoi_handle].radius;

	Posi bottomLeft;
	Posi topRight;
	GetArea(bottomLeft, center.x - radius.x, center.y - radius.y);
	GetArea(topRight, center.x + radius.x - 1, center.y + radius.y - 1);

	for (unsigned int x = bottomLeft.x; x <= topRight.x; ++x)
	{
		for (unsigned int y = bottomLeft.y; y <= topRight.y; ++y)
		{
			//printf("DestroyAOI erase (%d, %d).\n", x, y);
			m_area_matrix[x][y].EraseAOIHandle(aoi_handle);
		}
	}
}
