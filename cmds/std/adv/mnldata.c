/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : mnldata.c
 * Author : Msr@RevivalWorld
 * Date   : 2004-12-30
 * Note   : �ק� Mnlmudlist �C����O�C
 * Update :
 *  o 2005-08-12 Blueshadow �ק�ާ@����   
 *  o 2006-08-01 Msr �ק�ާ@����
 *  o 2019-03-03 Msr �W�[�ާ@�覡 
 *  o 
 -----------------------------------------
 */

#define ADDMUD              1
#define CHINESE_NAME		2
#define ENGLISH_NAME		3
#define USER_COUNT          4

#define SKIP            "~q"
#define SKIP2           ""
#define END             "."

#include <ansi.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit COMMAND;

private nosave string mud;

string help = @HELP
    �s�W���� Mnlmudlist �C�� Mud ���U����ơC

�ثe���ѼƦ�

-a      �s�W Mud ��T�� mnlmudlist
-r      ���� mnlmudlist �W�� Mud ��T
-q      �d�߬Y Mud �����
-u      �ק�d�߬Y Mud �H�ƪ��r��
ex: mnldata -u rw.muds.net 4000 to �ثe�@�� %d ��Ův %d �쪱�a�b�u�W�M�H�� %d ��ϥΪ̹��ճs�u���C

-c      ��� Mud ip port �s�u��T
ex: mnldata -c rw.muds.net 4000 to rw.muds.net 5000

HELP;

private nomask varargs void input_to_mud( int type, mapping mnldata, string arg);

int check_mnlmudlist( string arg )
{
    mapping mnl;

    if(!arg) return 0;

    mnl = MNLMUDLIST_D->query_mnlmudlist();

    if( !mapp(mnl[arg]) )
        return 0;

    return 1;
}

mapping query_mnlmudlist( string arg )
{
    mapping mnl;

    if(!arg) return 0;

    mnl = MNLMUDLIST_D->query_mnlmudlist();

    return mnl[arg];
}

private void do_command(object me, string arg)
{
    string ip, port, oldip, oldport;

    if( !is_command() ) return;

    if( !arg )
        return tell(me, "�п�J�n�ק諸����( help mnldata )�C\n");

    // ���� mnlmudlist �� Mud ��T
    if( sscanf(arg, "-r %s %s", ip, port)==2 ) 
    {
        string mud = ip + " " + port;

        if(check_mnlmudlist(mud))
        {
            MNLMUDLIST_D->remove_mud(mud);
            return tell(me, "���� Mud "+mud+"�C\n"); 

        } else return tell(me, sprintf("�S�� %s �o�� Mud ����ơC\n", mud ) );
    }

    // �s�W mnlmudlist �� Mud ��T
    if( arg == "-a" )
    {
        tell(me, "�п�J�n�s�W Mud�A�H�U�� . ��ܵ����A�� ~q ���L�����C\n"); 
        tell(me, "�п�J�n�s�W�� Mud�C(ex: mud.revivalworld.org 4000)\n");
        input_to( (: input_to_mud, ADDMUD, me :));
        return ;
    }

    // �ק� mnlmudlist �� ip ��T
    if( sscanf( arg, "-c %s %s to %s %s", oldip, oldport, ip, port ) == 4)
    {
        string mud, oldmud;

        oldmud = oldip + " " + oldport;

        if(check_mnlmudlist(oldmud))
        {
            mud = ip + " " + port;

            MNLMUDLIST_D->change_ipport(oldmud, mud);
            return tell(me, sprintf("Mud %s �w�令 %s�C\n", oldmud, mud)); 
        } else return tell(me, sprintf("�S�� %s �o�� Mud ����ơC\n", mud ) );
    }

    if( sscanf( arg, "-q %s %s", ip, port ) == 2 )
    {
        string mud = ip + " " + port;

        if( check_mnlmudlist(mud) )
            return tell( me, sprintf("Mud ��T�p�U\n%O\n", query_mnlmudlist(mud)));
        else
            return tell(me, sprintf("�S�� %s �o�� Mud ����ơC\n", mud ) );
    }

    if( sscanf( arg, "-u %s %s to %s", ip, port, oldip ) == 3 )
    {
        string mud = ip + " " + port;

        if( check_mnlmudlist(mud) )
        {
            MNLMUDLIST_D->add_mud_info(mud, "USERS_COUNT_PARSE", oldip);
            return tell( me, sprintf("Mud %s �ק�H�Ƭd�߻y�k��אּ %s\n", mud, oldip));
        }
        else
            return tell(me, sprintf("�S�� %s �o�� Mud ����ơC\n", mud ) );
    }     

    return tell( me, help );
}

private nomask varargs void input_to_mud( int type, object me, string arg)
{
    string ip, port;

    switch(type) 
    {
    case ADDMUD :
        {
            if( arg == SKIP || arg == SKIP2 )
            {
                tell(me, "�п�J MUD_CHINESE_NAME�C(ex: ���ͪ��p��)\n");
                input_to( (: input_to_mud, ADDMUD, me :));
                return;
            }
            if( arg == END )
                return me->finish_input();

            if( sscanf(arg, "%s %s", ip, port)==2 ) 
            {
                mud = arg;
                MNLMUDLIST_D->add_mud(mud);
                tell(me, "�п�J MUD_CHINESE_NAME�C(ex: ���ͪ��@��)\n");
                input_to( (: input_to_mud, CHINESE_NAME, me :) );
            } 
            else
            {
                tell(me, "�п�J�n�s�W�� Mud�C(ex: mud.revivalworld.org 4000)\n"); 
                input_to( (: input_to_mud, ADDMUD, me :));
            }
            break;
        }

    case CHINESE_NAME :
        {        
            if( !arg || arg == SKIP || arg == SKIP2 )
            {
                tell(me, "�п�J MUD_ENGLISH_NAME�C(ex: revival world)\n");
                input_to( (: input_to_mud, ENGLISH_NAME, me :) );
                return;
            }

            if( arg == END )
                return me->finish_input();

            if( is_chinese(arg) ) 
            {
                MNLMUDLIST_D->add_mud_info( mud, "MUD_CHINESE_NAME", arg );
                tell(me, "�п�J MUD_ENGLISH_NAME�C(ex: revival world)\n");
                input_to( (: input_to_mud, ENGLISH_NAME, me :) );
            } 
            else
            {
                tell(me, "�п�J MUD_CHINESE_NAME�C(ex: ���ͪ��@��)\n"); 
                input_to( (: input_to_mud, CHINESE_NAME, me :));
            }
            break;
        }

    case ENGLISH_NAME :
        {
            if( arg == SKIP || arg == SKIP2)
            {
                tell(me, "�п�J�H�ƧP�w�y�k(ex: �ثe�@�� %d ��Ův�B%d �쪱�a�b�u�W)�C\n");
                input_to( (: input_to_mud, USER_COUNT, me :) );
                return;

            }

            if( arg == END )
                return me->finish_input();

            if( is_english(arg) )
            {			
                MNLMUDLIST_D->add_mud_info( mud, "MUD_ENGLISH_NAME", arg );
                tell(me, "�п�J�H�ƧP�w�y�k(ex: �ثe�@�� %d ��Ův�B%d �쪱�a�b�u�W)�C\n");

                input_to( (: input_to_mud, USER_COUNT, me :) );
            }
            else
            {
                tell(me, "�п�J MUD_ENGLISH_NAME�C(ex: revival world)\n"); 
                input_to( (: input_to_mud, ENGLISH_NAME, me :));
            }
            break;
        }

    case USER_COUNT :
        {
            if( arg == SKIP || arg == SKIP2 || arg == END ) 
                return me->finish_input();

            if(arg)
            {
                MNLMUDLIST_D->add_mud_info(mud, "USERS_COUNT_PARSE", arg );
                tell(me, "�s�W Mud ��T�����C\n");

                return me->finish_input();
            }
            else
            {
                tell(me, "�п�J�H�ƧP�w�y�k(ex: �ثe�@�� %d ��Ův�B%d �쪱�a�b�u�W)�C\n");
                input_to( (: input_to_mud, USER_COUNT, me :));
            }
            break;
        }
    default: 
        return me->finish_input();
    }
}
