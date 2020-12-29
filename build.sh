ffmpeginstall=/home/eric/avtest/ffmpeg_install
x264install=/home/eric/avtest/x264_install
x265install=/home/eric/avtest/x265_install

ffmpeglib=$ffmpeginstall/lib
ffmpeginc=$ffmpeginstall/include
x264lib=$x264install/lib
x265lib=$x265install/lib

gcc -o decode264  decodeH264.c videodecoder.c -L$ffmpeglib  -L$x264lib -L$x265lib -I$ffmpeginc  -lavformat -lavcodec -lavutil -lswscale -lswresample -lx264 -lx265 -lm  -lpthread -ldl -lstdc++
