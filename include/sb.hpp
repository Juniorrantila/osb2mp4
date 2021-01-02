#pragma once

#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <cstddef>
#include <limits>
#include <unordered_map>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <cstdlib>
#include <exception>

static constexpr int frameWidth = 1920;
static constexpr int frameHeight = 1080;
static constexpr double fps = 30;

namespace sb
{
	void stringReplace(std::string& s, const std::string& search, const std::string& replace);

	std::vector<std::string> stringSplit(std::string s, const std::string& delimiter);

	void applyVariables(std::string& line, const std::unordered_map<std::string, std::string>& variables);

	std::string removePathQuotes(const std::string& s);

	enum class Layer
	{
		Background,
		Fail,
		Pass,
		Foreground,
		Overlay
	};

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

	enum class LoopType
	{
		LoopForever,
		LoopOnce,
		Custom
	};
    

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
    
    enum class ParameterType
	{
		FlipH,
		FlipV,
		Additive
	};
    
    static const std::unordered_map<std::string, ParameterType> ParameterTypeStrings =
	{
		{"H", ParameterType::FlipH},
		{"V", ParameterType::FlipV},
		{"A", ParameterType::Additive}
	};

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
    
    static const std::unordered_map<std::string, EventType> EventTypeStrings
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

    enum class Keyword
	{
		None,
		Sprite,
		Animation,
		Sample,
		T,
		L
	};
    
	static const std::unordered_map<std::string, Keyword> KeywordStrings =
	{
		{"Sprite", Keyword::Sprite},
		{"Animation", Keyword::Animation},
		{"Sample", Keyword::Sample},
		{"T", Keyword::T},
		{"L", Keyword::L},
	};
    
    struct Colour
	{
        union {
            struct {
                double R;
                double G;
                double B;
            };
            double color_arr[3];
        };
		Colour(){}
		Colour(double R, double G, double B){
            this->R = R;
            this->G = G;
            this->B = B;
        }
		double operator[](int index) const
		{
			return color_arr[index];
		};
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

	template<typename T>
	class Keyframe
	{
	public:
        double time;
        uint64_t frame;
		T value;
		Easing easing;
		double actualStarttime;
    
    public:
        Keyframe(){}
		Keyframe(double time, T value, Easing easing, double actualStarttime)
			:
			time(time),
			value(value),
			easing(easing),
			actualStarttime(actualStarttime)
		{}
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
		virtual ~IEvent() {}
	};
	template <typename T>
	class Event : public IEvent
	{
    private:
		EventType type;
		Easing easing;
		double starttime;
		double endtime;
        uint64_t startFrame;
        uint64_t endFrame;
        
		T startvalue;
		T endvalue;
        
	public:
		Event() = default;
		Event(EventType type, Easing easing, double starttime, double endtime, T startvalue, T endvalue)
			:
			type(type),
			easing(easing),
			starttime(starttime),
			endtime(endtime),
			startvalue(startvalue),
			endvalue(endvalue)
		{}
		std::unique_ptr<IEvent> copy() const { return std::make_unique<Event<T>>(type, easing, starttime, endtime, startvalue, endvalue); }
		EventType GetType() const { return type; }
		Easing GetEasing() const { return easing; }
		double GetStartTime() const { return starttime; }
		double GetEndTime() const { return endtime; }
		void SetStartTime(double time) { starttime = time; }
		void SetEndTime(double time) { endtime = time; }
		T GetStartValue() const { return startvalue; }
		T GetEndValue() const { return endvalue; }
	};

    template <typename T>
	T InterpolateLinear(T start, T end, double t) { return start + (end - start) * t; }
    Colour InterpolateLinear(Colour start, Colour end, double t);
    std::pair<double, double> InterpolateLinear(std::pair<double, double> start, std::pair<double, double> end, double t);
    
    template <typename T>
	T InterpolateBilinear(T topLeft, T topRight, T bottomLeft, T bottomRight, double tx, double ty)
	{
		return InterpolateLinear(
			InterpolateLinear(topLeft, topRight, tx),
			InterpolateLinear(bottomLeft, bottomRight, tx),
			ty
		);
	}
    
	template <typename T>
	T keyframeValueAt(const std::vector<Keyframe<T>>& keyframes, double time)
	{
		Keyframe<T> keyframe = Keyframe<T>();
		Keyframe<T> endKeyframe = Keyframe<T>();
		bool found = false;
		for (int i = 0; i < keyframes.size(); i++)
		{
			if (keyframes[i].time > time)
			{
				keyframe = keyframes[i - 1];
				endKeyframe = keyframes[i];
				found = true;
				break;
			}
		}
		
        if (!found) keyframe = *(keyframes.end() - 1);
        
		if (keyframe.easing == Easing::Step)
			return keyframe.value;
        
		double t = (time - keyframe.actualStarttime) / (endKeyframe.time - keyframe.actualStarttime);
		t = applyEasing(keyframe.easing, t);
		return InterpolateLinear(keyframe.value, endKeyframe.value, t);
	}
    
    template <typename T>
	T keyframeValueAtFrame(const std::vector<Keyframe<T>>& keyframes, uint64_t frame)
	{
		Keyframe<T> keyframe = Keyframe<T>();
		Keyframe<T> endKeyframe = Keyframe<T>();
		bool found = false;
		for (int i = 0; i < keyframes.size(); i++)
		{
			if (keyframes[i].time > time)
			{
				keyframe = keyframes[i - 1];
				endKeyframe = keyframes[i];
				found = true;
				break;
			}
		}
		
        if (!found) keyframe = *(keyframes.end() - 1);
        
		if (keyframe.easing == Easing::Step)
			return keyframe.value;
        
		double t = (time - keyframe.actualStarttime) / (endKeyframe.time - keyframe.actualStarttime);
		t = applyEasing(keyframe.easing, t);
		return InterpolateLinear(keyframe.value, endKeyframe.value, t);
	}
    
