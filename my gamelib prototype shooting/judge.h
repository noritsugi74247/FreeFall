#ifndef INCLUDED_JUDGE
#define INCLUDED_JUDGE
#include "obj2d.h"
#include "obj2d_data.h"

void judge();	// 各種当たり判定
void boostjudge(); // プレイヤーの加速圏内判定用
bool rectHitCheck(const mylib::fRECT&, const mylib::fRECT&); 
				//矩形同士の当たり判定です。マップチップとプレイヤーの当たり判定に使います
#endif