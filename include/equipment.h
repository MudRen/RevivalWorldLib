/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : /include/equipment.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define PART_ID			0	// ����N��
#define PART_AMOUNT		1	// ������i�˳ƪ��˳Ƽƶq
#define PART_USER_AMOUNT	2	// ���˳ƦP�ɥi�H�Q�h�֤H�˳�
#define PART_OUTSIDE		3	// ���˳ƬO�_������~���˳�
#define	PART_CONFLICT		4	// ���쪺�Ĭ���(�Ҧp "���u�˥�" ����M "�y�M"/"���k��" �P�ɸ˳�


// �˳Ƴ���w�q			({ �N��, 	�i�˳Ƽƶq, 	�i�P�ɸ˳ƤH��,	�O�_���~���˳�,	�˳Ƴ���Ĭ� })

#define EQ_FINGER		({ "finger", 		2, 		1, 		0,		({})		})	// �٫�
#define EQ_NECKLACE		({ "necklace", 		1, 		1, 		0,		({})		})	// ����
#define EQ_MOUNT		({ "mount",		1,		1, 		0,		({})		})	// �y�M
#define EQ_VEHICLE		({ "vehicle",		1,		1, 		0,		({})		})	// ����
#define EQ_BOAT			({ "boat",		1,		1, 		0,		({})		})	// �
#define EQ_AIRCRAFT		({ "aircraft",		1,		1, 		0,		({})		})	// ���澹
#define	EQ_BROOCH		({ "brooch",		1,		1, 		0,		({})		})	// �ݰw
#define EQ_EYE			({ "eye",		1,		1,		0,		({})		})	// ����
#define EQ_NOSE			({ "nose",		1,		1,		0,		({})		})	// ��l
#define EQ_ETC			({ "etc",		1,		1,		0,		({})		})	// �d��
#define EQ_HAND			({ "hand",		2,		1,		0,		({ "twohand" }) })	// ���
#define EQ_TWOHAND		({ "twohand",		1,		1,		0,		({ "hand" })	})	// ����
#define EQ_BREASTPLATE		({ "breastplate",	1,		1,		0,		({})		})	// �ݥ�
#define EQ_HEAD                 ({ "head",		1,              1,              0,              ({})            })	// �Y��
#define EQ_LEGGINGS		({ "leggings",		1,		1,		0,		({})		})	// �L��
#define EQ_GLOVES		({ "gloves",		1,		1,		0,		({})		})	// ��M
#define EQ_BELT			({ "belt",		1,		1,		0,		({})		})	// �y�a
#define EQ_BOOTS		({ "boots",		1,		1,		0,		({})		})	// �c�l
#define EQ_BRACERS		({ "bracers",		1,		1,		0,		({})		})	// �@��
#define EQ_CLOAK		({ "cloak",		1,		1,		0,		({})		})	// �ܭ�
#define EQ_MITT			({ "mitt",		1,		1,		0,		({})		})	// �βy��M
#define EQ_BAT			({ "bat",		1,		1,		0,		({})		})	// �βy�y��

