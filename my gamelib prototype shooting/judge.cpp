#include "game.h"
#include "player.h"
#include "judge.h"
void judge()
{
	// �e�픻���ǉ�
	auto pl = &(*Game::instance()->playerManager()->getList()->begin());
/********************************************
*		�����蔻��Ɏg���̈�ݒ�			*
********************************************/

	// �v���C���[�̗̈�ݒ�

	pl->hitRect =
	{
		pl->position.x - pl->size.x,
		pl->position.y - pl->size.y,
		pl->position.x + pl->size.x,
		pl->position.y,
	};

	// �u���b�N�̗̈�ݒ�
	for (auto& block : *Game::instance()->blockManager()->getList())
	{
		block.hitRect =
		{
			block.position.x - block.size.x / 2,
			block.position.y - block.size.y / 2,
			block.position.x + block.size.x / 2,
			block.position.y + block.size.y / 2,
		};	
	}
/********************************************
*		�����蔻��							*
********************************************/
	// �v���C���[�@vs �u���b�N
	for (auto& block : *Game::instance()->blockManager()->getList())
	{
		if (!block.judgeFlag) continue;

		// ���u�[�X�g�p�����蔻��
		mylib::fRECT strongrect =
		{ block.hitRect.left,
		  block.hitRect.top - CHIP_SIZE,
		  block.hitRect.right,
		  block.hitRect.bottom - CHIP_SIZE};

		// ���u�[�X�g�p�����蔻��
		mylib::fRECT middlerect =
		{ block.hitRect.left,
		  block.hitRect.top - CHIP_SIZE * 2,
		  block.hitRect.right,
		  block.hitRect.bottom - CHIP_SIZE * 2 };

		// ��u�[�X�g�p�����蔻��
		mylib::fRECT littlerect =
		{ block.hitRect.left,
		  block.hitRect.top - CHIP_SIZE * 3,
		  block.hitRect.right,
		  block.hitRect.bottom - CHIP_SIZE * 3};

		if (rectHitCheck(pl->hitRect,strongrect) && 
			!rectHitCheck(pl->hitRect,middlerect))
		{
			// ���u�[�X�g�̃t���O�𗧂Ă�B
			// ���u�[�X�g�̃t���O�������Ă�������B
			// �����u�[�X�g�������邩�ǂ����̃t���O�������ĂȂ������痧�Ă�B
			pl->flags |= static_cast<int>(Player::BOOSTER::STRONG_BOOST);
			if (pl->flags & static_cast<int>(Player::BOOSTER::MIDDLE_BOOST))
			{
				pl->flags &= ~static_cast<int>(Player::BOOSTER::MIDDLE_BOOST);
			}
			if (!(pl->flags & static_cast<int>(Player::BOOSTER::INSIDE_BOOST)))
			{
				pl->flags |= static_cast<int>(Player::BOOSTER::INSIDE_BOOST);
			}
		}
		if (rectHitCheck(pl->hitRect,middlerect) &&
			!rectHitCheck(pl->hitRect,littlerect))
		{
			// ���u�[�X�g�̃t���O�𗧂Ă�B
			// ��u�[�X�g�̃t���O�������Ă�������B
			// �����u�[�X�g�������邩�ǂ����̃t���O�������ĂȂ������痧�Ă�B
			pl->flags |= static_cast<int>(Player::BOOSTER::MIDDLE_BOOST);
			if (pl->flags & static_cast<int>(Player::BOOSTER::LITTLE_BOOST))
			{
				pl->flags &= ~static_cast<int>(Player::BOOSTER::LITTLE_BOOST);
			}
			if (!(pl->flags & static_cast<int>(Player::BOOSTER::INSIDE_BOOST)))
			{
				pl->flags |= static_cast<int>(Player::BOOSTER::INSIDE_BOOST);
			}
		}
		if (rectHitCheck(pl->hitRect,littlerect))
		{
			// ��u�[�X�g�̃t���O�𗧂Ă�B
			// ���łɁA���E���u�[�X�g�̃t���O�������Ă�������B
			// �����u�[�X�g�������邩�ǂ����̃t���O�������ĂȂ������痧�Ă�B
			pl->flags |= static_cast<int>(Player::BOOSTER::LITTLE_BOOST);
			if (pl->flags & static_cast<int>(Player::BOOSTER::STRONG_BOOST))
			{
				pl->flags &= ~static_cast<int>(Player::BOOSTER::STRONG_BOOST);
			}
			if (pl->flags & static_cast<int>(Player::BOOSTER::MIDDLE_BOOST))
			{
				pl->flags &= ~static_cast<int>(Player::BOOSTER::MIDDLE_BOOST);
			}
			if (!(pl->flags & static_cast<int>(Player::BOOSTER::INSIDE_BOOST)))
			{
				pl->flags |= static_cast<int>(Player::BOOSTER::INSIDE_BOOST);
			}
		}
	
		if (rectHitCheck(pl->hitRect, block.hitRect))
		{
			// �u���b�N�ɂ������Ă��܂�����A
			// �O�̈׃t���O��S������
			for (int i = 0; i <= 3; i++)
			{
				pl->flags &= ~(1 << i);
			}
			block.state++;
		}
	}
}

void boostjudge()
{
	auto pl = &(*Game::instance()->playerManager()->getList()->begin());
	for (auto& block : *Game::instance()->blockManager()->getList())
	{
		if (!block.judgeFlag) continue;
		mylib::fRECT strongrect =
		{ block.hitRect.left,
		  block.hitRect.top - CHIP_SIZE,
		  block.hitRect.right,
		  block.hitRect.bottom - CHIP_SIZE };

		mylib::fRECT middlerect =
		{ block.hitRect.left,
		  block.hitRect.top - CHIP_SIZE * 2,
		  block.hitRect.right,
		  block.hitRect.bottom - CHIP_SIZE * 2 };

		mylib::fRECT littlerect =
		{ block.hitRect.left,
		  block.hitRect.top - CHIP_SIZE * 3,
		  block.hitRect.right,
		  block.hitRect.bottom - CHIP_SIZE * 3 };

		if (!rectHitCheck(pl->hitRect, strongrect) &&
			!rectHitCheck(pl->hitRect, middlerect) &&
			!rectHitCheck(pl->hitRect, littlerect) &&
			pl->flags & static_cast<int>(Player::BOOSTER::INSIDE_BOOST))
		{
			// �u�[�X�g���������ԂŁA
			// �u���b�N�̓����蔻�肩���ꂽ��(���������)�A
			// �u�[�X�g�͈͂��ǂ����̃t���O��܂�B
			// �v���C���[�̉�����player.cpp��Player::booster�֐����ōs���Ă��܂��B
			pl->flags &= ~static_cast<int>(Player::BOOSTER::INSIDE_BOOST);
		}
	}
}
bool rectHitCheck(const mylib::fRECT& obj, const mylib::fRECT& other)
{
	//�@�����𖞂����Ȃ��Ȃ炷���ɏI��������
	if (obj.left > other.right) return false;
	if (obj.top > other.bottom) return false;
	if (obj.right < other.left) return false;
	if (obj.bottom < other.top) return false;

	// ���4�𖞂����Ă��Ȃ������炠�����Ă���B
	return true;
}