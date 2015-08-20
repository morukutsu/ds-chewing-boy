/* --------------------------- */
/* Natura no Story             */
/* --------------------------- */

#include "Text.h"

namespace LowLevel
{
    void drawString(int x, int y, std::string text, int color, int alpha)
    {
        ulSetTextColor(color);
		ulSetAlpha(UL_FX_ALPHA, alpha, 19);
		ulDrawString(x, y, text.c_str() );
		ulSetAlpha(UL_FX_DEFAULT, 0, 0);  
    }

	int getStringWidth(std::string text)
	{
		return ulGetStringWidth(text.c_str() );
	}

    Text::Text()
    {
        centerX = centerY = 0;
        posX = posY = 0;
        scaleX = scaleY = 0;
        angle = 0;
        mirrorX = mirrorY = false;
        alpha = 31;
        polygroup = 0;

        color = 0;
        isTextBox = isTimed = false;
		timer = 0;
    }

	void Text::ConvertToTimedText()
	{
		isTimed = true;
		timer = 0;
		tacker = 0;
	}

    Text::~Text()
    {

    }

	void Text::Reset()
	{
		timer = 0;
		tacker = 0;
		text2.clear();
	}

	void Text::End()
	{
		timer = 0;
		text2 = text;
		tacker = text.size();
	}

	bool Text::GetEnd()
	{
		if(tacker <= text.size() )
		{
			return false;
		}
		else
		{
			return true;
		}
	}

    void Text::Draw()
    {
        ulSetTextColor(color);
		if(isTimed)
		{
			if (!isTextBox)
			{
				ulSetAlpha(UL_FX_ALPHA, alpha, polygroup);
				ulDrawString(posX, posY, text2.c_str() );
				ulSetAlpha(UL_FX_DEFAULT, alpha, polygroup);
			}
			else
			{
				ulSetAlpha(UL_FX_ALPHA, alpha, polygroup);
				ulDrawTextBox(posX, posY, posX+scaleX, posY+scaleY, text2.c_str(), 0);
				ulSetAlpha(UL_FX_DEFAULT, alpha, polygroup);
			}
		}
		else
		{
			if (!isTextBox)
			{
				ulSetAlpha(UL_FX_ALPHA, alpha, polygroup);
				ulDrawString(posX, posY, text.c_str() );
				ulSetAlpha(UL_FX_DEFAULT, 0, 0);
			}
			else
			{
				ulSetAlpha(UL_FX_ALPHA, alpha, polygroup);
				ulDrawTextBox(posX, posY, posX+scaleX, posY+scaleY, text.c_str(), 0);
				ulSetAlpha(UL_FX_DEFAULT, 0, 0);
			}
		}
    }

	void Text::Play()
	{
		timer++;
		if(timer % 3 == 0 && tacker <= text.size() )
		{
			//On ajoute un caractère
			text2 += text.c_str()[tacker];
			tacker++;
		}
	}

    void Text::Tint(int c)
    {
        color = c;
    }

    void Text::UpdateText(std::string txt)
    {
        text = txt;
    }

    void Text::ConvertToTextBox(int w, int h)
    {
        isTextBox = true;
        scaleX = w;
        scaleY = h;
    }

    int Text::GetWidth()
    {
        return ulGetStringWidth(text.c_str() );
    }

    int Text::GetHeight()
    {
        if (isTextBox)
        {
            return ulGetTextBoxHeight(scaleX, 0, text.c_str(), 0);
        }
        else
        {
            return 8;
        }
    }
}

