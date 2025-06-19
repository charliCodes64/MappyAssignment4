#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "SpriteSheet.h"

Sprite::Sprite()
{
	image = NULL;
}
Sprite::~Sprite()
{
	al_destroy_bitmap(image);
}
void Sprite::InitSprites(int width, int height)
{
	x = 180;
	y = 120;

	maxFrame = 6;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 199;
	frameHeight = 180;
	animationColumns = 6;
	animationDirection = 1;
 	image = al_load_bitmap("spinnyAll.bmp");
	al_convert_mask_to_alpha(image, al_map_rgb(255, 255, 255));
}

void Sprite::UpdateSprites(int width, int height, int dir)
{
    int oldx = x;
    int oldy = y;

    if (y < 0) {
        y = 0;
    }
	if (dir == 1) { //right key lab 11
		animationDirection = 1;
		x += 5;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > 6)
				curFrame = 1;
		}
	}
	else if (dir == 0) { //left key
		animationDirection = 0;
		x -= 5;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > 6)
				curFrame = 1;
		}
	}
	else if (dir == 3) { //up
		animationDirection = 3;
		y -= 5;
		if (++frameCount > frameDelay) {
			frameCount = 0;
			if (++curFrame > 6)
				curFrame = 1;
		}
	}
	else if (dir == 4) { //down
		animationDirection = 4;
		y += 5;
		if (++frameCount > frameDelay) {
			frameCount = 0;
			if (++curFrame > 6)
				curFrame = 1;
		}
	}
    if (++frameCount > frameDelay) {
        frameCount = 0;
        curFrame++;
        if (curFrame >= maxFrame)
            curFrame = 0;
    }
	if (animationDirection == 0)
	{
		if (collided(x + 38, y + frameHeight)) { //collision detection to the left
			x = oldx;
			y = oldy;
		}

	}
	else if (animationDirection == 1)
	{
		if (collided(x + frameWidth - 38, y + frameHeight)) { //collision detection to the right
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 3) {
		if (collided(x + frameWidth / 2, y + 32)) {
			x = oldx;
			y = oldy;
		}
	}
	else if (animationDirection == 4) {
		if (collided(x + frameWidth / 2, y + frameHeight - 32)) {
			x = oldx;
			y = oldy;
		}
	}
}

bool Sprite::CollisionEndBlock()
{
	if (endValue(x + frameWidth / 2 , y + frameHeight - 15))
		return true;
	else
		return false;
}

void Sprite::DrawSprites(int xoffset, int yoffset)
{
    int fx = (curFrame % animationColumns) * frameWidth;
    int fy = 0;//(curFrame / animationColumns)* frameHeight;

    float angle = 0.f;

    if (animationDirection == 0) {//left
         ALLEGRO_BITMAP* frame = al_create_sub_bitmap(image, fx, fy, frameWidth, frameHeight);
        al_draw_rotated_bitmap(frame, frameWidth / 2.0f, frameHeight / 2.0f, x - xoffset + frameWidth / 2, y - yoffset + frameHeight / 2, 3 * ALLEGRO_PI / 2, 0);
    }
    else if (animationDirection == 1) {//right
         ALLEGRO_BITMAP* frame = al_create_sub_bitmap(image, fx, fy, frameWidth, frameHeight);
        al_draw_rotated_bitmap(frame, frameWidth / 2.0f, frameHeight / 2.0f, x - xoffset + frameWidth / 2, y - yoffset + frameHeight / 2, ALLEGRO_PI / 2, 0);
    }
    else if (animationDirection == 3) {//up
         ALLEGRO_BITMAP* frame = al_create_sub_bitmap(image, fx, fy, frameWidth, frameHeight);
        al_draw_rotated_bitmap(frame, frameWidth / 2.0f, frameHeight / 2.0f, x - xoffset + frameWidth / 2, y - yoffset + frameHeight / 2, 0, 0);
    }
    else if (animationDirection == 4) {//down
         ALLEGRO_BITMAP* frame = al_create_sub_bitmap(image, fx, fy, frameWidth, frameHeight);
        al_draw_rotated_bitmap(frame, frameWidth / 2.0f, frameHeight / 2.0f, x - xoffset + frameWidth / 2, y - yoffset + frameHeight / 2, ALLEGRO_PI, 0);
    }
}
 