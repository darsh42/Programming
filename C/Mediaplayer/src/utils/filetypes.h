#ifndef FILETYPES_H_INCLUDED
#define FILETYPES_H_INCLUDED

#include "../common.h"
#include <mad.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define UTILS_ERROR(func, msg) ERROR("filetypes.c", func, msg);        \

typedef struct wav_file wav_t;
typedef struct mp3_file mp3_t;
typedef struct ogg_file ogg_t;

struct buffer {
    unsigned char *start;
    unsigned long length;
};

struct wav_file {
    struct {
        uint16_t ChunkID;
        uint16_t ChunkSize;
        uint16_t Format;
        uint16_t Subchunk1ID;
        uint16_t Subchunk1Size;
        uint8_t  AudioFormat;
        uint8_t  NumChannels;
        uint16_t SampleRate;
        uint16_t ByteRate;
        uint8_t  BlockAlign;
        uint8_t  BitsPerSample;
        uint16_t Subchunk2ID;
        uint16_t Subchunk2Size;
        uint16_t Data;
    } header;

    uint8_t *data;
};

struct mp3_file {
    struct mad_decoder decoder;
    struct mad_stream  stream;
    struct mad_frame   frame;
    struct mad_synth   synth;

    struct buffer buffer;

    bool finished;
};


enum extension {
    WAV,
    MP3,
    ERR
};

struct sound {
    /* file data */
    char *filename;
    enum extension filetype;

    /* sound data */
    union {
        wav_t wav;
        mp3_t mp3;
    } data;

    int size;
    bool finished;
};

#endif // FILETYPES_H_INCLUDED
