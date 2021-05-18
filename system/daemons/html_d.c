/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : html_d.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-1-21
 * Note   : 
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <mudlib.h>
#include <time.h>

//----------------------------------------------------------------------
// 白底色 HEADER ----------------------------------------------------------------------
//----------------------------------------------------------------------
string WHITE_HEADER = @LONG
//<!-- Created by Revival World %^UPDATETIME%^ -->
<!DOCTYPE html>
<html lang="zh-TW">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=big5" />
  %^REFRESH%^
  <title>%^TITLE%^</title>
<style><!--
 a{text-decoration:none}
        -->
  </style>
</head>
<body bgcolor="#ffffff" text="#55555" link="#666688" vlink="#666688">
<table width=80% align=center>
  <tr>
    <td align=center style='font-familiy:verdana;font-size:15px;color:#555;'>本頁面由系統自動建置，更新時間為 %^UPDATETIME%^</td>
  </tr>
</table>
<hr width=80% align=center><p>
LONG;


//----------------------------------------------------------------------
// 黑底色 HEADER ----------------------------------------------------------------------
//----------------------------------------------------------------------
string BLACK_HEADER = @LONG
//<!-- Created by Revival World %^UPDATETIME%^ -->
<!DOCTYPE html>
<html lang="zh-TW">
<head>
<meta http-equiv="Content-Type" content="text/html;charset=big5" />
  %^REFRESH%^
  <title>%^TITLE%^</title>
<style><!--
 a{text-decoration:none}
        -->
  </style>
</head>
<body bgcolor="#000000" text="#bbbbbb" link="#666688" vlink="#666688">
<table width=80% align=center>
  <tr>
    <td align=center style='font-familiy:verdana;font-size:15px;color:#bbb;'>本頁面由系統自動建置，更新時間為 %^UPDATETIME%^</td>
  </tr>
</table>
<hr width=80% align=center><p>
LONG;


//----------------------------------------------------------------------
// 白底色 FOOTER ----------------------------------------------------------------------
//----------------------------------------------------------------------
string WHITE_FOOTER = @LONG
<p><hr width=80% align=center>
<table width=80% align=center>
  <tr>
    <td align=center><font face=verdana size=2 color=555555>Copyright &copy;2000-%^YEAR%^ <a href="https://www.revivalworld.org"><font color=111111><u>重生的世界</u></a> <font color=555555> All Rights Reserved</font></td>

  </tr>
</table>
</body>
</html>
<!-- Created by 重生的世界 %^UPDATETIME%^ -->
LONG;

//----------------------------------------------------------------------
// 黑底色 FOOTER ----------------------------------------------------------------------
//----------------------------------------------------------------------
string BLACK_FOOTER = @LONG
<p><hr width=80% align=center>
<table width=80% align=center>
  <tr>
    <td align=center><font face=verdana size=2 color=bbbbbb>Copyright &copy;2000-%^YEAR%^ <a href="https://www.revivalworld.org"><font color=fffff><u>重生的世界</u></a> <font color=bbbbbb> All Rights Reserved</font></td>

  </tr>
</table>
</body>
</html>
<!-- Created by 重生的世界 %^UPDATETIME%^ -->
LONG;

//----------------------------------------------------------------------
// JAVA Script ----------------------------------------------------------------------
//----------------------------------------------------------------------
string JAVA_SCRIPT = @long
<script language="JavaScript">
<!--
function show(e, helpmsg)
{
	if(document.layers)
	{
		x = e.layerX;
		y = e.layerY;
		document.layers[0].document.open();
		document.layers[0].document.write(helpmsg);
		document.layers[0].document.close();
		document.layers[0].moveTo(x + 10, y + 20);
		document.layers[0].visibility = "SHOW";
	}
	else if(document.all)
	{
		x = event.clientX;
		y = event.clientY;
		document.all("lay").innerHTML = helpmsg;
		document.all["lay"].style.pixelLeft = x+10;
		document.all["lay"].style.pixelTop = y+20;
		document.all["lay"].style.visibility = "visible";
	}
}

function hide()
{
	if(document.layers)
	{
		document.layers[0].visibility = "HIDE";
	}
	else if(document.all)
	{
		document.all["lay"].style.visibility = "hidden";
	}
}

