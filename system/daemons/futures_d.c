/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : futures_d.c
 * Author : Clode@Revival World
 * Date   : 2009-11-20
 * Note   : 股票精靈
 * Update : 資料來源 : http://tw.futures.finance.yahoo.com/future/index.html
 * 					   http://www.cnyes.com/fc/metal/
 *					   http://www.kitco.com/market/
 -----------------------------------------
 */
#include <ansi.h>
#include <money.h>
#include <daemon.h>
#include <socket.h>
#include <socket_err.h>
#include <futures.h>
#include <socket.h>
#include <socket_err.h>

int socket_fd;

#define DATA		"/data/daemon/futures.o"

#define SOCKET_PORT             8887
#define monitor(x)				CHANNEL_D->channel_broadcast("nch", "FUTURES_D: " + x)

private void listen_callback(int fd);
private void write_callback(int fd);
private void read_callback(int fd, string str);
private void close_callback(int fd);

void check_cover();

mapping orders = allocate_mapping(0);
mapping positions = allocate_mapping(0);
mapping listeners = allocate_mapping(0);

nosave array futures =
({
	// NUM  NAME			MARGIN			VALUE		PROCEDUREFUND	BUYPRICE,	SELLPRICE,	PRICE	VARIATION	UPDATETIME	VALIDTRADETIME
	({ 1,	"台指期", 		10000000, 		75000,		0,				0., 		0., 		0., 	0., 		0,			0 		}),//350000
	({ 2,	"小台指", 		2500000, 		18750,		0,				0., 		0., 		0., 	0., 		0,			0 		}),//87500
	({ 3,	"電子期", 		5000000, 		562500, 	0,				0., 		0., 		0., 	0., 		0,			0 		}),//125000
	({ 4,	"金融期", 		5000000, 		187500, 	0,				0., 		0., 		0., 	0., 		0,			0 		}),//125000
	({ 5,	"芝加哥小麥", 	5000000, 		50000, 		200000,			0., 		0., 		0., 	0., 		0,			5400 	}),
	({ 6,	"芝加哥玉米", 	5000000, 		50000,		150000,			0., 		0., 		0., 	0.,			0,			5400 	}),
	({ 7,	"芝加哥黃豆", 	50000000, 		500000,		1500000,		0., 		0., 		0., 	0., 		0,			5400 	}),
	({ 8,	"紐約白金",	 	50000000, 		500000, 	1500000,		0., 		0., 		0., 	0., 		0,			180 	}),
	({ 9,	"紐約黃金", 	50000000, 		1000000,	2000000,		0., 		0., 		0., 	0., 		0,			180 	}),
	({ 10,	"紐約輕原油", 	50000000, 		7500000, 	1250000,		0., 		0., 		0., 	0., 		0,			5400 	}),
	({ 11,	"紐約銅", 		15000000, 		1000000,	750000,			0., 		0., 		0., 	0., 		0,			5400 	}),
	({ 12,  "紐約鈀",		15000000,		500000,		750000,			0., 		0., 		0.,		0.,			0,			180  	}),
	({ 13,  "紐約銀",		200000000,		100000000,	8000000,		0., 		0., 		0.,		0.,			0,			180  	}),
	//({ 14,  "港恆生指數",	10000000,		200000,		0,				0., 		0., 		0.,		0.,			0,			0  		}),
	//({ 15,  "美道瓊工業",	10000000,		200000,		0,				0., 		0., 		0.,		0.,			0,			0  		}),
	//({ 16,  "英倫敦時報",	10000000,		200000,		0,				0., 		0., 		0.,		0.,			0,			0  		}),
	//({ 17,  "法證商公會",	10000000,		200000,		0,				0., 		0., 		0.,		0.,			0,			0  		}),
	//({ 18,  "德法蘭克福",	10000000,		200000,		0,				0., 		0., 		0.,		0.,			0,			0  		}),
});

nosave string context_taiwan;
nosave string context_newyork;
nosave string context_kitco;

array query_futures()
{
	return futures;
}

int is_settlement_day()
{
		string ctime = TIME_D->replace_ctime(time());
	
		int year = to_int(ctime[6..9]);
		int month = to_int(ctime[0..1]);
		int day = to_int(ctime[3..4]);
	
    switch (year)
    {
		case 2013:
    			switch (month)
    			{
                case 1: return 16 == day;
                case 2: return 20 == day;
                case 3: return 20 == day;
                case 4: return 17 == day;
                case 5: return 15 == day;
                case 6: return 19 == day;
                case 7: return 17 == day;
                case 8: return 21 == day;
                case 9: return 18 == day;
                case 10: return 16 == day;
                case 11: return 20 == day;
                case 12: return 18 == day;
    			}
    			break;
	    case 2012:
    			switch (month)
    			{
                case 1: return 18 == day;
                case 2: return 15 == day;
                case 3: return 21 == day;
                case 4: return 18 == day;
                case 5: return 16 == day;
                case 6: return 20 == day;
                case 7: return 18 == day;
                case 8: return 15 == day;
                case 9: return 19 == day;
                case 10: return 17 == day;
                case 11: return 21 == day;
                case 12: return 19 == day;
    			}
    			break;
    	case 2011:
    			switch (month)
    			{
                case 1: return 19 == day;
                case 2: return 16 == day;
                case 3: return 16 == day;
                case 4: return 20 == day;
                case 5: return 18 == day;
                case 6: return 15 == day;
                case 7: return 20 == day;
                case 8: return 17 == day;
                case 9: return 21 == day;
                case 10: return 19 == day;
                case 11: return 16 == day;
                case 12: return 21 == day;
    			}
    			break;
        case 2010:
            switch (month)
            {
                case 1: return 20 == day;
                case 2: return 22 == day;
                case 3: return 17 == day;
                case 4: return 21 == day;
                case 5: return 19 == day;
                case 6: return 17 == day;
                case 7: return 21 == day;
                case 8: return 18 == day;
                case 9: return 15 == day;
                case 10: return 20 == day;
                case 11: return 17 == day;
                case 12: return 15 == day;
            }
            break;
   }
   
   return 0;
}
   
