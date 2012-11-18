// g++ main2.cpp $(pkg-config --libs allegro-5.0 allegro_image-5.0 allegro_acodec-5.0)

//
//
//
//
//
//
//
//        MAGIC FILE
//		 DO NOT READ!
//
//
//
//
//
//

#ifndef __INCLUDE__
#include "includes.h"
#endif

using namespace std;

const float FPS = 60.0;
const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int CHAR_SIZE = 32;
const int HITBOX = 7;
int CHAR_SPEED = 4;
bool CHAR_FOCUS = false;
bool BOSS_CHECK = false;
bool BOSS_ACTIVE = false;
int BOSS_HEALTH = 100;
int CHARGE = 0;
bool HEALTH_SET = false;
const int BOSS_SIZE = 80;
int TIMER = 100;
int X_CHANGE = 0;
int Y_CHANGE = 0;
const int ENEMY_SIZE = 45;
const int ENEMY_S_SIZE = 25;
const int ENEMY_C_SIZE = 45;
const int IMG_SIZE = 20;
const int BULLET_SIZE = 6;
bool music = false;
int track = time(NULL) % 4;
bool inProgress = false;
bool inBoss = false;
//const int BULLET_CENTER = BULLET_SIZE / 2;

Bullet *HEALTHBAR;
Bullet *HEALTHBAR_BACKGROUND;
Bullet *HEALTHBAR_BACKGROUND_BACKGROUND;

int HEALTH = 100;
int LEVEL = 1;
int SCORE = 0;

Bullet *character;
Bullet *char_hitbox;
Bullet *hitbox;
bool dead = false;

vector<Enemy> enemies;
vector<Bullet> enemy_bullets;
//vector<Bullet> explosion;

ALLEGRO_SAMPLE *track1 = NULL;
ALLEGRO_SAMPLE *track2 = NULL;
ALLEGRO_SAMPLE *track3 = NULL;
ALLEGRO_SAMPLE *track4 = NULL;
ALLEGRO_SAMPLE *boss = NULL;
ALLEGRO_SAMPLE *warning = NULL;
ALLEGRO_SAMPLE *dest1 = NULL;
ALLEGRO_SAMPLE *dest2 = NULL;
ALLEGRO_SAMPLE *bdeath = NULL;

enum MYKEYS 
{
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE, KEY_Z, KEY_LSHIFT, KEY_1, KEY_2, KEY_3, KEY_4
};


void char_hit()
{
	HEALTH -= 10;
	
	if( HEALTH < 0 )
	{
		dead = true;
		BOSS_ACTIVE = false;
	}
	else
	{
		ALLEGRO_BITMAP *health_bmp = al_create_bitmap( HEALTH, 30 );
		HEALTHBAR->setBitmap( health_bmp );
		
		al_set_target_bitmap( HEALTHBAR->getBitmap() );
		al_clear_to_color( al_map_rgb( 0, 122, 122 ) );
	}
}



