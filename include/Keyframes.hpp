#pragma once

#include <Interpolation.hpp>
#include <Types.hpp>

#include <limits>
#include <vector>
#include <memory>

namespace sb
{
    template<typename T>
    class Keyframe
    {
    public:
        Keyframe()
        {}
        Keyframe(double time, T value, Easing easing, double interpolationOffset = std::numeric_limits<double>::infinity());
        double time;
        T value;
        Easing easing;
        double interpolationOffset;
    };

    template <typename T>
    T keyframeValueAt(const std::vector<Keyframe<T>>& keyframes, double time);

    template <class T>
    std::pair<T, T> keyframeValueAt(const std::pair<std::vector<Keyframe<T>>, std::vector<Keyframe<T>>>& keyframes, double time);
    
    template <typename T, typename V, typename Selector>
    void addKeyframe(Selector W, std::vector<Keyframe<T>>& keyframes, double time, V value, bool alt, Easing easing, double interpolationOffset = std::numeric_limits<double>::infinity());
    
    template <typename T = bool, typename V = ParameterType, typename Selector>
    void addKeyframe(Selector W, std::vector<Keyframe<T>>& keyframes, double time, ParameterType value, bool alt, Easing easing, double interpolationOffset = std::numeric_limits<double>::infinity());

    template <typename T, typename V, typename Selector>
    void generateKeyframes(std::vector<Keyframe<T>>& keyframes, const std::vector<std::unique_ptr<Event<V>>>& events, const std::vector<std::tuple<double, double, int>>& activations, Selector W);

    template <EventType T, typename R, ParameterType P = ParameterType::Additive>
    R generateKeyframesForEvent(const std::vector<std::unique_ptr<IEvent>>& events, std::pair<double, double> coordinates, const std::vector<std::tuple<double, double, int>>& activations);
}
