#ifndef INCLUDED_JUDGE
#define INCLUDED_JUDGE
#include "obj2d.h"
#include "obj2d_data.h"

void judge();	// �e�퓖���蔻��
void boostjudge(); // �v���C���[�̉�����������p
bool rectHitCheck(const mylib::fRECT&, const mylib::fRECT&); 
bool damaged; // �_���[�W���󂯂����ǂ���
typedef struct _Judge 
{
	int  left		= 0; // ��`�̍��̍��W�����p
	int  right		= 0; // ��`�̉E�̍��W�����p
	int  top		= 0; // ��`�̏�̍��W�����p
	int  bottom		= 0; // ��`�̉��̍��W�����p
	int  rate		= 0; // ���肷��͈͂ɂ�����{��(��A���A���ƕ�����Ƃ��Ɏg���Ă�������)
}Judge;

Judge rule;
	 //��`���m�̓����蔻��ł��B�}�b�v�`�b�v�ƃv���C���[�̓����蔻��Ɏg���܂�
#endif