    template <class T>
	std::pair<T, T> keyframeValueAt(const std::pair<std::vector<Keyframe<T>>, std::vector<Keyframe<T>>>& keyframes, double time)
	{
		T first = keyframeValueAt<T>(keyframes.first, time);
		T second = keyframeValueAt<T>(keyframes.second, time);
		return std::pair<T, T>(first, second);
	}
    
    template <typename T, typename V, typename Selector>
	void generateKeyframes(std::vector<Keyframe<T>>& keyframes, const std::vector<std::unique_ptr<sb::Event<V>>>& events, Selector W)
	{
		int i = -2;
		for (const std::unique_ptr<Event<V>>& event : events)
		{
			i += 2;
			bool appendEndtime = event->GetEndTime() > event->GetStartTime();
            
			if (i == 0)
			{
				keyframes.push_back(Keyframe<T>(
                    -std::numeric_limits<double>::infinity(),
                    W(event->GetStartValue()),
                    Easing::Step,
                    -std::numeric_limits<double>::infinity())
                    );
                    
				keyframes.push_back(
                    Keyframe<T>(event->GetStartTime(),
                    appendEndtime ? W(event->GetStartValue()) : W(event->GetEndValue()),
                    appendEndtime ? event->GetEasing() : Easing::Step,
                    event->GetStartTime())
                    );
				if (appendEndtime)
				{
					keyframes.push_back(Keyframe<T>
                        (
                            event->GetEndTime(),
                            W(event->GetEndValue()),
                            Easing::Step,
                            event->GetEndTime()
                        )
                    );
				}
				else i--;
                
				continue;
			}
			if (keyframes[i - 1].time >= event->GetStartTime())
			{
				keyframes.push_back(Keyframe<T>(
                    keyframes[i - 1].time,
                    appendEndtime ? W(event->GetStartValue()) : W(event->GetEndValue()),
                    appendEndtime ? event->GetEasing() : Easing::Step,
                    event->GetStartTime()
                    )
                );
				i--;
			}
			else
			{
				keyframes.push_back(Keyframe<T>(
                    event->GetStartTime(),
                    appendEndtime ? W(event->GetStartValue()) : W(event->GetEndValue()),
                    appendEndtime ? event->GetEasing() : Easing::Step,
                    event->GetStartTime()
                    )
                );
			}
			if (appendEndtime)
			{
				keyframes.push_back(Keyframe<T>(
                    event->GetEndTime(),
                    W(event->GetEndValue()), 
                    Easing::Step,
                    event->GetEndTime()
                    )
                );
			}
			else i--;
		}
	}
    
    template <typename T, typename V, typename Selector>
	void generateKeyframes(std::vector<Keyframe<T>>& keyframes, const std::vector<std::unique_ptr<Event<V>>>& events, Selector W);
    
	void generateParameterKeyframes(std::vector<Keyframe<bool>>& keyframes, const std::vector<std::unique_ptr<Event<ParameterType>>>& events);

	// a little bit of dumb bullshit down below
	template <typename T>
	struct nop
	{
		T operator()(T in)
		{
			return in;
		}
	};
    