int is_trading_day(int i)
{
	string ctime = TIME_D->replace_ctime(time());
	
	int year = to_int(ctime[6..9]);
	int month = to_int(ctime[0..1]);
	int day = to_int(ctime[3..4]);
	int result = 0;
	
	if( i>=4 )
	{
		result = 1;
	}
	else if( i>=0 && i<=3 )
	switch (year)
    {
		case 2013:
		  switch (month)
			{
				case 1:
					result |= day >= 2 && day <= 4;
					result |= day >= 7 && day <= 11;
					result |= day >= 14 && day <= 18;
					result |= day >= 21 && day <= 25;
					result |= day >= 28 && day <= 31;
					break;
				case 2:
					result |= day == 1;
					result |= day >= 4 && day <= 6;
					result |= day >= 18 && day <= 23;
					result |= day >= 25 && day <= 27;
					break;
				case 3:
					result |= day == 1;
					result |= day >= 4 && day <= 8;
					result |= day >= 11 && day <= 15;
					result |= day >= 18 && day <= 22;
					result |= day >= 25 && day <= 29;
					break;
				case 4:
					result |= day >= 1 && day <= 3;
					result |= day >= 8 && day <= 12;
					result |= day >= 15 && day <= 19;
					result |= day >= 22 && day <= 26;
					result |= day >= 29 && day <= 30;
					break;
				case 5:
					result |= day >= 2 && day <= 3;
					result |= day >= 6 && day <= 10;
					result |= day >= 13 && day <= 17;
					result |= day >= 20 && day <= 24;
					result |= day >= 27 && day <= 31;
					break;
				case 6:
					result |= day >= 3 && day <= 7;
					result |= day >= 10 && day <= 11;
					result |= day >= 13 && day <= 14;
					result |= day >= 17 && day <= 21;
					result |= day >= 24 && day <= 28;
					break;
				case 7:
					result |= day >= 1 && day <= 5;
					result |= day >= 8 && day <= 12;
					result |= day >= 15 && day <= 19;
					result |= day >= 22 && day <= 26;
					result |= day >= 29 && day <= 31;
					break;
				case 8:
					result |= day >= 1 && day <= 2;
					result |= day >= 5 && day <= 9;
					result |= day >= 12 && day <= 16;
					result |= day >= 19 && day <= 23;
					result |= day >= 26 && day <= 30;
					break;
				case 9:
					result |= day >= 2 && day <= 6;
					result |= day >= 9 && day <= 14;
					result |= day >= 16 && day <= 18;
					result |= day >= 23 && day <= 27;
					result |= day == 30;
					break;
				case 10:
					result |= day >= 1 && day <= 4;
					result |= day >= 7 && day <= 9;
					result |= day == 11;
					result |= day >= 14 && day <= 18;
					result |= day >= 21 && day <= 25;
					result |= day >= 28 && day <= 31;
					break;
				case 11:
					result |= day == 1;
					result |= day >= 4 && day <= 8;
					result |= day >= 11 && day <= 15;
					result |= day >= 18 && day <= 22;
					result |= day >= 25 && day <= 29;
					break;
				case 12:
					result |= day >= 2 && day <= 6;
					result |= day >= 9 && day <= 13;
					result |= day >= 16 && day <= 20;
					result |= day >= 23 && day <= 27;
					result |= day >= 30 && day <= 31;
					break;
			}// switch(month)
			break;
		case 2012:
        	switch (month)
            {
                case 1:
                    result |= day >= 2 && day <= 6;
                    result |= day >= 9 && day <= 13;
                    result |= day >= 16 && day <= 18;
                    result |= day >= 30 && day <= 31;
                    break;
                case 2:
                    result |= day >= 1 && day <= 4;
                    result |= day >= 6 && day <= 10;
                    result |= day >= 13 && day <= 17;
					result |= day >= 20 && day <= 24;
					result |= day == 29;
                    break;
                case 3:
                    result |= day >= 1 && day <= 3;
                    result |= day >= 5 && day <= 9;
                    result |= day >= 12 && day <= 16;
                    result |= day >= 19 && day <= 23;
                    result |= day >= 26 && day <= 30;
                    break;
                case 4:
                    result |= day >= 2 && day <= 3;
                    result |= day >= 5 && day <= 6;
                    result |= day >= 9 && day <= 13;
                    result |= day >= 16 && day <= 20;
                    result |= day >= 23 && day <= 27;
					result |= day == 30;
                    break;
                case 5:
                    result |= day >= 2 && day <= 4;
                    result |= day >= 7 && day <= 11;
                    result |= day >= 14 && day <= 18;
                    result |= day >= 21 && day <= 25;
                    result |= day >= 28 && day <= 31;
                    break;
                case 6:
                    result |= day == 1;
                    result |= day >= 4 && day <= 8;
                    result |= day >= 11 && day <= 15;
                    result |= day >= 18 && day <= 22;
                    result |= day >= 25 && day <= 29;
                    break;
                case 7:
                    result |= day >= 2 && day <= 6;
                    result |= day >= 9 && day <= 13;
                    result |= day >= 16 && day <= 20;
                    result |= day >= 23 && day <= 27;
                    result |= day >= 30 && day <= 31;
                    break;
                case 8:
                    result |= day >= 1 && day <= 3;
                    result |= day >= 6 && day <= 10;
                    result |= day >= 13 && day <= 17;
                    result |= day >= 20 && day <= 24;
                    result |= day >= 27 && day <= 31;
                    break;
                case 9:
                    result |= day >= 3 && day <= 7;
                    result |= day >= 10 && day <= 14;
                    result |= day >= 17 && day <= 21;
                    result |= day >= 24 && day <= 28;
                    break;
                case 10:
                    result |= day >= 1 && day <= 5;
                    result |= day >= 8 && day <= 9;
                    result |= day >= 11 && day <= 12;
                    result |= day >= 15 && day <= 19;
					result |= day >= 22 && day <= 26;
                    result |= day >= 29 && day <= 31;
                    break;
                case 11:
                    result |= day >= 1 && day <= 2;
                    result |= day >= 5 && day <= 9;
                    result |= day >= 12 && day <= 16;
                    result |= day >= 19 && day <= 23;
                    result |= day >= 26 && day <= 30;
                    break;
                case 12:
                    result |= day >= 3 && day <= 7;
                    result |= day >= 10 && day <= 14;
                    result |= day >= 17 && day <= 22;
                    result |= day >= 24 && day <= 28;
                    break;
            }// switch(month)
       		break;
    	case 2010:
        	switch (month)
            {
          		case 1:
        			result |= day >= 4 && day <= 8;
                    result |= day >= 11 && day <= 15;
                    result |= day >= 18 && day <= 22;
                    result |= day >= 25 && day <= 29;
                    break;
                case 2:
                    result |= day >= 1 && day <= 6;
                    result |= day >= 8 && day <= 10;
                    result |= day >= 22 && day <= 26;
                    break;
                case 3:
                    result |= day >= 1 && day <= 5;
                    result |= day >= 8 && day <= 12;
                    result |= day >= 15 && day <= 19;
                    result |= day >= 22 && day <= 26;
                    result |= day >= 29 && day <= 31;
                    break;
                case 4:
                    result |= day >= 1 && day <= 2;
                    result |= day >= 6 && day <= 9;
                    result |= day >= 12 && day <= 16;
                    result |= day >= 19 && day <= 23;
                    result |= day >= 26 && day <= 30;
                    break;
                case 5:
                    result |= day >= 3 && day <= 7;
                    result |= day >= 10 && day <= 14;
                    result |= day >= 17 && day <= 21;
                    result |= day >= 24 && day <= 28;
                    result |= day == 31;
                    break;
                case 6:
                    result |= day >= 1 && day <= 4;
                    result |= day >= 7 && day <= 11;
                    result |= day >= 14 && day <= 15;
                    result |= day >= 17 && day <= 18;
                    result |= day >= 21 && day <= 25;
                    result |= day >= 28 && day <= 30;
                    break;
                case 7:
                    result |= day >= 1 && day <= 2;
                    result |= day >= 5 && day <= 9;
                    result |= day >= 12 && day <= 16;
                    result |= day >= 19 && day <= 23;
                    result |= day >= 26 && day <= 30;
                    break;
                case 8:
                    result |= day >= 2 && day <= 6;
                    result |= day >= 9 && day <= 13;
                    result |= day >= 16 && day <= 20;
                    result |= day >= 23 && day <= 27;
                    result |= day >= 30 && day <= 31;
                    break;
                case 9:
                    result |= day >= 1 && day <= 3;
                    result |= day >= 6 && day <= 10;
                    result |= day >= 13 && day <= 17;
                    result |= day >= 20 && day <= 21;
                    result |= day >= 23 && day <= 24;
                    result |= day >= 27 && day <= 30;
                    break;
                case 10:
                    result |= day == 1;
                    result |= day >= 4 && day <= 8;
                    result |= day >= 11 && day <= 15;
                    result |= day >= 18 && day <= 22;
                    result |= day >= 25 && day <= 29;
                    break;
                case 11:
                    result |= day >= 1 && day <= 5;
                    result |= day >= 8 && day <= 12;
                    result |= day >= 15 && day <= 19;
                    result |= day >= 22 && day <= 26;
                    result |= day >= 29 && day <= 30;
                    break;
                case 12:
                    result |= day >= 1 && day <= 3;
                    result |= day >= 6 && day <= 10;
                    result |= day >= 13 && day <= 17;
                    result |= day >= 20 && day <= 24;
                    result |= day >= 27 && day <= 31;
                    break;
            } // switch(month)
        	break;
        case 2011:
        	switch (month)
            {
                case 1:
                    result |= day >= 3 && day <= 7;
                    result |= day >= 10 && day <= 14;
                    result |= day >= 17 && day <= 21;
                    result |= day >= 24 && day <= 28;
                    break;
                case 2:
                    result |= day >= 8 && day <= 11;
                    result |= day >= 14 && day <= 18;
                    result |= day >= 21 && day <= 25;
                    break;
                case 3:
                    result |= day >= 1 && day <= 4;
                    result |= day >= 7 && day <= 11;
                    result |= day >= 14 && day <= 18;
                    result |= day >= 21 && day <= 25;
                    result |= day >= 28 && day <= 31;
                    break;
                case 4:
                    result |= day == 1;
                    result |= day >= 6 && day <= 8;
                    result |= day >= 11 && day <= 15;
                    result |= day >= 18 && day <= 22;
                    result |= day >= 25 && day <= 29;
                    break;
                case 5:
                    result |= day >= 3 && day <= 6;
                    result |= day >= 9 && day <= 13;
                    result |= day >= 16 && day <= 20;
                    result |= day >= 23 && day <= 27;
                    result |= day >= 30 && day <= 31;
                    break;
                case 6:
                    result |= day >= 1 && day <= 3;
                    result |= day >= 7 && day <= 10;
                    result |= day >= 13 && day <= 17;
                    result |= day >= 20 && day <= 24;
                    result |= day >= 27 && day <= 30;
                    break;
                case 7:
                    result |= day == 1;
                    result |= day >= 4 && day <= 8;
                    result |= day >= 11 && day <= 15;
                    result |= day >= 18 && day <= 22;
                    result |= day >= 25 && day <= 29;
                    break;
                case 8:
                    result |= day >= 1 && day <= 5;
                    result |= day >= 8 && day <= 12;
                    result |= day >= 15 && day <= 19;
                    result |= day >= 22 && day <= 26;
                    result |= day >= 29 && day <= 31;
                    break;
                case 9:
                    result |= day >= 1 && day <= 2;
                    result |= day >= 5 && day <= 9;
                    result |= day >= 13 && day <= 16;
                    result |= day >= 19 && day <= 23;
                    result |= day >= 26 && day <= 30;
                    break;
                case 10:
                    result |= day >= 3 && day <= 7;
                    result |= day >= 11 && day <= 14;
                    result |= day >= 17 && day <= 21;
                    result |= day >= 24 && day <= 28;
                    result |= day == 31;
                    break;
                case 11:
                    result |= day >= 1 && day <= 4;
                    result |= day >= 7 && day <= 11;
                    result |= day >= 14 && day <= 18;
                    result |= day >= 21 && day <= 25;
                    result |= day >= 28 && day <= 30;
                    break;
                case 12:
                    result |= day >= 1 && day <= 2;
                    result |= day >= 5 && day <= 9;
                    result |= day >= 12 && day <= 16;
                    result |= day >= 19 && day <= 23;
                    result |= day >= 26 && day <= 30;
                    break;
            }// switch(month)
       		break;
        default:
        	
        	break;
	}// switch(year)
	
	return result;
}

