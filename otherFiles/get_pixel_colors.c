// To compile: gcc get_pixel_colors.c -O2 -o get_pixel_colors -lX11
// For successfull compilation: sudo apt install gcc libx11-dev

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <X11/Xutil.h>

static int get_int(const char* str, int base)
{
    assert(str != NULL);
    char* endptr;
    long ret = strtol(str, &endptr, base);
    if ((ret < 0) || (ret > INT_MAX))
    {
        fprintf(stderr, "Incorrect value: %s\n", str);
        exit(1);
    }
    return (int)ret;
}

int main(int argc, const char* argv[])
{
    if ((argc != 5) && (argc != 6))
    {
        fprintf(stderr, "Usage: %s x1, y1, x2, y2 [color]\n", argv[0]);
        return 2;
    }
    int x1 = get_int(argv[1], 10);
    int y1 = get_int(argv[2], 10);
    int x2 = get_int(argv[3], 10);
    int y2 = get_int(argv[4], 10);
    const int NO_COLOR = -1;
    int color = NO_COLOR;
    if (argc == 6)
        color = get_int(argv[5], 16);
    char* display_name = NULL;
    Display* display = XOpenDisplay(display_name);
    if (display == NULL)
    {
        fputs("XOpenDisplay() failed (NULL returned)\n", stderr);
        return 3;
    }
    Window root_window = XRootWindow(display, XDefaultScreen(display));
    if (root_window == 0)
    {
        fputs("XRootWindow() failed (NULL returned)\n", stderr);
        return 4;
    }
    int x_left = (x1 < x2) ? x1 : x2;
    int y_top = (y1 < y2) ? y1 : y2;
    unsigned width = (unsigned)abs(x1 - x2) + 1;
    unsigned height = (unsigned)abs(y1 - y2) + 1;
    XImage* ximage = XGetImage(display, root_window, x_left, y_top, width,
                               height, AllPlanes, ZPixmap);
    if (ximage == NULL)
    {
        fputs("XGetImage() failed (NULL returned)\n", stderr);
        return 5;
    }
    unsigned x, y;
    if (color == NO_COLOR)
    {
        // Print all pixel colors to stdout.
        for (x = 0; x < width; ++x)
        {
            for (y = 0; y < height; ++y)
            {
                unsigned long cur_color = XGetPixel(ximage, x, y);
                printf("%06lX ", cur_color);
            }
            puts("");
        }
    }
    else
    {
        // Print whether we found a pixel with a given color.
        int found = 0, x_found, y_found;
        for (x = 0; x < width; ++x)
            for (y = 0; y < height; ++y)
            {
                unsigned long cur_color = XGetPixel(ximage, x, y);
                if (cur_color == color)
                {
                    found = 1;
                    x_found = x_left + x;
                    y_found = y_top + y;
                    break;
                }
            }
        if (found)
            printf("Pixel found at (%d, %d)\n", x_found, y_found);
        else
            puts("Pixel not found");
    }
    int ret = XCloseDisplay(display);
    if (ret != 0)
    {
        fprintf(stderr, "XCloseDisplay() failed (%d returned)\n", ret);
        return 6;
    }
    return 0;
}