	template <class ... t>
	constexpr bool alwaysFalse = false;
	template <EventType T, typename R, ParameterType P = ParameterType::Additive>
	R generateKeyframesForEvent(const std::vector<std::unique_ptr<IEvent>>& events, std::pair<double, double> coordinates)
	{
		static_assert(T != EventType::P && P == ParameterType::Additive || T == EventType::P, "Invalid template arguments");
		auto XKeyframes = std::vector<Keyframe<double>>();
		auto YKeyframes = std::vector<Keyframe<double>>();
		R keyframes = R();
		auto applicableEvents = std::vector<std::unique_ptr<IEvent>>();
		struct isApplicable
		{
			bool operator()(const std::unique_ptr<IEvent>& event)
			{
				EventType type = event->GetType();
				if constexpr (T == EventType::M) return type == EventType::M || type == EventType::MX || type == EventType::MY;
				else if constexpr (T == EventType::R) return type == EventType::R;
				else if constexpr (T == EventType::S) return type == EventType::S || type == EventType::V;
				else if constexpr (T == EventType::C) return type == EventType::C;
				else if constexpr (T == EventType::F) return type == EventType::F;
				else if constexpr (T == EventType::P && P == ParameterType::Additive) return type == EventType::P && dynamic_cast<Event<ParameterType>*>(event.get())->GetStartValue() == ParameterType::Additive;
				else if constexpr (T == EventType::P && P == ParameterType::FlipH) return type == EventType::P && dynamic_cast<Event<ParameterType>*>(event.get())->GetStartValue() == ParameterType::FlipH;
				else if constexpr (T == EventType::P && P == ParameterType::FlipV) return type == EventType::P && dynamic_cast<Event<ParameterType>*>(event.get())->GetStartValue() == ParameterType::FlipV;
				else static_assert(alwaysFalse<T>, "Template argument T invalid");
			}
		};
		isApplicable isApplicable;
		for (const std::unique_ptr<IEvent>& event : events)
			if (isApplicable(event))
				applicableEvents.push_back(event->copy());
		if (applicableEvents.size() == 0)
		{
			if constexpr (T == EventType::M)
			{
				XKeyframes.push_back(Keyframe<double>(-std::numeric_limits<double>::infinity(), coordinates.first, Easing::Step, -std::numeric_limits<double>::infinity()));
				YKeyframes.push_back(Keyframe<double>(-std::numeric_limits<double>::infinity(), coordinates.second, Easing::Step, -std::numeric_limits<double>::infinity()));
				return std::pair<std::vector<Keyframe<double>>, std::vector<Keyframe<double>>>(XKeyframes, YKeyframes);
			}
			else if constexpr (T == EventType::R)
			{
				keyframes.push_back(Keyframe<double>(-std::numeric_limits<double>::infinity(), 0, Easing::Step, -std::numeric_limits<double>::infinity()));
				return keyframes;
			}
			else if constexpr (T == EventType::S)
			{
				XKeyframes.push_back(Keyframe<double>(-std::numeric_limits<double>::infinity(), 1, Easing::Step, -std::numeric_limits<double>::infinity()));
				YKeyframes.push_back(Keyframe<double>(-std::numeric_limits<double>::infinity(), 1, Easing::Step, -std::numeric_limits<double>::infinity()));
				return std::pair<std::vector<Keyframe<double>>, std::vector<Keyframe<double>>>(XKeyframes, YKeyframes);
			}
			else if constexpr (T == EventType::C)
			{
				keyframes.push_back(Keyframe<Colour>(-std::numeric_limits<double>::infinity(), Colour(1, 1, 1), Easing::Step, -std::numeric_limits<double>::infinity()));
				return keyframes;
			}
			else if constexpr (T == EventType::F)
			{
				keyframes.push_back(Keyframe<double>(-std::numeric_limits<double>::infinity(), 1, Easing::Step, -std::numeric_limits<double>::infinity()));
				return keyframes;
			}
			else if constexpr (T == EventType::P)
			{
				keyframes.push_back(Keyframe<bool>(-std::numeric_limits<double>::infinity(), false, Easing::Step, -std::numeric_limits<double>::infinity()));
				return keyframes;
			}
		}
		if constexpr (T == EventType::M || T == EventType::S)
		{
			bool compatibilityMode = applicableEvents[0]->GetType() == EventType::M || applicableEvents[0]->GetType() == EventType::V;
			auto applicableEventsXY = std::vector<std::unique_ptr<Event<std::pair<double, double>>>>();
			auto applicableEventsX = std::vector<std::unique_ptr<Event<double>>>();
			auto applicableEventsY = std::vector<std::unique_ptr<Event<double>>>();
			if (compatibilityMode)
			{
				for (const std::unique_ptr<IEvent>& event : applicableEvents)
				{
					EventType type = event->GetType();
					if (type == EventType::M)
						applicableEventsXY.push_back(std::make_unique<Event<std::pair<double, double>>>(*dynamic_cast<Event<std::pair<double, double>>*>(event->copy().get())));
					if (type == EventType::V)
						applicableEventsXY.push_back(std::make_unique<Event<std::pair<double, double>>>(*dynamic_cast<Event<std::pair<double, double>>*>(event->copy().get())));
				}
			}
			else
			{
				for (const std::unique_ptr<IEvent>& event : applicableEvents)
				{
					EventType type = event->GetType();
					if (type == EventType::MX)
						applicableEventsX.push_back(std::make_unique<Event<double>>(*dynamic_cast<Event<double>*>(event->copy().get())));
					if (type == EventType::MY)
						applicableEventsY.push_back(std::make_unique<Event<double>>(*dynamic_cast<Event<double>*>(event->copy().get())));
					if (type == EventType::S)
					{
						applicableEventsX.push_back(std::make_unique<Event<double>>(*dynamic_cast<Event<double>*>(event->copy().get())));
						applicableEventsY.push_back(std::make_unique<Event<double>>(*dynamic_cast<Event<double>*>(event->copy().get())));
					}
				}
			}
			struct first
			{
				double operator()(std::pair<double, double> in)
				{
					return in.first;
				}
			};
			struct second
			{
				double operator()(std::pair<double, double> in)
				{
					return in.second;
				}
			};
			if (compatibilityMode)
			{
				generateKeyframes<double, std::pair<double, double>>(XKeyframes, applicableEventsXY, first());
				generateKeyframes<double, std::pair<double, double>>(YKeyframes, applicableEventsXY, second());
			}
			else
			{
				if (applicableEventsX.size() == 0)
					if constexpr (T == EventType::M)
						XKeyframes.push_back(Keyframe<double>(-std::numeric_limits<double>::infinity(), coordinates.first, Easing::Step, -std::numeric_limits<double>::infinity()));
					else
						XKeyframes.push_back(Keyframe<double>(-std::numeric_limits<double>::infinity(), 1, Easing::Step, -std::numeric_limits<double>::infinity()));

				else
					generateKeyframes<double, double>(XKeyframes, applicableEventsX, nop<double>());
				if (applicableEventsY.size() == 0)
					if constexpr (T == EventType::M)
						YKeyframes.push_back(Keyframe<double>(-std::numeric_limits<double>::infinity(), coordinates.second, Easing::Step, -std::numeric_limits<double>::infinity()));
					else
						YKeyframes.push_back(Keyframe<double>(-std::numeric_limits<double>::infinity(), 1, Easing::Step, -std::numeric_limits<double>::infinity()));
				else
					generateKeyframes<double, double>(YKeyframes, applicableEventsY, nop<double>());
			}
			return std::pair<std::vector<Keyframe<double>>, std::vector<Keyframe<double>>>(XKeyframes, YKeyframes);
		}
		else if constexpr (T == EventType::C)
		{
			auto ev = std::vector<std::unique_ptr<Event<Colour>>>();
			for (const auto& e : applicableEvents)
			{
				ev.push_back(std::make_unique<Event<Colour>>(*dynamic_cast<Event<Colour>*>(e->copy().get())));
			}
			generateKeyframes<Colour, Colour>(keyframes, ev, nop<Colour>());
			return keyframes;
		}
		else if constexpr (T == EventType::P)
		{
			auto ev = std::vector<std::unique_ptr<Event<ParameterType>>>();
			for (const auto& e : applicableEvents)
			{
				ev.push_back(std::make_unique<Event<ParameterType>>(*dynamic_cast<Event<ParameterType>*>(e->copy().get())));
			}
			generateParameterKeyframes(keyframes, ev);
			return keyframes;
		}
		else
		{
			auto ev = std::vector<std::unique_ptr<Event<double>>>();
			for (const auto& e : applicableEvents)
			{
				ev.push_back(std::make_unique<Event<double>>(*dynamic_cast<Event<double>*>(e->copy().get())));
			}
			generateKeyframes<double, double>(keyframes, ev, nop<double>());
			return keyframes;
		}
	}