int is_trading_time(int i)
{
	if( !is_trading_day(i) ) return 0;	
	
	if( i>=4 )
		return 1;
	else if( i>=0 && i<=3 )
	{
		int hour = to_int(ctime()[11..12]);
		int min = to_int(ctime()[14..15]);
		
		if( hour == 8 )
			return min >= 45;
		else
			return hour >= 9 && hour <= 14;
	}
}

varargs mapping query_positions(string id, int num)
{
	if( undefinedp(id) )
		return positions;	
	else if( undefinedp(num) )
		return positions[id] || allocate_mapping(0);
	else if( mapp(positions[id]) )
		return positions[id][num] || allocate_mapping(0);
	else
		return allocate_mapping(0);
}

int save()
{
	return save_object(DATA);
}  


void start_listen(string id, int num)
{
	if( !arrayp(listeners[id]) )
		listeners[id] = allocate(0);
	
	listeners[id] |= ({ num });
	
	save();
}

varargs void stop_listen(string id, int num)
{
	if( undefinedp(num) )
	{
		map_delete(listeners, id);
		save();
	}
	else if( arrayp(listeners[id]) )
	{
		listeners[id] -= ({ num });
	
		if( !sizeof(listeners[id]) )
			map_delete(listeners, id);
		
		save();
	}
}

