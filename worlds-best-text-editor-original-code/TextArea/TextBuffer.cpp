

#include "TextBuffer.cpp"


#include <string>
#include <vector>
#include <fstream>


TextBuffer::TextBuffer()
    : mWrap(true) // TODO: just put these values in for testing
    , mScroll(0)
    , mScrollH(0)
{
    m_cursor_pos.push_back(std::make_pair(0, 0));
}


void TextBuffer::ReadFile(const std::string& filename)
{
    std::cout << __func__ << std::endl;

    std::ifstream ifs(filename);

    if(!ifs.is_open())
    {
        std::cout << "Failed to open file " << filename << std::endl;
    }
    else
    {
        std::string line;
        while(std::getline(ifs, line))
        {
            //std::cout << line << std::endl;
            mLines.emplace_back(std::move(line)); // TODO: check
        }
    }
}


void TextBuffer::WriteFile(const std::string& filename)
{
    std::ofstream ofs(filename);
    for(auto it{mLines.cbegin()}; it != mLines.cend(); )
    {
        ofs << *it;
        ++ it;
        if(it != mLines.cend())
        {
            ofs << "\n";
        }
    }
    ofs.close();
}


// should this be external or use inheritance?
void TextBuffer::ProcessEvent()
{

}

