#include <json/json.h>


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



ConfigOption::Config()
{
    initialize();
}


void ConfigOption::initialize()
{
    std::ifstream ifs(mDefaultFilename);
    if(ifs.is_open())
    {
        Json::CharReaderBuilder readerBuilder;
        std::unique_ptr<Json::CharReader> reader(readerBuilder.newCharReader());
        Json::Value root;
        Json::String errors;

        std::string ifsContents((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());

        const auto success =
            reader->parse(  ifsContents.c_str(),
                            ifsContents.c_str() + ifsContents.length(),
                            &root,
                            &errors);

        if(!success || !errors.empty())
        {
            throw std::runtime_error("Failed to parse json file");
        }
        else
        {
            processJsonRoot(root);
        }
    }
    else
    {
        throw std::runtime_error("Config file does not exsit");
    }
}


void ConfigOption::processJsonRoot(Json::Value root)
{
    if(root.isObject())
    {
        for(const auto &memberName : root.getMemberNames())
        {
            // This checks that the name of the configuration element is expected
            if(mapConfigMemberNameToJsonType.count(memberName))
            {
                // Check that we did not already process an object with the same name
                if(mConfig.count(memberName))
                {
                    throw std::runtime_error("Duplicate member in Json configuration");
                }

                // Verify type is correct
                Json::Value memberValue(root[memberName]);
                const auto expectedType(mapConfigMemberNameToJsonType.at(memberName));
                if(memberValue.type() == expectedType)
                {
                    // TODO: Ideally Json::Value would provide an `asType()` function
                    // which does the same thing as the block of if statements below
                    if(memberValue.isBool())
                    {
                        bool value(memberValue.asBool());
                        ConfigOption option(value);
                        mConfig[memberName] = option;
                    }
                    else if(memberValue.isString())
                    {
                        std::string value(memberValue.asString());
                        ConfigOption option(value);
                        mConfig[memberName] = option;
                    }
                    else if(memberValue.isInt())
                    {
                        int value(memberValue.asInt());
                        ConfigOption option(value);
                        mConfig[memberName] = option;
                    }
                    else if(memberValue.isDouble())
                    {
                        double value(memberValue.asDouble());
                        ConfigOption option(value);
                        mConfig[memberName] = option;
                    }
                    else
                    {
                        throw std::runtime_error("Cannot process Json member, logic for type is not known");
                    }
                }
                else
                {
                    throw std::runtime_error("Wrong type is Json configuraiton");
                }
            }
            else
            {
                // TODO: change this to be a warning statement which ignores the member
                throw std::runtime_error("Unrecogized member in Json configuration");
            }
        }
    }
    else
    {
        throw std::runtime_error("Expected Json::Object type");
    }
}

