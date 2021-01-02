#include <sb.hpp>

//hello

namespace sb {
    
    static const std::unordered_map<std::string, sb::Layer> LayerStrings =
	{
		{"Background", Layer::Background},
		{"Fail", Layer::Fail},
		{"Pass", Layer::Pass},
		{"Foreground", Layer::Foreground},
		{"Overlay", Layer::Overlay}
	};
    
    static const std::unordered_map<std::string, sb::Origin> OriginStrings =
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
    
    static const std::unordered_map<std::string, sb::LoopType> LoopTypeStrings =
	{
		{"LoopForever", LoopType::LoopForever},
		{"LoopOnce", LoopType::LoopOnce}
	};
    
    void stringReplace(std::string& s, const std::string& search, const std::string& replace){
    	std::size_t pos = 0;
		while ((pos = s.find(search, pos)) != std::string::npos) {
			s.replace(pos, search.length(), replace);
			pos += replace.length();
		}
	}
    
    std::vector<std::string> stringSplit(std::string s, const std::string& delimiter)
	{
		std::size_t pos = 0;
		std::vector<std::string> split;
		for (; (pos = s.find(delimiter)) != std::string::npos; s.erase(0, pos + delimiter.length()))
			split.emplace_back(s.substr(0, pos));
		split.emplace_back(s);
		return split;
	}

    void applyVariables(std::string& line, const std::unordered_map<std::string, std::string>& variables)
	{
		for (const std::pair<std::string, std::string>& e : variables)
			stringReplace(line, e.first, e.second);
	}
    
    std::string removePathQuotes(const std::string& s)
	{
		return s[0] == '"' && s[s.length() - 1] == '"' ? s.substr(1, s.length() - 2) : s;
	}
    
    
	double Reverse(double(*f)(double), double t) { return 1 - f(1 - t); }
	double ToInOut(double(*f)(double), double t) { return 0.5 * (t < 0.5 ? f(2 * t) : (2 - f(2 - 2 * t))); }
	double Step(double t) { return t >= 1 ? 1 : 0; }
	double Linear(double t) { return t; }
	double InQuad(double t) { return t * t; }
	double OutQuad(double t) { return Reverse(InQuad, t); }
	double InOutQuad(double t) { return ToInOut(InQuad, t); }
	double InCubic(double t) { return t * t * t; }
	double OutCubic(double t) { return Reverse(InCubic, t); }
	double InOutCubic(double t) { return ToInOut(InCubic, t); }
	double InQuart(double t) { return t * t * t * t; }
	double OutQuart(double t) { return Reverse(InQuart, t); }
	double InOutQuart(double t) { return ToInOut(InQuart, t); }
	double InQuint(double t) { return t * t * t * t * t; }
	double OutQuint(double t) { return Reverse(InQuint, t); }
	double InOutQuint(double t) { return ToInOut(InQuint, t); }
	double InSine(double t) { return 1 - std::cos(t * PI / 2); }
	double OutSine(double t) { return Reverse(InSine, t); }
	double InOutSine(double t) { return ToInOut(InSine, t); }
	double InExpo(double t) { return std::pow(2, 10 * (t - 1)); }
	double OutExpo(double t) { return Reverse(InExpo, t); }
	double InOutExpo(double t) { return ToInOut(InExpo, t); }
	double InCirc(double t) { return 1 - std::sqrt(1 - t * t); }
	double OutCirc(double t) { return Reverse(InCirc, t); }
	double InOutCirc(double t) { return ToInOut(InCirc, t); }
	double InBack(double t) { return t * t * ((1.70158 + 1) * t - 1.70158); }
	double OutBack(double t) { return Reverse(InBack, t); }
	double InOutBack(double t) { return ToInOut([](double y) { return y * y * ((1.70158 * 1.525 + 1) * y - 1.70158 * 1.525); }, t); }
	double OutBounce(double t) { return t < 1 / 2.75 ? 7.5625 * t * t : t < 2 / 2.75 ? 7.5625 * (t -= (1.5 / 2.75)) * t + .75 : t < 2.5 / 2.75 ? 7.5625 * (t -= (2.25 / 2.75)) * t + .9375 : 7.5625 * (t -= (2.625 / 2.75)) * t + .984375; }
	double InBounce(double t) { return Reverse(OutBounce, t); }
	double InOutBounce(double t) { return ToInOut(InBounce, t); }
	double OutElastic(double t) { return std::pow(2, -10 * t) * std::sin((t - 0.075) * (2 * t) / .3) + 1; }
	double InElastic(double t) { return Reverse(OutElastic, t); }
	double OutElasticHalf(double t) { return std::pow(2, -10 * t) * std::sin((0.5 * t - 0.075) * (2 * PI) / .3) + 1; }
	double OutElasticQuarter(double t) { return std::pow(2, -10 * t) * std::sin((0.25 * t - 0.075) * (2 * PI) / .3) + 1; }
	double InOutElastic(double t) { return ToInOut(InElastic, t); }

