#pragma once

#include <Enums.hpp>
#include <cmath>

namespace sb
{
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

    constexpr double PI = 3.14159265358979323846;
    double Reverse(double(*f)(double), double t);
    double ToInOut(double(*f)(double), double t);
    double Step(double t);
    double Linear(double t);
    double InQuad(double t);
    double OutQuad(double t);
    double InOutQuad(double t);
    double InCubic(double t);
    double OutCubic(double t);
    double InOutCubic(double t);
    double InQuart(double t);
    double OutQuart(double t);
    double InOutQuart(double t);
    double InQuint(double t);
    double OutQuint(double t);
    double InOutQuint(double t);
    double InSine(double t);
    double OutSine(double t);
    double InOutSine(double t);
    double InExpo(double t);
    double OutExpo(double t);
    double InOutExpo(double t);
    double InCirc(double t);
    double OutCirc(double t);
    double InOutCirc(double t);
    double InBack(double t);
    double OutBack(double t);
    double InOutBack(double t);
    double OutBounce(double t);
    double InBounce(double t);
    double InOutBounce(double t);
    double OutElastic(double t);
    double InElastic(double t);
    double OutElasticHalf(double t);
    double OutElasticQuarter(double t);
    double InOutElastic(double t);

    double applyEasing(Easing easing, double t);

    template <typename T>
    T InterpolateLinear(T start, T end, double t);

    std::pair<double, double> InterpolateLinear(std::pair<double, double> start, std::pair<double, double> end, double t);

    template <typename T>
    T InterpolateBilinear(T topLeft, T topRight, T bottomLeft, T bottomRight, double tx, double ty);

    /*
    template <typename T>
    T InterpolateBilinear(T topLeft, T topRight, T bottomLeft, T bottomRight, double tx, double ty)
    {
        return (topLeft + (topRight - topLeft) * tx) * (1 - ty) + (bottomLeft + (bottomRight - bottomLeft) * tx) * ty;
    }
    */
}
