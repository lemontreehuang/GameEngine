

#ifndef RAPIDJSON_DEFINE_H
#define RAPIDJSON_DEFINE_H

#ifdef __unix
#define RAPIDJSON_ASSERT(X)
#endif

#include "document.h"
#include "prettywriter.h"
#include "stringbuffer.h"

// 去除断言, todo 重新定义断言
// #ifdef RAPIDJSON_ASSERT
// #undef RAPIDJSON_ASSERT
// #define RAPIDJSON_ASSERT
// #endif // RAPIDJSON_ASSERT


// todo 优化一下
// 例如：做一个JsonRead（类似JsonWrite）,或者改一下JSON_READ_ARRAY_INT

enum
{
	ERR_IS_NOT_JSON = 1,	// 不是Json
	ERR_IS_NOT_OBJECT,		// 不是对象
	ERR_DATA_TYPE_ERR,		// 数据类型不对
	ERR_READ_DATA_ERR,		// 数据读取错误
	ERR_NO_THIS_FIELD		// 没有这个字段
};

// todo 完善error

#define JsonError(Code, Line)\
	false;

#define JSON_BASIC_CHECK(Str)\
	rapidjson::Document doc; \
	if (doc.Parse(Str).HasParseError())\
	{\
		return JsonError(ERR_IS_NOT_JSON, __LINE__); \
	}\
	if (!doc.IsObject())\
	{\
		return JsonError(ERR_IS_NOT_OBJECT, __LINE__); \
	}

#define JSON_WRITE_BASE_ARRAY_BEGIN(Writer, Name)\
	Writer.Key(Name); \
	Writer.StartArray();

#define JSON_WRITE_BASE_ARRAY_END(Writer)\
	Writer.EndArray();

#define JSON_READ_BASE_ARRAY_BEGIN(Name)\
	if (doc.HasMember(Name) && doc[Name].IsArray()){ \
		rapidjson::Value &array = doc[Name]; \
		unsigned int size = array.Size();

#define JSON_READ_BASE_ARRAY_END() \
	}\
	else return false;

#define JSON_READ_ONE_ARRAY_BEGIN(Name)\
	if (doc.HasMember(Name) && doc[Name].IsArray()){\
		rapidjson::Value &array = doc[Name]; \
		for (unsigned int i = 0; i < array.Size(); ++i)\
		{\
			const Value& sub = array[i];

#define JSON_READ_ONE_ARRAY_END() \
		}\
	}\
	else return Error(ERR_NO_THIS_FIELD, __LINE__);

#define JSON_READ_ARRAY_INT(Array, Index, Val) \
	if (size > Index && Array[Index].IsInt())\
	{\
		Val = Array[Index].GetInt(); \
	}\
	else return false;


// 以下一一对应
#define JSON_WRITE_TWO_ARRAY_BEGIN(Writer, Name, Vector, Itr)\
	Writer.Key(Name); \
	Writer.StartArray(); \
	for (Itr itr = Vector.begin(); itr != Vector.end(); ++itr)\
	{\
		Writer.StartArray();
	
#define JSON_WRITE_TWO_ARRAY_END(Writer) \
		Writer.EndArray(); \
	}\
	Writer.EndArray();

#define JSON_READ_TWO_ARRAY_BEGIN(Name)\
	if (doc.HasMember(Name) && doc[Name].IsArray()){\
		rapidjson::Value &array = doc[Name]; \
		for (unsigned int i = 0; i < array.Size(); ++i)\
		{\
			const rapidjson::Value& sub = array[i]; \
			if (!sub.IsArray()) return JsonError(ERR_IS_NOT_OBJECT, __LINE__); \

#define JSON_READ_TWO_ARRAY_END() \
		}\
	}

#define JSON_WRITE_ARRAY_INT_SET(Writer, Name, Set)\
	Writer.Key(Name); \
	Writer.StartArray(); \
	for (std::set<int>::iterator itr = Set.begin(); itr != Set.end(); ++itr)\
	{\
		JsonWrite(Writer, *itr); \
	}\
	Writer.EndArray();


#define JSON_READ_ARRAY_INT_SET(Name, Set) \
	JSON_READ_ONE_ARRAY_BEGIN(Name); \
	if (sub.IsInt())\
	{\
		Set.insert(sub.GetInt()); \
	}\
	else return Error(ERR_READ_DATA_ERR, __LINE__);\
	JSON_READ_ONE_ARRAY_END();


#define JSON_RETURN(Ret) \
	fclose(fp); \
	return Ret;

#define JSON_READ_FILE_BEGIN(File)\
	FILE * fp = fopen(File, "r"); \
	if (fp != NULL){ \
		fseek(fp, 0, SEEK_END); \
		int file_size = ftell(fp);\
		if (file_size < 1) {\
			printf("%s is empty file\n", File);\
			JSON_RETURN(false); \
		}\
		char *buf = new char[file_size + 1]; \
		fseek(fp, 0, SEEK_SET); \
		int read_size = fread(buf, 1, file_size, fp); \
		if (read_size < 1){ \
			printf("read %s error\n", File);\
			JSON_RETURN(false); \
		}\
		buf[read_size] = 0; \
		fclose(fp); \
		if (doc.Parse(buf).HasParseError() || !doc.IsObject())\
		{\
			delete[] buf;\
			return false;\
		}\
		delete[] buf;

		

#define JSON_READ_FILE_END(File)\
	} \
	else { \
		printf("can not find %s \n", File);\
		return false;\
	}\
	return true;
	


#endif