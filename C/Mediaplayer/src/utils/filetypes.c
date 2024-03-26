#include "filetypes.h"
#include <sys/mman.h>

/* FUNCTIONS */
enum mad_flow mp3_input(void *, struct mad_stream *);
enum mad_flow mp3_error(void *, struct mad_stream *, struct mad_frame *);
enum mad_flow mp3_output(void *, struct mad_header const *, struct mad_pcm *);
int create_mp3(mp3_t *mp3, char *filename);
int delete_mp3(mp3_t *mp3);

/* FILE GLOBALS */
static FILE *file;

int create_mp3(mp3_t *mp3, char *filename) {
    struct stat stat;
    file = fopen(filename, "r");
    if (fstat(fileno(file), &stat) == -1) {
        UTILS_ERROR("create_mp3", "Could not open file");
        return -1;
    }
    mp3->buffer.start = mmap(NULL,
                             stat.st_size,
                             PROT_READ,
                             MAP_SHARED,
                             fileno(file),
                             0);
    if (mp3->buffer.start == MAP_FAILED) {
        UTILS_ERROR("creat_mp3", "Could not read file");
        return -1;
    }
    mad_decoder_init(&mp3->decoder,
                     &mp3->buffer,
                     mp3_input,
                     0,  /* header */
                     0,  /* filter */
                     mp3_output,
                     mp3_error,
                     0); /* message */
    return 0;
}

int delete_mp3(mp3_t *mp3) {
    if (munmap((void *)mp3->buffer.start, mp3->buffer.length) == -1) {
        UTILS_ERROR("delete_mp3", "Error unmapping file");
        return -1;
    }
    mad_decoder_finish(&mp3->decoder);
    fclose(file);
    return 0;
}

enum mad_flow mp3_input(void *data, struct mad_stream *stream) {
    struct buffer *buffer = data;
    if (!buffer->length) {
        return MAD_FLOW_STOP;
    }
    mad_stream_buffer(stream, buffer->start, buffer->length);
    buffer->length = 0;
    return MAD_FLOW_CONTINUE;
}

static int downsample(mad_fixed_t sample) {
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE) sample = MAD_F_ONE - 1;
  if (sample < -MAD_F_ONE) sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

enum mad_flow mp3_output(void *data,
                         struct mad_header const *header,
                         struct mad_pcm *pcm) {
    unsigned int nchannels, nsamples;
    mad_fixed_t const *left, *right;
    unsigned char *ptr;
    struct buffer *buffer;

    nchannels = pcm->channels;
    nsamples  = pcm->length;
    left  = pcm->samples[0];
    right = pcm->samples[1];

    buffer = data;
    ptr = buffer->start;

    while(--nsamples) {
        int sample;

        sample = downsample(*left++);
		*ptr++  = (unsigned char) (sample >> 0);
        *ptr++  = (unsigned char) (sample >> 8);

        if (nchannels == 2) {
            sample = downsample(*right++);
            *ptr++  =(unsigned char) (sample >> 0);
            *ptr++  =(unsigned char) (sample >> 8);
        }
    }

    return MAD_FLOW_CONTINUE;
}

enum mad_flow mp3_error(void *data, struct mad_stream *stream, struct mad_frame *frame) {
    struct buffer *buffer = data;

    fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %ld\n",
            stream->error, mad_stream_errorstr(stream),
            stream->this_frame - buffer->start);

    return MAD_FLOW_BREAK;
}

int decode_mp3(mp3_t *mp3) {
    mad_decoder_run(&mp3->decoder, MAD_DECODER_MODE_SYNC);
    return 0;
}
