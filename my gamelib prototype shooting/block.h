#ifndef INCLUDED_ENEMY
#define INCLUDED_ENEMY

#include "Obj2D.h"
#include "obj2d_data.h"

class Block : public MoveAlg
{
public:
	void move(OBJ2D*);
	
private:
    void moveX(OBJ2D*);
};


class EraseBlock : public EraseAlg
{
public:
	void erase(OBJ2D*);
};

extern EraseBlock eraseblock;
//Enemy block;
//#define	pEnemy	Enemy::getInstance()

class BlockManager : public OBJ2DManager
{
private:
	u_int chipnumX, chipnumY;
public:
	void init(const char*);
	void draw();
};

#endif // !INCLUDED_ENEMY

