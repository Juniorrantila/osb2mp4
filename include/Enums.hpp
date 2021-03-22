#pragma once

#include <unordered_map>
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
    //extern const std::string LayerStrings[];
    extern const std::unordered_map<std::string, Layer> LayerStrings;

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
    //extern const std::string OriginStrings[];
    extern const std::unordered_map<std::string, Origin> OriginStrings;

    enum class LoopType
    {
        LoopForever,
        LoopOnce,
        Custom
    };
    //extern const std::string LoopTypeStrings[];
    extern const std::unordered_map<std::string, LoopType> LoopTypeStrings;

    enum class ParameterType
    {
        FlipH,
        FlipV,
        Additive
    };
    //extern const std::string ParameterTypeStrings[];
    extern const std::unordered_map<std::string, ParameterType> ParameterTypeStrings;

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
    //extern const std::string EventTypeStrings[];
    extern const std::unordered_map<std::string, EventType> EventTypeStrings;

    enum class Keyword
    {
        None,
        Sprite,
        Animation,
        Sample,
        T,
        L
    };
    //extern const std::string KeywordStrings[];
    extern const std::unordered_map<std::string, Keyword> KeywordStrings;

    enum class Easing
    {
        None,
        Out,
        In,
        InQuad,
        OutQuad,
        InOutQuad,
        InCubic,
        OutCubic,
        InOutCubic,
        InQuart,
        OutQuart,
        InOutQuart,
        InQuint,
        OutQuint,
        InOutQuint,
        InSine,
        OutSine,
        InOutSine,
        InExpo,
        OutExpo,
        InOutExpo,
        InCirc,
        OutCirc,
        InOutCirc,
        InElastic,
        OutElastic,
        OutElasticHalf,
        OutElasticQuarter,
        InOutElastic,
        InBack,
        OutBack,
        InOutBack,
        InBounce,
        OutBounce,
        InOutBounce,
        Step
    };
} // namespace sb
