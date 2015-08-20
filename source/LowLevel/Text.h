/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#ifndef H_TEXT
#define H_TEXT

#include "Drawable.h"
#include <ulib/ulib.h>
#include <string>

namespace LowLevel
{
    void drawString(int x, int y, std::string text, int color = 0, int alpha = 31);
	int getStringWidth(std::string text);

    class Text : public Drawable
    {
    public:
        Text();
        ~Text();

        void Draw();
        void Play();
		void Reset();
		void End();
		bool GetEnd();
		
		void ConvertToTimedText();
        void Tint(int color);
        void UpdateText(std::string txt);
        void ConvertToTextBox(int w, int h);
        int  GetWidth();
        int  GetHeight();

    private:
        int color;
        bool isTextBox, isTimed;
        std::string text, text2;
		int timer;
		int tacker;
		bool isDrawingFinished;
		int alpha;
    };
}

#endif