if(document.layers)
{
	document.write('<LAYER visibility="hide" bgcolor="#ffc"></LAYER>');
}
else if(document.all)
{
	document.write('<DIV id="lay" style="color:#000;border:2px solid #886;position:absolute;background-color:#ffc"></DIV>');
}

hide();

//-->
</script>
long;

mapping HI_TABLE_16 = 	
([
30		:	"<font color=999999>",
31		:	"<font color=ff0000>",
32		:	"<font color=00ff00>",
33		:	"<font color=ffff00>",
34		:	"<font color=0000ff>",
35		:	"<font color=ff00ff>",
36		:	"<font color=99ffff>",
37		:	"<font color=ffffff>",
]);

mapping LO_TABLE_16 =
([
30		:	"<font color=999999>",
31		:	"<font color=991111>",
32		:	"<font color=119911>",
33		:	"<font color=999911>",
34		:	"<font color=111199>",
35		:	"<font color=991199>",
36		:	"<font color=009999>",
37		:	"<font color=999999>",	
]);

mapping ANSI_TO_16;

string translate_to_html(string content)
{
	// 替代字元
	content = replace_string(content, "  ", "&nbsp;&nbsp;&nbsp;&nbsp;");
	content = replace_string(content,"□","█");
	content = replace_string(content,"Γ","Ｔ");
	content = replace_string(content,"ω","ｗ");
	content = replace_string(content, "\n", "<br>\n");

	foreach(string color, string b16 in ANSI_TO_16)
		content = replace_string(content, color, b16);
	
	content = remove_ansi(content);
	
	return content;
}

string replace_lt_gt(string content)
{
	content = replace_string(content, ">", " &gt; ");
	content = replace_string(content, "<", " &lt; ");
	
	return content;
}

void make_html(string content, mapping index)
{
	string text;
	string bgcolor = index["bgcolor"] || "white";
	int fontsize = index["fontsize"] || 12;
	string fontface = index["fontface"] || "新細明體";
	string filename = index["filename"] || "/www/tmp.html";
	string title = index["title"] || "無標題";
	int refresh = index["refresh"];
	

	text = bgcolor == "black" ? BLACK_HEADER : WHITE_HEADER;

	
	text += "<div style='font-size:"+fontsize+"px;font-family:"+fontface+";'><nobr>";
	text += translate_to_html(content);
	text += "</div>";
	text += bgcolor == "black" ? BLACK_FOOTER : WHITE_FOOTER;
	text += JAVA_SCRIPT;
	text = terminal_colour(text, ([
		"TITLE"		:	title,
		"REFRESH"	:	refresh?("<meta http-equiv='refresh' content='"+refresh+"'; url='"+filename+"'>"):"",
		"UPDATETIME"	:	TIME_D->replace_ctime(time()),
		"YEAR"		:	TIME_D->query_realtime_array()[YEAR]+"",
	]));
	write_file(filename, text, 1);
}

