/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : terrain.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-09-07
 * Note   : terrain 指令。
 * Update :
 *  o 2000-00-00
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <message.h>
#include <map.h>
#include <daemon.h>
#include <terrain.h>

inherit COMMAND;

string help = @HELP
　　手動建立地形的指令

可以使用的參數如下：

clear			- 將目前位置的座標資料完全清除成空地
forest			- 建立森林地形([1;32m＊[m)
mountain1		- 建立山脈地形([0;33m◢[m)
mountain2		- 建立山脈地形([0;33m◣[m)
river1			- 建立河流([0;44;1;34m∼[m流向往東)
river2			- 建立河流([0;44;1;34m∼[m流向往西)
river3			- 建立河流([0;44;1;34m∼[m流向往南)
river4			- 建立河流([0;44;1;34m∼[m流向往北)
road			- 建立道路
road '名稱'		- 建立道路與道路名稱
bridge			- 建立橋樑([0;41;1;31m井[m)
bridge '名稱'		- 建立橋樑與橋樑名稱
wall			- 建立牆壁屬性
farm			- 建立農田([0;42;1;32m〃[m)
pasture			- 建立牧場([0;43;1;33m〃[m)
fishfarm		- 建立養殖場([0;44;1;34m〃[m)
light			- 建立路燈([1;33mΓ[m)
pool			- 建立水池([1;36m☉[m)
pavilion		- 建立涼亭([1;37m☆[m)
tree			- 建立大樹([1;32m¥[m)
grass			- 建立草皮([0;32mω[m)
statue			- 建立雕像([1;35m〥[m)
icon '圖形'		- 單純在目前座標上繪圖(無屬性資料)
icon -d			- 刪除目前座標上的圖形
room '房間位置'		- 設定目前座標連結之房間檔案
room -d			- 刪除房間連結設定
room broken		- 設定成一踏糊塗的房間
name '座標名稱'		- 設定目前座標對應之名稱
name -d			- 刪除名稱設定

HELP;

private void command(object me, string arg)
{
	string arg2;
	array loc = query_temp("location", me);

	if( !is_command() ) return;

	if( !arg )
		return tell(me, help);

	if( !arrayp(loc) || !AREA_D->is_area_location(loc) )
		return tell(me, pnoun(2, me)+"只能在郊區設定地形。\n");
		
	sscanf(arg, "%s %s", arg, arg2);

	switch(arg)
	{
		// 清除本地所有資料
		case "clear":
		{
			// 清除座標資料
			AREA_D->delete_coor_data(loc);
			
			// 清除座標圖形
			AREA_D->delete_coor_icon(loc);
			
			msg("$ME將這塊土地夷為平地。\n", me, 0, 1);
			
			break;
		}
		// 建立森林地形
		case "forest":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, FOREST);
			AREA_D->set_coor_icon(loc, HIG"＊"NOR);
			
			msg("$ME在這塊土地上建立了森林「"HIG"＊"NOR"」的地形。\n", me, 0, 1);
			
			break;
		}
		// 建立山脈地形(左)
		case "mountain1":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, MOUNTAIN);
			AREA_D->set_coor_icon(loc, YEL"◢"NOR);
			
			msg("$ME在這塊土地上建立了山脈「"YEL"◢"NOR"」的地形。\n", me, 0, 1);
			
			break;
		}
		// 建立山脈地形(右)
		case "mountain2":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, MOUNTAIN);
			AREA_D->set_coor_icon(loc, YEL"◣"NOR);
			
			msg("$ME在這塊土地上建立了山脈「"YEL"◣"NOR"」的地形。\n", me, 0, 1);
			
			break;
		}
		// 建立河流地形(東)
		case "river1":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, RIVER);
			AREA_D->set_coor_icon(loc, BBLU HIB"∼"NOR);
			AREA_D->set_coor_data(loc, FLAGS, FLOW_EAST);

			msg("$ME在這塊土地上建立了河流「"BBLU HIB"∼"NOR"」(東向)的地形。\n", me, 0, 1);
			
			break;
		}
		// 建立河流地形(西)
		case "river2":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, RIVER);
			AREA_D->set_coor_icon(loc, BBLU HIB"∼"NOR);
			AREA_D->set_coor_data(loc, FLAGS, FLOW_WEST);

			msg("$ME在這塊土地上建立了河流「"BBLU HIB"∼"NOR"」(西向)的地形。\n", me, 0, 1);
			
			break;
		}
		// 建立河流地形(南)
		case "river3":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, RIVER);
			AREA_D->set_coor_icon(loc, BBLU HIB"∼"NOR);
			AREA_D->set_coor_data(loc, FLAGS, FLOW_SOUTH);

			msg("$ME在這塊土地上建立了河流「"BBLU HIB"∼"NOR"」(南向)的地形。\n", me, 0, 1);
			
			break;
		}
		// 建立河流地形(北)
		case "river4":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, RIVER);
			AREA_D->set_coor_icon(loc, BBLU HIB"∼"NOR);
			AREA_D->set_coor_data(loc, FLAGS, FLOW_NORTH);

			msg("$ME在這塊土地上建立了河流「"BBLU HIB"∼"NOR"」(北向)的地形。\n", me, 0, 1);
			
			break;
		}
		// 建立道路地形
		case "road":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, ROAD);
			AREA_D->set_coor_icon(loc, "　");

			if( !arg2 )
				msg("$ME在這塊土地上建立了道路的地形。\n", me, 0, 1);
			else
			{
				msg("$ME在這塊土地上建立了道路並取名為「"+(arg2+NOR)+"」。\n", me, 0, 1);
				AREA_D->set_coor_data(loc, "short", kill_repeat_ansi(arg2+NOR));				
			}
			break;
		}
		// 建立橋樑地形
		case "bridge":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, BRIDGE);
			AREA_D->set_coor_icon(loc, BRED HIR"井"NOR);

			if( !arg2 )
				msg("$ME在這塊土地上建立了橋樑「"+BRED HIR"井"NOR+"」的地形。\n", me, 0, 1);
			else
			{
				msg("$ME在這塊土地上建立了橋樑「"+BRED HIR"井"NOR+"」並取名為「"+(arg2+NOR)+"」。\n", me, 0, 1);
				AREA_D->set_coor_data(loc, "short", kill_repeat_ansi(arg2+NOR));				
			}
			break;
		}
		// 建立牆壁屬性
		case "wall":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, WALL);
			AREA_D->set_coor_data(loc, FLAGS, NO_MOVE);

			msg("$ME在這塊土地上建立了牆壁的屬性。\n", me, 0, 1);
			
			break;
		}
		// 建立農田
		case "farm":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, FARM);
			AREA_D->set_coor_icon(loc, HIG BGRN"〃"NOR);
			
			msg("$ME在這塊土地上建立了農田「"HIG BGRN"〃"NOR"」。\n", me, 0, 1);
			
			break;
		}
		// 建立牧場
		case "pasture":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, PASTURE);
			AREA_D->set_coor_icon(loc, HIY BYEL"〃"NOR);

			msg("$ME在這塊土地上建立了牧場「"HIY BYEL"〃"NOR"」。\n", me, 0, 1);
			
			break;
		}
		// 建立路燈
		case "light":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, LIGHT);
			AREA_D->set_coor_icon(loc, HIY"Γ"NOR);
			
			msg("$ME在這塊土地上建立了路燈「"HIY"Γ"NOR"」。\n", me, 0, 1);
			
			break;
		}
		// 建立水池
		case "pool":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, POOL);
			AREA_D->set_coor_icon(loc, HIC"☉"NOR);
			
			msg("$ME在這塊土地上建立了水池「"HIC"☉"NOR"」。\n", me, 0, 1);
			
			break;
		}
		// 建立涼亭
		case "pavilion":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, PAVILION);
			AREA_D->set_coor_icon(loc, HIW"☆"NOR);
			
			msg("$ME在這塊土地上建立了涼亭「"HIW"☆"NOR"」。\n", me, 0, 1);
			
			break;
		}
		// 建立大樹
		case "tree":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, TREE);
			AREA_D->set_coor_icon(loc, HIG"¥"NOR);
			
			msg("$ME在這塊土地上建立了大樹「"HIG"¥"NOR"」。\n", me, 0, 1);
			
			break;
		}
		// 建立草皮
		case "grass":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, GRASS);
			AREA_D->set_coor_icon(loc, NOR GRN"ω"NOR);
			
			msg("$ME在這塊土地上建立了草皮「"NOR GRN"ω"NOR"」。\n", me, 0, 1);
			
			break;
		}
		// 建立雕像
		case "statue":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, STATUE);
			AREA_D->set_coor_icon(loc, HIM"〥"NOR);
			
			msg("$ME在這塊土地上建立了雕像「"HIM"〥"NOR"」。\n", me, 0, 1);
			
			break;
		}
		// 建立養殖場
		case "fishfarm":
		{
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, TYPE, FISHFARM);
			AREA_D->set_coor_icon(loc, HIB BBLU"〃"NOR);
			
			msg("$ME在這塊土地上建立了養殖場「"HIB BBLU"〃"NOR"」。\n", me, 0, 1);
			
			break;
		}	
		// 單純繪圖(不處理屬性的部份)
		case "icon":
		{
			if( !arg2 )
				return tell(me, "請輸入正確的指令格式。\n");
	
			if( arg2 == "-d" )
			{
				AREA_D->delete_coor_icon(loc);
				
				msg("$ME刪除了這塊土地的圖形。\n", me, 0, 1);
				
				break;
			}
			AREA_D->set_coor_icon(loc, kill_repeat_ansi(arg2+NOR));
			
			msg("$ME在這塊土地上建立了「"+arg2+NOR+"」的圖形。\n", me, 0, 1);
		
			break;
		}
		// 建立對應的房間資料
		case "room":
		{
			if( !arg2 )
				return tell(me, "請輸入正確的指令格式。\n");

			if( arg2 == "-d" )
			{
				AREA_D->delete_coor_data(loc, ROOM);
				AREA_D->delete_coor_data(loc, SPECIAL_EXIT);
				
				msg("$ME刪除了這塊土地的房間連結資料。\n", me, 0, 1);
				
				break;
			}
			else if( arg2 == "broken" )
			{
				AREA_D->delete_coor_data(loc);	
				AREA_D->set_coor_data(loc, ROOM, "room");
				msg("$ME在這塊土地上建立了一間無法進入的房間。\n", me, 0, 1);
				return;
			}
			AREA_D->delete_coor_data(loc);
			AREA_D->set_coor_data(loc, SPECIAL_EXIT, arg2);
			AREA_D->set_coor_data(loc, ROOM, "room");

			msg("$ME在這塊土地上建立了與房間「"+arg2+"」的連接。\n", me, 0, 1);
		
			break;
		}
		// 建立土地的名稱
		case "name":
		{			
			if( !arg2 )
				return tell(me, "請輸入正確的指令格式。\n");
			
			if( arg2 == "-d" )
			{
				AREA_D->delete_coor_data(loc, "short");
				
				msg("$ME刪除了這塊土地的命名資料。\n", me, 0, 1);
				
				break;
			}

			AREA_D->set_coor_data(loc, "short", kill_repeat_ansi(arg2+NOR));
				
			msg("$ME將這塊土地命名為「"+arg2+NOR+"」。\n", me, 0, 1);
			
			break;
		}
		// 其他狀況
		default:
		{
			tell(me, "請輸入正確的參數。\n"+help);
			break;
		}
	}
}
