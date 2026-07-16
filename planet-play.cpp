#include "splashkit.h"

const int SCREEN_SIZE = 600;
const double THRUST = 0.5; // speed added each frame the up or down key is held
const double ROTATE_SPEED = 5; // degrees the ship turns per frame
const string SHIP_NAME = "aquarii-v";

struct player_data
{
    sprite ship;
    int score;
};

struct space_game_data
{
    player_data player;
    sprite coin;
};

/**
 * Procedure: load_resources
 * Purpose: Loads the ship, coin, and coin animation from the resource bundle
 */
void load_resources()
{
    load_resource_bundle("planet_play", "planet_play.txt");
}

/**
 * Procedure: give_random_position
 * Purpose: Moves a sprite to a random spot that is fully on screen
 *
 * @param sprt the sprite to reposition, updated in place
 */
void give_random_position(sprite &sprt)
{
    sprite_set_x(sprt, rnd(SCREEN_SIZE - sprite_width(sprt)));
    sprite_set_y(sprt, rnd(SCREEN_SIZE - sprite_height(sprt)));
}

/**
 * Function: new_coin
 * Purpose: Creates a spinning coin at a random spot on the screen
 *
 * @return the new coin sprite
 */
sprite new_coin()
{
    sprite coin = create_sprite("gold_coin", "coin_animation");
    sprite_start_animation(coin, "spin");
    give_random_position(coin);

    return coin;
}

/**
 * Procedure: load_player
 * Purpose: Creates the ship in the centre of the screen and clears the score
 *
 * @param player the player to set up, updated in place
 */
void load_player(player_data &player)
{
    player.score = 0;
    player.ship = create_sprite(SHIP_NAME);
    sprite_set_x(player.ship, (SCREEN_SIZE - sprite_width(player.ship)) / 2);
    sprite_set_y(player.ship, (SCREEN_SIZE - sprite_height(player.ship)) / 2);
}

/**
 * Procedure: load_game
 * Purpose: Sets up the player and the first coin ready for a new game
 *
 * @param game the game to set up, updated in place
 */
void load_game(space_game_data &game)
{
    load_player(game.player);
    game.coin = new_coin();
}

/**
 * Procedure: end_game
 * Purpose: Frees the sprites the game created
 *
 * @param game the game being closed down
 */
void end_game(space_game_data &game)
{
    free_sprite(game.player.ship);
    free_sprite(game.coin);
}

/**
 * Procedure: handle_input
 * Purpose: Thrusts the ship forwards or backwards with the up and down keys,
 * and turns it with the left and right keys
 *
 * @param player the player to move, updated in place
 */
void handle_input(player_data &player)
{
    process_events();

    // A sprite's velocity is applied in the direction it is facing,
    // so changing dy thrusts the ship forwards or backwards.
    if (key_down(UP_KEY))
    {
        sprite_set_dy(player.ship, sprite_dy(player.ship) - THRUST);
    }
    if (key_down(DOWN_KEY))
    {
        sprite_set_dy(player.ship, sprite_dy(player.ship) + THRUST);
    }
    if (key_down(RIGHT_KEY))
    {
        sprite_set_rotation(player.ship, sprite_rotation(player.ship) + ROTATE_SPEED);
    }
    if (key_down(LEFT_KEY))
    {
        sprite_set_rotation(player.ship, sprite_rotation(player.ship) - ROTATE_SPEED);
    }
}

/**
 * Procedure: wrap_sprite_offscreen
 * Purpose: Moves a sprite to the opposite edge once it has left the screen
 *
 * @param sprt the sprite to wrap, updated in place
 */
void wrap_sprite_offscreen(sprite &sprt)
{
    if (sprite_offscreen(sprt))
    {
        if (sprite_x(sprt) + sprite_width(sprt) < 0)
        {
            sprite_set_x(sprt, SCREEN_SIZE + sprite_width(sprt));
        }
        else if (sprite_x(sprt) - sprite_width(sprt) > SCREEN_SIZE)
        {
            sprite_set_x(sprt, -sprite_width(sprt));
        }

        if (sprite_y(sprt) + sprite_height(sprt) < 0)
        {
            sprite_set_y(sprt, SCREEN_SIZE + sprite_height(sprt));
        }
        else if (sprite_y(sprt) - sprite_height(sprt) > SCREEN_SIZE)
        {
            sprite_set_y(sprt, -sprite_height(sprt));
        }
    }
}

/**
 * Procedure: update_game
 * Purpose: Moves the sprites, wraps the ship at the edges, and scores a
 * point when the ship reaches the coin
 *
 * @param game the game to update, updated in place
 */
void update_game(space_game_data &game)
{
    update_sprite(game.player.ship);
    update_sprite(game.coin);

    wrap_sprite_offscreen(game.player.ship);

    if (sprite_collision(game.player.ship, game.coin))
    {
        game.player.score++;
        give_random_position(game.coin);
    }
}

/**
 * Procedure: draw_game
 * Purpose: Clears the screen and draws the score, coin, and ship
 *
 * @param game the game to draw
 */
void draw_game(const space_game_data &game)
{
    clear_screen(COLOR_BLACK);
    draw_text("Score " + to_string(game.player.score), COLOR_WHITE, 10, 10);
    draw_sprite(game.coin);
    draw_sprite(game.player.ship);
    refresh_screen(60);
}

int main()
{
    space_game_data game;

    open_window("Planet Play - by Khubaib Ejaz", SCREEN_SIZE, SCREEN_SIZE);

    load_resources();
    load_game(game);

    while (!quit_requested())
    {
        handle_input(game.player);
        update_game(game);
        draw_game(game);
    }

    end_game(game);

    return 0;
}