void set_positions(string id, int num, mapping data)
{
	if( undefinedp(positions[id]) )
		positions[id] = allocate_mapping(0);	
	
	positions[id][num] = data;
	
	if( data["amount"] == 0 )
		map_delete(positions[id], num);
		
	if( sizeof(positions[id]) == 0 )
		map_delete(positions, id);
		
	save();
}

varargs void remove_positions(string id, int num)
{
	if( undefinedp(num) )
		map_delete(positions, id);	
	else
		map_delete(positions[id], num);
		
	save();
}

nosave string syntax_taiwan = @SYNTAX
<!-- file:include/future_b.html -->
%*s台期指%*d</a></td>%*s-->  %f</font></td>%*s<font color=red><!--▲69.97--></font>%s</font></td>
%*s小台指%*d</a></td>%*s-->  %f</font></td>%*s<font color=red><!--▲69.97--></font>%s</font></td>
%*s電子期%*d</a></td>%*s-->  %f</font></td>%*s<font color=red><!--▲69.97--></font>%s</font></td>
%*s金融期%*d</a></td>%*s-->  %f</font></td>%*s<font color=red><!--▲69.97--></font>%s</font></td>
%*s
      </table>
SYNTAX;

nosave string syntax_newyork =
"<td><a href=\"/futures/flashchart/WCON.html\">%*s</a></td><td class=\"%*s\">%f</td><td class=\"%*s\">%f</td>%*s"
"<td><a href=\"/futures/flashchart/CCON.html\">%*s</a></td><td class=\"%*s\">%f</td><td class=\"%*s\">%f</td>%*s"
"<td><a href=\"/futures/flashchart/SCON.html\">%*s</a></td><td class=\"%*s\">%f</td><td class=\"%*s\">%f</td>%*s"
"<td><a href=\"/futures/flashchart/HGCON.html\">%*s</a></td><td class=\"%*s\">%f</td><td class=\"%*s\">%f</td>%*s"
"<td><a href=\"/futures/flashchart/CLCON.html\">%*s</a></td><td class=\"%*s\">%f</td><td class=\"%*s\">%f</td>%*s</table>"
;