	class Loop
	{
    private:
		std::vector<std::unique_ptr<IEvent>> events;
		double starttime;
		double endtime;
		double looplength;
		int loopcount;
        
	public:
		Loop(double starttime, int loopcount)
			:
			starttime(starttime),
			loopcount(loopcount)
		{
			endtime = 0;
			looplength = 0;
		}
		template <typename T>
		void AddEvent(std::unique_ptr<Event<T>> event)
		{
			events.push_back(std::move(event));
		}
		void Initialise()
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
			endtime = starttime + (looplength)*loopcount;
		}
		std::vector<std::unique_ptr<IEvent>>& GetEvents() { return events; }
		double GetStartTime()   const { return starttime; }
		double GetEndTime()     const { return endtime; }
		double GetLoopLength()  const { return looplength; }
		int GetLoopCount()      const { return loopcount; }
	};

	class Trigger
	{
    private:
		std::vector<std::unique_ptr<IEvent>> events;
		std::string triggerName;
		double starttime;
		double endtime;
		int groupNumber;
        
	public:
		Trigger(const std::string& triggerName, double starttime, double endtime, int groupNumber)
			:
			triggerName(triggerName),
			starttime(starttime),
			endtime(endtime),
			groupNumber(groupNumber)
		{}
		template <typename T>
		void AddEvent(std::unique_ptr<Event<T>> event) { events.push_back(std::move(event)); }
		void Initialise()
		{
			// TODO
		}
		const std::vector<std::unique_ptr<IEvent>>& GetEvents() const { return events; }
		const std::string& GetTriggerName() const { return triggerName; }
		double GetStartTime()   const { return starttime; }
		double GetEndTime()     const { return endtime; }
		int GetGroupNumber()    const { return groupNumber; }
	};

	class Sprite
	{
    protected:
		std::vector<Loop> loops;
		std::vector<Trigger> triggers;
		std::pair<double, double> activetime;
        std::pair<uint64_t, uint64_t> activeFrames;
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
	
    public:
		Sprite(Layer layer, Origin origin, const std::string& filepath, const std::pair<double, double>& coordinates)
			:
			layer(layer),
			origin(origin),
			filepath(filepath),
			coordinates(coordinates)
		{}
		
        template <typename T>
		void AddEvent(std::unique_ptr<Event<T>> event)
		{
			events.push_back(std::move(event));
		}
        
		template <typename T>
		void AddEventInLoop(std::unique_ptr<Event<T>> event)
		{
			(loops.end() - 1)->AddEvent(std::move(event));
		}
		
        template <typename T>
		void AddEventInTrigger(std::unique_ptr<Event<T>> event)
		{
			(triggers.end() - 1)->AddEvent(std::move(event));
		}
		
        void AddLoop(Loop loop)
		{
			loops.push_back(std::move(loop));
		}
		
        void AddTrigger(Trigger trigger)
		{
			triggers.push_back(std::move(trigger));
		}
		
        void Initialise()
		{
			initialised = true;
			for (Loop& loop : loops) loop.Initialise();
			for (Trigger& trigger : triggers) trigger.Initialise();
			for (Loop& loop : loops)
				for (std::unique_ptr<IEvent>& event : loop.GetEvents())
					events.emplace_back(std::move(event));
			std::sort(events.begin(), events.end(), [](const std::unique_ptr<IEvent>& a, const std::unique_ptr<IEvent>& b) { return a->GetStartTime() < b->GetStartTime(); });
			double endTime = std::numeric_limits<double>::min();
			for (const std::unique_ptr<IEvent>& event : events)
				endTime = std::max(endTime, event->GetEndTime());
			activetime = std::pair<double, double>({ events.size() > 0 ? (*events.begin())->GetStartTime() : std::numeric_limits<double>::max(), endTime });
            activeFrames = std::pair<double, double>({activetime.first * fps, activetime.second * fps});

			positionKeyframes = generateKeyframesForEvent<EventType::M, std::pair<std::vector<Keyframe<double>>, std::vector<Keyframe<double>>>>(events, coordinates);
			rotationKeyframes = generateKeyframesForEvent<EventType::R, std::vector<Keyframe<double>>>(events, coordinates);
			scaleKeyframes = generateKeyframesForEvent<EventType::S, std::pair<std::vector<Keyframe<double>>, std::vector<Keyframe<double>>>>(events, coordinates);
			colourKeyframes = generateKeyframesForEvent<EventType::C, std::vector<Keyframe<Colour>>>(events, coordinates);
			opacityKeyframes = generateKeyframesForEvent<EventType::F, std::vector<Keyframe<double>>>(events, coordinates);
			flipHKeyframes = generateKeyframesForEvent<EventType::P, std::vector<Keyframe<bool>>, ParameterType::FlipH>(events, coordinates);
			flipVKeyframes = generateKeyframesForEvent<EventType::P, std::vector<Keyframe<bool>>, ParameterType::FlipV>(events, coordinates);
			additiveKeyframes = generateKeyframesForEvent<EventType::P, std::vector<Keyframe<bool>>>(events, coordinates);
		}
		
        std::pair<double, double> PositionAt(double time) const
		{
			return keyframeValueAt<double>(positionKeyframes, time);
		}
		
        double RotationAt(double time) const
		{
			return keyframeValueAt<double>(rotationKeyframes, time);
		}
		
        std::pair<double, double> ScaleAt(double time) const
		{
			return keyframeValueAt<double>(scaleKeyframes, time);
		}
		
        Colour ColourAt(double time) const
		{
			return keyframeValueAt<Colour>(colourKeyframes, time);
		}
		
        double OpacityAt(double time) const
		{
			return keyframeValueAt<double>(opacityKeyframes, time);
		}
		
        bool EffectAt(double time, ParameterType effect) const
		{
			return keyframeValueAt<bool>(effect == ParameterType::FlipV ? flipVKeyframes : effect == ParameterType::FlipH ? flipHKeyframes : additiveKeyframes, time);
		}
		
