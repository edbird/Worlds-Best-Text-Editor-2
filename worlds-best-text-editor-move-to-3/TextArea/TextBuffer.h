#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H



#include "TextArea.h"


#include <string>
#include <vector>
#include <fstream>


// Seperate this out into a class which holds the data
class TextBuffer
{


    friend void fillTextGridFromTextBuffer(TextArea& textArea, TextBuffer &textbuffer);


public:

    TextBuffer();

    void ReadFile(const std::string& filename);

    void WriteFile(const std::string& filename);

    // should this be external or use inheritance?
    void ProcessEvent();
    // TODO: need some kind of function which can fill
    // a textArea object from a textbuffer object


private:

    // TODO: might write a new data structure to store text in blocks of 1024 k
    std::vector<std::string> mLines;
    std::vector<std::pair<std::size_t, std::size_t>> mCursorPos; // can have more than one
    bool mWrap;
    std::size_t mScroll;
    std::size_t mScrollH; // horizontal scroll

};






#endif // TEXTBUFFER_H