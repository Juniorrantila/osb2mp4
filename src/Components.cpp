
#include "Components.hpp"
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
Loop::Loop(double starttime, int loopcount)
:
    starttime(starttime),
    loopcount(loopcount)
{
    endtime = 0;
    looplength = 0;
}

template <typename T>
void Loop::AddEvent(std::unique_ptr<Event<T>> event)
{
    events.push_back(std::move(event));
}

void Loop::Initialise()
{
    if (loopcount < 1) loopcount = 1; // today i learned that loops behave like this
    looplength = (*(events.end() - 1))->GetEndTime();
    std::vector<std::unique_ptr<IEvent>> expandedEvents;
    std::vector<std::pair<double, double>> durations;
    for (int i = 0; i < loopcount; i++)
    {
        int j = 0;
        for (std::unique_ptr<IEvent>& event : events)
        {
            if (i == 0) durations.emplace_back(std::pair<double, double>{event->GetStartTime(), event->GetEndTime()});
            event->SetStartTime(starttime + durations[j].first + looplength * i);
            event->SetEndTime(starttime + durations[j].second + looplength * i);
            expandedEvents.push_back(event->copy());
            j++;
        }
    }
    events = std::move(expandedEvents);
    endtime = starttime + looplength * loopcount;
}

std::vector<std::unique_ptr<IEvent>>& Loop::GetEvents()
{
    return events;
}

double Loop::GetStartTime() const
{
    return starttime;
}

double Loop::GetEndTime() const
{
    return endtime;
}
double Loop::GetLoopLength() const
{
    return looplength;
}
int Loop::GetLoopCount() const
{
    return loopcount;
}


Trigger::Trigger(const std::string& triggerName, double starttime, double endtime, int groupNumber)
    :
    triggerName(triggerName),
    starttime(starttime),
    endtime(endtime),
    groupNumber(groupNumber)
{}

template <typename T>
void Trigger::AddEvent(std::unique_ptr<Event<T>> event)
{
    events.push_back(std::move(event));
}

void Trigger::Initialise(std::vector<std::pair<double, HitSound>>& hitSounds, std::vector<std::tuple<double, double, int>>& activations, int& id)
{
    if (!HitSound::IsHitSound(triggerName)) return; // TODO: ignoring failing and passing state triggers for now
    looplength = (*(events.end() - 1))->GetEndTime();
    std::vector<double> activationTimes;
    for (const std::pair<double, HitSound>& hitSound : hitSounds)
        if (hitSound.first >= starttime && hitSound.first < endtime
            && hitSound.second == HitSound(triggerName))
        {
            activations.push_back(std::tuple<double, double, int>(hitSound.first, hitSound.first + looplength, groupNumber));
            activationTimes.push_back(hitSound.first);
            activated = true;
        }
    std::vector<std::unique_ptr<IEvent>> expandedEvents;
    if (activated)
    {
        for (double activationTime : activationTimes)
        {
            for (std::unique_ptr<IEvent>& event : events)
            {
                std::unique_ptr<IEvent> copy = event->copy();
                copy->SetTriggerID(id, activationTime, groupNumber);
                copy->SetStartTime(activationTime + copy->GetStartTime());
                copy->SetEndTime(activationTime + copy->GetEndTime());
                expandedEvents.push_back(std::move(copy));
            }
            id++;
        }
        events = std::move(expandedEvents);
    }
}

std::vector<std::unique_ptr<IEvent>>& Trigger::GetEvents()
{
    return events;
}

const std::string& Trigger::GetTriggerName() const
{
    return triggerName;
}

double Trigger::GetStartTime() const
{
    return starttime;
}

double Trigger::GetEndTime() const
{
    return endtime;
}

int Trigger::GetGroupNumber() const
{
    return groupNumber;
}

bool Trigger::IsActivated() const
{
    return activated;
}


