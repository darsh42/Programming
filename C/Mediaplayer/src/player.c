#include "player.h"

struct MediaPlayer player;

int main(int argc, char **argv) {
    ERR_PLAYER(argc, 2, "Usage: ./player <song.mp3>");
    player.spec.format = PA_SAMPLE_S16NE;
    player.spec.channels = 2;
    player.spec.rate = 44100;
    player.device = pa_simple_new(NULL,
                                "MediaPlayer",
                                PA_STREAM_PLAYBACK,
                                NULL,
                                "Music",
                                &player.spec,
                                NULL,
                                NULL,
                                &player.error);
    ERR_PLAYER(player.error, 0, "New pulse audio stream could not be opened");
    ERR_PLAYER(create_mp3(&player.file, argv[1]), 0, "Cannot create mp3 struct");
    while (true) {
        ERR_PLAYER(decode_mp3(&player.file), 0, "Cannot decode file");
        pa_simple_write(player.device,
                        &player.file.buffer.start,
                        player.file.buffer.length,
                        &player.error);
        printf("%d", player.error);
    }
    pa_simple_drain(player.device, &player.error);
    ERR_PLAYER(player.error, 0, "Could not drain pulse audio stream");
    ERR_PLAYER(delete_mp3(&player.file), 0, "Could not delete mp3 struct");
    pa_simple_free(player.device);
    return 0;
}
