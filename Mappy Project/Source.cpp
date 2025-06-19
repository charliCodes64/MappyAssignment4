#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "SpriteSheet.h"
#include "mappy_A5.h"
#include <iostream>
#include <sstream>
//Charlton Infinger
using namespace std;

int collided(int x, int y);  //Tile Collision
bool endValue(int x, int y); //End Block with the User Value = 8
int main(void)
{
	const int WIDTH = 900;
	const int HEIGHT = 480;
	bool keys[] = { false, false, false, false, false };
	enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
	//variables
	bool done = false;
	bool render = false;
	bool levelOver = false;
	bool gameOver = false;
	//Player Variable
	Sprite player;
	int nextLev = 1;
	int startTime = 60;
	char name[80];
	//sprintf(name, "map%i", nextLev);


	//allegro variable
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer;
	ALLEGRO_TIMER* secCount;//timer for countdown
	//program init
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if (!display)										//test display object
		return -1;

	//addon init
	al_install_keyboard();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_FONT* font24 = al_load_font("GoldenAge.ttf", 24, 2);

	player.InitSprites(WIDTH, HEIGHT);

	int xOff = 0;
	int yOff = 0;
	if (MapLoad("map1.FMP", 1))//loads in map 1 as first map
		return -5;

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);
	secCount = al_create_timer(1.0);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_timer_event_source(secCount));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);
	al_start_timer(secCount);
	//draw the background tiles
	MapDrawBG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1);

	//draw foreground tiles
	MapDrawFG(xOff, yOff, 0, 0, WIDTH - 1, HEIGHT - 1, 0);
	player.DrawSprites(0, 0);
	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));

    std:cout << "try32" << std::endl;
	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
				if (ev.timer.source == secCount) {
					startTime--;
					if (startTime <= 0) {
						done = true;
						gameOver = true;
				}
			}
				if (levelOver == true) {
					nextLev++;
					startTime = 60;
					levelOver = false;
					sprintf(name, "map%i.FMP", nextLev);
					if (MapLoad(name, 1)) exit(0);
					if (nextLev < 4) {//setting spawn point
						player.getX();
						player.getY();
					}
				}

			render = true;
			if (keys[UP])
				player.UpdateSprites(WIDTH, HEIGHT, 3);
			else if (keys[DOWN])
				player.UpdateSprites(WIDTH, HEIGHT, 4);
			else if (keys[LEFT])
				player.UpdateSprites(WIDTH, HEIGHT, 0);
			else if (keys[RIGHT])
				player.UpdateSprites(WIDTH, HEIGHT, 1);
			else
				player.UpdateSprites(WIDTH, HEIGHT, 5);

			if (player.CollisionEndBlock()) {//setting levelOver to true
				cout << "Hit an End Block\n";
				levelOver = true;
			}

		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;

			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}
		}
		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;
			//al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(75, 80, 100));
 
			//update the map scroll position
			xOff = player.getX() + player.getWidth() - WIDTH / 2;
			yOff = player.getY() + player.getHeight() - HEIGHT / 2;

			//avoid moving beyond the map edge
			if (xOff < 0) xOff = 0;

			if (xOff > (mapwidth * mapblockwidth - WIDTH))
				xOff = mapwidth * mapblockwidth - WIDTH;
			if (yOff < 0)
				yOff = 0;
			if (yOff > (mapheight * mapblockheight - HEIGHT))
				yOff = mapheight * mapblockheight - HEIGHT;
			MapUpdateAnims();
			//draw the background tiles
			MapDrawBG(xOff, yOff, 0, 0, WIDTH, HEIGHT);
			//draw foreground tiles
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, HEIGHT, 0);
			//jump = player.jumping(jump, JUMPIT);
			if (gameOver == true) {
				al_draw_textf(font24, al_map_rgb(250, 50, 50), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "OUT OF TIME TRY AGIAN!!");
				al_flip_display();
				al_rest(10);
				done = true;
			}
			player.DrawSprites(xOff, yOff);
			al_draw_textf(font24, al_map_rgb(75, 80, 100), 20, 2, ALLEGRO_ALIGN_LEFT, "Time: %d", startTime);//timer display

			if (levelOver && nextLev == 3)
			{
				al_draw_textf(font24, al_map_rgb(50, 50, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTER, "You Won!!");
				al_flip_display();
				al_rest(10);
				done = true;
			}
			//if (winner == true) {//win message 10sec delay
			//	al_flip_display();
			//	al_rest(10);
			//	done = true;
			//}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}
 
	MapFreeMem();
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						//destroy our display object

	return 0;
}



int collided(int x, int y)
{
	BLKSTR* blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return blockdata->tl;
}

bool endValue(int x, int y)
{

	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 8)
	{
		return true;
	}
	else
		return false;
}