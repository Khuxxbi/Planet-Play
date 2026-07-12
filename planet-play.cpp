#include "splashkit.h"

int main()
{
    const int SCREEN_SIZE = 600;
    const int BOTTOM_GAP = 50; // gap between the ship and the bottom edge at the start
    const int SPEED = 3; // pixels moved per frame

    open_window("Planet Play - by Khubaib Ejaz", SCREEN_SIZE, SCREEN_SIZE);

    bitmap ship = load_bitmap("aquarii", "aquarii-v.png");

    // Start the ship centred horizontally, near the bottom of the screen.
    double player_x = (SCREEN_SIZE - bitmap_width(ship)) / 2;
    double player_y = SCREEN_SIZE - bitmap_height(ship) - BOTTOM_GAP;

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

        clear_screen(COLOR_BLACK);
        draw_bitmap(ship, player_x, player_y);
        refresh_screen(60);
    }

    return 0;
}