/*
nosave string syntax_kitco =
"<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#000000\" width=\"540\" align=\"center\">"
"%*sGOLD%*s<td %*s>%*d/%*d/%*d</td>%*s<td %*s>%*d:%*d</td>%*s<td %*s>%f</td>%*s<td %*s>%*f</td>%*s<td %*s>%s</td>"
"%*sSILVER%*s<td %*s>%*d/%*d/%*d</td>%*s<td %*s>%*d:%*d</td>%*s<td %*s>%f</td>%*s<td %*s>%*f</td>%*s<td %*s>%s</td>"
"%*sPLATINUM%*s<td %*s>%*d/%*d/%*d</td>%*s<td %*s>%*d:%*d</td>%*s<td %*s>%f</td>%*s<td %*s>%*f</td>%*s<td %*s>%s</td>"
"%*sPALLADIUM%*s<td %*s>%*d/%*d/%*d</td>%*s<td %*s>%*d:%*d</td>%*s<td %*s>%f</td>%*s<td %*s>%*f</td>%*s<td %*s>%s</td>%*s</table>"
;*/

nosave string syntax_kitco =
"<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#000000\" width=\"540\" align=\"center\">"
"%*sGOLD%*s<td %*s>%*s</td>%*s<td %*s>%*d:%*d</td>%*s<td %*s>%f</td>%*s<td %*s>%*f</td>%*s<td %*s>%s</td>"
"%*sSILVER%*s<td %*s>%*s</td>%*s<td %*s>%*d:%*d</td>%*s<td %*s>%f</td>%*s<td %*s>%*f</td>%*s<td %*s>%s</td>"
"%*sPLATINUM%*s<td %*s>%*s</td>%*s<td %*s>%*d:%*d</td>%*s<td %*s>%f</td>%*s<td %*s>%*f</td>%*s<td %*s>%s</td>"
"%*sPALLADIUM%*s<td %*s>%*s</td>%*s<td %*s>%*d:%*d</td>%*s<td %*s>%f</td>%*s<td %*s>%*f</td>%*s<td %*s>%s</td>%*s</table>"
;

int get_world_total_gain()
{
	int totalgain = 0;
	
	foreach( array data in TOP_D->query_top("futures") )
	{
		totalgain += to_int(data[1]);
	}
	
	return totalgain;
}

// 解析資料
void parse_data_taiwan()
{
	float* prices = allocate(4);
	string* variations = allocate(4);
	
	context_taiwan = context_taiwan[strsrch(context_taiwan, "<!-- file:include/future_b.html -->")..];
	context_taiwan = context_taiwan[0..strsrch(context_taiwan, "</table>")+8];
	
	if( sscanf(context_taiwan, syntax_taiwan, 
		prices[0], variations[0],
		prices[1], variations[1],
		prices[2], variations[2],
		prices[3], variations[3]) 
		!= 25 )
	{
		CHANNEL_D->channel_broadcast("sys", "無法解析臺灣期貨資料");
		CHANNEL_D->channel_broadcast("nch", context_taiwan);
	}
	else
	{
		float variation;
		string direction;
		
		if( abs(prices[0] - prices[1]) > 10 )
		{
			CHANNEL_D->channel_broadcast("sys", "臺灣期貨大台("+to_int(prices[0])+")小台("+to_int(prices[1])+")差價過大，放棄資料更新");			
			return;
		}

		for(int i=0;i<=3;++i)
		{
			futures[i][FUTURES_PRICE] = futures[i][FUTURES_BUYPRICE] = futures[i][FUTURES_SELLPRICE] = prices[i];
			
			if( variations[i] == "0" )
			{
				futures[i][FUTURES_VARIATION] = 0.;
			}
			else if( sscanf(variations[i], "<font color=%*s>%s%f</font>", direction, variation) == 3 && abs(variation) < prices[i] )
			{
				if( direction == "△" )
					futures[i][FUTURES_VARIATION] = variation;
				else if( direction == "▽" )
					futures[i][FUTURES_VARIATION] = -variation;
			}

			futures[i][FUTURES_UPDATETIME] = time();
			
			log_file("/daemon/futures-"+i, futures[i][FUTURES_NAME]+" "+futures[i][FUTURES_PRICE]);
		}
		
		CHANNEL_D->channel_broadcast("nch", "成功解析臺灣期貨資料");
		
		check_cover();
	}
}

// 解析資料
void parse_data_newyork()
{
	int test;

	context_newyork = replace_string(context_newyork, ",", "");
	context_newyork = context_newyork[strsrch(context_newyork, "<td><a href=\"/futures/flashchart/WCON.html\">")..];
	context_newyork = context_newyork[0..strsrch(context_newyork, "</table>")+8];
	
	test = sscanf(context_newyork, syntax_newyork, 
		futures[4][FUTURES_PRICE], futures[4][FUTURES_VARIATION], 
		futures[5][FUTURES_PRICE], futures[5][FUTURES_VARIATION], 
		futures[6][FUTURES_PRICE], futures[6][FUTURES_VARIATION],
		futures[10][FUTURES_PRICE], futures[10][FUTURES_VARIATION],
		futures[9][FUTURES_PRICE], futures[9][FUTURES_VARIATION] 
		);
		
	if( test != 30	)
	{
		foreach(int i in ({ 4, 5, 6, 9, 10 }))
			futures[i][FUTURES_UPDATETIME] = 0;
		
		CHANNEL_D->channel_broadcast("sys", "無法解析紐約期貨資料("+test+")");
		CHANNEL_D->channel_broadcast("nch", context_newyork);
	}	
	else
	{
		foreach(int i in ({ 4, 5, 6, 9, 10 }))
		{
			futures[i][FUTURES_UPDATETIME] = time();
			
			futures[i][FUTURES_BUYPRICE] = futures[i][FUTURES_SELLPRICE] = futures[i][FUTURES_PRICE];
			
			log_file("/daemon/futures-"+i, futures[i][FUTURES_NAME]+" "+futures[i][FUTURES_PRICE]);
		}
			
		CHANNEL_D->channel_broadcast("nch", "成功解析紐約期貨資料");
		
		check_cover();
	}
}

// 解析資料
void parse_data_kitco()
{
	int sscanfidx;

	context_kitco = context_kitco[strsrch(context_kitco, "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" bgcolor=\"#000000\" width=\"540\" align=\"center\">")..];
	context_kitco = context_kitco[0..strsrch(context_kitco, "</table>")+8];
	
	sscanfidx = sscanf(context_kitco, syntax_kitco, 
		futures[8][FUTURES_PRICE], futures[8][FUTURES_VARIATION], 
		futures[12][FUTURES_PRICE], futures[12][FUTURES_VARIATION], 
		futures[7][FUTURES_PRICE], futures[7][FUTURES_VARIATION], 
		futures[11][FUTURES_PRICE], futures[11][FUTURES_VARIATION]);
		
	if( sscanfidx != 69 )
	{
		foreach(int i in ({ 8, 12, 7, 11 }))
			futures[i][FUTURES_UPDATETIME] = 0;

		CHANNEL_D->channel_broadcast("sys", "無法解析 Kitco 期貨資料(" + sscanfidx+")");
		CHANNEL_D->channel_broadcast("nch", context_kitco);
	}	
	else
	{	
		foreach(int i in ({ 8, 12, 7, 11 }))
		{
			if( sscanf(futures[i][FUTURES_VARIATION], "<p class=%*s>%f</p>", futures[i][FUTURES_VARIATION]) != 2 )
				futures[i][FUTURES_VARIATION] = 0.;
				
			futures[i][FUTURES_UPDATETIME] = time();
			
			futures[i][FUTURES_BUYPRICE] = futures[i][FUTURES_SELLPRICE] = futures[i][FUTURES_PRICE];
			
			log_file("/daemon/futures-"+i, futures[i][FUTURES_NAME]+" "+futures[i][FUTURES_PRICE]);
		}
		CHANNEL_D->channel_broadcast("nch", "成功解析 Kitco 期貨資料");
		
		check_cover();
	}	
}

void get_futures_data()
{                       
	int err, fd;  

	context_newyork = "";
	context_kitco = "";

	/* 開啟網路連結 */

	fd = socket_create(STREAM, "read_callback_newyork", "close_socket_newyork");
	
	if ( fd >= 0 )
	{
		err = socket_connect(fd, "211.72.252.30 80", "read_callback_newyork", "write_callback_newyork");

		if( err != EESUCCESS )
			CHANNEL_D->channel_broadcast("sys", "無法連線紐約期貨資料網頁。");
	}
	
	fd = socket_create(STREAM, "read_callback_kitco", "close_socket_kitco");
	
	if ( fd >= 0 )
	{
		err = socket_connect(fd, "69.17.152.33 80", "read_callback_kitco", "write_callback_kitco");

		if( err != EESUCCESS )
			CHANNEL_D->channel_broadcast("sys", "無法連線 Kitco 期貨資料網頁。");
	}

	call_out((: get_futures_data :), range_random(60, 120));
}

// called by TIME_D
void refresh_futures_data()
{
	CHANNEL_D->channel_broadcast("nch", "開始更新期貨資料。");
	get_futures_data();
}

int close_socket_taiwan(int fd)
{
	socket_close(fd);

	parse_data_taiwan();
}

void read_callback_taiwan(int fd, mixed message)
{
	/* 把每次取得的資料存進 context 裡 */
	context_taiwan += message;
}

void write_callback_taiwan(int fd)
{
	socket_write(fd, "GET /future/index.html\r\n");
}

int close_socket_newyork(int fd)
{
	socket_close(fd);

	parse_data_newyork();
}

void read_callback_newyork(int fd, mixed message)
{
	/* 把每次取得的資料存進 context 裡 */
	context_newyork += message;
}

void write_callback_newyork(int fd)
{
	socket_write(fd, "GET /fc/metal/\r\n");
}

int close_socket_kitco(int fd)
{
	socket_close(fd);

	parse_data_kitco();
}

