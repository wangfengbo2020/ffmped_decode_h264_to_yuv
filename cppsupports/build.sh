ffmpeginstall=/home/wfb/av/ffmpeg/install
x264install=/home/wfb/av/x264/install
x265install=/home/wfb/av/x265/install

ffmpeglib=$ffmpeginstall/lib
ffmpeginc=$ffmpeginstall/include
x264lib=$x264install/lib
x265lib=$x265install/lib

g++ -o decode264  decodeH264.cpp videodecoder.cpp -L$ffmpeglib  -L$x264lib -L$x265lib -I$ffmpeginc  -lavformat -lavcodec -lavutil -lswscale -lswresample -lx264 -lx265 -lm  -lpthread -ldl -lstdc++
