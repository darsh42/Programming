#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <pulse/sample.h>
#include <pulse/simple.h>

#include "common.h"
#include "utils/filetypes.h"

#define ERR_PLAYER(V, S, msg)                                                  \
  if (V != S) {                                                                \
        ERROR("player.c", "main", msg);                                        \
        return 0;                                                              \
  }

extern int create_mp3(mp3_t *mp3, char *filename);
extern int delete_mp3(mp3_t *mp3);
extern int decode_mp3(mp3_t *mp3);

struct MediaPlayer {
    /* Contains all sound file data */
    mp3_t file;

    int volume;
    int location;
    int speed;

    pa_simple *device;
    pa_sample_spec spec;

    int error;
};

#endif // PLAYER_H_INCLUDED
