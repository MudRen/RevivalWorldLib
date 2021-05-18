/* This program is a part of RW mudlib
 * See /doc/help/copyright for more info
 -----------------------------------------
 * File   : autokill.h
 * Author : Clode@RevivalWorld
 * Date   : 
 * Note   : INCLUDE file
 * Update :
 *  o 2000-00-00  
 *
 -----------------------------------------
 */

#define BASEINCOME		400000000
#define TICKTIME		14
#define POSTSEASON_TICKTIME	9
#define PREPARETIME		600
#define HINTTIMES		6

#define HANDSIDE_NONE		0
#define HANDSIDE_LEFTHAND	(1<<0)
#define HANDSIDE_RIGHTHAND	(1<<1)
#define HANDSIDE_TWOHANDS	(HANDSIDE_LEFTHAND|HANDSIDE_RIGHTHAND)

#define RECORD_TEAM			0
#define RECORD_PLAYER		1
#define RECORD_SPECIAL		2

#define STATUS_IDLE			0
#define STATUS_PREPARING	1
#define STATUS_PLAYING		2

#define TICK			0	// �p��
#define TEAM1			1	// �D���y��
#define TEAM2			2	// �ȳ��y��
#define INNING			3	// ����
#define OUT				4	// �X����
#define STRIKE			5	// �n�y
#define BALL			6	// �a�y
#define RUNNER1ST		7	// �@�S�]��
#define RUNNER2ND		8	// �G�S�]��
#define RUNNER3RD		9	// �T�S�]��
#define TEAM1NUMBER		10	// �D���y���ثe�Φ�
#define TEAM2NUMBER		11	// �ȳ��y���ثe�Φ�
#define TEAM1SCORE		12	// �D���y���o��
#define TEAM2SCORE		13	// �ȳ��y���o��
#define TEAM1HIT		14	// �D���y���w����
#define TEAM2HIT		15	// �ȳ��y���w����
#define TEAM1MISS		16	// �D���y�����~��
#define TEAM2MISS		17	// �ȳ��y�����~��
#define TEAM1K			18	// �D���y�����T����
#define TEAM2K			19	// �ȳ��y�����T����
#define TEAM1WALK		20	// �D���y���O�e��
#define TEAM2WALK		21	// �ȳ��y���O�e��

#define BASETYPE_STRIKEOUT		0	// �T��
#define BASETYPE_BALL			1	// �O�e�W�S
#define BASETYPE_HIT			2	// �w���W�S
#define BASETYPE_BUNT			3	// Ĳ���u��
#define BASETYPE_TOUCHKILL		4	// ���
#define BASETYPE_CATCHKILL		5	// ����
#define BASETYPE_DOUBLEPLAY		6	// ����
#define BASETYPE_TRIPLEPLAY		7	// �T��
#define BASETYPE_STEAL			8	// �s�S

#define SCORETYPE_HOMERUN		1	// ���S��
#define SCORETYPE_FOURBALL		2	// �O�e
#define SCORETYPE_SACRIFICE		3	// �묹��
#define SCORETYPE_HIT			4	// �w��

#define HINTTYPE_BATTER			1	
#define HINTTYPE_PITCHER		2	
#define HINTTYPE_BUNT			3	
#define HINTTYPE_WALK			4
