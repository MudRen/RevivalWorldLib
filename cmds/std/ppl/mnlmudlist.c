/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mnlmudlist.c
 * Author : Clode@RevivalWorld
 * Date   : 2004-06-18
 * Note   : ��ʫإ� Mudlist �C����O�C
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>
#include <time.h>

inherit COMMAND;

string help = @HELP
    �C�X�O�W�D�n Mud �Y�ɺʴ��C��C
    �t�i�� http://www.revivalworld.org/mud/taiwanmudlist �H�ɬd�\�Y�ɸ��

HELP;

#define HOR 	24
#define VER	14

string *listcolor = ({ NOR, HIW });

// �����ɮɥN code �����ޥ�, �g���u�t -.- ���L�]�i�o�g�s���F
string online_user_record()
{
	int *realtime = TIME_D->query_realtime_array();
	int i, j, hour = realtime[HOUR];
	string str,b="�i";
	string s="                                                                                                            ";
	string *list=({s,s,s,s,s,s,s,s,s,s,s,s,s,s});
	mapping record;	
	float w, max_num=0., min_num=10000.;
	string time = "    0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23\n";
	int *num= allocate(HOR);

	record = query("mnlmudlist/maxusers/hour/", SYSDATABASE_D->query_ob());

	if( !mapp(record) ) return "";

	foreach( string hourstr, int sizeofusers in record  )
	{
		num[to_int(hourstr)] = sizeofusers;
		if( sizeofusers > max_num ) max_num = sizeofusers;
		if( sizeofusers < min_num ) min_num = sizeofusers;
	}

	w = ((max_num-min_num)/VER);
	if( w<=0 )
		w=0.001;
	str = NOR WHT"��"HIW"�O�W Mud ���a�W�u�ɶ��Ͷչ�\n"NOR;
	for(i=0;i<VER;i++)
	{
		for(j=0;j<HOR;j++)
		{
			if((num[j]-min_num)/w > 11-i)
			{
				if(list[i][j*4..j*4+3] != " "+b+" ")
					list[i][j*4..j*4+3] = sprintf("%|4d",num[j]);
				if( i < VER-1 )
					list[i+1][j*4..j*4+3]=" "+b+" ";
			}
		}
		str +=NOR WHT"��"NOR+list[i]+"\n";
	}

	str += NOR WHT"�|�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w��\n"NOR;
	str += HIC+time[0..(hour*4+1)]+HIM+"["+HIR+sprintf("%2d",hour)+HIM+"]"+HIC+time[(hour*4+6)..<0]+NOR+"\n\n";

	return str;
}

int mnlmudlist_sort(mapping mnlmudlist, string mud1, string mud2)
{
	string mud1_chinese = mnlmudlist[mud1]["MUD_CHINESE_NAME"];
	string mud2_chinese = mnlmudlist[mud2]["MUD_CHINESE_NAME"];
	string mud1_english = mnlmudlist[mud1]["MUD_ENGLISH_NAME"];
	string mud2_english = mnlmudlist[mud2]["MUD_ENGLISH_NAME"];

	if( sizeof(mud1_chinese) != sizeof(mud2_chinese) )
		return sizeof(mud1_chinese) > sizeof(mud2_chinese) ? 1 : -1;
	
	return strcmp(mud1_chinese || mud1_english, mud2_chinese || mud2_english);
}

varargs string mnlmudlist_list(int flag)
{
	string msg, ip, mudname;
	int port, allmuders, time;
	mapping mnlmudlist, data;
	string *mudnamesort;

	mnlmudlist = MNLMUDLIST_D->query_mnlmudlist();

	msg = HIM"�O�W Mud �Y�ɺʴ��C��\n"NOR;

	msg +=HIM"����W�� - �^��W��                        ������m                 ��   ���A ���a �̫�s�u�ɶ�\n"NOR;
	msg +="�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";

	mudnamesort = sort_array(keys(mnlmudlist)-({0}), (: mnlmudlist_sort($(mnlmudlist), $1, $2) :)) ;

	time = time();

	foreach( string ipport in mudnamesort )
	{
		data = mnlmudlist[ipport];
		sscanf(ipport, "%s %d", ip, port);

		if( data["MUD_CHINESE_NAME"] && data["MUD_ENGLISH_NAME"] )
			mudname = data["MUD_CHINESE_NAME"]+" - "+data["MUD_ENGLISH_NAME"];
		else if( data["MUD_CHINESE_NAME"] || data["MUD_ENGLISH_NAME"] )
			mudname = data["MUD_CHINESE_NAME"] || data["MUD_ENGLISH_NAME"];
		else
			mudname = "--�����W��--";

		allmuders += data["USERS"];

		msg += sprintf("%-43s%-25s%-4d"NOR" %-5s%-5s%-26s\n",
		    mudname,
		    ip,
		    port,
		    (data["CONNECT_FAILED_TIMES"] ? NOR CYN"�_�u"NOR : HIC"�s�u"NOR),
		    HIG+(!data["CONNECT_FAILED_TIMES"] ? (data["USERS_COUNT_PARSE"] ? data["USERS"] : "NA") : "")+NOR,
		    data["LAST_CONTACT_TIME"] ? TIME_D->replace_ctime(data["LAST_CONTACT_TIME"]) : "--/--/---- --:--",
		);
		
	}
	msg += NOR "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n";
	msg += "* �����s���ä[��}�� http://www.revivalworld.org/mud/taiwanmudlist\n";
	msg += "* ���a��� NA �N��t�εL�k�����d�߱o�즹 Mud �u�W���a�ƶq\n";
	msg += "* �Y�z�� Mud ���[�J�C��B�ק�άO���Ʊ�b�C��X�{�A�Ц� mud.revivalworld.org 4000 �P Clode �pô\n"NOR;
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	msg += NOR WHT"�@�� "HIW+sizeof(mudnamesort)+NOR WHT" �� Mud�A"HIW+allmuders+NOR WHT" �쪱�a�b�C��\n"NOR;
	msg += "�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

	msg+= online_user_record();
	
	return msg;
}

private void do_command(object me, string arg)
{
	me->more(mnlmudlist_list());
}