void read_callback_kitco(int fd, mixed message)
{
	/* 把每次取得的資料存進 context 裡 */
	context_kitco += message;
}

void write_callback_kitco(int fd)
{
	socket_write(fd, "GET /market/?sitetype=fullsite HTTP/1.1\r\nHOST:\r\n\r\n");
}

// 檢查平倉
void check_cover()
{
	array fdata;

	foreach(string id, mapping position in positions)	
	{
		foreach(int num, mapping data in position)
		{	
			fdata = futures[num];
			
			if( fdata[FUTURES_UPDATETIME] < time() - 180 )
				continue;
				
			if( data["amount"] > 0 )
			{
				// 斷頭砍倉
				if( (data["price"] - fdata[FUTURES_PRICE]) * fdata[FUTURES_VALUE] > fdata[FUTURES_MARGIN] )
				{
					object me = load_user(id);
					
					CHANNEL_D->channel_broadcast("stock", me->query_idname()+"持有價格 "HIY+sprintf("%.2f", data["price"])+NOR"(現價 "HIY+sprintf("%.2f", fdata[FUTURES_PRICE])+NOR")之「"HIW+fdata[FUTURES_NAME]+NOR"」期貨共 "HIY+data["amount"]+NOR" 口多單遭到"HIR"斷頭砍倉"NOR"，損益 "HIY+money(DEFAULT_MONEY_UNIT, -data["amount"]*(fdata[FUTURES_MARGIN]+fdata[FUTURES_PROCEDUREFUND]))+NOR"("NOR GRN"-"HIG"100%"NOR")");
					
					addn("total_futures_gain", -data["amount"]*(fdata[FUTURES_MARGIN]+fdata[FUTURES_PROCEDUREFUND]), me);
					
					TOP_D->update_top_futures(me);
					
					remove_positions(id, num);
					
					me->save();
					
					if( !userp(me) )
						destruct(me);
				}
			}
			else
			{
				// 斷頭砍倉
				if( (fdata[FUTURES_PRICE] - data["price"]) * fdata[FUTURES_VALUE] > fdata[FUTURES_MARGIN] )
				{
					object me = load_user(id);
					
					CHANNEL_D->channel_broadcast("stock", me->query_idname()+"持有價格 "HIY+sprintf("%.2f", data["price"])+NOR"(現價 "HIY+sprintf("%.2f", fdata[FUTURES_PRICE])+NOR")之「"HIW+fdata[FUTURES_NAME]+NOR"」期貨共 "HIY+(-data["amount"])+NOR" 口空單遭到"HIR"斷頭砍倉"NOR"，損益 "HIY+money(DEFAULT_MONEY_UNIT, -data["amount"]*(fdata[FUTURES_MARGIN]+fdata[FUTURES_PROCEDUREFUND]))+NOR"("NOR GRN"-"HIG"100%"NOR")");
					
					addn("total_futures_gain", data["amount"]*(fdata[FUTURES_MARGIN]+fdata[FUTURES_PROCEDUREFUND]), me);
					
					TOP_D->update_top_futures(me);
					
					remove_positions(id, num);
					
					me->save();
					
					if( !userp(me) )
						destruct(me);
				}
			}
		}
	}
}

// 檢查台指結算平倉
void check_fitx_settlement()
{
	array fdata;
	int margin, gain;
	
	foreach(string id, mapping position in positions)	
	{
		foreach(int num, mapping data in position)
		{	
			if( num > 3 ) continue;

			fdata = futures[num];
					
			// 持有多單	強制平倉
			if( data["amount"] > 0 )
			{
				object me = load_user(id);
				
				gain = data["amount"] * ( to_int(fdata[FUTURES_PRICE]*100) - to_int(data["price"]*100) ) * fdata[FUTURES_VALUE] / 100;
				margin = data["amount"] * fdata[FUTURES_MARGIN];
				
				CHANNEL_D->channel_broadcast("stock", me->query_idname()+"持有價格 "HIY+sprintf("%.2f", data["price"])+NOR"(現價 "HIY+sprintf("%.2f", fdata[FUTURES_PRICE])+NOR")之「"HIW+fdata[FUTURES_NAME]+NOR"」期貨共 "HIY+data["amount"]+NOR" 口多單因結算日強制回補平倉，損益 "HIY+money(DEFAULT_MONEY_UNIT, gain));

				addn("total_futures_gain", gain, me);
				
				me->earn_money(DEFAULT_MONEY_UNIT, margin + gain);
				
				TOP_D->update_top_futures(me);
				
				remove_positions(id, num);
				
				me->save();
				
				if( !userp(me) )
					destruct(me);
			}
			// 持有空單 強制平倉
			else if( data["amount"] < 0 )
			{
				// 斷頭砍倉
				object me = load_user(id);
				
				gain = -data["amount"] * ( to_int(data["price"]*100) - to_int(fdata[FUTURES_PRICE]*100) ) * fdata[FUTURES_VALUE] / 100;
				margin = -data["amount"] * fdata[FUTURES_MARGIN];
				
				CHANNEL_D->channel_broadcast("stock", me->query_idname()+"持有價格 "HIY+sprintf("%.2f", data["price"])+NOR"(現價 "HIY+sprintf("%.2f", fdata[FUTURES_PRICE])+NOR")之「"HIW+fdata[FUTURES_NAME]+NOR"」期貨共 "HIY+(-data["amount"])+NOR" 口空單因結算日強制回補平倉，損益 "HIY+money(DEFAULT_MONEY_UNIT, gain));
				
				addn("total_futures_gain", gain, me);
				
				me->earn_money(DEFAULT_MONEY_UNIT, margin + gain);
				
				TOP_D->update_top_futures(me);
				
				remove_positions(id, num);
				
				me->save();
				
				if( !userp(me) )
					destruct(me);
			}
		}
	}
}

