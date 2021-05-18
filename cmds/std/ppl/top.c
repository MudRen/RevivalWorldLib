/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : top.c
 * Author : Clode@RevivalWorld
 * Date   : 2002-8-15
 * Note   : top ���O
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <daemon.h>
#include <feature.h>
#include <message.h>
#include <skill.h>
#include <map.h>
#include <money.h>

inherit COMMAND;

string help = @HELP
�C�X�U�رƦ�]�G

top �ﶵ		- �w�]�C�X�@�ɫe 30 �W�C��
top �ﶵ 100 		- �C�X�@�ɫe 100 �W�C��
top �ﶵ all		- �C�X�����ƦW(�̦h 1000 �W)

���ئp�U

�g��
top rich		- �@�ɴI���Ʀ�](�{���ϩЦa���`��)
top cash		- �@�ɲ{���Ʀ�]
top estate		- �@�ɩЦa���Ʀ�]
top product		- �@�ɲ��~���ȱƦ�]
top store		- �@�ɳ̨ΰө��Ʀ�]
top building		- �@�ɰ��ӫؿv�Ʀ�]
top enterprise		- �@�ɥ��~�Ʀ�]
top stock		- �@�ɪѤ��Ʀ�]
top maintain		- �@�ɤg�a���űƦ�]
top city		- �@�ɫ����c�a�Ʀ�]
top futures		- �@�ɴ��f�l�q�Ʀ�]

�԰�
top killnpc		- �@�������ƶq�Ʀ�]
top killplayer		- �@���������a�Ʀ�]
top battlescore		- �@�ɾԳ��\�Z�Ʀ�]
top attack		- �@�ɧ����O�Ʀ�]
top defend		- �@�ɨ��m�O�Ʀ�]
top speed		- �@�ɧ����t�ױƦ�]
top damage		- �@�ɤ@��԰��̰��ˮ`�ȱƦ�]
top battledamage	- �@�ɾԳ��԰��̰��ˮ`�ȱƦ�]

����
top skill		- �@�ɧޯ�v�v�Ʀ�]
top skill �ޯ�W	- �@�ɳ�W�ޯ൥�űƦ�]
top attr �ݩʦW		- �@���ݩʼƭȱƦ�]
top socialexp		- �@�ɪ��|�g��Ʀ�]
top combatexp		- �@�ɾ԰��g��Ʀ�]
top quest		- �@�ɥ��ȧ����ƶq�Ʀ�]
top age			- �@�ɪ��a�n�J�ɼƱƦ�]
top title		- �@�ɪ��a�ٸ��ƶq�Ʀ�]
HELP;

private void do_command(object me, string arg)
{
	int i, top;
	string msg, arg1, arg2, arg3;

	if( !arg ) 
		return tell(me, help);

	if( sscanf(arg, "%s %s %s", arg1, arg2, arg3) == 3 )
	{
		if( arg3 == "all" ) 
			i = 1000;
		else 
			i = to_int(arg3);
	}
	else if( sscanf(arg, "%s %s", arg1, arg2) == 2 )
	{
		if( arg2 == "all" )
			i = 1000;
		else if( to_int(arg2) )
		{
			i = to_int(arg2);
			arg2= 0;
		}
		else
			i = 30;
	}
	else
	{
		i = 30;
		arg1 = arg;
	}

	if( i < 1 || i > 1000 )
		return tell(me, "��ܱƦW�ƶq���o�p�� 1 �Τj�� 1000�C\n");

	switch( arg1 )
	{
		case "rich":
			{

				msg = NOR YEL"�@��"HIY"�e "+i+" �I��"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "�ƦW", "���a", "����", "�`�겣($RW)", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach(array data in TOP_D->query_top("rich"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
						top,
						data[3],
						(CITY_D->query_city_name(data[4]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "cash":
			{

				msg = NOR YEL"�@��"HIY"�e "+i+" �{��"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "�ƦW", "���a", "����", "�{��($RW)", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach(array data in TOP_D->query_top("cash"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
						top,
						data[3],
						(CITY_D->query_city_name(data[4]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "estate":
			{

				msg = NOR YEL"�@��"HIY"�e "+i+" �Цa��"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "�ƦW", "���a", "����", "�Цa��($RW)", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach(array data in TOP_D->query_top("estate"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
						top,
						data[3],
						(CITY_D->query_city_name(data[4]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "product":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ���~����"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-34s%-20s%-15s%10s\n"NOR, "�ƦW", "���~", "���~����", "�}�o��", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("products"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-34s"HIY"%-20s"HIG"%-15s"NOR YEL"%10s\n"NOR,
						top,
						data[3],
						NUMBER_D->number_symbol(data[1]),
						//sizeof(data)>5 ? data[5] : 0,
						capitalize(data[4]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "store":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �̨ΰө��Ʀ�]"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-31s %14s %-15s %12s\n"NOR, "�ƦW", "�ө�", "�P���`��", "����", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("storesell"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."NOR" %-31s"HIW" %14s"HIG" %-15s"NOR YEL" %12s\n"NOR,
						top,
						data[4],
						NUMBER_D->number_symbol(data[1]),
						//(CITY_D->query_city_id(data[5]) || ""),
						capitalize(data[3]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "skill":
			{
				if( !arg2 )
				{
					msg = NOR YEL"�@��"HIY"�e "+i+" �ޯ�v�v"NOR YEL"�Ʀ�]�G\n"NOR;
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
					msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "�ƦW", "���a", "����", "���޼�", "����");
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
					foreach(array data in TOP_D->query_top("skill"))
					{
						if( ++top > i ) break;
						msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
							top,
							data[3],
							(CITY_D->query_city_name(data[4]) || ""),
							NUMBER_D->number_symbol(data[1]),
							TIME_D->replace_ctime(data[2])[0..9],
							);
					}
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				}
				else
				{
					string owner;
					
					arg2 = lower_case(arg2);

					if( !SKILL_D->skill_exists(arg2) )
						return tell(me, "�èS���o�اޯ�C\n");
					
					msg = NOR YEL"�@��"HIY"�e "+i+" "+(SKILL(arg2))->query_idname()+"�ޯ൥��"NOR YEL"�Ʀ�]�G\n"NOR;
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
					msg += sprintf(HIW"%4s %-28s %-24s %8s %12s\n"NOR, "�ƦW", "���a", "���ݫ���/����", "�ޯ൥��", "����");
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
					foreach(array data in TOP_D->query_top("skill/"+arg2))
					{
						if( ++top > i ) break;
						
						owner = 0;	
						if( sizeof(data) > 4 )
						{
							owner = CITY_D->query_city_name(data[4]);
							
							if( !owner )
							{
								object user = load_user(data[4]);
								
								if( objectp(user) )
								{
									owner = user->query_idname();
								
									if( !userp(user) )
										destruct(user);
								}
							}
						}

						msg += sprintf(HIW"%3d."NOR" %-28s %-24s "HIY"%8s"NOR YEL" %12s\n"NOR,
							top,
							data[3],
							owner || "",
							NUMBER_D->number_symbol(data[1]),
							TIME_D->replace_ctime(data[2])[0..9],
							);
					}
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
					
					
				}
				return me->more(msg+"\n");
			}
			break;
		case "attr":
			{
				if( !arg2 )
					return tell(me, pnoun(2, me)+"�Q�n�d�߭��@���ݩʡH(strength�Bphysique�Bintelligence�Bagility�Bcharisma�Bstamina�Bhealth�Benergy)\n");
				else
				{
					array topdata;

					arg2 = lower_case(arg2);

					topdata = TOP_D->query_top("attr/"+arg2);
					
					if( !sizeof(topdata) )
						return tell(me, pnoun(2, me)+"�Q�n�d�߭��@���ݩʡH(strength�Bphysique�Bintelligence�Bagility�Bcharisma�Bstamina�Bhealth�Benergy)\n");
					
					msg = NOR YEL"�@��"HIY"�e "+i+" "+capitalize(arg2)+" �ݩʵ���"NOR YEL"�Ʀ�]�G\n"NOR;
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
					msg += sprintf(HIW"%4s %-28s %-12s %20s %12s\n"NOR, "�ƦW", "���a", "����", "�ݩʼƭ�", "����");
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
	
					foreach(array data in topdata)
					{
						if( ++top > i ) break;
						msg += sprintf(HIW"%3d."NOR" %-28s %-12s "HIY"%20s"NOR YEL" %12s\n"NOR,
							top,
							data[3],
							sizeof(data) > 4 ? (CITY_D->query_city_name(data[4]) || "") : "",
							NUMBER_D->number_symbol(data[1]),
							TIME_D->replace_ctime(data[2])[0..9],
							);
					}
					msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				}
				return me->more(msg+"\n");
			}
			break;
		case "building":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ���ӫؿv"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf(HIW"%4s %-14s %-32s %-10s %4s %10s\n"NOR, "�ƦW", "���a", "�ө�", "����", "�Ӽh", "����");
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach(array data in TOP_D->query_top("building"))
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%3d."HIY" %-14s "NOR"%-32s %-10s "HIG"%4s"NOR YEL" %10s\n"NOR,
						top,
						data[3],
						data[4],
						(CITY_D->query_city_name(data[5]) || ""),
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9],
						);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;

		case "stock":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �Ѥ�"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          �����Ѳ��{��        �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("stockvalue") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;

		case "quest":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ���ȧ���"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          ���ȧ����ƶq        �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("quest") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "enterprise":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ���~"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���~                     �겣�`��		               �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("stock") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s"HIY"%-22s"NOR YEL"%s\n"NOR,
						top,
						data[2],
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "socialexp":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ���|�g���`��"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          ���|�g���`��        �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("social_exp") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "combatexp":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �԰��g���`��"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          �԰��g���`��        �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("combat_exp") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "maintain":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �g�a����"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                      �y��                 ���� ����   �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("maintain") )
				{
					if( ++top > i ) break;
						
					switch(data[4])
					{
						case FARM: arg1 = HIG"�A"NOR GRN"��"NOR; break;
						case PASTURE: arg1 = HIY"��"NOR YEL"��"NOR; break;
						case FISHFARM: arg1 = HIB"�i"NOR BLU"��"NOR; break;
					}
					msg += sprintf(HIW"%-3d."NOR" %-25s%-22s%4s"HIR"%5s"NOR YEL" %s\n"NOR,
						top,
						data[3],
						loc_short(restore_variable(data[0])),
						arg1,
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "killnpc":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �����q������ƶq"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          �����ƶq            �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("combat_kill_win") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "killplayer":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �������a�ƶq"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          �����ƶq            �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("combat_kill_winplayer") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "battlescore":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �Գ��\�Z"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          �Գ��\�Z            �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("combat_battle_score") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "attack":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ������O"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          ������O            �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("combat_attack") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "defend":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ���m��O"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          ���m��O            �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("combat_defend") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "speed":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �����t��"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          �����t��            �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("combat_speed") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
		case "city":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" ����"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ����                     �W��     �c�a��     �~����        �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("city") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-9s"HIG"%-11s"HIW"%-12s"NOR YEL"%s\n"NOR,
						top,
						CITY_D->query_city_name(data[3], data[4]),
						CITY_D->query_city_scale(data[3], data[4]),
						NUMBER_D->number_symbol(data[1]),
						NUMBER_D->number_symbol(CITY_D->query_section_info(data[3], data[4], "resident")),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
		case "age":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �n�J�ɼ�"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     �n�J�ɼ�                          �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("age") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s"HIG"%-32s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						FINGER_D->query_time_long(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "damage":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �@��԰��̰��ˮ`"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          �̰��ˮ`            �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("combat_max_damage") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "battledamage":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �Գ��԰��̰��ˮ`"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          �̰��ˮ`            �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("battle_max_damage") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "title":
			{
				msg = NOR YEL"�@��"HIY"�e "+i+" �ٸ��ƶq"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                     ����          �ٸ��ƶq            �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("title") )
				{
					if( ++top > i ) break;
					msg += sprintf(HIW"%-3d."NOR" %-25s%-14s"HIG"%-18s"NOR YEL"%s\n"NOR,
						top,
						data[3],
						CITY_D->query_city_name(data[4])||"",
						NUMBER_D->number_symbol(data[1]),
						TIME_D->replace_ctime(data[2])[0..9]);
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				return me->more(msg+"\n");
			}
			break;
		case "futures":
			{
				int totalgain = 0;
				
				msg = NOR YEL"�@��"HIY"�e "+i+" �����f�l�q"NOR YEL"�Ʀ�]�G\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg +=     HIW"�ƦW ���a                    ����       �����f�l�q            �����ɶ�\n"NOR;
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;

				foreach( array data in TOP_D->query_top("futures") )
				{
					totalgain += data[1];
					
					if( ++top <= i )
					{
						msg += sprintf(HIW"%-3d."NOR" %-24s%-11s"HIY"%-22s"NOR YEL"%s\n"NOR,
							top,
							data[3],
							CITY_D->query_city_name(data[4])||"",
							money(DEFAULT_MONEY_UNIT, data[1]),
							TIME_D->replace_ctime(data[2])[0..9]);
					}
				}
				msg += NOR WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
				msg += sprintf("%72s", NOR YEL"�@�ɴ��f���l�q�`�X "HIY+money(DEFAULT_MONEY_UNIT, totalgain)+NOR);
				return me->more(msg+"\n");
			}
			break;
		default:
			tell(me, help);
			break;
	}
}		
