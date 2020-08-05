#ifndef INCLUDED_JUDGE
#define INCLUDED_JUDGE
#include "obj2d.h"
#include "obj2d_data.h"

void judge();	// 各種当たり判定
void boostjudge(); // プレイヤーの加速圏内判定用
bool rectHitCheck(const mylib::fRECT&, const mylib::fRECT&); 
bool damaged; // ダメージを受けたかどうか
typedef struct _Judge 
{
	int  left		= 0; // 矩形の左の座標調整用
	int  right		= 0; // 矩形の右の座標調整用
	int  top		= 0; // 矩形の上の座標調整用
	int  bottom		= 0; // 矩形の下の座標調整用
	int  rate		= 0; // 判定する範囲にかける倍率(弱、中、強と分けるときに使ってください)
}Judge;

Judge rule;
	 //矩形同士の当たり判定です。マップチップとプレイヤーの当たり判定に使います
#endif