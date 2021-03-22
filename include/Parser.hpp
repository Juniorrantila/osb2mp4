#pragma once

#include <Components.hpp>
#include <Helpers.hpp>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <memory>
#include <exception>

namespace sb
{
    // written mostly in reference to the parser used in osu!lazer (https://github.com/ppy/osu/blob/master/osu.Game/Beatmaps/Formats/LegacyStoryboardDecoder.cs)
    void parseFile(std::ifstream& file, size_t& lineNumber, std::unordered_map<std::string, std::string>& variables, std::vector<std::unique_ptr<Sprite>>& sprites, std::vector<Sample>& samples, std::vector<std::pair<double, HitSound>>& hitSounds, Background& background, Video& video, std::unordered_map<std::string, std::string>& info);
    
    void ParseStoryboard(const std::filesystem::path& directory, const std::string& osb, const std::string& diff,
        std::vector<std::unique_ptr<Sprite>>& sprites,
        std::vector<Sample>& samples,
        std::vector<std::pair<double, HitSound>>& hitSounds,
        Background& background,
        Video& video,
        std::unordered_map<std::string, std::string>& info
    );
}