Sprite::Sprite(Layer layer, Origin origin, const std::string& filepath, const std::pair<double, double>& coordinates)
    :
    layer(layer),
    origin(origin),
    filepath(filepath),
    coordinates(coordinates)
{}

template <typename T>
void Sprite::AddEvent(std::unique_ptr<Event<T>> event)
{
    events.push_back(std::move(event));
}

template <typename T>
void Sprite::AddEventInLoop(std::unique_ptr<Event<T>> event)
{
    (loops.end() - 1)->AddEvent(std::move(event));
}

template <typename T>
void Sprite::AddEventInTrigger(std::unique_ptr<Event<T>> event)
{
    (triggers.end() - 1)->AddEvent(std::move(event));
}

void Sprite::AddLoop(Loop loop)
{
    loops.push_back(std::move(loop));
}

void Sprite::AddTrigger(Trigger trigger)
{
    triggers.push_back(std::move(trigger));
}

void Sprite::Initialise(std::vector<std::pair<double, HitSound>>& hitSounds)
{
    initialised = true;
    for (Loop& loop : loops) loop.Initialise();
    std::vector<std::tuple<double, double, int>> activations;
    int id = 1;
    for (Trigger& trigger : triggers) trigger.Initialise(hitSounds, activations, id);
    for (Loop& loop : loops)
        for (std::unique_ptr<IEvent>& event : loop.GetEvents())
            events.emplace_back(std::move(event));
    for (Trigger& trigger : triggers)
        if (trigger.IsActivated())
            for (std::unique_ptr<IEvent>& event : trigger.GetEvents())
                events.emplace_back(std::move(event));
    std::stable_sort(events.begin(), events.end(), [](const std::unique_ptr<IEvent>& a, const std::unique_ptr<IEvent>& b) {
        int aT = a->GetStartTime();
        int bT = b->GetStartTime();
        int aID = a->GetTriggerID();
        int bID = b->GetTriggerID();
        return aID != 0 && bID != 0 && aID != bID ? aID < bID : aT < bT;
        });

    double endTime = std::numeric_limits<double>::min();
    double startTime = std::numeric_limits<double>::max();
    for (const std::unique_ptr<IEvent>& event : events)
    {
        endTime = std::max(endTime, event->GetEndTime());
        startTime = std::min(startTime, event->GetStartTime());
    }
    activetime = std::pair<double, double>({ startTime, endTime });

    std::optional<double> visibleEndTime;
    std::optional<double> visibleStartTime;
    for (auto it = events.begin(); it != events.end(); it++)
        if ((*it)->GetType() == EventType::F)
        {
            if (dynamic_cast<Event<double>*>((*it).get())->GetStartValue() == 0)
                visibleStartTime = (*it)->GetStartTime();
            break;
        }
    for (auto it = events.rbegin(); it != events.rend(); it++)
        if ((*it)->GetType() == EventType::F)
        {
            if (dynamic_cast<Event<double>*>((*it).get())->GetEndValue() == 0)
                visibleEndTime = (*it)->GetEndTime();
            break;
        }
    visibletime = std::pair<double, double>({
        visibleStartTime.value_or(startTime),
        visibleEndTime.value_or(endTime)
        });

    positionKeyframes = generateKeyframesForEvent<EventType::M, std::pair<std::vector<Keyframe<double>>, std::vector<Keyframe<double>>>>(events, coordinates, activations);
    rotationKeyframes = generateKeyframesForEvent<EventType::R, std::vector<Keyframe<double>>>(events, coordinates, activations);
    scaleKeyframes = generateKeyframesForEvent<EventType::S, std::pair<std::vector<Keyframe<double>>, std::vector<Keyframe<double>>>>(events, coordinates, activations);
    colourKeyframes = generateKeyframesForEvent<EventType::C, std::vector<Keyframe<Colour>>>(events, coordinates, activations);
    opacityKeyframes = generateKeyframesForEvent<EventType::F, std::vector<Keyframe<double>>>(events, coordinates, activations);
    flipHKeyframes = generateKeyframesForEvent<EventType::P, std::vector<Keyframe<bool>>, ParameterType::FlipH>(events, coordinates, activations);
    flipVKeyframes = generateKeyframesForEvent<EventType::P, std::vector<Keyframe<bool>>, ParameterType::FlipV>(events, coordinates, activations);
    additiveKeyframes = generateKeyframesForEvent<EventType::P, std::vector<Keyframe<bool>>>(events, coordinates, activations);
}

