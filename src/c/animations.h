#pragma once
	
unsigned int animation_loops = 3;
	
void feed_animation(int *sprite, GBitmap *bitmap, BitmapLayer *status)
{
	*sprite = 1;
	
	bitmap = gbitmap_create_with_resource(RESOURCE_ID_SICK_SPRITE_1);
	bitmap_layer_set_alignment(status, GAlignCenter);
	//sleep(0.25);
}

void heal_animation(void)
{
	
}

void clean_animation(void)
{
	
}