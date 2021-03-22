#pragma once

#include <string>

namespace sb
{
    enum class Layer
    {
        Background,
        Fail,
        Pass,
        Foreground,
        Overlay
    };
    extern const std::string LayerStrings[];

    enum class Origin
    {
        TopLeft,
        TopCentre,
        TopRight,
        CentreLeft,
        Centre,
        CentreRight,
        BottomLeft,
        BottomCentre,
        BottomRight
    };
    extern const std::string OriginStrings[];

    enum class LoopType
    {
        LoopForever,
        LoopOnce,
        Custom
    };
    extern const std::string LoopTypeStrings[];

    enum class ParameterType
    {
        FlipH,
        FlipV,
        Additive
    };
    extern const std::string ParameterTypeStrings[];

    enum class EventType
    {
        None,
        F,
        S,
        V,
        R,
        M,
        MX,
        MY,
        C,
        P
    };
    extern const std::string EventTypeStrings[];

    enum class Keyword
    {
        None,
        Sprite,
        Animation,
        Sample,
        T,
        L
    };
    extern const std::string KeywordStrings[];
}
