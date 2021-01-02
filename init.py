
import shutil
import os

cc = input("Compiler name: ")

opencv_dir = input("OpenCV directory path: ")
if (opencv_dir[-1] != '/' and opencv_dir[-1] != '\\'):
    opencv_dir += '/'

ffmpeg_dll_name = "opencv_videoio_ffmpeg451_64.dll"
ffmpeg_dll_path = opencv_dir + "build/x64/vc15/bin/" + ffmpeg_dll_name

opencv_world_name = "opencv_world451.dll"
opencv_world_path = opencv_dir + "build/x64/vc15/bin/" + opencv_world_name

shutil.copyfile(ffmpeg_dll_path, "./bin/"+ffmpeg_dll_name)
shutil.copyfile(opencv_world_path, "./bin/"+opencv_world_name)

opencv_lib_name = "opencv_world451"
opencv_lib_path = opencv_dir + "build/x64/vc15/lib"

opencv_include_path = opencv_dir + "/build/include"

target = "osb2mp4.exe"
cflags = "-fopenmp -O3 -std=c++17"
include = f"-I./include -I{opencv_include_path}"
libs = f"-L {opencv_lib_path} -l{opencv_lib_name}"
src = "src/*.cpp"

makefile = f"{cc} {src} -o ./bin/{target} {cflags} {libs} {include}"

f = open("make.ps1", "w+")
f.write(makefile)
f.close()