        Layer GetLayer() const { return layer; }
		
        Origin GetOrigin() const { return origin; }
		
        virtual const std::string GetFilePath(double time) const { return filepath; }
		
        virtual std::vector<std::string> GetFilePaths() const { return std::vector<std::string>({ filepath }); }
		
        const std::pair<double, double>& GetCoordinates() const { return coordinates; }
		
        const std::pair<double, double>& GetActiveTime() const { return activetime; }
        
	};

	class Animation : public Sprite
	{
    private:
		const int framecount;
		const double framedelay;
		const LoopType looptype;
        
	public:
		Animation(Layer layer, Origin origin, const std::string& filepath, const std::pair<double, double>& coordinates, int framecount, double framedelay, LoopType looptype)
			:
			Sprite(layer, origin, filepath, coordinates),
			framecount(framecount),
			framedelay(framedelay),
			looptype(looptype)
		{}
		int frameIndexAt(double time) const
		{
			if (time - activetime.first < framecount * framedelay || looptype == LoopType::LoopForever)
			{
				return (int)std::fmod(((time - activetime.first) / framedelay), (double)framecount);
			}
			else return framecount - 1;
		}
		const std::string GetFilePath(double time) const
		{
			std::size_t pos = filepath.rfind(".");
			std::string base = filepath.substr(0, pos);
			std::string ext = filepath.substr(pos);
			return base + std::to_string(frameIndexAt(time)) + ext;
		}
		std::vector<std::string> GetFilePaths() const
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
	};

	class Sample
	{
	public:
		Sample(double starttime, Layer layer, const std::string& filepath, float volume)
			:
			starttime(starttime),
			layer(layer),
			filepath(filepath),
			volume(volume)
		{}
		const double starttime;
		const Layer layer;
		const std::string filepath;
		const float volume;
	};

	class Background
	{
	public:
        const std::string filepath;
		std::pair<double, double> offset;
    public:
		Background(const std::string& filepath, std::pair<double, double> offset)
			:
			filepath(filepath),
			offset(offset)
		{}
	};

	// taken from https://stackoverflow.com/questions/478898#478960
	std::string exec(const std::string& cmd);
    
	double getAudioDuration(const std::string& filepath);

	cv::Mat readImageFile(const std::string& filepath);

	class Storyboard
	{
    private:
		std::filesystem::path directory;
		std::filesystem::path osb;
		std::vector<std::unique_ptr<Sprite>> sprites;
		std::vector<Sample> samples;
		std::unordered_map<std::string, std::string> info;
		std::pair<double, double> activetime;
		// const Background background;
		// const Video video;
		const std::pair<std::size_t, std::size_t> resolution;
		double musicVolume;
		double effectVolume;
		double audioDuration;
		double audioLeadIn;
		std::unordered_map<std::string, cv::Mat> spriteImages;
		cv::Mat backgroundImage;
		double frameScale;
		double xOffset;
        
	public:
		Storyboard(const std::filesystem::path& directory, const std::filesystem::path& osb, std::vector<std::unique_ptr<Sprite>>& sprites, std::vector<Sample>& samples, std::vector<Background> backgrounds, /*Video video, */ const std::unordered_map<std::string, std::string>& info, std::pair<std::size_t, std::size_t> resolution, double musicVolume, double effectVolume)
			:
			directory(directory),
			osb(osb),
			samples(samples),
			info(info),
			resolution(resolution),
			musicVolume(musicVolume),
			effectVolume(effectVolume),
			frameScale(resolution.second / 480.0),
			xOffset((resolution.first - resolution.second * 4 / 3.0) * 0.5)
		{
			this->sprites = std::move(sprites);
			std::cout << "Initialising storyboard (" << this->sprites.size() << " sprites, " << samples.size() << " samples)" << "\n";
			for (std::unique_ptr<Sprite>& sprite : this->sprites)
				sprite->Initialise();
			std::pair<double, double> activetime = { std::numeric_limits<int>::max(), std::numeric_limits<int>::min() };

			bool backgroundIsASprite = false;
			for (const std::unique_ptr<Sprite>& sprite : this->sprites)
			{
				std::pair<double, double> at = sprite->GetActiveTime();
				activetime.first = std::min(activetime.first, at.first);
				activetime.second = std::max(activetime.second, at.second);
				if (!backgrounds.empty())
					backgroundIsASprite = std::max(backgrounds.front().filepath == sprite->GetFilePath(0), backgroundIsASprite);
			}
			this->activetime = activetime;
			auto k = info.find("AudioFilename");
			if (k != info.end()) this->audioDuration = 1000 * getAudioDuration((directory / k->second).generic_string());
			else this->audioDuration = 0;
			auto l = info.find("AudioLeadIn");
			if (k != info.end() && l != info.end()) this->audioLeadIn = std::stoi(l->second);
			else this->audioLeadIn = 0;
			std::cout << "Initialised " << this->sprites.size() << " sprites/animations\n";

			backgroundImage = cv::Mat::zeros(resolution.second, resolution.first, CV_8UC3);
			if (!backgrounds.empty() && !backgroundIsASprite)
			{
				Background background = backgrounds.front();
				cv::Mat image = readImageFile((directory / background.filepath).generic_string());
				cv::RotatedRect quadRect = cv::RotatedRect(
					cv::Point2f(
						resolution.first / 2.0f + background.offset.first * frameScale,
						resolution.second / 2.0f + background.offset.second * frameScale
					),
					cv::Size2f(resolution.second * image.cols / (double)image.rows, resolution.second),
					0
				);
				cv::Point2f quad[4];
				quadRect.points(quad);
				RasteriseQuad(backgroundImage.begin<cv::Vec<uint8_t, 3>>(), image.begin<cv::Vec<float, 4>>(), image.cols, image.rows, quad, Colour(1, 1, 1), false, 1);
			}

			for (const std::unique_ptr<Sprite>& sprite : this->sprites)
			{
				std::vector<std::string> filePaths = sprite->GetFilePaths();
				for (std::string filePath : filePaths)
				{
					if (spriteImages.find(filePath) != spriteImages.end()) continue;
					cv::Mat image = readImageFile((directory / filePath).generic_string());
					spriteImages.emplace(filePath, image);
				}
			}
		}
		std::pair<double, double> GetActiveTime() const { return activetime; }
        
