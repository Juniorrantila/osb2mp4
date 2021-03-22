
#include "Enums.hpp"

/*
namespace sb
{
    const std::string LayerStrings[] =
    {
        "Background",
        "Fail",
        "Pass", 
        "Foreground",
        "Overlay",
    };    

    const std::string OriginStrings[] =
    {
        "TopLeft",
        "TopCentre",
        "TopRight",
        "TopRight",
        "CentreLeft",
        "Centre",
        "CentreRight",
        "BottomLeft",
        "BottomCentre",
        "BottomRight",
    };

    const std::string LoopTypeStrings[] =
    {
        "LoopForever",
        "LoopOnce"
    };

    const std::string ParameterTypeStrings[] =
    {
        "H",
        "V",
        "A",
    };

    const std::string EventTypeStrings[] =
    {
        "F",
        "S",
        "V",
        "R",
        "M",
        "MX",
        "MY",
        "C",
        "P",
    };

    const std::string KeywordStrings[] =
    {
        "Sprite",
        "Animation",
        "Sample",
        "T",
        "L",
    };
}   // namespace sb
*/

namespace sb
{

    

    const std::unordered_map<std::string, Layer> LayerStrings =
    {
        {"Background", Layer::Background},
        {"Fail", Layer::Fail},
        {"Pass", Layer::Pass},
        {"Foreground", Layer::Foreground},
        {"Overlay", Layer::Overlay}
    };

    const std::unordered_map<std::string, Origin> OriginStrings =
    {
        {"TopLeft", Origin::TopLeft},
        {"TopCentre", Origin::TopCentre},
        {"TopRight", Origin::TopRight},
        {"TopRight", Origin::TopRight},
        {"CentreLeft", Origin::CentreLeft},
        {"Centre", Origin::Centre},
        {"CentreRight", Origin::CentreRight},
        {"BottomLeft", Origin::BottomLeft},
        {"BottomCentre", Origin::BottomCentre},
        {"BottomRight", Origin::BottomRight}
    };

    const std::unordered_map<std::string, LoopType> LoopTypeStrings =
    {
        {"LoopForever", LoopType::LoopForever},
        {"LoopOnce", LoopType::LoopOnce}
    };

    const std::unordered_map<std::string, ParameterType> ParameterTypeStrings =
    {
        {"H", ParameterType::FlipH},
        {"V", ParameterType::FlipV},
        {"A", ParameterType::Additive}
    };

    const std::unordered_map<std::string, EventType> EventTypeStrings
    {
        {"F", EventType::F},
        {"S", EventType::S},
        {"V", EventType::V},
        {"R", EventType::R},
        {"M", EventType::M},
        {"MX", EventType::MX},
        {"MY", EventType::MY},
        {"C", EventType::C},
        {"P", EventType::P}
    };

    const std::unordered_map<std::string, Keyword> KeywordStrings =
    {
        {"Sprite", Keyword::Sprite},
        {"Animation", Keyword::Animation},
        {"Sample", Keyword::Sample},
        {"T", Keyword::T},
        {"L", Keyword::L},
    };
}
