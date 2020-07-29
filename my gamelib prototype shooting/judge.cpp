#include "game.h"
#include "player.h"
#include "judge.h"
void judge()
{
	// 各種判定を追加
	auto pl = &(*Game::instance()->playerManager()->getList()->begin());
/********************************************
*		当たり判定に使う領域設定			*
********************************************/

	// プレイヤーの領域設定

	pl->hitRect =
	{
		pl->position.x - pl->size.x,
		pl->position.y - pl->size.y,
		pl->position.x + pl->size.x,
		pl->position.y,
	};

	// ブロックの領域設定
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
*		当たり判定							*
********************************************/
	// プレイヤー　vs ブロック
	for (auto& block : *Game::instance()->blockManager()->getList())
	{
		if (!block.judgeFlag) continue;

		// 強ブースト用当たり判定
		mylib::fRECT strongrect =
		{ block.hitRect.left,
		  block.hitRect.top - CHIP_SIZE,
		  block.hitRect.right,
		  block.hitRect.bottom - CHIP_SIZE};

		// 中ブースト用当たり判定
		mylib::fRECT middlerect =
		{ block.hitRect.left,
		  block.hitRect.top - CHIP_SIZE * 2,
		  block.hitRect.right,
		  block.hitRect.bottom - CHIP_SIZE * 2 };

		// 弱ブースト用当たり判定
		mylib::fRECT littlerect =
		{ block.hitRect.left,
		  block.hitRect.top - CHIP_SIZE * 3,
		  block.hitRect.right,
		  block.hitRect.bottom - CHIP_SIZE * 3};

		if (rectHitCheck(pl->hitRect,strongrect) && 
			!rectHitCheck(pl->hitRect,middlerect))
		{
			// 強ブーストのフラグを立てる。
			// 中ブーストのフラグが立ってたら消す。
			// もしブーストがかかるかどうかのフラグが立ってなかったら立てる。
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
			// 中ブーストのフラグを立てる。
			// 弱ブーストのフラグが立ってたら消す。
			// もしブーストがかかるかどうかのフラグが立ってなかったら立てる。
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
			// 弱ブーストのフラグを立てる。
			// ついでに、中・強ブーストのフラグが立ってたら消す。
			// もしブーストがかかるかどうかのフラグが立ってなかったら立てる。
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
			// ブロックにあたってしまったら、
			// 念の為フラグを全部消す
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
			// ブーストがかかる状態で、
			// ブロックの当たり判定から逸れたら(回避したら)、
			// ブースト範囲かどうかのフラグを折る。
			// プレイヤーの加速はplayer.cppのPlayer::booster関数内で行っています。
			pl->flags &= ~static_cast<int>(Player::BOOSTER::INSIDE_BOOST);
		}
	}
}
bool rectHitCheck(const mylib::fRECT& obj, const mylib::fRECT& other)
{
	//　条件を満たさないならすぐに終了させる
	if (obj.left > other.right) return false;
	if (obj.top > other.bottom) return false;
	if (obj.right < other.left) return false;
	if (obj.bottom < other.top) return false;

	// 上の4つを満たしていなかったらあたっている。
	return true;
}