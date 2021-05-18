/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : baseball.c
 * Author : Clode@RevivalWorld
 * Date   : 2006-02-21
 * Note   : �βy��
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#include <ansi.h>
#include <map.h>
#include <feature.h>
#include <message.h>
#include <daemon.h>
#include <npc.h>
#include <condition.h>
#include <delay.h>
#include <baseball.h>
#include <time.h>

inherit ROOM_ACTION_MOD;

void do_setup(object me, string arg)
{
	string myid = me->query_id(1);
	int status = BASEBALL_D->query_status(myid);
	int in_season = BASEBALL_D->in_season(myid);
	string owner = query("owner", environment(me));
	
	mapping setup;

	if( owner != myid && arg )
		return tell(me, "�o�̤��O"+pnoun(2, me)+"���βy���A�L�k�i�����]�w�C\n");
		
	setup = BASEBALL_D->query_setup(owner);

	switch(arg)
	{
		case 0:
		{
			string msg = pnoun(2, me)+"���y���W�٬��u"+(setup["name"]||"���R�W")+"�v���A�ثe�w�ƪ��y���u�Ʀ�m�P�����Φ���Ʀp�U�G\n";
			object labor;
			string position;
			string file;
			
			msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
			msg += sprintf(HIW" %-22s %-20s %-40s\n"NOR, "", "�u�Ʀ�m", "�y���W��" );
			msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
			
			labor = 0;
			position = 0;

			if( mapp(setup["roster"][0]) )
			{
				file = setup["roster"][0]["file"];
				if( file_exists(file) )
					labor = load_object(file);

				position = BASEBALL_D->query_positionname(setup["roster"][0]["position"]);
			}
		
			msg += sprintf(" %-22s %-20s %-40s\n", 
				"",
				position || WHT"���]�w"NOR, 
				objectp(labor) ? labor->query_idname() : WHT"���]�w"NOR
			);
				
			msg += WHT"\n"NOR;
			msg += sprintf(HIW" %-22s %-20s %-40s\n"NOR, "�Φ�", "�u�Ʀ�m", "�y���W��");
			msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
			
			for(int i=1;i<10;++i)
			{
				labor = 0;
				position = 0;

				if( mapp(setup["roster"][i]) )
				{
					file = setup["roster"][i]["file"];
					if( file_exists(file) )
						labor = load_object(file);

					position = BASEBALL_D->query_positionname(setup["roster"][i]["position"]);
				}

				msg += sprintf(" %-22s %-20s %-40s\n", 
					NOR GRN"�� "HIG+i+NOR GRN" ��"NOR, 
					position || WHT"���]�w"NOR, 
					objectp(labor) ? labor->query_idname() : WHT"���]�w"NOR
				);
			}
		
			msg += WHT"�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w�w\n"NOR;
			me->more(msg);
			
			break;	
		}
		case "join":
		{
			object labor;
			
			//if( !wizardp(me) )
			//	return tell(me, "�ثe�t�Υ��b���դ��A�L�k�[�J�u�ɡC\n");

			if( in_season )
				return tell(me, "�y���w�[�J�u�ɡA�е��ݨt�Φw���ɨơC\n");
				
			if( !mapp(setup) )
				return tell(me, "�Х��i��y���U����Ƴ]�w�A�аѦ� help setup�C\n");
				
			if( !setup["name"] )
				return tell(me, "�|���]�w�y���W�١C\n");
				
			foreach(mapping data in setup["roster"])
			{
				if( !mapp(data) || !file_exists(data["file"]) || !objectp(labor = load_object(data["file"])) )
					return tell(me, "�y����Ƥ�����A�ЧQ�� setup �d�ߴΦ��P�u�Ʀ�m�]�w�C\n");
					
				if( !same_environment(labor, me) || query("job/type", labor) != SPORTER )
					return tell(me, "�W�椤���y�����b���B�Υ��N�u�@�]�w���y���C\n");
			}

			// �y���]�w���X�k
			if( !BASEBALL_D->valid_setup(myid) )
				return tell(me, "�y���]�w������A�иԾ\ help setup �öi�槹��]�w�C\n");
		
			setup["master"] = base_name(environment(me)->query_master());

			msg("$ME�ŧi��y���u"+setup["name"]+"�v�q�{�b�}�l�����[�J�u�ɡC\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("sport", me->query_idname()+"�ŧi��y���u"+setup["name"]+"�v�q�{�b�}�l�����[�J�u�ɡC");

			BASEBALL_D->join_season(myid);
			break;
		}
		case "leave":
		{	
			if( status == STATUS_PLAYING )
				return tell(me, "�ثe���b�i����ɡA�������ݤ��ɵ����C\n");
				
			if( !in_season )
				return tell(me, pnoun(2, me)+"�쥻�N�S���ѥ[�u�ɡC\n");

			msg("$ME�ŧi��y���u"+setup["name"]+"�v�h�X�u�ɡC\n", me, 0, 1);
			CHANNEL_D->channel_broadcast("sport", me->query_idname()+"�ŧi��y���u"+setup["name"]+"�v�h�X�u�ɡC");

			BASEBALL_D->leave_season(myid);
			
			break;
		}
		default:
		{
			string value, role;
			int number, n1, n2;

			if( sscanf(arg, "name %s", value) == 1 )
			{
				int len = noansi_strlen(value);
				
				if( status == STATUS_PLAYING )
					return tell(me, "�ثe���b�i����ɡA�������ݤ��ɵ����C\n");
				
				if( len < 4 || len > 12 )
					return tell(me, "�y���W�٪��פ��o�C�� 4 �Ӧr���ΰ��� 12 �Ӧr��("+len+")�C\n");
					
				value = kill_repeat_ansi(value+NOR);

				setup["name"] = value;
				
				CHANNEL_D->channel_broadcast("sport", me->query_idname()+"�N��βy�����W�]�w���u"+value+"�v�C");
				
				BASEBALL_D->set_setup(myid, setup);
			}
			else if( sscanf(arg, "change %d %d", n1, n2) == 2 )
			{
				mapping temp;
				
				if( n1 < 1 || n1 > 9 || n2 < 1 || n2 > 9 )
					return tell(me, "�п�J���T���Φ��C\n");
				
				if( status == STATUS_PLAYING )
					return tell(me, "�ثe���b�i����ɡA�������ݤ��ɵ����C\n");
					
				temp = setup["roster"][n1];
				setup["roster"][n1] = setup["roster"][n2];
				setup["roster"][n2] = temp;
				
				BASEBALL_D->set_setup(myid, setup);
				
				tell(me, pnoun(2, me)+"�N�Φ� "+n1+" �P�Φ� "+n2+" ���y���ﴫ�C\n");
			}			
			else if( sscanf(arg, "%s play %s %d", value, role, number) == 3 || sscanf(arg, "%s play %s", value, role) == 2 )
			{
				object labor = present(value);
				object oldlabor;
				
				if( status == STATUS_PLAYING )
					return tell(me, "�ثe���b�i����ɡA�������ݤ��ɵ����C\n");
				
				if( !objectp(labor) )
					return tell(me, "�o�̨S�� "+value+" �o����u�C\n");
				
				if( query("boss", labor) != me->query_id(1) )
					return tell(me, labor->query_idname()+"�ä��O"+pnoun(2, me)+"�����u�C\n");
						
				if( query("job/type", labor) != SPORTER )
					return tell(me, "�Х��]�w���u���u�@�������y���C\n");

				role = upper_case(role);
				
				if( role != "P" )
				{
					if( number < 1 || number > 9  )
						return tell(me, "�Φ��]�w�ȯ�]�w�b 1 - 9 �Τ����C\n");
				}
				else
				{
					if( !undefinedp(number) )
						return tell(me, "��⤣�|�W�������A���ݳ]�w�Φ��C\n");
				}

				// ���аt�m���y���Φu�Ʀ�m
				for(int i=0;i<10;++i)
				{
					if( !mapp(setup["roster"][i]) ) continue;
						
					if( setup["roster"][i]["file"] == base_name(labor) || setup["roster"][i]["position"] == role || number == i )
					{
						oldlabor = load_object(setup["roster"][i]["file"]);
						
						if( role == "P" )
							tell(me, "�����쥻��"+oldlabor->query_idname()+"������o"+BASEBALL_D->query_positionname(role)+"���]�w�C\n");
						else
							tell(me, "�����쥻��"+oldlabor->query_idname()+"������o"+BASEBALL_D->query_positionname(role)+"�P�� "+number+" �����Φ����]�w�C\n");

						setup["roster"][i] = 0;
					}
				}
				
				switch(role)
				{
					case "P":

						if( labor->query_skill_level("twohands") > 0 )
							return tell(me, "����m���i��ܥ��k�}�}���y������C\n");

						setup["roster"][0] = allocate_mapping(0);
						setup["roster"][0]["file"] = base_name(labor);
						setup["roster"][0]["object"] = labor;
						setup["roster"][0]["position"] = role;
						msg("$ME����$YOU�@���y�������o"+BASEBALL_D->query_positionname(role)+"�C\n", me, labor, 1);
						break;
					case "C":
					case "1B":
					case "2B":
					case "3B":
					case "SS":
					case "RF":
					case "CF":
					case "LF":
					case "DH":
						setup["roster"][number] = allocate_mapping(0);
						setup["roster"][number]["file"] = base_name(labor);
						setup["roster"][number]["object"] = labor;
						setup["roster"][number]["position"] = role;
						msg("$ME����$YOU�@���y�������o"+BASEBALL_D->query_positionname(role)+"�A�ë��w���� "+number+" �Φ��C\n", me, labor, 1);
						break;
					default:
						return tell(me, "�S�� "+role+" �o�ئu�Ʀ�m�A�аѦ� help setup �i��]�w�C\n");
						break;
				}
				
				BASEBALL_D->set_setup(myid, setup);
			}
			else
				return tell(me, "���O�榡���~�A�аѦ� help setup�C\n");
		
			break;		
		}
	}
}

// ��Q
void do_gain(object me, string arg)
{
	string env_city;
	int money;
	object env;
	
	env = environment(me)->query_master();
	
	if( !me->is_user_ob() || query("owner", environment(me)) != me->query_id(1) )
		return tell(me, pnoun(2, me)+"�L�k�����������C\n");
	
	env_city = env->query_city();
	
	money = query("money", env);
	
	if( money <= 0 )
		return msg("$ME��۪ťժ��i�b�A���Ѫ��y�U�T�w�\���C\n"NOR, me, 0, 1);

	me->earn_money(MONEY_D->query_default_money_unit(), money);
	
	msg("$ME�w�ѳߦa���N��o���Q���F�_�ӡA�@�o "+HIY+money(MONEY_D->query_default_money_unit(), money)+NOR"�C\n"NOR, me, 0, 1);
	
	CHANNEL_D->channel_broadcast("sport", HIY"����"NOR YEL"���J "NOR+me->query_idname()+"�w�ѳߦa���N��o���Q���F�_�ӡA�@�o "+HIY+money(MONEY_D->query_default_money_unit(), money)+NOR"�C");
	
	delete("money", env);
	
	log_file("command/gain_baseball", me->query_idname()+"�����Q�� "+money(MONEY_D->query_default_money_unit(), money));

	env->save();
}


// �]�w�ؿv�����ж����A����
nosave mapping action_info =
([
	"rest"	:
	([
		"short"	: HIG"�𮧫�"NOR,
		"help"	:
			([
"topics":
@HELP
    �y���A�Ψӳ]�w�y������P���ɪ��a��
HELP,

"setup":
@HELP
�]�w���ɪ����O�A�Ϊk�p�U�G[�޲z���O]
  setup 				- �d�ߥثe�y���]�w
  setup join				- �������ɡA�t�αN�|�۰ʦw���ɨ�
  setup leave				- �h�X����
  setup name '�y���W��'			- �]�w�o�Ӵβy�����W��
  setup change '�Φ�' '�Φ�'		- �ﴫ��ӴΦ�(�� : setup change 1 9)
  setup '���u' play '�u�Ʀ�m' '�Φ�'	- �]�w���u���u�Ʀ�m(�p�U�N�X)�P�Φ�(1~9)
  					  �ҡGsetup girl 1B 2

�u�Ʀ�m�N�X�p�U
  
  P	- ���		�ݨD�ޯ�G�U����y�B�u�Ƨޯ�
  C	- ����		�ݨD�ޯ�G�U�������B�u�Ƨޯ�
  1B	- �@�S��	�ݨD�ޯ�G�U�������B�u�Ƨޯ�
  2B	- �G�S��	�ݨD�ޯ�G�U�������B�u�Ƨޯ�
  3B	- �T�S��	�ݨD�ޯ�G�U�������B�u�Ƨޯ�
  SS	- ������	�ݨD�ޯ�G�U�������B�u�Ƨޯ�
  RF	- �k�~����	�ݨD�ޯ�G�U�������B�u�Ƨޯ�
  CF	- ���~����	�ݨD�ޯ�G�U�������B�u�Ƨޯ�
  LF	- ���~����	�ݨD�ޯ�G�U�������B�u�Ƨޯ�
  DH	- ���w����	�ݨD�ޯ�G�U�������ޯ�
  
���j�ݩʼv�T
  
  �O�q�G�v�T�򥻧�y�B�����¤O
  ���G�v�T�򥻭@�[�O
  �ӱ��G�v�T�򥻦u�Ư�O
  ���z�G�v�T�򥻶]�S��O
  �y�O�G�v�T�򥻲y�������H�ơB���Ц��J
  
�T�j��O�ȼv�T

  ��O�G��O�����N�v�T���ɦܫ�b�q�ɪ���{
  �ͩR�G�ͩR�v�T���ɤ��i����˪����v�A�������ˮɱN�v�T��{
  �믫�G�y�����Ƹ��᪺���p�U�A�믫�����N�v�T��{
  	
HELP,
			]),
		"heartbeat":0,	// ��ڮɶ� 1 �����
		"job": SPORTER,
		"action":
			([
				"setup"		: (: do_setup :),
				"gain"		: (: do_gain :),
			]),
	]),
]);

// �]�w�ؿv�����
nosave array building_info = ({
	
	// �ؿv��������W��
	HIW"�βy"NOR WHT"��"NOR

	// �}�i���ؿv�����̤֩ж�����
	,4

	// �������̤j�ۦP�ؿv�ƶq����(0 �N������)
	,0

	// �ؿv���ؿv����, GOVERNMENT(�F��), ENTERPRISE(���~����), INDIVIDUAL(�p�H)
	,INDIVIDUAL

	// �ؿv���i�ؿv�ϰ�, AGRICULTURE_REGION(�A�~), INDUSTRY_REGION(�u�~), COMMERCE_REGION(�ӷ~)
	,AGRICULTURE_REGION | INDUSTRY_REGION | COMMERCE_REGION

	// �}�i�����O��
	,120000000
	
	// �ؿv���������ռаO
	,0

	// �c�a�^�m��
	,120
	
	// �̰��i�[�\�Ӽh
	,1
	
	// �̤j�ۦP�ؿv���ƶq(0 �N������)
	,0
	
	// �ؿv���ɥN
	,3
});