    double applyEasing(Easing easing, double t)
	{
		switch (easing)
		{
		case sb::Easing::Step: return Step(t); break;
		case sb::Easing::None: return Linear(t); break;
		case sb::Easing::Out: return OutQuad(t); break;
		case sb::Easing::In: return InQuad(t); break;
		case sb::Easing::InQuad: return InQuad(t); break;
		case sb::Easing::OutQuad: return OutQuad(t); break;
		case sb::Easing::InOutQuad: return InOutQuad(t); break;
		case sb::Easing::InCubic: return InCubic(t); break;
		case sb::Easing::OutCubic: return OutCubic(t); break;
		case sb::Easing::InOutCubic: return InOutCubic(t); break;
		case sb::Easing::InQuart: return InQuart(t); break;
		case sb::Easing::OutQuart: return OutQuart(t); break;
		case sb::Easing::InOutQuart: return InOutQuart(t); break;
		case sb::Easing::InQuint: return InQuint(t); break;
		case sb::Easing::OutQuint: return OutQuint(t); break;
		case sb::Easing::InOutQuint: return InOutQuint(t); break;
		case sb::Easing::InSine: return InSine(t); break;
		case sb::Easing::OutSine: return OutSine(t); break;
		case sb::Easing::InOutSine: return InOutSine(t); break;
		case sb::Easing::InExpo: return InExpo(t); break;
		case sb::Easing::OutExpo: return OutExpo(t); break;
		case sb::Easing::InOutExpo: return InOutExpo(t); break;
		case sb::Easing::InCirc: return InCirc(t); break;
		case sb::Easing::OutCirc: return OutCirc(t); break;
		case sb::Easing::InOutCirc: return InOutCirc(t); break;
		case sb::Easing::InElastic: return InElastic(t); break;
		case sb::Easing::OutElastic: return OutElastic(t); break;
		case sb::Easing::OutElasticHalf: return OutElasticHalf(t); break;
		case sb::Easing::OutElasticQuarter: return OutElasticQuarter(t); break;
		case sb::Easing::InOutElastic: return InOutElastic(t); break;
		case sb::Easing::InBack: return InBack(t); break;
		case sb::Easing::OutBack: return OutBack(t); break;
		case sb::Easing::InOutBack: return InOutBack(t); break;
		case sb::Easing::InBounce: return InBounce(t); break;
		case sb::Easing::OutBounce: return OutBounce(t); break;
		case sb::Easing::InOutBounce: return InOutBounce(t); break;
		default: return Linear(t); break;
		}
	}
    
    // TODO: will want to merge these but i managed to dig myself a little hole at the beginning
	void generateParameterKeyframes(std::vector<Keyframe<bool>>& keyframes, const std::vector<std::unique_ptr<Event<ParameterType>>>& events)
	{
		int i = -2;
		for (const std::unique_ptr<Event<ParameterType>>& event : events)
		{
			i += 2;
			bool appendEndtime = event->GetEndTime() > event->GetStartTime();
			if (i == 0)
			{
				keyframes.push_back(Keyframe<bool>(-std::numeric_limits<double>::infinity(), true, Easing::Step, -std::numeric_limits<double>::infinity()));
				if (appendEndtime)
				{
					keyframes.push_back(Keyframe<bool>(event->GetEndTime(), false, Easing::Step, event->GetEndTime()));
				}
				else i--;
				continue;
			}
			if (keyframes[i - 1].time >= event->GetStartTime())
			{
				keyframes.push_back(Keyframe<bool>(keyframes[i - 1].time, true, appendEndtime ? event->GetEasing() : Easing::Step, event->GetStartTime()));
				i--;
			}
			else
			{
				keyframes.push_back(Keyframe<bool>(event->GetStartTime(), true, appendEndtime ? event->GetEasing() : Easing::Step, event->GetStartTime()));
			}
			if (appendEndtime)
			{
				keyframes.push_back(Keyframe<bool>(event->GetEndTime(), false, Easing::Step, event->GetEndTime()));
			}
			else i--;
		}
	}
    
