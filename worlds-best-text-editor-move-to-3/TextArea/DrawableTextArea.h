#ifndef DRAWABLETEXTAREA_H
#define DRAWABLETEXTAREA_H


class DrawableTextArea
{

public:

    DrawableTextArea()
        , mBackgroundColor(COLOR_WHITE)
        , mCursorType{cursortype_t::BLOCK}
    {

    }

    void Draw()
    {
        // for each line of text in mTextArea
        // call writeString function
    }

    // TODO: implement these ?
    void drawCharAnywhere(
        std::shared_ptr<SDL_Window> sdlWindow,
        const char c,
        const int x, const int y);

    // this function cannot be easily implemented because it
    // requires knowing the advance of each character which had been
    // printed before it on the same line
    void drawChar(
        std::shared_ptr<SDL_Window> sdlWindow,
        const char c,
        const unsigned int posX,
        const unsigned int posY);

private:

    TextArea mTextArea;



    // Note: could inherit and create a new class which has cursors,
    // but for now we simply insert cursor variables and logic into
    // this monolithic class
    // TODO: initialize in class constructor and add setter function for cursortype
    cursortype_t mCursorType;
    unsigned int mCursorPosX;
    unsigned int mCursorPosY;

    // in order for the cursor logic to work, we need to remember the size
    // of each line in the grid.
    // currently the grid is always populated with characters, some of which
    // are just blank spaces by default
    // should perhaps solve this by making the cursor a member of the text
    // buffer class?
    // or creating a new class which holds the textarea data, but as individual
    // lines, and this class can then render the textarea by creating a new
    // object from the class used to hold the textarea data?
    

};


#endif // DRAWABLETEXTAREA_H