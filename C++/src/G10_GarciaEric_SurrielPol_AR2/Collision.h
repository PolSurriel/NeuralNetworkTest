#pragma once
#include <list>
#include "Types.h"

/*
 *
 *Clase de funciones estaticas que ayudan a determinar colisiones
 *y emular fisicas.
 *
 *Esta clase hace compobaciones y opera con entidades matematicas.
 *
 *NO conoce NI DEBE conocer la lógica del juego en cuestión.
 *
 *Esta clase es usada especialmente por la clase Collider.
 *
 *No hemos introducido más que las funcionalidades necesarias para suplir
 *los requisitos de la entrega. Pero logicamente la idea es que se puede
 *escalar.
 * 
 */
class Collision
{
public:

	static bool SquareToSquareDetection(Vector2 position1, float radio1, Vector2 position2, float radio2);
	static bool CircleToCircleDetection(Vector2 position1, float radio1, Vector2 position2, float radio2);
	static bool LineToLine(Vector2 position1, Vector2 position2, Vector2 position3, Vector2 position4);
	static bool LineToRect(Vector2 linePosition1, Vector2 linePosition2, Vector2 rectPosition1, float rectWidth, float rectHeight);

	static Vector2 SquareToSquareReaction(Vector2 position1, float radio1, Vector2 velocity1, Vector2 position2, float radio2);
	static Vector2 CircleToCircleReaction(Vector2 position1, float radio1, Vector2 velocity1, Vector2 position2, float radio2);
	static Vector2 CircleToSquareReaction(Vector2 circlePosition, float radius, Vector2 lastPosition, Vector2 squarePosition, float squareSize);
	
	
};
