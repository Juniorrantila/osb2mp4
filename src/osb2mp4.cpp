
#include <sb.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstddef>
#include <limits>
#include <unordered_map>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <exception>

int main(int argc, char* argv[]) {
	if (argc <= 2) {
        std::cout <<
        "USAGE: " << argv[0] << " level_dir_name difficulty_file_name [options]\n\n"
        "OPTIONS:\n"
        "\t--start\tstart time (in milliseconds)\n"
        "\t--end\tend time (in milliseconds)\n";
        return -1;
    }
    
    std::string directory = argv[1];
	std::string diff = argv[2];
    double starttime = -1;
    double duration = -1;
    
    for (int i = 2; i<argc; i++){
        #define cmp(a, b) std::strncmp(a, b, std::strlen(b)) == 0
        if (cmp(argv[i], "--start"))
            starttime = std::stod(argv[++i]);
        else if (cmp(argv[i], "--end"))    
            duration = std::stod(argv[++i]);
        #undef cmp
    }
    
    std::unique_ptr<sb::Storyboard> sb = sb::ParseStoryboard(directory, diff, { frameWidth, frameHeight }, 0.2, 0.2);
	std::pair<double, double> activetime = sb->GetActiveTime();
    
    double audioLeadIn = sb->GetAudioLeadIn();
	double audioDuration = sb->GetAudioDuration();
    
	if (starttime < 0){
        starttime = std::min(activetime.first, audioLeadIn);
    }
    
    if (duration < 0){
        if (activetime.second < audioDuration + 60000){
            duration = std::max(activetime.second, audioDuration) - starttime;
        }
        else {
            duration = audioDuration;
        }
    }
    	
    std::string outputFile = "video.mp4";
	
    int frameCount = (int)(fps / (1000.0 / (float)duration)) + 1;
	std::cout << "Rendering video...\n";
	cv::VideoWriter writer = cv::VideoWriter(
		"temp.avi",
		cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
		fps,
		cv::Size(frameWidth, frameHeight)
	);
    
    int startframe = starttime / 1000 * fps;
    
    auto last_time = std::chrono::high_resolution_clock::now();
    
    #pragma omp parallel for ordered schedule(dynamic)
	for (int i = 0; i < frameCount; i++)
	{
        if (i % (frameCount / 10) == 0)
        {
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> time_delta = current_time - last_time;
            std::cout << "frame " << i + 1 << "/" << frameCount
            << "\tfps: " << (int) (frameCount/10.0)*(1000.0/time_delta.count()) << "\n";
            last_time = current_time;
        }
        
		cv::Mat frame = sb->DrawFrame(startframe + i);
    //#pragma omp ordered
		writer.write(frame);
	}
	writer.release();
    
    std::cout << "Generating audio...\n";
    sb->generateAudio("temp.mp3");
	
    std::cout << "Merging audio and video...\n";
    std::stringstream command;
    command << "ffmpeg -y -v error -stats -i temp.avi -ss " << starttime + sb->GetAudioLeadIn() << "ms -to "
		<< starttime + duration + sb->GetAudioLeadIn() << "ms -accurate_seek -i temp.mp3 -c:v copy " << outputFile;
	
    system(command.str().c_str());
    
    std::cout << "Removing temporary files\n";
    system("rm temp.mp3 temp.avi");
	
    std::cout << "Done\n";
	
	return 0;
}