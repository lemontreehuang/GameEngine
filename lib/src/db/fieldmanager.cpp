
#include "fieldmanager.h"
#include "field.h"
#include "datafield.h"
#include "preparestatic.h"
#include "result.h"
#include "lib/include/base/function.h"

FieldManager::FieldManager(MysqlHandler *mysql, char *name, unsigned short field_len, CreateField *field)
: m_field_len(field_len)
, m_all_field(field)
, m_mp(mysql, field_len + 1)
, m_s(new rapidjson::StringBuffer[m_field_len])
{
	m_base_sql = "REPLACE INTO ";
	m_base_sql += name;
	m_base_sql += " (rid,";
	std::string temp = "SELECT ";
	for (unsigned short i = 0; i < m_field_len; ++i)
	{
		temp += m_all_field[i].name;
		temp += ",";
	}
	temp.replace(temp.size() - 1, 1, " FROM ");
	temp += name;
	temp += " WHERE rid=?;";
	m_load = new MysqlPrepareStatic(mysql, 1, temp.c_str());
}

FieldManager::~FieldManager()
{
	if (m_load != NULL)
	{
		delete m_load;
	}

	if (m_s != NULL)
	{
		delete [] m_s;
		m_s = NULL;
	}
}

bool FieldManager::Init()
{
	return m_load->Init()/* && m_data_update.Init("../lua/update.lua")*/;
}

bool FieldManager::Load(RoleField *df)
{
// 	m_load->BindLong(0, &rf->rid);
// 	if (!m_load->Execute())
// 	{
// 		return false;
// 	}
// 	char *temp = NULL;
// 	int len = 0;
// 	int num = 0;
// 	MysqlResult mr(m_load);
// 	while (m_load->HasResult() && num < m_field_len)
// 	{
// 		temp = mr.ReadStr(num, len);
// 		if (temp != NULL && len > 0)
// 		{
// 			TextField *field = m_all_field[num].func();
// 			if (field->Read(m_data_update, len, temp))
// 			{
// 				rf->fields.push_back(field);
// 			}
// 			else
// 			{
// 				Function::Error("role module data was error %ld %s %s", rf->rid, field->GetName(), temp);
// 			}
// 		}
// 		else
// 		{
// 			// todo write log
// 		}
// 		++num;
// 	}
	return true;
}

bool FieldManager::Save(RoleField *rf)
{
// 	int field_size = rf->fields.size();
// 	if (field_size <= 0) return true;
// 	else if (field_size > m_field_len)
// 	{
// 		// todo 写log
// 		return false;
// 	}
// 	
// 	m_mp.BindLong(0, &rf->rid);
// 	m_sql = m_base_sql;
// 	m_data = ") VALUES (?,";
// 
// 	int num = 1;
// 	for (int i = 0; i < field_size; ++i)
// 	{
// 		m_writer.Reset(m_s[i]);
// 		if (!rf->fields[i]->Write(m_writer))
// 		{
// 			// todo write log
// 			continue;
// 		}
// 		m_sql += rf->fields[i]->GetName();
// 		m_sql += ",";
// 		m_data += "?,";
// 		m_mp.BindText(num, (char *)m_s[i].GetString(), m_s[i].GetSize());
// 		++num;
// 	}
// 
// 	bool ret =  false;
// 	if (num > 1)
// 	{
// 		m_sql.replace(m_sql.size() - 1, 1, m_data.replace(m_data.size() - 1, 1, ");"));
// 		ret = m_mp.Execute(m_sql.c_str(), m_sql.size());
// 	}
// 
// 	return ret;
	return true;
}