		double GetAudioDuration() const { return audioDuration; }
		double GetAudioLeadIn() const { return audioLeadIn; }
		void generateAudio(const std::string& outFile) const
		{
			std::string command = "ffmpeg -y -hide_banner -v error -stats";
			auto k = info.find("AudioFilename");
			if (k != info.end()) command += " -i \"" + (directory / k->second).generic_string() + "\"";
			std::vector<int> delays;
			std::vector<double> volumes;
			volumes.push_back((samples.size() + 1) * musicVolume);
			auto l = info.find("AudioLeadIn");
			if (k != info.end() && l != info.end()) delays.push_back(std::stoi(l->second));
			else if (k != info.end()) delays.push_back(1);
			int maxDelay = samples.size() == 0 ? 0 : std::numeric_limits<int>::lowest();
			double maxDuration = samples.size() == 0 ? 0 : std::numeric_limits<double>::lowest();
			for (const Sample& sample : samples)
			{
				command += " -i \"" + (directory / sample.filepath).generic_string() + "\"";
				int delay = (int)sample.starttime;
				delays.push_back(delay);
				volumes.push_back(sample.volume / 100.0 * (samples.size() + 1) * effectVolume);
				maxDelay = std::max(maxDelay, delay);
				maxDuration = std::max(maxDuration, getAudioDuration((directory / sample.filepath).generic_string()));
			}
			command += " -filter_complex ";
			std::string filters;
			for (int i = 0; i < delays.size(); i++)
				filters += "[" + std::to_string(i) + ":a]volume=" + std::to_string(volumes[i]) + ",adelay=delays=" + std::to_string(delays[i]) + ":all=1[d" + std::to_string(i) + "];";
			for (int i = 0; i < delays.size(); i++)
				filters += "[d" + std::to_string(i) + "]";
			filters += "amix=inputs=" + std::to_string(delays.size()) + ":dropout_transition=" + std::to_string(maxDelay + maxDuration) + "[a]";
			command += "\"" + filters + "\"";
			command += " -map [a] ";
			command += outFile;
			//std::cout << command << "\n";
			system(command.c_str());
		}
		cv::Mat DrawFrame(double time) const
		{
			cv::Mat frame = backgroundImage.clone();
			cv::MatIterator_<cv::Vec<uint8_t, 3>> frameStart = frame.begin<cv::Vec<cv::uint8_t, 3>>();
			for (const std::unique_ptr<Sprite>& sprite : sprites)
			{
				if (!(sprite->GetActiveTime().first <= time && sprite->GetActiveTime().second > time))
					continue;
				double alpha = sprite->OpacityAt(time);
				if (alpha == 0) continue;
				std::pair<double, double> scale = sprite->ScaleAt(time);
				if (scale.first == 0 || scale.second == 0) continue;

				std::unordered_map<std::string, cv::Mat>::const_iterator k = spriteImages.find(sprite->GetFilePath(time));
				if (k == spriteImages.end()) continue;
				const cv::Mat image = k->second;
				const cv::MatConstIterator_<cv::Vec<float, 4>> imageStart = image.begin<cv::Vec<float, 4>>();

				scale = std::pair<double, double>(scale.first * frameScale, scale.second * frameScale);
				std::pair<double, double> newSize = std::pair<double, double>(image.cols * std::abs(scale.first), image.rows * std::abs(scale.second));

				if (newSize.first < 1 || newSize.second < 1) continue;

				// create frame-space quad
				float width = newSize.first;
				float height = newSize.second;
				std::pair<double, double> position = sprite->PositionAt(time);
				cv::Vec2f positionVec = { (float)position.first, (float)position.second };
				cv::Vec2f origin = GetOriginVector(sprite->GetOrigin(), width, height);
				cv::Vec2f centre = GetOriginVector(Origin::Centre, width, height);
				float rotation = sprite->RotationAt(time);
				if (rotation != 0)
				{
					centre -= origin;
					centre =
					{
						centre[0] * std::cos(rotation) - centre[1] * std::sin(rotation),
						centre[1] * std::cos(rotation) + centre[0] * std::sin(rotation)
					};
					centre += origin;
				}
				cv::Vec2f frameQuadCentrePoint = positionVec * frameScale
					+ centre - origin
					+ cv::Vec2f(xOffset, 0);
				cv::RotatedRect quadRect = cv::RotatedRect(frameQuadCentrePoint, cv::Size2f(width, height), rotation != 0 ? rotation * 180.0f / PI : 0);
				cv::Point2f quad[4]; // bottomLeft, topLeft, topRight, bottomRight
				quadRect.points(quad);

				// flip quad if needed
				bool flipH = sprite->EffectAt(time, ParameterType::FlipH) || scale.first < 0;
				bool flipV = sprite->EffectAt(time, ParameterType::FlipV) || scale.second < 0;
				if (flipH && flipV)
				{
					std::swap(quad[0], quad[2]);
					std::swap(quad[1], quad[3]);
				}
				else if (flipH)
				{
					std::swap(quad[0], quad[3]);
					std::swap(quad[1], quad[2]);
				}
				else if (flipV)
				{
					std::swap(quad[0], quad[1]);
					std::swap(quad[2], quad[3]);
				}

				Colour colour = sprite->ColourAt(time);
				bool additive = sprite->EffectAt(time, ParameterType::Additive);

				RasteriseQuad(frameStart, imageStart, image.cols, image.rows, quad, colour, additive, alpha);
			}
			return frame;
		}
        
