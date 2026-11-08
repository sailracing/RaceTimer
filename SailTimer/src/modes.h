/***************************************
 * This file defines the structures
 * needed for the race timer project
************************************** */

#define EDSHORT 0
#define EDLONG 1
#define EDXLONG 2

#define ETBEEP 0
#define ETHORN 1

struct mode_event
{
    int event_time;
    int event_type;
    int event_duration;
    int event_repeat;
};


struct mode_descriptor
{
    char rule[20];
    char length[20];
    char alert[20];
    char rolling[20];
};

struct mode
{
    int mode_number;
    mode_descriptor mode_header;
    int mode_length;
    bool mode_running;
    mode_event mode_events[50];
};

struct mode_dictionary
{
    int number_of_modes;
    timer_mode timer_modes[50];
};

