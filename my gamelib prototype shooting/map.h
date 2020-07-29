#ifndef INCLUDED_MAP
#define INCLUDED_MAP
#include "Obj2D.h"
#include "obj2d_data.h"
#include <sstream>
class MapChip  : public MoveAlg
{
private:
	VECTOR2 scroll;
	char** data;
	const int CHIP_NUM_PER_LINE = 5; // マップチップ画像でチップが横に並んでいる数
	static constexpr float SCROLL_MERGIN_X = 600.0f;
	static constexpr float SCROLL_MERGIN_Y = 880.0f;
public:
	u_int chipnumX, chipnumY;
	float WIDTH, HEIGHT;
	MapChip();
	~MapChip();
	void clear();
	void init(const char*);
	void move(OBJ2D*) override;
	void update();
	void uninit();
	bool loadMapData(const char*, char**);
	void draw(int, char**);
	void drawchips();
	VECTOR2 getScrollPos() { return scroll; }
};
#define CHIP_SIZE 64 // マップチップの1個の大きさ
bool getCSVSize(const char*, u_int*, u_int*);
#endif