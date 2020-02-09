#pragma once

#include "Plane.h"
#include "Obstacles.h"

/* I used Clouds just as a workaround to represent obstacles.
*/
class Collision3D {
public:
	Plane& plane;
	Obstacles& blocks;
	Obstacles& fuelRegs;
	static Collision3D* getCollision3D();
	static Collision3D* getCollision3D(Plane&, Obstacles&, Obstacles&);

	/* Tests if in this frame a collision hapened. If the plane collides a obstacle,
	it also removes the obstacle. If the plane collides a fuel regenerator,
	it also removes the fuel regenerator.
	@return true if there was a collision
	@return false else
	*/
	bool testAndApplyCollision();
	int updateScore();

private:
	Collision3D(Plane&, Obstacles&, Obstacles&);
	Collision3D();
	~Collision3D();

	/* Next are the helper functions.
	*/
	float minX(const Form3D&);
	float maxX(const Form3D&);
	float minY(const Form3D&);
	float maxY(const Form3D&);
	float minZ(const Form3D&);
	float maxZ(const Form3D&);

	static Collision3D* collision;
};