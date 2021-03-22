#pragma once
#include "Enums.h"

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
}