        cv::Mat DrawFrame(int _frame) const
		{
            
            double time = _frame * 1000.0 / fps;
            
			cv::Mat frame = backgroundImage.clone();
			cv::MatIterator_<cv::Vec<uint8_t, 3>> frameStart = frame.begin<cv::Vec<cv::uint8_t, 3>>();
			for (const std::unique_ptr<Sprite>& sprite : sprites)
			{
				if (!(sprite->GetActiveTime().first <= _frame && sprite->GetActiveTime().second > _frame))
					continue;
				double alpha = sprite->OpacityAt(time);
				if (alpha == 0) continue;
				std::pair<double, double> scale = sprite->ScaleAt(time);
				if (scale.first == 0 || scale.second == 0) continue;

				std::unordered_map<std::string, cv::Mat>::const_iterator k = spriteImages.find(sprite->GetFilePath(time));
				if (k == spriteImages.end()) continue;
				const cv::Mat image = k->second;
				const cv::MatConstIterator_<cv::Vec<float, 4>> imageStart = image.begin<cv::Vec<float, 4>>();

				scale = std::pair<double, double>(scale.first * frameScale, scale.second * frameScale);
				std::pair<double, double> newSize = std::pair<double, double>(image.cols * std::abs(scale.first), image.rows * std::abs(scale.second));

				if (newSize.first < 1 || newSize.second < 1) continue;

				// create frame-space quad
				float width = newSize.first;
				float height = newSize.second;
				std::pair<double, double> position = sprite->PositionAt(time);
				cv::Vec2f positionVec = { (float)position.first, (float)position.second };
				cv::Vec2f origin = GetOriginVector(sprite->GetOrigin(), width, height);
				cv::Vec2f centre = GetOriginVector(Origin::Centre, width, height);
				float rotation = sprite->RotationAt(time);
				if (rotation != 0)
				{
					centre -= origin;
					centre =
					{
						centre[0] * std::cos(rotation) - centre[1] * std::sin(rotation),
						centre[1] * std::cos(rotation) + centre[0] * std::sin(rotation)
					};
					centre += origin;
				}
				cv::Vec2f frameQuadCentrePoint = positionVec * frameScale
					+ centre - origin
					+ cv::Vec2f(xOffset, 0);
				cv::RotatedRect quadRect = cv::RotatedRect(frameQuadCentrePoint, cv::Size2f(width, height), rotation != 0 ? rotation * 180.0f / PI : 0);
				cv::Point2f quad[4]; // bottomLeft, topLeft, topRight, bottomRight
				quadRect.points(quad);

				// flip quad if needed
				bool flipH = sprite->EffectAt(time, ParameterType::FlipH) || scale.first < 0;
				bool flipV = sprite->EffectAt(time, ParameterType::FlipV) || scale.second < 0;
				if (flipH && flipV)
				{
					std::swap(quad[0], quad[2]);
					std::swap(quad[1], quad[3]);
				}
				else if (flipH)
				{
					std::swap(quad[0], quad[3]);
					std::swap(quad[1], quad[2]);
				}
				else if (flipV)
				{
					std::swap(quad[0], quad[1]);
					std::swap(quad[2], quad[3]);
				}

				Colour colour = sprite->ColourAt(time);
				bool additive = sprite->EffectAt(time, ParameterType::Additive);

				RasteriseQuad(frameStart, imageStart, image.cols, image.rows, quad, colour, additive, alpha);
			}
			return frame;
		}
	