std::pair<double, double> Sprite::PositionAt(double time) const
{
    return keyframeValueAt<double>(positionKeyframes, time);
}

double Sprite::RotationAt(double time) const
{
    return keyframeValueAt<double>(rotationKeyframes, time);
}

std::pair<double, double> Sprite::ScaleAt(double time) const
{
    return keyframeValueAt<double>(scaleKeyframes, time);
}

Colour Sprite::ColourAt(double time) const
{
    return keyframeValueAt<Colour>(colourKeyframes, time);
}

double Sprite::OpacityAt(double time) const
{
    return keyframeValueAt<double>(opacityKeyframes, time);
}

bool Sprite::EffectAt(double time, ParameterType effect) const
{
    return keyframeValueAt<bool>(effect == ParameterType::FlipV ? flipVKeyframes : effect == ParameterType::FlipH ? flipHKeyframes : additiveKeyframes, time);
}

Layer Sprite::GetLayer() const
{
    return layer;
}

Origin Sprite::GetOrigin() const
{
    return origin;
}

const std::string Sprite::GetFilePath(double time) const
{
    return filepath;
}

std::vector<std::string> Sprite::GetFilePaths() const
{
    return std::vector<std::string>({ filepath });
}

const std::pair<double, double>& Sprite::GetCoordinates() const
{
    return coordinates;
}

const std::pair<double, double>& Sprite::GetActiveTime() const
{
    return activetime;
}

const std::pair<double, double>& Sprite::GetVisibleTime() const
{
    return visibletime;
}



Animation::Animation(Layer layer, Origin origin, const std::string& filepath, const std::pair<double, double>& coordinates, int framecount, double framedelay, LoopType looptype)
    :
    Sprite(layer, origin, filepath, coordinates),
    framecount(framecount),
    framedelay(framedelay),
    looptype(looptype)
{}

int Animation::frameIndexAt(double time) const
{
    if (time - activetime.first < framecount * framedelay || looptype == LoopType::LoopForever)
    {
        return (int)std::fmod(((time - activetime.first) / framedelay), (double)framecount);
    }
    else return framecount - 1;
}

const std::string Animation::GetFilePath(double time) const
{
    std::size_t pos = filepath.rfind(".");
    std::string base = filepath.substr(0, pos);
    std::string ext = filepath.substr(pos);
    return base + std::to_string(frameIndexAt(time)) + ext;
}

std::vector<std::string> Animation::GetFilePaths() const
{
    std::size_t pos = filepath.rfind(".");
    std::string base = filepath.substr(0, pos);
    std::string ext = filepath.substr(pos);
    std::vector<std::string> paths = std::vector<std::string>();
    for (int i = 0; i < framecount; i++)
    {
        paths.emplace_back(base + std::to_string(i) + ext);
    }
    return paths;
}

Sample::Sample(double starttime, Layer layer, const std::string& filepath, float volume)
    :
    starttime(starttime),
    layer(layer),
    filepath(filepath),
    volume(volume)
{}


Background::Background(const std::string& filepath, std::pair<double, double> offset)
    :
    filepath(filepath),
    offset(offset)
{
    exists = true;
}

Video::Video(double starttime, const std::string& filepath, std::pair<double, double> offset)
    :
    starttime(starttime),
    filepath(filepath),
    offset(offset)
{
    exists = true;
}

} // namespace sb
