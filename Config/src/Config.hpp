#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <json/json.h>

#include <fstream>
#include <map>


// TODO: change so that config stores all options as string
// and then conversion is done afterwards or even manually by
// the calling context


std::string convertToString(const std::string& input)
{
    return input;
}

std::string convertToString(const int input)
{
    return std::to_string(input);
}

std::string convertToString(const double input)
{
    return std::to_string(input);
}



class ConfigOption
{

public:

    ConfigOption()
        : pData(nullptr)
    {
    }

    template<typename T>
    ConfigOption(T data)
        : pData(std::make_unique<Model<T>>(std::move(data)))
    {
    }

    ConfigOption(const ConfigOption& other)
        : pData(other.pData->clone())
    {
    }

    ConfigOption(ConfigOption&& other) noexcept = default;

    ConfigOption& operator=(ConfigOption other) noexcept;

    ~ConfigOption() = default;

    // "fish" for type T
    template<typename T>
    T fish() const
    {
        ConfigOptionConcept *base(pData.get());
        if(const auto *const derived(dynamic_cast<ConfigOptionModel<T>*>(base)))
        {
            const T* const pDataDerived(static_cast<const T*>(derived->address()));
            return *pDataDerived;
        }
    }

    std::string getString() const
    {
        return pData->asString();
    }


private:

    class ConfigOptionConcept
    {

    public:


        ConfigOptionConcept()
        {
        }

        virtual ~ConfigOptionConcept()
        {
        }

        virtual std::string asString() const = 0;
    
    };


    template<typename T>
    class ConfigOptionModel
    {

    public:

        explicit
        ConfigOptionModel(T t)
            : pData(std::move(t))
        {
        }

        virtual ~ConfigOptionModel<T>()
        {
        }

        std::string asString() const override
        {
            convertToString(data);
        }
        

    private:

        T data;

    };
    

    std::unique_ptr<T> pData;

};


class Config
{

public:

    Config();


private:

    void initialize();

    void processJsonRoot(Json::Value root);

    constexpr std::map<std::string, Json::ValueType> mapConfigMemberNameToJsonType {
        {"font_size", Json::ValueType::intValue},
        {"cursor_blink_delay", Json::ValueType::intValue},
        {"start_line_number", Json::ValueType::intValue},
        {"target_refresh_rate", Json::ValueType::realValue},
        {"default_window_width", Json::ValueType::intValue},
        {"default_window_height", Json::ValueType::intValue}
    }


private:

    const std::string mDefaultFilename{"config.json"};
    std::map<std::string, ConfigOption> mConfig;

};

#endif // CONFIG_HPP