void create_utf8_map()
{
	int x, y;
	array worldmapdata;
	int maptype;
	int no_row;
	string text = "";
	string banner;
	string msg;
	string showname;
	int img;

	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px double #555;width:600;'>";
	for(y=20;y>=-20;y--)
	{
		no_row = 1;

		for(x=-15;x<=15;x++)
			if( MAP_D->query_world_map(x, y) )
				no_row = 0;
				
		if( no_row ) continue;				
				
		text += "<tr height=20>";
		
		for(x=-15;x<=15;x++)
		{
			worldmapdata = MAP_D->query_world_map(x, y);
			
			if( !worldmapdata )
				text += "<td width=20 bgcolor=#ffffff><?php echo iconv('Big5', 'UTF-8', '　'); ?></td>";
			else if( CITY_D->city_exist(worldmapdata[0], worldmapdata[1]) )
			{
				banner = CITY_D->query_city_info(worldmapdata[0], "banner");
				
				msg = "";
				img = 0;

				if( banner )
				{
					msg += "<center><img src="+replace_lt_gt(banner)+" height=64 width=64 /></center><br />";
					showname = "<img src='"+replace_lt_gt(banner)+"' height=20 width=20 />";
					img = 1;
				}
				else if( CITY_D->query_city_info(worldmapdata[0], "fallen") )
					showname = "<font size=3 color=555577><?php echo iconv('Big5', 'UTF-8', '█'); ?>";
				else
					showname = "<font color=ffffff style='font-size:8pt;'><?php echo iconv('Big5', 'UTF-8', '"+remove_ansi(CITY_D->query_city_name(worldmapdata[0]))[0..1]+"'); ?>";

				msg += replace_lt_gt(remove_ansi(CITY_D->query_city_idname(worldmapdata[0], worldmapdata[1])));
				msg += "<br />市長："+(capitalize(CITY_D->query_city_info(worldmapdata[0], "government/mayor"))||"");
				msg += "<br />繁榮："+NUMBER_D->number_symbol(CITY_D->query_section_info(worldmapdata[0], worldmapdata[1], "flourish"));
				
				msg = "<?php echo iconv('Big5', 'UTF-8', '"+msg+"'); ?>";
				
				text += "<td width=20 bgcolor="+(img?"#ffffff style='border:1px solid #ccc;padding:0px;'":"#555577 style='border:1px solid #ccc;'")+"><center><a href='https://www.revivalworld.org/online/rw/map/citymap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank onMouseOver='show(event,\""+msg+"\");' onMouseOut='hide();' >"+showname+"</a></center></td>";
			}
			else if( AREA_D->area_exist(worldmapdata[0], worldmapdata[1]) )
			{
				msg = replace_lt_gt(remove_ansi(AREA_D->query_area_idname(worldmapdata[0], worldmapdata[1])));
				
				maptype = AREA_D->query_area_info(worldmapdata[0], "maptype");
				
				msg = "<?php echo iconv('Big5', 'UTF-8', '"+msg+"'); ?>";
				
				if( maptype == 0 )
					text += "<td width=20 bgcolor=#55aa55><center><a href='https://www.revivalworld.org/online/rw/map/areamap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank onMouseOver='show(event,\""+msg+"\");' onMouseOut='hide();' ><font size=3 color=55aa55><?php echo iconv('Big5', 'UTF-8', '█'); ?></a></td>";
				else if( maptype == 1 )
					text += "<td width=20 bgcolor=#000077><center><a href='https://www.revivalworld.org/online/rw/map/areamap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank onMouseOver='show(event,\""+msg+"\");' onMouseOut='hide();' ><font size=3 color=000077><?php echo iconv('Big5', 'UTF-8', '█'); ?></a></td>";
			}
		}
		
		text += "</tr>";
	}
	
	text += "</table>";
	
	text += JAVA_SCRIPT;
	write_file("/www/map.php", text, 1);
}

