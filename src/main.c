#include <stdlib.h>
#include <stdio.h>

#include <dirent.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <libevdev/libevdev.h>
#include <libevdev/libevdev-uinput.h>


// Input device ID: bus 0x3 vendor 0x1b1c product 0x2b00 version 0x111
#define SCIMITAR_WIRELESS_VENDOR 0x1b1c
#define SCIMITAR_WIRELESS_PRODUCT 0x2b00
#define SCIMITAR_WIRELESS_VERSION 0x111

int match_device(char* file_path) {
    int ret = 0;

    int file = open(file_path, O_RDONLY);
    struct libevdev* dev = libevdev_new();
    libevdev_set_fd(dev, file);

    // very sketchy but there are two keypads exposed and I can't figure out what the second one is
    // on the actual mouse
    // only difference I could find is that the correct one has a couple EV_LED codes
    // so I'll use that
    if (libevdev_get_id_vendor(dev) == SCIMITAR_WIRELESS_VENDOR
        && libevdev_get_id_product(dev) == SCIMITAR_WIRELESS_PRODUCT
        && libevdev_get_id_version(dev) == SCIMITAR_WIRELESS_VERSION
        && libevdev_has_event_code(dev, EV_KEY, KEY_A)
        && libevdev_has_event_code(dev, EV_LED, LED_NUML)) {
            ret = 1;
    }

    libevdev_free(dev);
    close(file);
    return ret;
}

int get_keypad_fd() {
    int ret = -1;

    struct dirent **file_list;
    int file_no = scandir("/dev/input", &file_list, NULL, NULL);

    for(int i = 0; i < file_no; i++){
       char abs_path[sizeof "/dev/input/" + strlen(file_list[i]->d_name)];
       sprintf(abs_path, "%s%s", "/dev/input/", file_list[i]->d_name);

        if (file_list[i]->d_type != 4) { // 4 means directory (d_type is not posix, FIX LATER)
            if (match_device(abs_path)) {
                ret = open(abs_path, O_RDONLY);
            }
        }
    }

    for(int i = 0; i < file_no; i++) {
        free(file_list[i]);
    }
    free(file_list);

    return ret;
}

int main() {
    // what are the odds there's no input devices attached?
    if (access("/dev/input/event0", R_OK | W_OK)) {
        fputs("permission error, are you root?\n", stderr);
        return -1;
    }

    int fd = get_keypad_fd();

    if (fd == -1) {
        fputs("failed to find mouse :(\n", stderr);
        return -1;
    }

    struct libevdev* dev;
    libevdev_new_from_fd(fd, &dev);
    libevdev_grab(dev, LIBEVDEV_GRAB);

    // set up output
    struct libevdev* template = libevdev_new();
    libevdev_set_name(template, "Scimitar Keypad Remapped");

    for (int i = 0; i <= 12; i++) {
        libevdev_enable_event_code(template, EV_KEY, KEY_F14 + i, NULL);
    }


    struct libevdev_uinput* uidev;
    if (libevdev_uinput_create_from_device(template, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev)) {
        fputs("failed to create virtual output\n", stderr);
        return -1;
    }

    libevdev_free(template);

    struct input_event* event = malloc(sizeof(struct input_event));
    while (1) {
        libevdev_next_event(dev, LIBEVDEV_READ_FLAG_BLOCKING, event);

        // switch (event->code) {
        //     case KEY_1:
        //         libevdev_uinput_write_event(uidev, EV_KEY, KEY_F14, event->value);
        //         break;
        //     case KEY_2:
        //         libevdev_uinput_write_event(uidev, EV_KEY, KEY_F15, event->value);
        //         break;
        // }
        if (event->type == EV_KEY) {
            libevdev_uinput_write_event(uidev, EV_KEY, event->code + 182, event->value);
        }
        libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 1);
    }

    libevdev_uinput_destroy(uidev);
    free(event);
    libevdev_grab(dev, LIBEVDEV_UNGRAB);
    libevdev_free(dev);

    return 0;
}