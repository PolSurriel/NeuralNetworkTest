#include "Collision.h"

bool Collision::SquareToSquareDetection(Vector2 position1, float radio1, Vector2 position2, float radio2)
{
	float size1 = radio1 + radio1;
	float size2 = radio2 + radio2;

	return	position1.x < position2.x + size2 &&
			position1.x + size1 > position2.x &&
			position1.y < position2.y + size2 &&
			position1.y + size1 > position2.y;
}

bool Collision::CircleToCircleDetection(Vector2 position1, float radio1, Vector2 position2, float radio2)
{
	return position1.DistanceTo(position2) < radio1 + radio2;
}

Vector2 Collision::SquareToSquareReaction(Vector2 position1, float radio1, Vector2 velocity1, Vector2 position2, float radio2) {
	Vector2 actualDistance = position2 - position1;
	float totalDistance = radio1 + radio2;
	float margin = fabs(((velocity1.x == 0) ? actualDistance.y : actualDistance.x)) - totalDistance;
	velocity1.Normalize();

	return position1 + velocity1 * margin;
}

Vector2 Collision::CircleToCircleReaction(Vector2 position1, float radio1, Vector2 velocity1, Vector2 position2, float radio2) {
	Vector2 vectorToPosition2 = position2 - position1;
	vectorToPosition2.Normalize();
	return position2 - vectorToPosition2 * (radio1 + radio2);
}

Vector2 Collision::CircleToSquareReaction(Vector2 circlePosition, float radius, Vector2 lastPosition, Vector2 squarePosition, float squareSize)
{
	//TODO
    return lastPosition;
	
}

 bool Collision::LineToLine(Vector2 position1, Vector2 position2, Vector2 position3, Vector2 position4)
 {


	 // calculate the direction of the lines
	 float uA = ((position4.x - position3.x)*(position1.y - position3.y) - (position4.y - position3.y)*(position1.x - position3.x)) / ((position4.y - position3.y)*(position2.x - position1.x) - (position4.x - position3.x)*(position2.y - position1.y));
	 float uB = ((position2.x - position1.x)*(position1.y - position3.y) - (position2.y - position1.y)*(position1.x - position3.x)) / ((position4.y - position3.y)*(position2.x - position1.x) - (position4.x - position3.x)*(position2.y - position1.y));

	 // if uA and uB are between 0-1, lines are colliding
	 return (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1);
 }

 bool Collision::LineToRect(Vector2 linePosition1, Vector2 linePosition2, Vector2 rectPosition1, float rectWidth, float rectHeight)
{

	 bool left =   Collision::LineToLine(linePosition1, linePosition2, rectPosition1, Vector2(rectPosition1.x, rectPosition1.y + rectHeight));
	 bool right =  Collision::LineToLine(linePosition1, linePosition2, Vector2(rectPosition1.x + rectWidth, rectPosition1.y), Vector2(rectPosition1.x + rectWidth, rectPosition1.y + rectHeight));
	 bool top =    Collision::LineToLine(linePosition1, linePosition2, rectPosition1, Vector2(rectPosition1.x + rectWidth, rectPosition1.y));
	 bool bottom = Collision::LineToLine(linePosition1, linePosition2, Vector2(rectPosition1.x, rectPosition1.y + rectHeight), Vector2(rectPosition1.x + rectWidth, rectPosition1.y + rectHeight));
	
	 return (left || right || top || bottom);
	
}