    private:
		void RasteriseQuad(cv::MatIterator_<cv::Vec<uint8_t, 3>> frameStart, cv::MatConstIterator_<cv::Vec<float, 4>> imageStart, int imageWidth, int imageHeight, cv::Point2f quad[4], Colour colour, bool additive, double alpha) const
		{
			alpha /= 255.0;
			std::vector<std::pair<int, int>> ContourX; int y;
			ContourX.reserve(resolution.second);

			for (y = 0; y < resolution.second; y++)
			{
				ContourX.push_back({ std::numeric_limits<int>::max(), std::numeric_limits<int>::min() });
			}

			ScanLine(quad[0].x, quad[0].y, quad[1].x, quad[1].y, ContourX);
			ScanLine(quad[1].x, quad[1].y, quad[2].x, quad[2].y, ContourX);
			ScanLine(quad[2].x, quad[2].y, quad[3].x, quad[3].y, ContourX);
			ScanLine(quad[3].x, quad[3].y, quad[0].x, quad[0].y, ContourX);

			float imageX = 0;
			float imageY = 0;

			float minY = std::numeric_limits<float>::max();
			float maxY = std::numeric_limits<float>::min();
			for (int i = 0; i < 4; i++)
			{
				minY = std::min(quad[i].y, minY);
				maxY = std::max(quad[i].y, maxY);
			}
			minY = std::max(minY, 0.0f);
			maxY = std::min(maxY, (float)resolution.second - 1);

			for (y = (int)minY; y <= (int)maxY; y++)
			{
				if (ContourX[y].second >= ContourX[y].first)
				{
					int x = ContourX[y].first;
					int len = 1 + ContourX[y].second - ContourX[y].first;

					while (len--)
					{
						// image-space coords
						float u = (-(x - quad[0].x) * (quad[1].y - quad[0].y) + (y - quad[0].y) * (quad[1].x - quad[0].x))
							/ (-(quad[2].x - quad[0].x) * (quad[1].y - quad[0].y) + (quad[2].y - quad[0].y) * (quad[1].x - quad[0].x));
						float v = (-(x - quad[1].x) * (quad[2].y - quad[1].y) + (y - quad[1].y) * (quad[2].x - quad[1].x))
							/ (-(quad[0].x - quad[1].x) * (quad[2].y - quad[1].y) + (quad[0].y - quad[1].y) * (quad[2].x - quad[1].x));

						// sample colour with bilinear interpolation
						Colour imageColour;
						float imageAlpha;
						SampleColourAndAlpha(imageStart, imageWidth, imageHeight, u, v, imageColour, imageAlpha);

						cv::Vec<uint8_t, 3> framePixel = GetPixel(frameStart, resolution.first, x, y);

						float newAlpha = imageAlpha * alpha;

						// additive (linear dodge) or normal blend mode
						cv::Vec<uint8_t, 3> newPixel;
						if (additive)
						{
							newPixel = cv::Vec<uint8_t, 3>(
								cv::saturate_cast<uint8_t>(framePixel[0] + newAlpha * imageColour[2] * colour[2]),
								cv::saturate_cast<uint8_t>(framePixel[1] + newAlpha * imageColour[1] * colour[1]),
								cv::saturate_cast<uint8_t>(framePixel[2] + newAlpha * imageColour[0] * colour[0])
								);
						}
						else
						{
							newPixel = cv::Vec<uint8_t, 3>(
								cv::saturate_cast<uint8_t>((1 - newAlpha) * framePixel[0] + newAlpha * imageColour[2] * colour[2]),
								cv::saturate_cast<uint8_t>((1 - newAlpha) * framePixel[1] + newAlpha * imageColour[1] * colour[1]),
								cv::saturate_cast<uint8_t>((1 - newAlpha) * framePixel[2] + newAlpha * imageColour[0] * colour[0])
								);
						}

						SetPixel(frameStart, resolution.first, x++, y, newPixel);
					}
				}
			}
		}
		void SetPixel(cv::MatIterator_<cv::Vec<uint8_t, 3>> imageStart, int width, int x, int y, cv::Vec<uint8_t, 3> pixel) const
		{
			*(imageStart + y * width + x) = pixel; 
        }
		cv::Vec<uint8_t, 3> GetPixel(const cv::MatIterator_<cv::Vec<uint8_t, 3>> imageStart, int width, int x, int y) const
		{
			return *(imageStart + y * width + x);
		}
		void SampleColourAndAlpha(cv::MatConstIterator_<cv::Vec<float, 4>> imageStart, int width, int height, float u, float v, Colour& outputColour, float& outputAlpha) const
		{
			// i sure hope i'm doing this correctly
			float x = u * width;
			float y = v * height;
			if (y < 0 || x < 0 || x >= width || y >= height)
			{
				outputColour = Colour(0, 0, 0);
				outputAlpha = 0;
				return;
			}
			auto it = imageStart + (int)y * width + (int)x;

			float dx = x - (int)x;
			float dy = y - (int)y;
			bool onRightEdge = (int)x == width - 1;
			bool onLastRow = (int)y == height - 1;
			auto right = onRightEdge ? it : it + 1;
			auto down = onLastRow ? it : it + width;
			auto downright = onRightEdge ? onLastRow ? it : it + width : onLastRow ? it + 1 : it + width + 1;

			// interpolate nearest neighbour
			// cv::Vec<float, 4> sample = dx < 0.5f ? dy < 0.5f ? *it : *down : dy < 0.5f ? *right : *downright;

			cv::Vec<float, 4> sample = InterpolateBilinear(*it, *right, *down, *downright, dx, dy);

			outputColour = Colour(sample[2], sample[1], sample[0]);
			outputAlpha = sample[3];
		}
		cv::Vec2f GetOriginVector(const Origin origin, const int width, const int height) const
		{
			switch (origin)
			{
                case Origin::TopLeft:       return cv::Vec2f(0, 0);
                case Origin::TopCentre:     return cv::Vec2f(width * 0.5f, 0);
                case Origin::TopRight:      return cv::Vec2f(width, 0);
                case Origin::CentreLeft:    return cv::Vec2f(0, height * 0.5f);
                case Origin::Centre:        return cv::Vec2f(width * 0.5f, height * 0.5f);
                case Origin::CentreRight:   return cv::Vec2f(width, height * 0.5f);
                case Origin::BottomLeft:    return cv::Vec2f(0, height);
                case Origin::BottomCentre:  return cv::Vec2f(width * 0.5f, height);
                case Origin::BottomRight:   return cv::Vec2f(width, height);
                default: return cv::Vec2f(width * 0.5f, height * 0.5f);
			}
		}
		// i couldn't be bothered to write my own rasterizer so this next function is adapted from
		// https://stackoverflow.com/questions/7870533/c-triangle-rasterization#7870925
		// this algorithm can be used for any convex polygon apparently. i'm using quads for the sprites
		void ScanLine(int x1, int y1, int x2, int y2, std::vector<std::pair<int, int>>& ContourX) const
		{
			int sx, sy, dx1, dy1, dx2, dy2, x, y, m, n, k, cnt;

			sx = x2 - x1;
			sy = y2 - y1;

			if (sx > 0) dx1 = 1;
			else if (sx < 0) dx1 = -1;
			else dx1 = 0;

			if (sy > 0) dy1 = 1;
			else if (sy < 0) dy1 = -1;
			else dy1 = 0;

			m = std::abs(sx);
			n = std::abs(sy);
			dx2 = dx1;
			dy2 = 0;

			if (m < n)
			{
				m = std::abs(sy);
				n = std::abs(sx);
				dx2 = 0;
				dy2 = dy1;
			}

			x = x1; y = y1;
			cnt = m + 1;
			k = n / 2;

			while (cnt--)
			{
				if ((y >= 0) && (y < resolution.second))
				{
					if (x < ContourX[y].first) ContourX[y].first = std::max(x, 0);
					if (x > ContourX[y].second) ContourX[y].second = std::min(x + 1, (int)resolution.first - 1);
				}

				k += n;
				if (k < m)
				{
					x += dx2;
					y += dy2;
				}
				else
				{
					k -= m;
					x += dx1;
					y += dy1;
				}
			}
		}
	};

	void parseFile(std::ifstream& file, std::vector<std::unique_ptr<Sprite>>& sprites, std::vector<Sample>& samples, std::vector<Background>& backgrounds, std::unordered_map<std::string, std::string>& variables, std::unordered_map<std::string, std::string>& info, size_t& lineNumber);

	std::unique_ptr<Storyboard> ParseStoryboard(const std::string& directory, const std::string& diff, std::pair<size_t, size_t> resolution, double musicVolume, double effectVolume);
}