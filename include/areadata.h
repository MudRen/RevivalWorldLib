/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : /include/areadata.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

// ¦ô­p¥i¼µ¨ì 100
#define MAX_AREA	50

#define AREA_ROOT				"/area/"
#define AREA_PATH(area)				AREA_ROOT+(string)area+"/"
#define AREA_INFO(area)				AREA_PATH(area)+"info"
#define AREA_NUM(area, num)			AREA_PATH(area)+(string)num+"/"
#define AREA_NUM_DATA(area, num)		AREA_NUM(area, num)+"data"
#define AREA_NUM_MAP(area, num)			AREA_NUM(area, num)+"map"
#define AREA_NUM_ROOM(area, num)		AREA_NUM(area, num)+"room/"
#define AREA_NUM_ESTATE(area, num)		AREA_NUM(area, num)+"estate"

#define AREA_ROOM_MODULE(area, num, x, y, roomtype)		AREA_NUM_ROOM(area, num)+(string)x+"_"+(string)y+"_"+(string)roomtype

#define MAP		"map"
#define RMAP		"realtime_map"
#define DATA		"data"
#define INFO		"info"
#define AROOM		"arearoom"
