#pragma once

#include <memory>
#include <string>

namespace sb
{
    class Colour
    {
    public:
        Colour(){}
        Colour(double R, double G, double B);
        double operator[](const int index) const;
        Colour operator+(const Colour& other) const;
        Colour operator-(const Colour& other) const;
        Colour operator*(const double other) const;
        double R;
        double G;
        double B;
    };

    class IEvent
    {
    public:
        virtual std::unique_ptr<IEvent> copy() const = 0;
        virtual EventType GetType() const = 0;
        virtual Easing GetEasing() const = 0;
        virtual double GetStartTime() const = 0;
        virtual double GetEndTime() const = 0;
        virtual void SetStartTime(double) = 0;
        virtual void SetEndTime(double) = 0;
        virtual int GetTriggerID() const = 0;
        virtual double GetTriggerST() const = 0;
        virtual int GetTriggerGP() const = 0;
        virtual void SetTriggerID(int, double, int) = 0;
        virtual ~IEvent() {}
    };
    
    template <typename T>
    class Event : public IEvent
    {
    public:
        Event() = default;
        Event(EventType type, Easing easing, double starttime, double endtime, T startvalue, T endvalue, int triggerID = 0, double triggerST = 0, double triggerGP = 0);
        std::unique_ptr<IEvent> copy() const;
        EventType GetType() const;
        Easing GetEasing() const;
        double GetStartTime() const;
        double GetEndTime() const;
        void SetStartTime(double time);
        void SetEndTime(double time);
        T GetStartValue() const;
        T GetEndValue() const;
        int GetTriggerID() const;
        double GetTriggerST() const;
        int GetTriggerGP() const;
        void SetTriggerID(int id, double st, int gp);
    private:
        EventType type;
        Easing easing;
        double starttime;
        double endtime;
        T startvalue;
        T endvalue;
        int triggerID;
        double triggerST;
        int triggerGP;
    };

    class HitSound
    {
    public:
        HitSound() {}
        HitSound(int normalSet, int additionSet, int additionFlag, int index);
        HitSound(const std::string& triggerType);
        bool operator==(const HitSound& other) const;
        static bool IsHitSound(const std::string& triggerType);

    private:
        char normalSet = 0; // 0 - no sample set, 1 - normal, 2 - soft, 3 - drum
        char additionSet = 0; // 0 - no sample set, 1 - normal, 2 - soft, 3 - drum
        char additionFlag = 0; // bitflag: 0 - normal, 1 - whistle, 2 - finish, 3 - clap
        char index = 0;
    };
}
