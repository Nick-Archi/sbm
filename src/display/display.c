#include "display.h"
#include "driver.h"

void display_init(Display* display)
{
    display->driver->init(display);
}

void display_update(Display* display)
{
    display->driver->update(display);
}
void display_clear(Display* display)
{
    display->driver->clear(display);
}

void display_write_string(Display* display, char* str, size_t pg_start, size_t pos_start, size_t total_size)
{
    // actual implementation here..
}