    // taken from https://stackoverflow.com/questions/478898#478960
	std::string exec(const std::string& cmd)
	{
		char buffer[128];
		std::string result = "";
		FILE* pipe = _popen(cmd.c_str(), "r");
		if (!pipe) throw std::runtime_error("popen() failed!");
		try {
			while (fgets(buffer, sizeof buffer, pipe) != NULL) {
				result += buffer;
			}
		}
		catch (...) {
			_pclose(pipe);
			throw;
		}
		_pclose(pipe);
		return result;
	}

    double getAudioDuration(const std::string& filepath)
	{
		return std::stod(exec("ffprobe -v quiet -show_entries format=duration -of default=noprint_wrappers=1:nokey=1 \"" + filepath + "\""));
	}

    cv::Mat readImageFile(const std::string& filepath)
	{
		cv::Mat image = cv::imread(filepath, cv::IMREAD_UNCHANGED);
		int depth = image.depth();
		cv::Mat converted;
		cv::cvtColor(image, converted, cv::COLOR_BGR2BGRA);
		double scale = depth == CV_16U ? 1.0 / 257 : 1;
		converted.convertTo(image, CV_32F, scale);
		return image;
	}
    
    // written mostly in reference to the parser used in osu!lazer (https://github.com/ppy/osu/blob/master/osu.Game/Beatmaps/Formats/LegacyStoryboardDecoder.cs)
	void parseFile(std::ifstream& file, std::vector<std::unique_ptr<Sprite>>& sprites, std::vector<Sample>& samples, std::vector<Background>& backgrounds, std::unordered_map<std::string, std::string>& variables, std::unordered_map<std::string, std::string>& info, size_t& lineNumber)
	{
		std::string line;
		bool inLoop = false;
		bool inTrigger = false;
		enum class Section
		{
			None,
			Events,
			Variables,
			Info
		};
		Section section = Section::None;

		while (file.good())
		{
			lineNumber++;
			constexpr int lineMaxReadLength = 10000;
			char lineTemp[lineMaxReadLength];
			file.getline(lineTemp, lineMaxReadLength);
			line = std::string(lineTemp);

			if (line.length() == 0) continue;
			if (line.rfind("//", 0) == 0) continue;

			// Determine start of a new section
			if (line.find("[Events]") == 0)
			{
				section = Section::Events;
				continue;
			}
			else if (line.find("[Variables]") == 0)
			{
				section = Section::Variables;
				continue;
			}
			else if (line.find("[General]") == 0 || line.find("[Metadata]") == 0)
			{
				section = Section::Info;
				continue;
			}
			else if (line[0] == '[')
			{
				section = Section::None;
				continue;
			}

			switch (section)
			{
			case Section::None:
				continue;

			case Section::Events:
			{
				std::size_t depth = 0;
				while (line[depth] == ' ' || line[depth] == '_') depth++;
				line.erase(0, depth);

				applyVariables(line, variables);
				std::vector<std::string> split = stringSplit(line, ",");

				if (inTrigger && depth < 2) inTrigger = false;
				if (inLoop && depth < 2) inLoop = false;

				std::unordered_map<std::string, Keyword>::const_iterator k = KeywordStrings.find(split[0]);
				Keyword keyword = k == KeywordStrings.end() ? Keyword::None : k->second;
				switch (keyword)
				{
				case Keyword::Sprite:
				{
					// TODO: Error handling
					Layer layer = LayerStrings.find(split[1])->second;
					Origin origin = OriginStrings.find(split[2])->second;
					std::string path = removePathQuotes(split[3]);
					float x = std::stof(split[4]);
					float y = std::stof(split[5]);
					sprites.push_back(std::make_unique<Sprite>(layer, origin, path, std::pair<double, double>(x, y)));
				}
				break;
				case Keyword::Animation:
				{
					Layer layer = LayerStrings.find(split[1])->second;
					Origin origin = OriginStrings.find(split[2])->second;
					std::string path = removePathQuotes(split[3]);
					float x = std::stof(split[4]);
					float y = std::stof(split[5]);
					int frameCount = std::stoi(split[6]);
					double frameDelay = std::stod(split[7]);
					auto l = LoopTypeStrings.find(split[8]); // TODO: parse enums either by integer or name
					LoopType loopType = split.size() > 8 && l != LoopTypeStrings.end() ? l->second : LoopType::LoopForever;
					sprites.push_back(std::make_unique<class Animation>(layer, origin, path, std::pair<double, double>(x, y), frameCount, frameDelay, loopType));
				}
				break;
				case Keyword::Sample:
				{
					double time = std::stod(split[1]);
					Layer layer = static_cast<Layer>(std::stoi(split[2]));
					std::string path = removePathQuotes(split[3]);
					float volume = std::stof(split[4]);
					samples.emplace_back(time, layer, path, volume);
				}
				break;
				case Keyword::T:
				{
					if (inTrigger || inLoop)
					{
						// TODO: Error
					}
					std::string triggerName = split[1];
					double startTime = std::stod(split[2]);
					double endTime = std::stod(split[3]);
					int groupNumber = split.size() > 4 ? std::stoi(split[4]) : 0;
					(*(sprites.end() - 1))->AddTrigger({ triggerName, startTime, endTime, groupNumber });
					inTrigger = true;
				}
				break;
				case Keyword::L:
				{
					if (inLoop || inTrigger)
					{
						// TODO: Error
					}
					double startTime = std::stod(split[1]);
					int loopCount = std::stoi(split[2]);
					(*(sprites.end() - 1))->AddLoop({ startTime, loopCount });
					inLoop = true;
				}
				break;
				case Keyword::None:
				default:
				{
					if (split[0] == "0" && split[1] == "0" && depth == 0)
					{
						std::string path = removePathQuotes(split[2]);
						std::pair<double, double> offset = split.size() < 3 ? std::pair<double, double>(std::stoi(split[3]), std::stoi(split[4])) : std::pair<double, double>(0, 0);
						backgrounds.emplace_back(path, offset);
						break;
					}
					if ((split[0] == "Video" || split[0] == "1") && depth == 0)
					{
						// TODO: video
						break;
					}
					if (depth == 0) break;
					if (split[3].length() == 0)
						split[3] = split[2];

					Easing easing = static_cast<Easing>(std::stoi(split[1]));
					double startTime = std::stod(split[2]);
					double endTime = std::stod(split[3]);

					std::unordered_map<std::string, EventType>::const_iterator k = EventTypeStrings.find(split[0]);
					EventType eventType = k == EventTypeStrings.end() ? EventType::None : k->second;

					switch (eventType)
					{
					case EventType::F:
					{
						double startValue = std::stod(split[4]);
						double endValue = split.size() > 5 ? std::stod(split[5]) : startValue;
						std::unique_ptr<Event<double>> event = std::make_unique<Event<double>>(EventType::F, easing, startTime, endTime, startValue, endValue);
						if (inTrigger) (*(sprites.end() - 1))->AddEventInTrigger(std::move(event));
						else if (inLoop) (*(sprites.end() - 1))->AddEventInLoop(std::move(event));
						else (*(sprites.end() - 1))->AddEvent(std::move(event));
					}
					break;
					case EventType::S:
					{
						double startValue = std::stod(split[4]);
						double endValue = split.size() > 5 ? std::stod(split[5]) : startValue;
						std::unique_ptr<Event<double>> event = std::make_unique<Event<double>>(EventType::S, easing, startTime, endTime, startValue, endValue);
						if (inTrigger) (*(sprites.end() - 1))->AddEventInTrigger(std::move(event));
						else if (inLoop) (*(sprites.end() - 1))->AddEventInLoop(std::move(event));
						else (*(sprites.end() - 1))->AddEvent(std::move(event));
					}
					break;
					case EventType::V:
					{
						double startX = std::stod(split[4]);
						double startY = std::stod(split[5]);
						double endX = split.size() > 6 ? std::stod(split[6]) : startX;
						double endY = split.size() > 7 ? std::stod(split[7]) : startY;
						std::unique_ptr<Event<std::pair<double, double>>> event = std::make_unique<Event<std::pair<double, double>>>(EventType::V, easing, startTime, endTime, std::pair<double, double> { startX, startY }, std::pair<double, double>{ endX, endY });
						if (inTrigger) (*(sprites.end() - 1))->AddEventInTrigger(std::move(event));
						else if (inLoop) (*(sprites.end() - 1))->AddEventInLoop(std::move(event));
						else (*(sprites.end() - 1))->AddEvent(std::move(event));
					}
					break;
					case EventType::R:
					{
						double startValue = std::stod(split[4]);
						double endValue = split.size() > 5 ? std::stod(split[5]) : startValue;
						std::unique_ptr<Event<double>> event = std::make_unique<Event<double>>(EventType::R, easing, startTime, endTime, startValue, endValue);
						if (inTrigger) (*(sprites.end() - 1))->AddEventInTrigger(std::move(event));
						else if (inLoop) (*(sprites.end() - 1))->AddEventInLoop(std::move(event));
						else (*(sprites.end() - 1))->AddEvent(std::move(event));
					}
					break;
					case EventType::M:
					{
						double startX = std::stod(split[4]);
						double startY = std::stod(split[5]);
						double endX = split.size() > 6 ? std::stod(split[6]) : startX;
						double endY = split.size() > 7 ? std::stod(split[7]) : startY;
						std::unique_ptr<Event<std::pair<double, double>>> event = std::make_unique<Event<std::pair<double, double>>>(EventType::M, easing, startTime, endTime, std::pair<double, double> { startX, startY }, std::pair<double, double>{ endX, endY });
						if (inTrigger) (*(sprites.end() - 1))->AddEventInTrigger(std::move(event));
						else if (inLoop) (*(sprites.end() - 1))->AddEventInLoop(std::move(event));
						else (*(sprites.end() - 1))->AddEvent(std::move(event));
					}
					break;
					case EventType::MX:
					{
						double startValue = std::stod(split[4]);
						double endValue = split.size() > 5 ? std::stod(split[5]) : startValue;
						std::unique_ptr<Event<double>> event = std::make_unique<Event<double>>(EventType::MX, easing, startTime, endTime, startValue, endValue);
						if (inTrigger) (*(sprites.end() - 1))->AddEventInTrigger(std::move(event));
						else if (inLoop) (*(sprites.end() - 1))->AddEventInLoop(std::move(event));
						else (*(sprites.end() - 1))->AddEvent(std::move(event));
					}
					break;
					case EventType::MY:
					{
						double startValue = std::stod(split[4]);
						double endValue = split.size() > 5 ? std::stod(split[5]) : startValue;
						std::unique_ptr<Event<double>> event = std::make_unique<Event<double>>(EventType::MY, easing, startTime, endTime, startValue, endValue);
						if (inTrigger) (*(sprites.end() - 1))->AddEventInTrigger(std::move(event));
						else if (inLoop) (*(sprites.end() - 1))->AddEventInLoop(std::move(event));
						else (*(sprites.end() - 1))->AddEvent(std::move(event));
					}
					break;
					case EventType::C:
					{
						int startR = std::stoi(split[4]);
						int startG = std::stoi(split[5]);
						int startB = std::stoi(split[6]);
						int endR = split.size() > 7 ? std::stoi(split[7]) : startR;
						int endG = split.size() > 8 ? std::stoi(split[8]) : startG;
						int endB = split.size() > 9 ? std::stoi(split[9]) : startB;
						std::unique_ptr<Event<Colour>> event = std::make_unique<Event<Colour>>(EventType::C, easing, startTime, endTime, Colour { startR / 255.0f, startG / 255.0f, startB / 255.0f }, Colour{ endR / 255.0f, endG / 255.0f, endB / 255.0f });
						if (inTrigger) (*(sprites.end() - 1))->AddEventInTrigger(std::move(event));
						else if (inLoop) (*(sprites.end() - 1))->AddEventInLoop(std::move(event));
						else (*(sprites.end() - 1))->AddEvent(std::move(event));
					}
					break;
					case EventType::P:
					{
						ParameterType parameterType = ParameterTypeStrings.find(split[4])->second;
						std::unique_ptr<Event<ParameterType>> event;
						switch (parameterType)
						{
						case ParameterType::Additive:
							event = std::make_unique<Event<ParameterType>>(EventType::P, easing, startTime, endTime, ParameterType::Additive, ParameterType::Additive);
							break;
						case ParameterType::FlipH:
							event = std::make_unique<Event<ParameterType>>(EventType::P, easing, startTime, endTime, ParameterType::FlipH, ParameterType::FlipH);
							break;
						case ParameterType::FlipV:
							event = std::make_unique<Event<ParameterType>>(EventType::P, easing, startTime, endTime, ParameterType::FlipV, ParameterType::FlipV);
							break;
						}
						if (inTrigger) (*(sprites.end() - 1))->AddEventInTrigger(std::move(event));
						else if (inLoop) (*(sprites.end() - 1))->AddEventInLoop(std::move(event));
						else (*(sprites.end() - 1))->AddEvent(std::move(event));
					}
					break;
					case EventType::None:
						break;
					}
				}
				break;
				}
			}
			break;

			case Section::Variables:
			{
				std::size_t splitPos = line.find('=');
				if (splitPos == std::string::npos || splitPos == line.length() - 1) continue;
				std::string key = line.substr(0, splitPos);
				std::string value = line.substr(splitPos + 1, line.length() - splitPos - 1);
				variables.emplace(key, value); // doesn't overwrite previous values TODO: Check if this is correct behaviour
			}
			break;

			case Section::Info:
			{
				std::size_t splitPos = line.find(':');
				if (splitPos == std::string::npos || splitPos == line.length() - 1) continue;
				std::string key = line.substr(0, splitPos);
				std::string value = line.substr(splitPos + 1, line.length() - splitPos - 1);
				while (std::isspace(value[0])) value.erase(0, 1);
				info.emplace(key, value); // TODO: Check if this is correct behaviour
			}
			break;
			}
		}
	}
    
