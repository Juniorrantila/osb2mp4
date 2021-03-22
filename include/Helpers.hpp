#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <exception>

namespace sb
{
    void stringReplace(std::string& s, const std::string& search, const std::string& replace);

    std::vector<std::string> stringSplit(std::string s, const std::string& delimiter);

    void applyVariables(std::string& line, const std::unordered_map<std::string, std::string>& variables);

    std::string removePathQuotes(const std::string& s);

    // taken from https://stackoverflow.com/questions/478898#478960
    std::string exec(const std::string& cmd);

    double getAudioDuration(const std::string& filepath);

    cv::Mat convertImage(cv::Mat image);

    cv::Mat readImageFile(const std::string& filepath);
}