void process_per_month()
{
	object user;
	int total_futures_gain;
	
	foreach(string id in all_player_ids())
	{
		user = load_user(id);

		total_futures_gain = query("total_futures_gain", user);
		
		if( total_futures_gain != 0 )
		{
			addn("total_futures_gain_history", total_futures_gain, user);
			delete("total_futures_gain", user);
			user->save();
		}
		
		if( !userp(user) ) destruct(user);
	}
	
	TOP_D->delete_top("futures");

	CHANNEL_D->channel_broadcast("news", "完成當月期貨損益結算");
}

void create()
{
	int res;
	int err, fd;  
		
	if( file_size(DATA) < 1 )
		save();
	
	restore_object(DATA);
	
	/* 建立 socket */
	socket_fd = socket_create(STREAM, (: read_callback :), (: close_callback :));
	if( socket_fd < 0 )
	{
		monitor("socket_create: " + socket_error(socket_fd)); 
		return;
	}

	/* 設定 socket 連結埠 */
	res = socket_bind(socket_fd, SOCKET_PORT);
	if( res < 0 )
	{
		monitor("socket_bind: " + socket_error(res)); 
		socket_close(socket_fd);
		return;
	}

	/* 開始等待 socket 連結 */
	res = socket_listen(socket_fd, (: listen_callback :));
	if( res < 0 )
	{
		monitor("socket_listen: " + socket_error(res)); 
		socket_close(socket_fd);
		return;
	}
	
	context_taiwan = "";
	fd = socket_create(STREAM, "read_callback_taiwan", "close_socket_taiwan");

	if ( fd >= 0 )
	{
		err = socket_connect(fd, "119.160.245.26 80", "read_callback_taiwan", "write_callback_taiwan");

		if( err != EESUCCESS )
			CHANNEL_D->channel_broadcast("sys", "無法連線臺灣期貨資料網頁。");
	}
	
	get_futures_data();
	
	set_heart_beat(1);
}   

/* socket 傳送資料完成通知 */
private void listen_callback(int fd) 
{
	int res_fd;

	/* 接受連結 */
	res_fd = socket_accept(fd, (: read_callback :), (: write_callback :));
	if( res_fd < 0 )
	{
		monitor("listen_callback: " + socket_error(res_fd));
		return;
	}

	monitor("listen_callback: "+socket_address(res_fd)+" is connecting");
}

/* socket 傳送資料完成通知 */
private void write_callback(int fd) 
{

}

/* socket 接收資料通知 */
private void read_callback(int fd, string str)
{
	int findex;
	float buyprice, sellprice, price, variation;
	string s;
	
	foreach(s in explode(str, "\n"))
	{
		if( sizeof(s) < 2 ) continue;

		switch(s[0])
		{
			case ':': CHANNEL_D->channel_broadcast("stock", YEL"雲端自動交易"NOR" - " + s[1..]); continue;
			case 'A': findex = 0; break;
			case 'B': findex = 1; break;
			case 'C': findex = 2; break;
			case 'D': findex = 3; break;
			default: continue;
		}
		
		if( sscanf(s[1..], "|%f|%f|%f|%f|", buyprice, sellprice, price, variation) == 4 && buyprice > 0 && sellprice > 0 && price > 0 )
		{			
			futures[findex][FUTURES_BUYPRICE] = buyprice;
			futures[findex][FUTURES_SELLPRICE] = sellprice;
			futures[findex][FUTURES_PRICE] = price;
			futures[findex][FUTURES_VARIATION] = variation;
			
			futures[findex][FUTURES_UPDATETIME] = time();
		}
	}
}

nosave int heart_beat_tick = 0;

private void heart_beat()
{
	string id;
	int *arr;
	object ob;
	
	heart_beat_tick++;
	
	for(int findex = 0;findex < sizeof(futures);++findex)
	{
		foreach(id, arr in listeners)
		{
			if( member_array(findex, arr) != -1 )
			{
				if( objectp(ob = find_player(id)) )
				{
					if( query("futures_listen_tick/"+findex, ob) > 0 )
					{
						if( !(heart_beat_tick % query("futures_listen_tick/"+findex, ob)) )
						{
							tell(ob, sprintf("%s 買價 %.2f 賣價 %.2f 成交 %.2f 漲跌 %.2f\n", 
								futures[findex][FUTURES_NAME],
								futures[findex][FUTURES_BUYPRICE],
								futures[findex][FUTURES_SELLPRICE],
								futures[findex][FUTURES_PRICE],
								futures[findex][FUTURES_VARIATION] ));
						}
					}
				}
			}	
		}
	}
	
	if( !(heart_beat_tick % 5) )
		check_cover();
		
	if( !(heart_beat_tick % 3600) && is_settlement_day() && TIME_D->replace_ctime(time())[11..12] == "15" )
		check_fitx_settlement();
}

/* socket 關閉通知 */
private void close_callback(int fd)
{

}

int remove()
{
	return save();
}

string query_name()
{
	return "期貨系統(FUTURES_D)";
}

