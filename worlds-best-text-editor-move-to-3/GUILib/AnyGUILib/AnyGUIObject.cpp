#include "TypeErased.h"


std::string serialize(const float& value)
{
    return std::to_string(value);
}

std::string serialize(const int& value)
{
    return std::to_string(value);
}

std::string serialize(const char* value)
{
    return value;
}

std::string serialize(const std::string& value)
{
    return value;
}

void swap(TypeErased& l, TypeErased& r) noexcept
{
    using std::swap;
    swap(l.p_data, r.p_data);
}

std::ostream& operator<<(std::ostream &os, const TypeErased& typeErased)
{
    os << typeErased.serialize();
    //os << typeErased.p_data->serialize();
    return os;
}