    std::unique_ptr<Storyboard> ParseStoryboard(const std::string& directory, const std::string& diff, std::pair<size_t, size_t> resolution, double musicVolume, double effectVolume)
	{
		std::string osb = std::string();
		for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directory))
		{
			if (entry.path().extension() == ".osb")
			{
				osb = entry.path().string();
                std::cout << "Found osb file: " << osb << "\n";
				break;
			}
		}
		if (osb.empty())
		{
			throw std::exception("No .osb file found.\n");
		}
        
		std::ifstream osbFile(osb);
		if (!osbFile.is_open()) throw std::exception(("Failed to open \"" + osb + "\".\n").c_str());
        
		std::ifstream diffFile(std::filesystem::path(directory) / diff);
		if (!diffFile.is_open()){
            std::cerr << "failed to open diff file" << diff << "\n";
            throw std::exception(("Failed to open \"" + diff + "\".\n").c_str());
        }
        
		std::vector<std::unique_ptr<Sprite>> sprites;
		std::vector<Sample> samples;
		std::vector<Background> backgrounds;
		std::unordered_map<std::string, std::string> variables;
		std::unordered_map<std::string, std::string> info;
		bool inLoop = false;
		bool inTrigger = false;
		std::size_t lineNumber = 0;

		std::cout << "Parsing " << osb << "...\n";
		parseFile(osbFile, sprites, samples, backgrounds, variables, info, lineNumber);
		osbFile.close();
		std::cout << "Parsed " << lineNumber << " lines\n";

		lineNumber = 0;
		std::cout << "Parsing " << diff << "...\n";
		parseFile(diffFile, sprites, samples, backgrounds, variables, info, lineNumber);
		diffFile.close();
		std::cout << "Parsed " << lineNumber << " lines\n";

		std::unique_ptr<Storyboard> sb = std::make_unique<Storyboard>(directory, osb, sprites, samples, backgrounds, info, resolution, musicVolume, effectVolume);
		return sb;
	}
    
    Colour InterpolateLinear(Colour start, Colour end, double t)
	{
		return Colour(InterpolateLinear(start.R, end.R, t), InterpolateLinear(start.G, end.G, t), InterpolateLinear(start.B, end.B, t));
	}
    
    std::pair<double, double> InterpolateLinear(std::pair<double, double> start, std::pair<double, double> end, double t)
	{
		return std::pair<double, double>(InterpolateLinear(start.first, end.first, t), InterpolateLinear(start.second, end.second, t));
	}

}