void create_map_html()
{
	int x, y, num;
	string map, text = "", msg;
	array worldmapdata;
	int maptype;
	int no_row;
	string banner;
	string showname;
	int img;

	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px solid #555;width:800px;'><tr><td style='background:#fff;font-size:20px;color:#884;'><b>．即時世界地圖</b><hr></td></tr></table>";
	
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px double #555;width:900px;'>";
	for(y=20;y>=-20;y--)
	{
		no_row = 1;

		for(x=-15;x<=15;x++)
			if( MAP_D->query_world_map(x, y) )
				no_row = 0;
				
		if( no_row ) continue;				
				
		text += "<tr height=30>";
		
		for(x=-15;x<=15;x++)
		{
			worldmapdata = MAP_D->query_world_map(x, y);
			
			if( !worldmapdata )
				text += "<td width=30 bgcolor=#ffffff>　</td>";
			else if( CITY_D->city_exist(worldmapdata[0], worldmapdata[1]) )
			{
				banner = CITY_D->query_city_info(worldmapdata[0], "banner");
				
				msg = "";
				img = 0;
				if( banner )
				{
					msg += "<center><img src="+replace_lt_gt(banner)+" height=64 width=64 /></center><br />";
					showname = "<img src='"+replace_lt_gt(banner)+"' height=30 width=30 />";
					img = 1;
				}
				else if( CITY_D->query_city_info(worldmapdata[0], "fallen") )
					showname = "<font size=3 color=555577>█";
				else
					showname = "<font color=ffffff style='font-size:8pt;'>"+remove_ansi(CITY_D->query_city_name(worldmapdata[0]))[0..7];

				msg += replace_lt_gt(remove_ansi(CITY_D->query_city_idname(worldmapdata[0], worldmapdata[1])));
				msg += "<br />市長："+(capitalize(CITY_D->query_city_info(worldmapdata[0], "government/mayor"))||"");
				msg += "<br />繁榮："+NUMBER_D->number_symbol(CITY_D->query_section_info(worldmapdata[0], worldmapdata[1], "flourish"));
				
				text += "<td width=30 bgcolor="+(img?"#ffffff style='border:1px solid #ccc;padding:0px;'":"#555577 style='border:1px solid #ccc;'")+"><center><a href='map/citymap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank onMouseOver='show(event,\""+msg+"\");' onMouseOut='hide();' >"+showname+"</a></center></td>";
			}
			else if( AREA_D->area_exist(worldmapdata[0], worldmapdata[1]) )
			{
				msg = replace_lt_gt(remove_ansi(AREA_D->query_area_idname(worldmapdata[0], worldmapdata[1])));
				
				maptype = AREA_D->query_area_info(worldmapdata[0], "maptype");
				
				if( maptype == 0 )
					text += "<td width=30 bgcolor=#55aa55><center><a href='map/areamap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank onMouseOver='show(event,\""+msg+"\");' onMouseOut='hide();' ><font size=3 color=55aa55>█</a></td>";
				else if( maptype == 1 )
					text += "<td width=30 bgcolor=#000077><center><a href='map/areamap_"+replace_string(worldmapdata[0], " ", "%20")+"_"+worldmapdata[1]+".html' target=_blank onMouseOver='show(event,\""+msg+"\");' onMouseOut='hide();' ><font size=3 color=000077>█</a></td>";
			}
		}
		
		text += "</tr>";
	}
	
	text += "</table>";
	text += "<center><table cellpadding=0 cellspacing=0 align=center style='border:0px double #555;width:800px;'><tr height=30><td width=80 bgcolor=#555577 align=center style='border:1px solid #ccc;'><font color=#ffffff>城市</font></td><td width=10>&nbsp;</td><td width=80 bgcolor=#55aa55 align=center><font color=#ffffff>郊區</font></td><td width=10>&nbsp;</td><td width=80 bgcolor=#000077 align=center><font color=#ffffff>海洋</font></td><td width=530>&nbsp;</td></tr></table></center><br><br><br>";

	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px solid #555;width:800px;'><tr><td style='background:#fff;font-size:20px;color:#448;'><b>．即時城市地圖</b><hr></td></tr></table>";
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:3px double #555;padding:3px;width:800px;'>";
	foreach(map in sort_array(CITY_D->query_cities(), (: sizeof(CITY_D->query_city_info($1, "citizens")) < sizeof(CITY_D->query_city_info($2, "citizens")) ? 1 : -1 :)))
	{
		if( map == "wizard" ) continue;

		num = 0;
		while(CITY_D->city_exist(map, num))
		{
			text += "<tr>";
			text += "<td style='font-size:18px;width:100px;background:#000;'>"+CITY_D->query_city_name(map)+"</b></td>";
			text += "<td style='font-size:18px;width:200px;background:#000;'>"+(CITY_D->query_section_info(map, num, "name")||"第 "+(num+1)+" 衛星都市")+"</td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='/online/rw/map/citymap_"+replace_string(map, " ", "%20")+"_"+num+".html' target=_blank><font color=#ffffff>HTML文字圖</font></a></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=citymap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=10' target=_blank><font color=#ffffff>JAVA圖(小)</font></a></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=citymap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=14' target=_blank><font color=#ffffff>JAVA圖(大)</font></a></td>";
			text += "</tr>";
			num++;
		}
		text += "<tr><td colspan=5 style='border-top:1px solid #111;background:#000;font-size:1px;'>&nbsp;</td></tr>";
	}
	text += "</table><br><br><br>";
	
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:0px solid #555;width:800px;'><tr><td style='background:#fff;font-size:20px;color:#484;'><b>．即時郊區地圖</b><hr></td></tr></table>";
	text += "<table cellpadding=0 cellspacing=0 align=center style='border:3px double #555;padding:3px;width:800px;'>";
	foreach(map in AREA_D->query_areas())
	{
		num = 0;
		while(AREA_D->area_exist(map, num))
		{
			text += "<tr>";
			text += "<td style='font-size:18px;width:100px;background:#000;'>"+AREA_D->query_area_name(map)+"</td>";
			text += "<td style='font-size:18px;width:200px;background:#000;'>"+(AREA_D->query_section_info(map, num, "name")||"第 "+(num+1)+" 分區")+"</td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='/online/rw/map/areamap_"+replace_string(map, " ", "%20")+"_"+num+".html' target=_blank><font color=#ffffff>HTML文字圖</a></font></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=areamap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=10' target=_blank><font color=#ffffff>JAVA圖(小)</font></a></td>";
			text += "<td style='font-size:12px;width:100px;background:#000;'><a href='rwmap.php?map=areamap_"+replace_string(map, " ", "%20")+"_"+num+"_ansi&size=14' target=_blank><font color=#ffffff>JAVA圖(大)</font></a></td>";
			text += "</tr>";
			num++;
		}
		text += "<tr><td colspan=5 style='border-top:1px solid #111;background:#000;font-size:1px;'>&nbsp;</td></tr>";
	}
	text += "</table>";
	
	make_html(text, (["filename":"/www/map.html", "title":"即時地圖", "fontsize":15,"fontface":"細明體"]));
}

