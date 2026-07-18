#include "splashkit.h"
#include "splashkit-arrays.h"

const int SCREEN_SIZE = 600;
const double THRUST = 0.5; // speed added each frame the up or down key is held
const double ROTATE_SPEED = 5; // degrees the ship turns per frame
const double RANDOM_CHANCE = 0.01; // chance of a new coin appearing each frame
const string SHIP_NAME = "aquarii-v";

// A single spinning coin the player can collect.
class coin
{
    private:
        sprite _sprite;

        /**
         * Procedure: give_random_position
         * Purpose: Moves the coin to a random spot that is fully on screen
         */
        void give_random_position()
        {
            sprite_set_x(_sprite, rnd(SCREEN_SIZE - sprite_width(_sprite)));
            sprite_set_y(_sprite, rnd(SCREEN_SIZE - sprite_height(_sprite)));
        }
    public:
        /**
         * Constructor: coin
         * Purpose: Creates a spinning coin at a random spot on the screen
         */
        coin()
        {
            _sprite = create_sprite("gold_coin", "coin_animation");
            sprite_start_animation(_sprite, "spin");
            give_random_position();
        }

        /**
         * Procedure: update
         * Purpose: Advances the coin's spin animation
         */
        void update()
        {
            update_sprite(_sprite);
        }

        /**
         * Procedure: draw
         * Purpose: Draws the coin to the screen
         */
        void draw()
        {
            draw_sprite(_sprite);
        }

        /**
         * Function: collided_with
         * Purpose: Reports whether the coin is touching the given sprite
         *
         * @param sprt the sprite to test against
         * @return true if the two are colliding
         */
        bool collided_with(sprite sprt) const
        {
            return sprite_collision(_sprite, sprt);
        }

        /**
         * Procedure: free_resources
         * Purpose: Frees the coin's sprite before it is removed from the game
         */
        void free_resources()
        {
            free_sprite(_sprite);
        }
};

// The player's ship: how it moves, its score, and how it draws.
class player
{
    private:
        int _score;
        sprite _ship;
    
        /**
         * Procedure: wrap_offscreen
         * Purpose: Moves the ship to the opposite edge once it has left the screen
         */
        void wrap_offscreen()
        {
            if (sprite_x(_ship) + sprite_width(_ship) < 0)
            {
                sprite_set_x(_ship, SCREEN_SIZE + sprite_width(_ship));
            }
            else if (sprite_x(_ship) - sprite_width(_ship) > SCREEN_SIZE)
            {
                sprite_set_x(_ship, -sprite_width(_ship));
            }
    
            if (sprite_y(_ship) + sprite_height(_ship) < 0)
            {
                sprite_set_y(_ship, SCREEN_SIZE + sprite_height(_ship));
            }
            else if (sprite_y(_ship) - sprite_height(_ship) > SCREEN_SIZE)
            {
                sprite_set_y(_ship, -sprite_height(_ship));
            }
        }
    public:
        /**
         * Constructor: player
         * Purpose: Creates the ship in the centre of the screen and clears the score
         */
        player()
        {
            _score = 0;
            _ship = create_sprite(SHIP_NAME);
            sprite_set_x(_ship, (SCREEN_SIZE - sprite_width(_ship)) / 2);
            sprite_set_y(_ship, (SCREEN_SIZE - sprite_height(_ship)) / 2);
        }

        /**
         * Procedure: handle_input
         * Purpose: Thrusts the ship forwards or backwards with the up and down keys,
         * and turns it with the left and right keys
         */
        void handle_input()
        {
            process_events();

            // A sprite's velocity is applied in the direction it is facing,
            // so changing dy thrusts the ship forwards or backwards.
            if (key_down(UP_KEY))
            {
                sprite_set_dy(_ship, sprite_dy(_ship) - THRUST);
            }
            if (key_down(DOWN_KEY))
            {
                sprite_set_dy(_ship, sprite_dy(_ship) + THRUST);
            }
            if (key_down(RIGHT_KEY))
            {
                sprite_set_rotation(_ship, sprite_rotation(_ship) + ROTATE_SPEED);
            }
            if (key_down(LEFT_KEY))
            {
                sprite_set_rotation(_ship, sprite_rotation(_ship) - ROTATE_SPEED);
            }
        }

        /**
         * Procedure: update
         * Purpose: Moves the ship and wraps it to the opposite edge when it leaves the screen
         */
        void update()
        {
            update_sprite(_ship);

            if (sprite_offscreen(_ship))
            {
                wrap_offscreen();
            }
        }

        /**
         * Procedure: draw
         * Purpose: Draws the ship to the screen
         */
        void draw()
        {
            draw_sprite(_ship);
        }

        /**
         * Procedure: draw_score
         * Purpose: Draws the current score in the top-left corner
         */
        void draw_score()
        {
            draw_text("Score " + to_string(_score), COLOR_WHITE, 10, 10);
        }

        /**
         * Procedure: increase_score
         * Purpose: Adds one point to the player's score
         */
        void increase_score()
        {
            _score++;
        }

        /**
         * Function: collide_with
         * Purpose: Reports whether the ship is touching the given coin
         *
         * @param c the coin to test against
         * @return true if the ship and coin are colliding
         */
        bool collide_with(const coin &c) const
        {
            return c.collided_with(_ship);
        }

        /**
         * Procedure: free_resources
         * Purpose: Frees the ship sprite when the game ends
         */
        void free_resources()
        {
            free_sprite(_ship);
        }
};

// The whole game: owns the player and coins, and runs the main loop.
class space_game
{
    private:
        player _player;
        dynamic_array<coin> _coins;
        
        /**
         * Procedure: update
         * Purpose: Spawns new coins, moves everything, and scores a point for
         * each coin the ship reaches
         */
        void update()
        {
            if (rnd() < RANDOM_CHANCE)
            {
                add(_coins, coin());
            }
            
            for (int i = 0; i < length(_coins); i++)
            {
                _coins[i].update();
            }
            
            _player.update();
            
            // Looping backwards means removing a coin cannot skip the next one.
            for (int i = length(_coins) - 1; i >= 0; i--)
            {
                if (_player.collide_with(_coins[i]))
                {
                    _player.increase_score();
                    _coins[i].free_resources();
                    remove(_coins, i);
                }
            }
        }
        
        /**
         * Procedure: draw
         * Purpose: Clears the screen and draws the score, every coin, and the ship
         */
        void draw()
        {
            clear_screen(COLOR_BLACK);
            _player.draw_score();
            
            for (int i = 0; i < length(_coins); i++)
            {
                _coins[i].draw();
            }
            
            _player.draw();
            refresh_screen(60);
        }
        
        /**
         * Procedure: end_game
         * Purpose: Frees the ship and every coin still on screen
         */
        void end_game()
        {
            _player.free_resources();
            
            for (int i = 0; i < length(_coins); i++)
            {
                _coins[i].free_resources();
            }
        }
    public:
        /**
         * Procedure: run
         * Purpose: Runs the game loop until the window is closed, then frees resources
         */
        void run()
        {
            while (!quit_requested())
            {
                _player.handle_input();
                update();
                draw();
            }
    
            end_game();
        }
};

/**
 * Procedure: load_resources
 * Purpose: Loads the ship, coin, and coin animation from the resource bundle
 */
void load_resources()
{
    load_resource_bundle("planet_play", "planet_play.txt");
}

int main()
{
    open_window("Planet Play - by Khubaib Ejaz", SCREEN_SIZE, SCREEN_SIZE);

    load_resources();

    space_game game;
    game.run();

    return 0;
}