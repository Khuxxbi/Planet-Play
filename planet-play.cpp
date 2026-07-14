#include "splashkit.h"

const int SCREEN_SIZE = 600;
const int BOTTOM_GAP = 50; // gap between the ship and the bottom edge at the start
const int SPEED = 3; // pixels moved per frame
const string SHIP_NAME = "aquarii-v";

/**
 * Procedure: handle_input
 * Purpose: Reads the arrow keys and moves the ship one step per frame
 *
 * @param player_x the ship's horizontal position, updated in place
 * @param player_y the ship's vertical position, updated in place
 */
void handle_input(double &player_x, double &player_y)
{
    process_events();

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
}

/**
 * Procedure: update_game
 * Purpose: Moves the coin to a new random spot when the ship collects it,
 * then advances the coin's spin animation
 *
 * @param coin the coin sprite, updated in place
 * @param ship the ship bitmap, used for the collision check
 * @param player_x the ship's horizontal position
 * @param player_y the ship's vertical position
 */
void update_game(sprite coin, bitmap ship, double player_x, double player_y)
{
    if (sprite_bitmap_collision(coin, ship, player_x, player_y))
    {
        sprite_set_x(coin, rnd(SCREEN_SIZE - sprite_width(coin)));
        sprite_set_y(coin, rnd(SCREEN_SIZE - sprite_height(coin)));
    }

    update_sprite(coin);
}

/**
 * Procedure: draw_game
 * Purpose: Clears the screen and draws the ship and coin for this frame
 *
 * @param ship the ship bitmap
 * @param coin the coin sprite
 * @param player_x the ship's horizontal position
 * @param player_y the ship's vertical position
 */
void draw_game(bitmap ship, sprite coin, double player_x, double player_y)
{
    clear_screen(COLOR_BLACK);
    draw_bitmap(ship, player_x, player_y);
    draw_sprite(coin);
    refresh_screen(60);
}

int main()
{
    open_window("Planet Play - by Khubaib Ejaz", SCREEN_SIZE, SCREEN_SIZE);

    // Load the ship, coin, and coin animation from the resource bundle.
    load_resource_bundle("planet_play", "planet_play.txt");

    bitmap ship = bitmap_named(SHIP_NAME);

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
        handle_input(player_x, player_y);
        update_game(coin, ship, player_x, player_y);
        draw_game(ship, coin, player_x, player_y);
    }

    return 0;
}
