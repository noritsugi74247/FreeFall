#ifndef INCLUDED_JUDGE
#define INCLUDED_JUDGE
#include "obj2d.h"
#include "obj2d_data.h"

void judge();	// �e�퓖���蔻��
void boostjudge(); // �v���C���[�̉�����������p
bool rectHitCheck(const mylib::fRECT&, const mylib::fRECT&); 
				//��`���m�̓����蔻��ł��B�}�b�v�`�b�v�ƃv���C���[�̓����蔻��Ɏg���܂�
#endif