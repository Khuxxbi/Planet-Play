#include "splashkit.h"

int main()
{
    const int SCREEN_SIZE = 600;
    const int BOTTOM_GAP = 50; // gap between the ship and the bottom edge at the start
    const int SPEED = 3; // pixels moved per frame

    open_window("Planet Play - by Khubaib Ejaz", SCREEN_SIZE, SCREEN_SIZE);

    // Load the ship, coin, and coin animation from the resource bundle.
    load_resource_bundle("planet_play", "planet_play.txt");

    bitmap ship = bitmap_named("aquarii-v");

    // Start the ship centred horizontally, near the bottom of the screen.
    double player_x = (SCREEN_SIZE - bitmap_width(ship)) / 2;
    double player_y = SCREEN_SIZE - bitmap_height(ship) - BOTTOM_GAP;

    // Create the spinning coin and drop it at a random spot on the screen.
    sprite coin = create_sprite("gold_coin", "coin_animation");
    sprite_start_animation(coin, "spin");
    sprite_set_x(coin, rnd(SCREEN_SIZE - sprite_width(coin)));
    sprite_set_y(coin, rnd(SCREEN_SIZE - sprite_height(coin)));

    while (!quit_requested())
    {
        process_events();

        // One direction per frame, based on the arrow keys.
        if (key_down(UP_KEY))
        {
            player_y -= SPEED;
        }
        else if (key_down(DOWN_KEY))
        {
            player_y += SPEED;
        }
        else if (key_down(RIGHT_KEY))
        {
            player_x += SPEED;
        }
        else if (key_down(LEFT_KEY))
        {
            player_x -= SPEED;
        }

        // When the ship touches the coin, move the coin to a new random spot.
        if (sprite_bitmap_collision(coin, ship, player_x, player_y))
        {
            sprite_set_x(coin, rnd(SCREEN_SIZE - sprite_width(coin)));
            sprite_set_y(coin, rnd(SCREEN_SIZE - sprite_height(coin)));
        }

        update_sprite(coin);

        clear_screen(COLOR_BLACK);
        draw_bitmap(ship, player_x, player_y);
        draw_sprite(coin);
        refresh_screen(60);
    }

    return 0;
}