void create_stock_html()
{
	string number;
	string enterprise;
	mapping data;
	string text = "<center><table cellpadding=0 cellspacing=0 style='border:3px double #555;padding:10px;'>";
	
	foreach(enterprise, data in ENTERPRISE_D->query_enterprises())
	{
		number = data["enterprise_id_number"];
		text += "<tr><td style='font-size:15px;color:#fff;background:#000;'>企業名稱:"+data["color_id"]+"</td>";
		text += "<td style='font-size:15px;color:#000'>目前股價:"+data["stockvalue"]+"</td></tr>";
		text += "<tr><td colspan=2><a href='/online/rw/mrtg/stock/"+number+"/stock_"+number+".html'><img src='/online/rw/mrtg/stock/"+number+"/stock_"+number+"-day.png'></a></td></tr>";
	}

	text += "</table>";
	make_html(text, (["filename":"/www/stock.html", "title":"即時股票", "fontsize":15, "fontface":"細明體"]));
}

void create_html()
{
	create_map_html();
	//create_utf8_map();
	create_stock_html();
}
	
void create()
{
	int fcolor, bcolor;
	
	ANSI_TO_16 = allocate_mapping(0);

	ANSI_TO_16[ESC+"[0m"] = "<font color=999999>";
	ANSI_TO_16[ESC+"[m"] = "<font color=999999>";

	for(fcolor=30;fcolor<=37;fcolor++)
	{
		ANSI_TO_16[ESC+"["+fcolor+"m"] = LO_TABLE_16[fcolor];
		ANSI_TO_16[ESC+"[0;"+fcolor+"m"] = LO_TABLE_16[fcolor];
		ANSI_TO_16[ESC+"[1;"+fcolor+"m"] = HI_TABLE_16[fcolor];
		ANSI_TO_16[ESC+"["+fcolor+";1m"] = LO_TABLE_16[fcolor];
	
		for(bcolor=41;bcolor<=47;bcolor++)
		{
			ANSI_TO_16[ESC+"["+fcolor+";"+bcolor+"m"] = HI_TABLE_16[fcolor];
			ANSI_TO_16[ESC+"["+bcolor+";"+fcolor+"m"] = HI_TABLE_16[fcolor];

			ANSI_TO_16[ESC+"[0;"+fcolor+";"+bcolor+"m"] = LO_TABLE_16[fcolor];
			ANSI_TO_16[ESC+"[0;"+bcolor+";"+fcolor+"m"] = LO_TABLE_16[fcolor];
			
			ANSI_TO_16[ESC+"[1;"+fcolor+";"+bcolor+"m"] = HI_TABLE_16[fcolor];
			ANSI_TO_16[ESC+"[1;"+bcolor+";"+fcolor+"m"] = HI_TABLE_16[fcolor];
			
			ANSI_TO_16[ESC+"["+fcolor+";1;"+bcolor+"m"] = HI_TABLE_16[fcolor];
			ANSI_TO_16[ESC+"["+bcolor+";1;"+fcolor+"m"] = HI_TABLE_16[fcolor];
			
			ANSI_TO_16[ESC+"["+fcolor+";"+bcolor+";1m"] = HI_TABLE_16[fcolor];
			ANSI_TO_16[ESC+"["+bcolor+";"+fcolor+";1m"] = HI_TABLE_16[fcolor];
		}
	}
	
	create_html();
}

string query_name()
{
	return "HTML 系統(HTML_D)";
}
