#pragma once

#include "Keyframes.hpp"

#include <opencv2/opencv.hpp>
#include <string>
#include <limits>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <optional>

namespace sb
{
    class Loop
    {
        public:
        Loop(double starttime, int loopcount);

        template <typename T>
        void AddEvent(std::unique_ptr<Event<T>> event);

        void Initialise();

        std::vector<std::unique_ptr<IEvent>>& GetEvents();

        double GetStartTime() const;

        double GetEndTime() const;

        double GetLoopLength() const;

        int GetLoopCount() const;
    
    private:
        std::vector<std::unique_ptr<IEvent>> events;
        double starttime;
        double endtime;
        double looplength;
        int loopcount;
    };

    class Trigger
    {
    public:
        Trigger(const std::string& triggerName, double starttime, double endtime, int groupNumber);
        
        template <typename T>
        void AddEvent(std::unique_ptr<Event<T>> event);
        
        void Initialise(std::vector<std::pair<double, HitSound>>& hitSounds, std::vector<std::tuple<double, double, int>>& activations, int& id);
        
        std::vector<std::unique_ptr<IEvent>>& GetEvents();
        
        const std::string& GetTriggerName() const;
        
        double GetStartTime() const;
        
        double GetEndTime() const;
        
        int GetGroupNumber() const;
        
        bool IsActivated() const;

    private:
        std::vector<std::unique_ptr<IEvent>> events;
        std::string triggerName;
        double starttime;
        double endtime;
        double looplength = 0;
        int groupNumber;
        bool activated = false;
    };

    class Sprite
    {
    public:
        Sprite(Layer layer, Origin origin, const std::string& filepath, const std::pair<double, double>& coordinates);
        
        template <typename T>
        void AddEvent(std::unique_ptr<Event<T>> event);
        
        template <typename T>
        void AddEventInLoop(std::unique_ptr<Event<T>> event);
        
        template <typename T>
        void AddEventInTrigger(std::unique_ptr<Event<T>> event);
        
        void AddLoop(Loop loop);
        
        void AddTrigger(Trigger trigger);
        
        void Initialise(std::vector<std::pair<double, HitSound>>& hitSounds);
        
        std::pair<double, double> PositionAt(double time) const;
        
        double RotationAt(double time) const;
        
        std::pair<double, double> ScaleAt(double time) const;
        
        Colour ColourAt(double time) const;
        
        double OpacityAt(double time) const;
        
        bool EffectAt(double time, ParameterType effect) const;
        
        Layer GetLayer() const;
        
        Origin GetOrigin() const;
        
        virtual const std::string GetFilePath(double time) const;
        
        virtual std::vector<std::string> GetFilePaths() const;
        
        const std::pair<double, double>& GetCoordinates() const;
        
        const std::pair<double, double>& GetActiveTime() const;
        
        const std::pair<double, double>& GetVisibleTime() const;

    protected:
        std::vector<Loop> loops;

        std::vector<Trigger> triggers;

        std::pair<double, double> activetime;

        std::pair<double, double> visibletime;

        const std::string filepath;

    private:
        std::vector<std::unique_ptr<IEvent>> events;

        bool initialised = false;

        const Layer layer;

        const Origin origin;

        const std::pair<double, double> coordinates;

        std::pair<std::vector<Keyframe<double>>, std::vector<Keyframe<double>>> positionKeyframes;

        std::vector<Keyframe<double>> rotationKeyframes;

        std::pair<std::vector<Keyframe<double>>, std::vector<Keyframe<double>>> scaleKeyframes;

        std::vector<Keyframe<Colour>> colourKeyframes;

        std::vector<Keyframe<double>> opacityKeyframes;

        std::vector<Keyframe<bool>> flipVKeyframes;

        std::vector<Keyframe<bool>> flipHKeyframes;

        std::vector<Keyframe<bool>> additiveKeyframes;
    };

    class Animation : public Sprite
    {
    public:
        Animation(Layer layer, Origin origin, const std::string& filepath, const std::pair<double, double>& coordinates, int framecount, double framedelay, LoopType looptype);
        int frameIndexAt(double time) const;
        const std::string GetFilePath(double time) const;
        std::vector<std::string> GetFilePaths() const;
        
    private:
        const int framecount;
        const double framedelay;
        const LoopType looptype;
    };

    class Sample
    {
    public:
        Sample(double starttime, Layer layer, const std::string& filepath, float volume);
        const double starttime;
        const Layer layer;
        const std::string filepath;
        const float volume;
    };

    class Background
    {
    public:
        Background() = default;
        Background(const std::string& filepath, std::pair<double, double> offset);
        Background& operator=(const Background&) = default;
        std::string filepath = "";
        std::pair<double, double> offset = { 0,0 };
        bool exists = false;
    };
    class Video
    {
    public:
        Video() = default;
        Video(double starttime, const std::string& filepath, std::pair<double, double> offset);
        Video& operator=(const Video&) = default;
        double starttime = 0;
        std::string filepath = "";
        std::pair<double, double> offset = { 0, 0 };
        bool exists = false;
    };
}
