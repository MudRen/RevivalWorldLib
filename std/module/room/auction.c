/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : auction.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-03-11
 * Note   : �Ҳ� - �T������
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <npc.h>
#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <daemon.h>
#include <message.h>

inherit ROOM_ACTION_MOD;

void do_auction_list(object me, string arg)
{
	AUCTION_D->do_auction_list(me, arg);
}

void do_bid(object me, string arg)
{
	AUCTION_D->do_bid(me, arg);
}

void do_auction_receive(object me, string arg)
{
	AUCTION_D->do_auction_receive(me, arg);
}

void do_request_list(object me, string arg)
{
	AUCTION_D->do_request_list(me, arg);
}

void do_request_receive(object me, string arg)
{
	AUCTION_D->do_request_receive(me, arg);
}

void do_request(object me, string arg)
{
	AUCTION_D->do_request(me, arg);
}

void do_support(object me, string arg)
{
	AUCTION_D->do_support(me, arg);
}

// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"hall"	:
	([
		"short"	: HIG"���"NOR GRN"�j�U"NOR,
		"help"	: 
			([ 
"topics":
@HELP
    ���ҬO��檫�~���a��C
HELP,

"bid":
@HELP
�v�Ы��O

bid '�s��' '����'	- ���X�v�л��A���ȳ�쬰 $RW (��: bid 1 1000)
bid '�s��' auto		- �۰ʥH�̧C���\����ӳۻ�
bid '�s��' cancel	- ���̥i�Q�Φ����O�Ӳפ�Y�ө|�L�H�X�������(��: bid 1 cancel)
HELP,

"receive":
@HELP
�o�Ъ��~�I�ڻP����

receive '�s��'			- �N�o�Ъ��~���쨭�W
receive '�s��' to 1 50,50	- �����o�Ъ��~�P�I�ڨê����N���~�B���� 1 �ìP������ 50,50 �y�гB

��1�G�y�Ф����~��ݥѩ��̨ϥΦ����O��^
��2�G����o�Ъ��~�@�ߦ����o�л��� 1% ����O
HELP,

"request":
@HELP
���X�ӫ~�q��

request '�ƶq' '���~�W��' per '���'	- ���X�Y���~���q��
request '�s��' cancel			- �����Y���q��
HELP,

"list":
@HELP
�C�X��檫�~

list			- �C�X�ثe���b��檺���~(�ȦC�X�����ת��~)
list all		- �C�X�ثe���b��檺���~(�t�w���ת��~)
list '�s��'		- �d�߬Y�ӽs����檫�~���Բө����

HELP,

			]),
		"master":1,
		"action":
			([
				"list"		: (: do_auction_list($1, $2) :),
				"receive"	: (: do_auction_receive($1, $2) :),
				"bid"		: (: do_bid($1, $2) :),
			]),
	]),

	"trade"	:
	([
		"short"	: HIC"���"NOR CYN"�j�U"NOR,
		"help"	: 
			([ 
"topics":
@HELP
    ����j�U�O�U�q�檺�a��C
HELP,

"support":
@HELP
�ѳf���O

support '�s��'				- �q���W�ѳf
support '�s��' from 1 50,50		- �q�� 1 �ìP������ 50,50 �y�Эܮw�ѵ��f��
					  (�����N�ѳf���~��m�b�ܮw���u�q��ѳf�v������)
HELP,

"receive":
@HELP
�q�檫�~����

receive '�s��'				- �N�ѳf���쨭�W
receive '�s��' to 1 50,50		- �����ѳf�ê����N���~�B���� 1 �ìP������ 50,50 �y�гB

HELP,

"request":
@HELP
���X�ӫ~�q��

request '�ƶq' '���~�W��' per '���'	- ���X�Y�S�w���~�Τu�~���~���q��(�ݤ���O 10000)
request '�s��' cancel			- �����Y���q��
HELP,

"list":
@HELP
�C�X�q��

list			- �C�X�ثe���b��檺���~(�ȦC�X�����ת��~)
list all		- �C�X�ثe���b��檺���~(�t�w���ת��~)
list '�s��'		- �d�߬Y�ӽs����檫�~���Բө����

HELP,

			]),
		"master":1,
		"action":
			([
				"list"		: (: do_request_list($1, $2) :),
				"receive"	: (: do_request_receive($1, $2) :),
				"support"	: (: do_support($1, $2) :),
				"request"	: (: do_request($1, $2) :),
			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIB"����"NOR

	// �}�i���ؿv�����̤֩ж�����
	,1

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,GOVERNMENT

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,COMMERCE_REGION

	// �}�i�����O��
	,5000000
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,20
	
	// �̰��i�[�\�Ӽh
	,2
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,1
});

