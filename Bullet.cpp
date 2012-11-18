#ifndef __INCLUDES__
#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"
#endif



/**
 * basic bullet class, stores (x,y) and changes in x, y
 * 
*/

class Bullet
{
public:
	ALLEGRO_BITMAP *bullet;
	float bullet_x;
	float bullet_y;
	
	float bullet_dx;
	float bullet_dy;
	
	bool isReadyBool;
	bool isAttack;
	
	int counter;
	
	Bullet( float x, float y, float dx, float dy )
	{
		isReadyBool = false;
		
		
		bullet_x = x;
		bullet_y = y;
		bullet_dx = dx;
		bullet_dy = dy;
		isAttack = true;
		counter = 0;
		
		isReady();		
	}
	
	Bullet( float x, float y )
	{
		isReadyBool = false;
		bullet_x = x;
		bullet_y = y;
	}
	
	~Bullet( void )
	{
		if( bullet != NULL )
		{
			//al_destroy_bitmap(bullet);
		}
	}
	
	bool isReady( void )
	{
		if( bullet_x != 0 && bullet_y != 0 && bullet_dx != 0 && bullet_dy != 0 && bullet != 0 )
		{
			isReadyBool = true;
		}
		
		return isReadyBool;
	}
	
	void setBitmap( int x, int y )
	{
		bullet = al_create_bitmap( x, y );
	}
	
	void setBitmap( ALLEGRO_BITMAP *img )
	{
		bullet = img;
	}
	
	ALLEGRO_BITMAP* getBitmap( void )
	{
		return bullet;
	}
	
	void print()
	{
		std::cout << "enemy" << bullet_x << bullet_y << std::endl;
	}
	
	void set_attack()
	{
		isAttack = false;
	}
	
	void setY( float y )
	{
		bullet_y = y;
	}
	
	void setX( float x )
	{
		bullet_x = x;
	}
	
	float getY( void )
	{
		return bullet_y;
	}
	
	float getX( void )
	{
		return bullet_x;
	}
	
	void setDX( float dx )
	{
		bullet_dx = dx;
	}
	
	void setDY( float dy )
	{
		bullet_dy = dy;
	}
	
	void changeY( float y )
	{
		bullet_y += y;
	}
	
	void changeX( float x )
	{
		bullet_x += x;
	}			
	
	void change_count( int x ) {
		counter += x;
	}
	
	int get_count() {
		return counter;
	}
	
	void set_counter()
	{
		counter = rand()%50 + 50;
	}
	
	float getDX( void )
	{
		return bullet_dx;
	}
	
	float getDY( void )
	{
		return bullet_dy;
	}
	
};
	
	
	
