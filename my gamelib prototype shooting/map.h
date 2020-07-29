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
	const int CHIP_NUM_PER_LINE = 5; // �}�b�v�`�b�v�摜�Ń`�b�v�����ɕ���ł��鐔
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
#define CHIP_SIZE 64 // �}�b�v�`�b�v��1�̑傫��
bool getCSVSize(const char*, u_int*, u_int*);
#endif