void add_enemy( void )
{
	int bullet_type = rand() % 2;

	if (BOSS_CHECK != true && BOSS_ACTIVE != true)
	{
		Enemy *bullet = new Enemy( rand() % ( SCREEN_W - ENEMY_SIZE ) , -( ENEMY_SIZE ), 0, 1 );
		
		
		if( bullet_type == 0)
		{
			
			ALLEGRO_BITMAP *bullet_bmp = al_load_bitmap( "Alien_2.png" );
			bullet->setBitmap( bullet_bmp );
			bullet->set_health( 4 + LEVEL );
		}
		else if ( bullet_type == 1 )
		{
			
			ALLEGRO_BITMAP *bullet_bmp = al_load_bitmap( "Alien_alt2.png" );
			bullet->set_health( 0 + LEVEL );
			bullet->setDY( 1.5 );
			bullet->setBitmap( bullet_bmp );
		}
		
		
		bullet->set_bullet_type( bullet_type );
		
		enemies.push_back( *bullet );
	}
	else if (BOSS_ACTIVE != true)
	{
		{
			inBoss = true;
			al_stop_samples();
			al_play_sample(boss, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
			al_play_sample(warning, 0.75, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
			Enemy *bullet = new Enemy( ( SCREEN_W / 2 - BOSS_SIZE / 2 ) , -( BOSS_SIZE ), 0, 1 );
			
			
			ALLEGRO_BITMAP *bullet_bmp = al_load_bitmap( "Mayank_1.png" );
			bullet->setBitmap( bullet_bmp );
			bullet->set_health( BOSS_HEALTH + 50 * LEVEL );
			HEALTH_SET = true;
			
			
			bullet->set_bullet_type( 3 );
			enemies.push_back( *bullet );
			
			BOSS_ACTIVE = true;
			BOSS_CHECK = false;
		}
	}
}

void circle_bullets( int enemy_number, bool atk )
{
	int center_x = enemies[ enemy_number ].getX() + ( ENEMY_SIZE / 2 );
	int center_y = enemies[ enemy_number ].getY() + ( ENEMY_SIZE / 2 );
	float dx;
	float dy;
	int NUM_OF_BULLET = 0;
	if( atk )
	{
		if (enemies[enemy_number].get_bullet_type() == 3 )
		{
			NUM_OF_BULLET = 7 + LEVEL;
		}
		else { NUM_OF_BULLET = 4 + LEVEL; }
	}
	else 
	{
		if (enemies[enemy_number].get_bullet_type() == 3 )
		{
			NUM_OF_BULLET = 25;
		}
		else { NUM_OF_BULLET = 15; }
	}
	Bullet *bullet;// = new Bullet( center_x, center_y, dx, dy );
	for( int i = 0; i < NUM_OF_BULLET; i++ )
	{
		dx = (float)rand()/(float)RAND_MAX * 5 - 2.5;
		dy = (float)rand()/(float)RAND_MAX * 5 - 2.5;
		
		if( dx == 0 && dy == 0 ) dy++;
		
		bullet = new Bullet( center_x, center_y, dx, dy );
		
		ALLEGRO_BITMAP *bullet_bmp = al_load_bitmap( "enemy_bullet_1.png" );
		bullet->setBitmap( bullet_bmp );
		
		bullet->setBitmap( bullet_bmp );
		
		al_set_target_bitmap( bullet->getBitmap() );
		if( atk ){
			//al_clear_to_color( al_map_rgb( 255, 0, 0 ) );
		}
		else {
			ALLEGRO_BITMAP *bullet_bmp = al_load_bitmap( "debris.png" );
			bullet->setBitmap( bullet_bmp );
			bullet->set_attack();
			bullet->set_counter();
		}
		enemy_bullets.push_back( *bullet );	
	}
}

void straight_bullet( int enemy_number )
{
	int center_x, center_y;
	if( enemies[ enemy_number ].get_bullet_type() == 3 )
	{
		center_x = enemies[ enemy_number ].getX() + ( BOSS_SIZE / 2 );
		center_y = enemies[ enemy_number ].getY() + ( BOSS_SIZE * .9 );
	}
	else
	{
		center_x = enemies[ enemy_number ].getX() + ( ENEMY_SIZE / 2 );
		center_y = enemies[ enemy_number ].getY() + ( ENEMY_SIZE / 2 );
	}
	
	float dx;
	float dy;
	float length;
	
	dx = character->getX() + HITBOX - center_x;
	dy = character->getY() + HITBOX - center_y;
	
	length = sqrt ( pow( dx, 2 ) + pow( dy, 2 ) );
	
	int multiple = 5;
	length = length / multiple;
	
	Bullet *bullet = new Bullet( center_x, center_y, dx / length, dy / length );
	
	ALLEGRO_BITMAP *bullet_bmp = al_load_bitmap( "enemy_bullet_2.png" );
	bullet->setBitmap( bullet_bmp );
	
	bullet->setBitmap( bullet_bmp );
	
	enemy_bullets.push_back( *bullet );	
}

void lazer( int enemy_number )
{
	int center_x, center_y;
	if( enemies[ enemy_number ].get_bullet_type() == 3 )
	{
		center_x = enemies[ enemy_number ].getX() + ( BOSS_SIZE / 2 );
		center_y = enemies[ enemy_number ].getY() + ( BOSS_SIZE * .9 );
	}
	else
	{
		center_x = enemies[ enemy_number ].getX() + ( ENEMY_SIZE / 2 );
		center_y = enemies[ enemy_number ].getY() + ( ENEMY_SIZE / 2 );
	}
	
	float dx;
	float dy;
	float length;
	
	dx = character->getX() + HITBOX - center_x;
	dy = character->getY() + HITBOX - center_y;
	
	length = sqrt ( pow( dx, 2 ) + pow( dy, 2 ) );
	
	int multiple = 5;
	length = length / multiple;
	
	Bullet *bullet = new Bullet( center_x, center_y, dx / length, dy / length );
	
	ALLEGRO_BITMAP *bullet_bmp = al_load_bitmap( "lazer.png" );
	bullet->setBitmap( bullet_bmp );
	
	bullet->setBitmap( bullet_bmp );

	
	enemy_bullets.push_back( *bullet );	
}

void remove_boss_bullets( void )
{
	for( int k = 0; k < enemy_bullets.size(); k++ )
	{
		enemy_bullets[k].setY(SCREEN_H + 500);
	}
}

void move_enemies( void )
{
	for( int i = 0; i < enemies.size(); i++ )
	{
		if (enemies[i].get_bullet_type() != 3)
		{
			enemies[i].changeY( enemies[i].getDY() );
			
			if( enemies[i].ready_to_fire() == 1 )
			{
				enemies[i].set_fire_zero();
				switch( enemies[i].get_bullet_type() )
				{
				case 0: 
					circle_bullets( i, true );
					break;
				case 1:
					straight_bullet( i );
					break;
				}
			}
		}
		else
		{
			if ( !HEALTH_SET )
			{
				HEALTH_SET = true;
				enemies[i].set_health( BOSS_HEALTH);
			}
			
			int y = enemies[i].getY();
			int x = enemies[i].getX();
			
			if (y <= 0) { Y_CHANGE = 2; }
			else if (y > SCREEN_H / 3) { Y_CHANGE = -2; }
			
			if (x <= 0) { X_CHANGE = 2; }
			else if (x >= SCREEN_W - BOSS_SIZE) { X_CHANGE = -2; }
			
			if (TIMER >= 100 ) 
			{
				
				Y_CHANGE = ((rand() % 3) - (rand() % 3)) ;
				
				X_CHANGE = ( (rand() % 3) - (rand() % 3) );
				
				TIMER = 0;
				CHARGE++;
				
			}
			else { TIMER++; }
			
			enemies[i].changeY( Y_CHANGE );
			enemies[i].changeX( X_CHANGE );
			
			if( enemies[i].ready_to_fire() == 1  && CHARGE > 10 )
			{
				enemies[i].setBitmap( al_load_bitmap( "Mayank_2.png" ) );
				if (CHARGE == (200 + 25*LEVEL) )
				{
					enemies[i].set_fire_zero();
					enemies[i].setBitmap (al_load_bitmap( "Mayank_1.png" ) );
					CHARGE = 0;
				}
				CHARGE++;
				Y_CHANGE = 0;
				X_CHANGE = 0;
				if ( CHARGE % 5 == 0 ) { circle_bullets( i, true ); }
				lazer( i );
				
			}
			else if (enemies[i].ready_to_fire() == 1 )
			{
				enemies[i].set_fire_zero();
				circle_bullets( i, true );
				circle_bullets( i, true );
				circle_bullets( i, true );
				circle_bullets( i, true );
				straight_bullet( i );
			}
			
			
		}
	}
}

int playMusic(int t)
{
	if (inProgress)
	{
		al_stop_samples();
	}

	if(!track1)
		track1 = al_load_sample( "love.ogg" );
	if(!track2)
		track2 = al_load_sample( "hope.ogg" );
	if(!track3)
		track3 = al_load_sample( "hey.ogg" );
	if(!track4)
		track4 = al_load_sample( "harmony.ogg" );

	switch (t)
	{
	case 0:
		al_play_sample(track1, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
		music = true;
		break;
	case 1:
		al_play_sample(track2, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
		music = true;
		break;
	case 2:
		al_play_sample(track3, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
		music = true;
		break;
	case 3:
		al_play_sample(track4, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
		music = true;
		break;
	}
	inProgress = true;
}

void init( )
{
	cout << "Level:" << LEVEL << "\t\tScore: " << SCORE << endl;
	SCORE = 0;
	LEVEL = 1;
	HEALTH = 110;
	char_hit();
	TIMER = 0;
	
	BOSS_CHECK = false;
	BOSS_ACTIVE = false;
	
	enemies.clear();
	enemy_bullets.clear();
	
	character->setX( SCREEN_W / 2 );
	character->setY( SCREEN_H - CHAR_SIZE - 10 );
	
	char_hitbox->setX( character->getX() + HITBOX );
	char_hitbox->setY( character->getY() + HITBOX );
	
	dead = false;
}

int main( int argc, char **argv )
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	
	bool redraw = true;
	bool key[11] = { false, false, false, false, false, false, false, false, false, false, false };

	srand( time( NULL ) );	

	// character object (spot on screen, direction moving ) and hitbox
	Bullet *c = new Bullet( SCREEN_W / 2.0, SCREEN_H - ( CHAR_SIZE + 10 ), 0, 0 );
	character = c;
	
	Bullet *c_hitbox = new Bullet( character->getX() + HITBOX, character->getY() + HITBOX, 0, 0 );
	char_hitbox = c_hitbox;
	

	//initialize allegro
	al_init();
	
	//initialize the audio
	al_install_audio();
	al_init_acodec_addon();
	if ( !al_reserve_samples(8) ) {
		cout << "failed to play" << endl;
		return -1;
	}
	cout << "Loading sounds..." << endl;
	if(!warning)
		warning = al_load_sample( "warning.ogg" );
	if(!dest1)
		dest1 = al_load_sample( "tan01.wav" );
	if(!dest2)
		dest2 = al_load_sample( "tan00.wav" );
	if(!boss)
		boss = al_load_sample( "baws.ogg" );
	if (!bdeath)
		bdeath = al_load_sample( "bdead.wav" );
	playMusic(track);

	//initialize grafics
	al_init_image_addon();
	
	//initialize keyboard controls
	al_install_keyboard();

	//create allegro related objects
	timer = al_create_timer( 1.0 / FPS );
	display = al_create_display( SCREEN_W, SCREEN_H );

	//set bitmaps
	ALLEGRO_BITMAP *img = al_load_bitmap( "Spaceship.png" );
	character->setBitmap( img );
	
	ALLEGRO_BITMAP *img_hitbox = al_load_bitmap( "etama.png" );
	char_hitbox->setBitmap( img_hitbox );
	


	//color bitmaps	
	
	ALLEGRO_BITMAP *background = al_load_bitmap( "space.png" );

	al_set_target_bitmap( al_get_backbuffer( display ) );

	event_queue = al_create_event_queue();

	al_register_event_source( event_queue, al_get_display_event_source( display ) ) ;
	al_register_event_source( event_queue, al_get_timer_event_source( timer ) );
	al_register_event_source( event_queue, al_get_keyboard_event_source() );

	
	
	al_draw_bitmap( background, 0, 0, 0 );
	
	al_flip_display();
	
	al_start_timer( timer );


	//background stars
	vector<Bullet> stars;
	for( int i = 0; i < 50; i++ )
	{
		Bullet *bullet = new Bullet( rand() % SCREEN_W, rand() % SCREEN_H, 0, ( rand() % 2 ) + 1 );
		
		int bullet_size = rand() % 5;
		ALLEGRO_BITMAP *bullet_bmp = al_create_bitmap( bullet_size, bullet_size );
		bullet->setBitmap( bullet_bmp );
		
		al_set_target_bitmap( bullet->getBitmap() );
		al_clear_to_color( al_map_rgb( 255,255,255 ) );
		
		stars.push_back( *bullet );
	}

	//vector of bullets character fires
	vector<Bullet> bullets;
	vector<Bullet> newBullets;
	
	vector<Bullet> newEnemy_bullets;
	
	int fire_delay = 0;
	int enemy_counter = 0;
	
	///////////////////////
	//set health bar
	
	Bullet *health = new Bullet( 10, 10, 0, 0 );
	HEALTHBAR = health;
	
	ALLEGRO_BITMAP *health_bmp = al_create_bitmap( 100, 30 );
	HEALTHBAR->setBitmap( health_bmp );
	
	al_set_target_bitmap( HEALTHBAR->getBitmap() );
	al_clear_to_color( al_map_rgb( 0, 122, 122 ) );
	
	// background_background
	health = new Bullet( 8, 8, 0, 0 );
	HEALTHBAR_BACKGROUND_BACKGROUND = health;
	
	health_bmp = al_create_bitmap( 104, 34 );
	HEALTHBAR_BACKGROUND_BACKGROUND->setBitmap( health_bmp );
	
	al_set_target_bitmap( HEALTHBAR_BACKGROUND_BACKGROUND->getBitmap() );
	al_clear_to_color( al_map_rgb( 255, 255, 255 ) );
	
	//background
	health = new Bullet( 10, 10, 0, 0 );
	HEALTHBAR_BACKGROUND = health;
	
	health_bmp = al_create_bitmap( 100, 30 );
	HEALTHBAR_BACKGROUND->setBitmap( health_bmp );
	
	al_set_target_bitmap( HEALTHBAR_BACKGROUND->getBitmap() );
	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );
	
	
	
	
	bool hit = false;
	
	bool doexit = false;
	
	cout << "Good luck!" << endl;
	//loop for game
	while( !doexit )
	{
		if (!music)
		{
			playMusic(track);
		}
		
		ALLEGRO_EVENT ev;
		al_wait_for_event( event_queue, &ev );

		if( ev.type == ALLEGRO_EVENT_TIMER )
		{	
			CHAR_FOCUS = false;
			if( key[ KEY_LSHIFT ] )
			{
				CHAR_FOCUS = true;
			}
			if( key[ KEY_UP ] )
			{
				if( character->getY() > CHAR_SIZE )
				{
					if( CHAR_FOCUS )
					{
						character->changeY( -(CHAR_SPEED/2) );
						char_hitbox->changeY( -(CHAR_SPEED / 2 ) );
					}
					else
					{
						character->changeY( -CHAR_SPEED );
						char_hitbox->changeY( -CHAR_SPEED );
					}
				}
			}
			else if( key[ KEY_DOWN] )
			{
				if( character->getY() < SCREEN_H - CHAR_SIZE )
				{
					if( CHAR_FOCUS )
					{
						character->changeY( (CHAR_SPEED/2) );
						char_hitbox->changeY( (CHAR_SPEED/2) );
					}
					else
					{
						character->changeY( CHAR_SPEED );
						char_hitbox->changeY( CHAR_SPEED );
					}
				}
			}
			if( key[ KEY_LEFT ] )
			{
				if( character->getX() > 0 )
				{
					if ( CHAR_FOCUS )
					{
						character->changeX ( -(CHAR_SPEED / 2) );
						char_hitbox->changeX ( -(CHAR_SPEED / 2) );
					}
					else
					{
						character->changeX( -CHAR_SPEED );
						char_hitbox->changeX( -CHAR_SPEED );
					}
				}
			}
			else if( key[ KEY_RIGHT ] )
			{
				if( character->getX() < SCREEN_W - CHAR_SIZE )
				{
					if( CHAR_FOCUS )
					{
						character->changeX( (CHAR_SPEED/2) );
						char_hitbox->changeX( (CHAR_SPEED/2) );
					}
					else
					{					
						character->changeX( CHAR_SPEED );
						char_hitbox->changeX( CHAR_SPEED );
					}
				}
			}
			if( key[ KEY_1 ] )
			{
				track = 0;
			}
			else if( key[ KEY_2 ] )
			{
				track = 1;
			}
			else if( key[ KEY_3 ] )
			{
				track = 2;
			}
			else if( key[ KEY_4 ] )
			{
				track = 3;
			}
			if( (key[ KEY_SPACE ]) || (key[ KEY_Z ]) )
			{
				
				fire_delay++;
				
				int DELAY;
				if( !CHAR_FOCUS ) { DELAY = 10; }
				else { DELAY = 5; }
				
				if( fire_delay > DELAY )
				{
					int bullet_dy = -5;
					int bullet_dx = 0;
					if( key[ KEY_UP ] )
					{
						if( !CHAR_FOCUS )
						bullet_dy = -7;
					}
					else if( key[ KEY_DOWN ] )
					{
						if ( !CHAR_FOCUS )
						bullet_dy = -3;
					}
					if( key[ KEY_LEFT ] )
					{
						bullet_dx = 0;
					}
					else if ( key[ KEY_RIGHT ] )
					{
						bullet_dx = 0;
					}
					if( !CHAR_FOCUS )
					{
						Bullet *bullet = new Bullet( character->getX() + ( 0.5 * CHAR_SIZE ) - ( 0.5 * BULLET_SIZE ), character->getY(), bullet_dx, bullet_dy );
						Bullet *bulletL = new Bullet( character->getX() + ( 0.5 * CHAR_SIZE ) - ( 0.5 * BULLET_SIZE ), character->getY(), -1.5, bullet_dy );
						Bullet *bulletR = new Bullet( character->getX() + ( 0.5 * CHAR_SIZE ) - ( 0.5 * BULLET_SIZE ), character->getY(), 1.5, bullet_dy );
						
						ALLEGRO_BITMAP *bullet_bmp = al_load_bitmap( "bullet_2.png" );
						ALLEGRO_BITMAP *bulletL_bmp = al_load_bitmap( "bullet_2.png" );
						ALLEGRO_BITMAP *bulletR_bmp = al_load_bitmap( "bullet_2.png" );
						
						bullet->setBitmap( bullet_bmp );
						bulletL->setBitmap( bulletL_bmp );
						bulletR->setBitmap( bulletR_bmp );
						
						bullets.push_back( *bullet );
						bullets.push_back( *bulletL );
						bullets.push_back( *bulletR );
						
						fire_delay = 0;
					}
					else
					{
						Bullet *bulletL = new Bullet( character->getX() + ( 0.20 * CHAR_SIZE) - ( 0.5 * BULLET_SIZE ), character->getY(), bullet_dx, bullet_dy );
						Bullet *bulletR = new Bullet( character->getX() + ( CHAR_SIZE - ( 0.20 * CHAR_SIZE ) ) - ( 0.5 * BULLET_SIZE ), character->getY(), bullet_dx, bullet_dy );
						
						ALLEGRO_BITMAP *bulletL_bmp = al_load_bitmap( "bullet_1.png" );
						ALLEGRO_BITMAP *bulletR_bmp = al_load_bitmap( "bullet_1.png" );
						
						bulletL->setBitmap( bulletL_bmp );
						bulletR->setBitmap( bulletR_bmp );
						
						bullets.push_back( *bulletL );
						bullets.push_back( *bulletR );
						
						fire_delay = 0;
					}
				}
			}
			
			//	TIMER DINGS
			
			for( int i = 0; i < bullets.size(); i++ )
			{
				bullets[ i ].changeY( bullets[i].getDY() );
				bullets[ i ].changeX( bullets[i].getDX() );
			}
			
			for( int i = 0; i < stars.size(); i++ )
			{
				stars[i].changeY( stars[i].getDY() );
				
				if( stars[i].getY() > SCREEN_H )
				{
					stars[i].setX( rand() % SCREEN_W );
					stars[i].setY( 1 );
					stars[i].setDY( ( rand() % 4 ) + 1 );
				}	
			}
			if (BOSS_ACTIVE != true) { enemy_counter++; }
			
			//add enemies
			if( enemy_counter > 51 - LEVEL )
			{
				add_enemy();
				enemy_counter = 0;
			}			
			
			for( int i = 0; i < enemies.size(); i++ )
			{
				enemies[i].prepare_to_fire( 10 );
				if (enemies[i].get_bullet_type() == 3 && HEALTH_SET != true) 
				{
					enemies[i].set_health(BOSS_HEALTH);
					HEALTH_SET = true;
				}
			}
			
			move_enemies();
			
			for( int i = 0; i < enemies.size(); i++ )
			{
				if( enemies[i].getY() > SCREEN_H )
				{
					al_destroy_bitmap( enemies[i].getBitmap() );
					enemies.erase(enemies.begin()+i);
				}
			}
			
			for( int i = 0; i < enemy_bullets.size(); i++ )
			{
				enemy_bullets[i].changeX( enemy_bullets[i].getDX() );
				enemy_bullets[i].changeY( enemy_bullets[i].getDY() );
			}
			
			//for getting rid of old bullets
			if( bullets.size() > 0 && bullets[0].getY() < 0 )
			{
				for( int i = 0; i < bullets.size(); i++ )
				{
					if( bullets[i].getY() > 0 )
					{
						//newBullets.push_back( bullets[i] );
					}
					else
					{
						al_destroy_bitmap( bullets[i].getBitmap() );
						bullets.erase(bullets.begin()+i);
					}
				}
			}
			//
			// player bullet / enemy collision
			//
			//
			for( int i = 0; i < bullets.size(); i++ )
			{
				for( int j = 0; j < enemies.size(); j++ )
				{
					int enemySize;
					switch( enemies[j].get_bullet_type() )
					{
					case 0:
						enemySize = ENEMY_C_SIZE;
						break;
					case 1:
						enemySize = ENEMY_S_SIZE;
						break;
					case 3:
						enemySize = BOSS_SIZE;
						break;
					}
					if( bullets[i].getX() + BULLET_SIZE > enemies[j].getX() &&
							bullets[i].getX() < enemies[j].getX() + enemySize &&
							bullets[i].getY() + BULLET_SIZE > enemies[j].getY() &&
							bullets[i].getY() < enemies[j].getY() + enemySize )
					{
						if (BOSS_ACTIVE) { SCORE++; }
						else {SCORE += 2; }
						if( SCORE > 1500 * ( LEVEL * LEVEL ) )
						{
							LEVEL++;
							if (BOSS_ACTIVE == false) { BOSS_CHECK = true; }
						}
						
						bullets[i].setY(-50);
						enemies[j].change_health( );
						if( enemies[j].get_health() == 0 )
						{
							SCORE += 100;
							if (BOSS_ACTIVE == true && enemies[j].get_bullet_type() == 3 ) 
							{
								BOSS_ACTIVE = false; 
								HEALTH = 100;
								ALLEGRO_BITMAP *health_bmp = al_create_bitmap( HEALTH, 30 );
								HEALTHBAR->setBitmap( health_bmp );
		
								al_set_target_bitmap( HEALTHBAR->getBitmap() );
								al_clear_to_color( al_map_rgb( 0, 122, 122 ) );
								TIMER = 0;
							}
							
							HEALTH_SET = false;
							switch( enemies[j].get_bullet_type() )
							{
							case 0:
								al_play_sample(dest1, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
								circle_bullets(j, false);
								break;
							case 1:
								al_play_sample(dest2, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
								circle_bullets(j, false);
								break;
							case 3:
								remove_boss_bullets();
								al_stop_samples();
								circle_bullets(j, false);
								circle_bullets(j, false);
								circle_bullets(j, false);
								playMusic(track);
								al_play_sample(bdeath, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
								inBoss = false;
								break;
							}
							enemies[j].setY(SCREEN_H + ENEMY_SIZE);
						}
					}
				}
			}
			if( enemy_bullets.size() > 1 )
			{
				bool current_hit = false;
				for( int i = 0; i < enemy_bullets.size(); i++ )
				{
					enemy_bullets[i].change_count( -2 );
					if( enemy_bullets[i].getY() < 0 || enemy_bullets[i].getY() > SCREEN_H || 
							enemy_bullets[i].getX() < 0 || enemy_bullets[i].getX() > SCREEN_W )
					{
						current_hit = true;
						al_destroy_bitmap(enemy_bullets[i].getBitmap());
						enemy_bullets.erase(enemy_bullets.begin()+i);
					}
					else
					{
						if( !enemy_bullets[i].isAttack)
						{
							if( enemy_bullets[i].get_count() < 0 ) 
							{
								current_hit = true;
								al_destroy_bitmap(enemy_bullets[i].getBitmap());
								enemy_bullets.erase(enemy_bullets.begin()+i);
							}
						}
						else if( enemy_bullets[i].getX() + BULLET_SIZE > character->getX() + HITBOX && 
								enemy_bullets[i].getX() < character->getX() + CHAR_SIZE - HITBOX )
						{
							if( enemy_bullets[i].getY() + BULLET_SIZE > character->getY() + HITBOX && 
									enemy_bullets[i].getY() < character->getY() + CHAR_SIZE - HITBOX )
							{
								hit = true;
								current_hit = true;
								al_destroy_bitmap(enemy_bullets[i].getBitmap());
								enemy_bullets.erase(enemy_bullets.begin()+i);
								char_hit();
							}
						}
					}
					current_hit = false;
				}
			}
			redraw = true;
		}
		else if( ev.type == ALLEGRO_EVENT_KEY_DOWN )
		{
			switch( ev.keyboard.keycode )
			{
			case ALLEGRO_KEY_UP:
				key[ KEY_UP ] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				key[ KEY_DOWN ] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[ KEY_RIGHT ] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				key[ KEY_LEFT ] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				key[ KEY_SPACE ] = true;
				break;
			case ALLEGRO_KEY_LSHIFT:
				key[ KEY_LSHIFT ] = true;
				break;
			case ALLEGRO_KEY_Z:
				key[ KEY_Z ] = true;
				break;
			case ALLEGRO_KEY_1:
				key[ KEY_1 ] = true;
				break;
			case ALLEGRO_KEY_2:
				key[ KEY_2 ] = true;
				break;
			case ALLEGRO_KEY_3:
				key[ KEY_3 ] = true;
				break;
			case ALLEGRO_KEY_4:
				key[ KEY_4 ] = true;
				break;
			}
		}
		else if( ev.type == ALLEGRO_EVENT_KEY_UP )
		{
			switch( ev.keyboard.keycode )
			{
			case ALLEGRO_KEY_UP:
				key[ KEY_UP ] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				key[ KEY_DOWN ] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[ KEY_RIGHT ] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				key[ KEY_LEFT ] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				key[ KEY_SPACE ] = false;
				break;
			case ALLEGRO_KEY_LSHIFT:
				key[ KEY_LSHIFT ] = false;
				break;
			case ALLEGRO_KEY_Z:
				key[ KEY_Z ] = false;
				break;
			case ALLEGRO_KEY_1:
				key[ KEY_1 ] = false;
				music = false;
				break;
			case ALLEGRO_KEY_2:
				key[ KEY_2 ] = false;
				music = false;
				break;
			case ALLEGRO_KEY_3:
				key[ KEY_3 ] = false;
				music = false;
				break;
			case ALLEGRO_KEY_4:
				key[ KEY_4 ] = false;
				music = false;
				break;
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}
		
		if( redraw && al_is_event_queue_empty( event_queue ) )
		{
			redraw = false;
			
			if( dead )
			{
				init();
				if(inBoss)
					{
						al_stop_samples();
						playMusic(track);
					}
			}
			
			al_set_target_bitmap( al_get_backbuffer( display ) );
			
			if( hit )
			{
				al_clear_to_color( al_map_rgb( 150,0,0 ) );	
				hit = false;
			}
			else
			{
				
				al_draw_bitmap( background, 0, 0, 0 );
			}
			
			
			for( int i = 0; i < bullets.size(); i++ )
			{
				al_draw_bitmap( bullets[i].getBitmap(), bullets[i].getX(), bullets[i].getY(), 0 );
			}
			
			for( int i = 0; i < stars.size(); i++ )
			{
				al_draw_bitmap( stars[i].getBitmap(), stars[i].getX(), stars[i].getY(), 0 );
			}
			
			al_draw_bitmap( character->getBitmap(), character->getX(), character->getY(), 0 );
			al_draw_bitmap( char_hitbox->getBitmap(), char_hitbox->getX(), char_hitbox->getY(), 0 );
			
			
			for( int i = 0; i < enemy_bullets.size(); i++ )
			{
				al_draw_bitmap( enemy_bullets[i].getBitmap(), enemy_bullets[i].getX(), enemy_bullets[i].getY(), 0 );
			}
			
			//add enemies
			for( int i = 0; i < enemies.size(); i++ )
			{
				al_draw_bitmap( enemies[i].getBitmap(), enemies[i].getX(), enemies[i].getY(), 0 );
			}
			
			al_draw_bitmap( HEALTHBAR_BACKGROUND_BACKGROUND->getBitmap(), HEALTHBAR_BACKGROUND_BACKGROUND->getX(), HEALTHBAR_BACKGROUND_BACKGROUND->getY(), 0);
			al_draw_bitmap( HEALTHBAR_BACKGROUND->getBitmap(), HEALTHBAR_BACKGROUND->getX(), HEALTHBAR_BACKGROUND->getY(), 0 );
			al_draw_bitmap( HEALTHBAR->getBitmap(), HEALTHBAR->getX(), HEALTHBAR->getY(), 0 );
			
			al_flip_display();
		}
	}

	al_destroy_bitmap( character->getBitmap() );
	al_destroy_display( display );
	switch (track)
	{
	case 0:
		al_destroy_sample(track1);
		break;
	case 1:
		al_destroy_sample(track2);
		break;
	case 2:
		al_destroy_sample(track3);
		break;
	case 3:
		al_destroy_sample(track4);
		break;
	case 4:
		al_destroy_sample(warning);
		break;
	}
	al_destroy_event_queue( event_queue );

	cout << "Level:" << LEVEL << "\t\tScore: " << SCORE << endl;
	
	return 0;

}
