#ifndef VECTOR2D
#define VECTOR2D

#include <math.h>

class Vector2D
{
public:
	float x,y;

	Vector2D()
	{
		x = 0;
		y = 0;
	}
	Vector2D(float newX, float newY)
	{
		x = newX;
		y = newY;
	}
	float GetX()
	{
		return x;
	}
	float GetY()
	{
		return y;
	}
	void SetXY(float newX, float newY)
	{
		x = newX;
		y = newY;
	}
	void SetX(float newX)
	{
		x = newX;
	}
	void SetY(float newY)
	{
		y = newY;
	}
	float GetLength()
	{
		return sqrt((x*x) + (y*y));
	}
	void Zero()
	{
		x = 0;
		y = 0;
	}
	void Normalize()
	{
		float length = GetLength();
		x = x / length;
		y = y / length;
	}
	void SetLength(float newLength)
	{
		Normalize();
		x = x * newLength;
		y = y * newLength;
	}
	float DistanceFrom(Vector2D target)
	{
		target = target - Vector2D(x, y);
		return target.GetLength();
	}
	/*Dot()
	{

	}*/
	bool IsRight()
	{
		if(x < 0)
			return true;
		return false;
	}
	bool IsLeft()
	{
		if(x > 0)
			return true;
		return false;
	}
	bool IsUp()
	{
		if(y < 0)
			return true;
		return false;
	}
	bool IsDown()
	{
		if(y > 0)
			return true;
		return false;
	}
	/*void Turn(float angle)
	{

	}*/
	void Flip()
	{
		x = -x;
		y = -y;
	}
	void Turn90Left()
	{
		//x = y; y = -x
		float temp = x;
		x = y;
		y = -temp;
	}	
	void Turn90Right()
	{
		//x = -y; y = x
		float temp = x;
		x = -y;
		y = temp;
	}
	Vector2D operator + (Vector2D in)
	{
		return Vector2D(x + in.x, y + in.y);
	}
	Vector2D operator + (float in)
	{
		return Vector2D(x + in, y + in);
	}
	Vector2D operator ++ ()
	{
		return Vector2D(x++, y++);
	}
	Vector2D operator - (Vector2D in)
	{
		return Vector2D(x - in.x, y - in.y);
	}
	Vector2D operator - (float in)
	{
		return Vector2D(x - in, y - in);
	}
	Vector2D operator -- ()
	{
		return Vector2D(x--, y--);
	}
	Vector2D operator * (Vector2D in)
	{
		return Vector2D(x * in.x, y * in.y);
	}
	Vector2D operator * (float in)
	{
		return Vector2D(x * in, y * in);
	}
	Vector2D operator / (Vector2D in)
	{
		return Vector2D(x / in.x, y / in.y);
	}
	Vector2D operator / (float in)
	{
		return Vector2D(x / in, y / in);
	}
	/*bool operator > (Vector2D in)
	{

	}
	bool operator > (float in)
	{

	}
	bool operator >= (Vector2D in)
	{

	}
	bool operator >= (float in)
	{

	}
	bool operator < (Vector2D in)
	{

	}
	bool operator < (float in)
	{

	}
	bool operator <= (Vector2D in)
	{

	}
	bool operator <= (float in)
	{

	}*/
	void Add(Vector2D in)
	{
		x = x + in.x;
		y = y + in.y;
	}
	void Add(float in)
	{
		x = x + in;
		y = y + in;
	}
	void Sub(Vector2D in)
	{
		x = x - in.x;
		y = y - in.y;
	}
	void Sub(float in)
	{
		x = x - in;
		y = y - in;
	}
	void Mul(Vector2D in)
	{
		x = x * in.x;
		y = y * in.y;
	}
	void Mul(float in)
	{
		x = x * in;
		y = y * in;
	}
	void Div(Vector2D in)
	{
		x = x / in.x;
		y = y / in.y;
	}
	void Div(float in)
	{
		x = x / in;
		y = y / in;
	}
};

inline float Dist(Vector2D a, Vector2D b)
{
	b = b - a;
	return b.GetLength();
}

#endif 