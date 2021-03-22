

#include "Types.hpp"

#include <memory>
#include <string>

namespace sb
{

Colour::Colour(){}
Colour::Colour(double R, double G, double B)
    :
    R(R),
    G(G),
    B(B)
{}

double Colour::operator[](const int index) const
{
    switch (index) {
        case 0: return R;
        case 1: return G;
        case 2: return B;
        default: return -1;
    }
};

Colour Colour::operator+(const Colour& other) const
{
    return Colour(R + other.R, G + other.G, B + other.B);
};

Colour Colour::operator-(const Colour& other) const
{
    return Colour(R - other.R, G - other.G, B - other.B);
};

Colour Colour::operator*(const double other) const
{
    return Colour(R * other, G * other, B * other);
};



// https://osu.ppy.sh/wiki/en/osu%21_File_Formats/Osu_%28file_format%29#hitsounds
// https://osu.ppy.sh/wiki/en/Storyboard_Scripting/Compound_Commands

HitSound::HitSound() {}

HitSound::HitSound(int normalSet, int additionSet, int additionFlag, int index)
    :
    normalSet(normalSet),
    additionSet(additionSet),
    additionFlag(additionFlag),
    index(index)
{}

HitSound::HitSound(const std::string& triggerType)
{
    int p = 8;
    if (triggerType.find("All", p) == p) { p += 3; normalSet = -1; }
    else if (triggerType.find("Normal", p) == p) { p += 6; normalSet = 1; }
    else if (triggerType.find("Soft", p) == p) { p += 4; normalSet = 2; }
    else if (triggerType.find("Drum", p) == p) { p += 4; normalSet = 3; }
    else normalSet = -1;

    if (triggerType.find("All", p) == p) { p += 3; additionSet = -1; }
    else if (triggerType.find("Normal", p) == p) { p += 6; additionSet = 1; }
    else if (triggerType.find("Soft", p) == p) { p += 4; additionSet = 2; }
    else if (triggerType.find("Drum", p) == p) { p += 4; additionSet = 3; }
    else additionSet = -1;

    if (triggerType.find("Whistle", p) == p) { p += 7; additionFlag = 2; }
    else if (triggerType.find("Finish", p) == p) { p += 6; additionFlag = 4; }
    else if (triggerType.find("Clap", p) == p) { p += 4; additionFlag = 8; }
    else additionFlag = -1;

    index = triggerType.size() > p ? std::stoi(triggerType.substr(p)) : -1;
}

bool HitSound::operator==(const HitSound& other) const
{
    return (other.normalSet == -1 ? true : normalSet == other.normalSet)
        && (other.additionSet == -1 ? true : additionSet == other.additionSet)
        && (other.additionFlag == -1 ? true :
            (additionFlag & 2) >> 1 && (other.additionFlag & 2) >> 1
            || (additionFlag & 4) >> 2 && (other.additionFlag & 4) >> 2
            || (additionFlag & 8) >> 3 && (other.additionFlag & 8) >> 3)
        && (other.index == -1 ? true : index == other.index);
}

bool HitSound::IsHitSound(const std::string& triggerType)
{
    return triggerType.find("HitSound") == 0;
}

} // namespace sb
