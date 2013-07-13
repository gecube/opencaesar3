// This file is part of openCaesar3.
//
// openCaesar3 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// openCaesar3 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with openCaesar3.  If not, see <http://www.gnu.org/licenses/>.

#include "oc3_editbox.hpp"
#include "oc3_stringarray.hpp"
#include "oc3_event.hpp"
#include "oc3_guienv.hpp"
#include "oc3_gfx_engine.hpp"
#include "oc3_picture.hpp"
#include "oc3_time.hpp"
#include "oc3_gui_paneling.hpp"

class EditBox::Impl
{
public:
  StringArray brokenText;
  std::vector< int > brokenTextPositions;
  Rect currentTextRect;
  Rect cursorRect; // temporary values
  Rect markAreaRect;
  Font overrideFont;
  Font lastBreakFont;
	bool mouseMarking;
	bool border;
	bool drawBackground;
	bool overrideColorEnabled;
	int markBegin;
	int markEnd;
	//unsigned int resultTransparent;
  bool needUpdateTexture;
	NColor overrideColor;
	//core::AbstractOSOperator* osOperator;

	int cursorPos, oldCursorPos;
	int horizScrollPos, vertScrollPos; // scroll position in characters
	unsigned int max;
  std::string holderText;
  Picture bgPicture;
  PictureRef picture;
  PictureRef cursorPic;
  PictureRef textPicture;

	bool wordWrapEnabled, multiLine, autoScrollEnabled, isPasswordBox;
	char passwordChar;

  Impl() : currentTextRect(0,0,1,1)
	{

	}

oc3_signals public:
  Signal1<std::string> onTextChangedSignal;
};

//! constructor
EditBox::EditBox( Widget* parent, const Rect& rectangle, const std::string& text, int id, bool border )
	: Widget( parent, id, rectangle), _d( new Impl )
{
	_d->mouseMarking = false;
	_d->overrideColorEnabled = false;
	_d->markBegin = 0;
	_d->markEnd = 0;
	_d->border = border;
	_d->drawBackground = true;
	_d->overrideColor = NColor(101,255,255,255);
	//_d->osOperator = 0;
	_d->cursorPos = 0;
	_d->horizScrollPos = 0;
	_d->vertScrollPos = 0;
	_d->max = 0;
	_d->wordWrapEnabled = false;
	_d->multiLine = false; 
	_d->autoScrollEnabled = true;
	_d->isPasswordBox = false;
	_d->passwordChar = '*';
  _d->lastBreakFont = getActiveFont();  

	#ifdef _DEBUG
	    setDebugName( "EditBox");
	#endif

	_text = text;

	// this element can be tabbed to
	setTabStop(true);
	setTabOrder(-1);

	breakText();

	calculateScrollPos();

	setTextAlignment( alignUpperLeft, alignCenter );
}

Signal1<std::string>& EditBox::onTextChanged()
{
	return _d->onTextChangedSignal;
}

//! destructor
EditBox::~EditBox()
{
}

//! Sets another skin independent font.
void EditBox::setFont( const Font& font )
{
  _d->overrideFont = font;

	breakText();
}

//! Gets the override font (if any)
Font EditBox::getFont() const
{
    return _d->overrideFont;
}

//! Get the font which is used right now for drawing
Font EditBox::getActiveFont()
{
  if ( _d->overrideFont.isValid() )
    return _d->overrideFont;
  
  return Font::create( FONT_2 );
}

//! Sets another color for the text.
void EditBox::setOverrideColor( const NColor& color)
{
	_d->overrideColor = color;
	_d->overrideColorEnabled = true;
}

//! Turns the border on or off
void EditBox::setDrawBorder(bool border)
{
	_d->border = border;
}

//! Sets if the text should use the overide color or the color in the gui skin.
void EditBox::setEnabledColor(bool enable)
{
	_d->overrideColorEnabled = enable;
}

bool EditBox::isOverrideColorEnabled() const
{
	return _d->overrideColorEnabled;
}

//! Enables or disables word wrap
void EditBox::setWordWrap(bool enable)
{
	_d->wordWrapEnabled = enable;
	breakText();
}

void EditBox::resizeEvent_()
{
  breakText();
  calculateScrollPos();
  _d->needUpdateTexture = true ;
}


//! Checks if word wrap is enabled
bool EditBox::isWordWrapEnabled() const
{

	return _d->wordWrapEnabled;
}


//! Enables or disables newlines.
void EditBox::setMultiLine(bool enable)
{
	_d->multiLine = enable;
}


//! Checks if multi line editing is enabled
bool EditBox::isMultiLineEnabled() const
{

	return _d->multiLine;
}


void EditBox::setPasswordBox(bool passwordBox, char passwordChar)
{
	_d->isPasswordBox = passwordBox;
	if (_d->isPasswordBox)
	{
		_d->passwordChar = passwordChar;
		setMultiLine(false);
		setWordWrap(false);
		_d->brokenText.clear();
	}
}

bool EditBox::isPasswordBox() const
{

	return _d->isPasswordBox;
}

//! Sets text justification
void EditBox::setTextAlignment(TypeAlign horizontal, TypeAlign vertical)
{
	Widget::setTextAlignment( horizontal, vertical );
}

//! called if an event happened.
bool EditBox::onEvent(const NEvent& event)
{
	if (isEnabled())
	{

		switch(event.EventType)
		{
		case OC3_GUI_EVENT:
			if (event.GuiEvent.EventType == OC3_ELEMENT_FOCUS_LOST)
			{
				if (event.GuiEvent.Caller == this)
				{
					_d->mouseMarking = false;
					setTextMarkers(0,0);
				}
			}
			break;
		case OC3_KEYBOARD_EVENT:
			if (processKey(event))
				return true;
			break;
		case OC3_MOUSE_EVENT:
			if (processMouse(event))
				return true;
			break;
		default:
			break;
		}
	}

	return Widget::onEvent(event);
}


bool EditBox::processKey(const NEvent& event)
{
	if (!event.KeyboardEvent.PressedDown)
		return false;

	bool textChanged = false;
	int newMarkBegin = _d->markBegin;
	int newMarkEnd = _d->markEnd;

	// control shortcut handling

	if (event.KeyboardEvent.Control)
	{
		// german backlash '\' entered with control + '?'
		if ( event.KeyboardEvent.Char == L'\\' )
		{
			inputChar(event.KeyboardEvent.Char);
			return true;
		}

		switch(event.KeyboardEvent.Key)
		{
		case KEY_KEY_A:
			// select all
			newMarkBegin = 0;
			newMarkEnd = _text.size();
			break;
		case KEY_KEY_C:
			// copy to clipboard
			/*if (!_d->isPasswordBox && _d->osOperator && _d->markBegin != _d->markEnd)
			{
				const int realmbgn = _d->markBegin < _d->markEnd ? _d->markBegin : _d->markEnd;
				const int realmend = _d->markBegin < _d->markEnd ? _d->markEnd : _d->markBegin;

				String s;
				s = _text.subString(realmbgn, realmend - realmbgn).c_str();
				_d->osOperator->copyToClipboard( s );
			}*/
	  break;
		
    case KEY_KEY_X:
			// cut to the clipboard
			/*if (!_d->isPasswordBox && _d->osOperator && _d->markBegin != _d->markEnd)
			{
				const int realmbgn = _d->markBegin < _d->markEnd ? _d->markBegin : _d->markEnd;
				const int realmend = _d->markBegin < _d->markEnd ? _d->markEnd : _d->markBegin;

				// copy
				String sc;
				sc = _text.subString(realmbgn, realmend - realmbgn).c_str();
				_d->osOperator->copyToClipboard( sc );

				if (isEnabled())
				{
					// delete
					String s;
					s = _text.subString(0, realmbgn);
					s.append( _text.subString(realmend, _text.size()-realmend) );
					_text = s;

					_d->cursorPos = realmbgn;
					newMarkBegin = 0;
					newMarkEnd = 0;
					textChanged = true;
				}
			}*/
	  break;
		case KEY_KEY_V:
			if ( !isEnabled() )
				break;

			// paste from the clipboard
			/*if (_d->osOperator)
			{
				const int realmbgn = _d->markBegin < _d->markEnd ? _d->markBegin : _d->markEnd;
				const int realmend = _d->markBegin < _d->markEnd ? _d->markEnd : _d->markBegin;

				// add new character
                String p( _d->osOperator->getTextFromClipboard() );
				if( p.size() )
				{
					if (_d->markBegin == _d->markEnd)
					{
						// insert text
						String s = _text.subString(0, _d->cursorPos);
                        s.append( p );
						s.append( _text.subString(_d->cursorPos, _text.size()-_d->cursorPos) );

						if (!_d->max || s.size()<=_d->max) // thx to Fish FH for fix
						{
							_text = s;
							s = p;
							_d->cursorPos += s.size();
						}
					}
					else
					{
						// replace text

						String s = _text.subString(0, realmbgn);
						s.append(p);
						s.append( _text.subString(realmend, _text.size()-realmend) );

						if (!_d->max || s.size()<=_d->max)  // thx to Fish FH for fix
						{
							_text = s;
							s = p;
							_d->cursorPos = realmbgn + s.size();
						}
					}
				}

				newMarkBegin = 0;
				newMarkEnd = 0;
				textChanged = true;
			}*/
	  break;
		
    case KEY_HOME:
			// move/highlight to start of text
			if (event.KeyboardEvent.Shift)
			{
				newMarkEnd = _d->cursorPos;
				newMarkBegin = 0;
				_d->cursorPos = 0;
			}
			else
			{
				_d->cursorPos = 0;
				newMarkBegin = 0;
				newMarkEnd = 0;
			}
			break;
		case KEY_END:
			// move/highlight to end of text
			if (event.KeyboardEvent.Shift)
			{
				newMarkBegin = _d->cursorPos;
				newMarkEnd = _text.size();
				_d->cursorPos = 0;
			}
			else
			{
				_d->cursorPos = _text.size();
				newMarkBegin = 0;
				newMarkEnd = 0;
			}
			break;
		default:
			return false;
		}
	}
	// default keyboard handling
	else
	switch(event.KeyboardEvent.Key)
	{
	case KEY_END:
		{
			int p = _text.size();
			if (_d->wordWrapEnabled || _d->multiLine)
			{
				p = getLineFromPos(_d->cursorPos);
				p = _d->brokenTextPositions[p] + (int)_d->brokenText[p].size();
				if (p > 0 && (_text[p-1] == L'\r' || _text[p-1] == L'\n' ))
					p-=1;
			}

			if (event.KeyboardEvent.Shift)
			{
				if (_d->markBegin == _d->markEnd)
					newMarkBegin = _d->cursorPos;

				newMarkEnd = p;
			}
			else
			{
				newMarkBegin = 0;
				newMarkEnd = 0;
			}
			_d->cursorPos = p;
		}
		break;
	case KEY_HOME:
		{

			int p = 0;
			if (_d->wordWrapEnabled || _d->multiLine)
			{
				p = getLineFromPos(_d->cursorPos);
				p = _d->brokenTextPositions[p];
			}

			if (event.KeyboardEvent.Shift)
			{
				if (_d->markBegin == _d->markEnd)
					newMarkBegin = _d->cursorPos;
				newMarkEnd = p;
			}
			else
			{
				newMarkBegin = 0;
				newMarkEnd = 0;
			}
			_d->cursorPos = p;
		}
		break;
	case KEY_RETURN:
		if (_d->multiLine)
		{
			inputChar(L'\n');
			return true;
		}
		else
		{
		    sendGuiEvent( OC3_EDITBOX_ENTER );
		}
		break;
	case KEY_LEFT:

		if (event.KeyboardEvent.Shift)
		{
			if (_d->cursorPos > 0)
			{
				if (_d->markBegin == _d->markEnd)
					newMarkBegin = _d->cursorPos;

				newMarkEnd = _d->cursorPos-1;
			}
		}
		else
		{
			newMarkBegin = 0;
			newMarkEnd = 0;
		}

		if (_d->cursorPos > 0) _d->cursorPos--;
		break;

	case KEY_RIGHT:
		if (event.KeyboardEvent.Shift)
		{
			if (_text.size() > (unsigned int)_d->cursorPos)
			{
				if (_d->markBegin == _d->markEnd)
					newMarkBegin = _d->cursorPos;

				newMarkEnd = _d->cursorPos+1;
			}
		}
		else
		{
			newMarkBegin = 0;
			newMarkEnd = 0;
		}

		if (_text.size() > (unsigned int)_d->cursorPos) _d->cursorPos++;
		break;
	case KEY_UP:
		if (_d->multiLine || (_d->wordWrapEnabled && _d->brokenText.size() > 1) )
		{
			int lineNo = getLineFromPos(_d->cursorPos);
			int mb = (_d->markBegin == _d->markEnd) ? _d->cursorPos : (_d->markBegin > _d->markEnd ? _d->markBegin : _d->markEnd);
			if (lineNo > 0)
			{
				int cp = _d->cursorPos - _d->brokenTextPositions[lineNo];
				if ((int)_d->brokenText[lineNo-1].size() < cp)
					_d->cursorPos = _d->brokenTextPositions[lineNo-1] + (int)_d->brokenText[lineNo-1].size()-1;
				else
					_d->cursorPos = _d->brokenTextPositions[lineNo-1] + cp;
			}

			if (event.KeyboardEvent.Shift)
			{
				newMarkBegin = mb;
				newMarkEnd = _d->cursorPos;
			}
			else
			{
				newMarkBegin = 0;
				newMarkEnd = 0;
			}

		}
		else
		{
			return false;
		}
		break;
	case KEY_DOWN:
		if (_d->multiLine || (_d->wordWrapEnabled && _d->brokenText.size() > 1) )
		{
			int lineNo = getLineFromPos(_d->cursorPos);
			int mb = (_d->markBegin == _d->markEnd) ? _d->cursorPos : (_d->markBegin < _d->markEnd ? _d->markBegin : _d->markEnd);
			if (lineNo < (int)_d->brokenText.size()-1)
			{
				int cp = _d->cursorPos - _d->brokenTextPositions[lineNo];
				if ((int)_d->brokenText[lineNo+1].size() < cp)
					_d->cursorPos = _d->brokenTextPositions[lineNo+1] + _d->brokenText[lineNo+1].size()-1;
				else
					_d->cursorPos = _d->brokenTextPositions[lineNo+1] + cp;
			}

			if (event.KeyboardEvent.Shift)
			{
				newMarkBegin = mb;
				newMarkEnd = _d->cursorPos;
			}
			else
			{
				newMarkBegin = 0;
				newMarkEnd = 0;
			}

		}
		else
		{
			return false;
		}
		break;

	case KEY_BACK:
		if ( !isEnabled() )
			break;

		if (_text.size())
		{
      std::string s;

			if (_d->markBegin != _d->markEnd)
			{
				// delete marked text
				const int realmbgn = _d->markBegin < _d->markEnd ? _d->markBegin : _d->markEnd;
				const int realmend = _d->markBegin < _d->markEnd ? _d->markEnd : _d->markBegin;

				s = _text.substr(0, realmbgn);
				s.append( _text.substr(realmend, _text.size()-realmend) );
				_text = s;

				_d->cursorPos = realmbgn;
			}
			else
			{
				// delete text behind cursor
        s = _d->cursorPos>0 ? _text.substr(0, _d->cursorPos-1) : "";

				s.append( _text.substr(_d->cursorPos, _text.size()-_d->cursorPos) );
				_text = s;
				--_d->cursorPos;
			}

			if (_d->cursorPos < 0)
				_d->cursorPos = 0;
			newMarkBegin = 0;
			newMarkEnd = 0;
			textChanged = true;
        }
		break;
	case KEY_DELETE:
		if ( !isEnabled() )
			break;

		if (_text.size() != 0)
		{
      std::string s;

			if (_d->markBegin != _d->markEnd)
			{
				// delete marked text
				const int realmbgn = _d->markBegin < _d->markEnd ? _d->markBegin : _d->markEnd;
				const int realmend = _d->markBegin < _d->markEnd ? _d->markEnd : _d->markBegin;

				s = _text.substr(0, realmbgn);
				s.append( _text.substr(realmend, _text.size()-realmend) );
				_text = s;

				_d->cursorPos = realmbgn;
			}
			else
			{
				// delete text before cursor
				s = _text.substr(0, _d->cursorPos);
				s.append( _text.substr(_d->cursorPos+1, _text.size()-_d->cursorPos-1) );
				_text = s;
			}

			if (_d->cursorPos > (int)_text.size())
				_d->cursorPos = (int)_text.size();

            newMarkBegin = 0;
			newMarkEnd = 0;
			textChanged = true;
		}
		break;

	case KEY_ESCAPE:
	case KEY_TAB:
	case KEY_SHIFT:
	case KEY_F1:
	case KEY_F2:
	case KEY_F3:
	case KEY_F4:
	case KEY_F5:
	case KEY_F6:
	case KEY_F7:
	case KEY_F8:
	case KEY_F9:
	case KEY_F10:
	case KEY_F11:
	case KEY_F12:
	case KEY_F13:
	case KEY_F14:
	case KEY_F15:
	case KEY_F16:
	case KEY_F17:
	case KEY_F18:
	case KEY_F19:
	case KEY_F20:
	case KEY_F21:
	case KEY_F22:
	case KEY_F23:
	case KEY_F24:
		// ignore these keys
		return false;

	default:
        inputChar(event.KeyboardEvent.Char);
		return true;
	}

    // Set new text markers
    setTextMarkers( newMarkBegin, newMarkEnd );

	// break the text if it has changed
	if (textChanged)
	{
		breakText();
    _d->needUpdateTexture = true;
		sendGuiEvent(OC3_EDITBOX_CHANGED);
	}

	calculateScrollPos();

	return true;
}

void EditBox::DrawHolderText_( Font font, Rect* clip )
{
  if( isFocused() )
  {
    NColor holderColor( 0xffC0C0C0 );
    setTextRect( 0, _d->holderText );
    Font holderFont = font;

    if( holderFont.isValid() )
    {
        holderFont.draw( *_d->textPicture, _d->holderText, 0, 0 );
    }
  }
}

void EditBox::beforeDraw( GfxEngine& painter )
{
    int startPos = 0;

    if( _d->needUpdateTexture )
    {
      _d->needUpdateTexture = false;
      if( !_d->picture || ( _d->picture && getSize() != _d->picture->getSize()) )
      {
        _d->picture.reset( Picture::create( getSize() ) );
      }

      if( _d->cursorPic.isNull() )
      {
        _d->cursorPic.reset( Picture::create( Size(1, getHeight() / 5 * 4 ) ) );
        _d->cursorPic->fill( 0xff000000, Rect( 0, 0, 0, 0) );
      }

      if( !_d->textPicture || ( _d->textPicture && getSize() != _d->textPicture->getSize()) )
      {
        _d->textPicture.reset( Picture::create( getSize() ) );
      }

      if( _d->bgPicture.isValid() )
      {
        _d->picture->draw( _d->bgPicture, 0, 0 );
      }
      else
      {
        GuiPaneling::instance().draw_black_frame( *_d->picture, 0, 0, getWidth(), getHeight() );
      }

      Rect localClipRect = getAbsoluteRect();
      _d->markAreaRect = Rect( 0, 0, -1, -1 );
      localClipRect.clipAgainst( getAbsoluteClippingRect() );

      NColor simpleTextColor, markTextColor;

      simpleTextColor = 0xff000000;

      markTextColor = 0xffffffff;

//         if( _d->lastBreakFont != font )
//         {
//             breakText();
//         }

      if( _d->lastBreakFont.isValid() )
      {
          // calculate cursor pos
        std::string* txtLine = &_text;

        std::string s2;

        // get mark position
        const bool ml = (!_d->isPasswordBox && (_d->wordWrapEnabled || _d->multiLine));
        const int realmbgn = _d->markBegin < _d->markEnd ? _d->markBegin : _d->markEnd;
        const int realmend = _d->markBegin < _d->markEnd ? _d->markEnd : _d->markBegin;
        const int hlineStart = ml ? getLineFromPos(realmbgn) : 0;
        const int hlineCount = ml ? getLineFromPos(realmend) - hlineStart + 1 : 1;
        const int lineCount = ml ? _d->brokenText.size() : 1;

        if( _text.size() )
        {
          _d->textPicture->fill( 0x00000000, Rect(0, 0, 0, 0) );
          for (int i=0; i < lineCount; ++i)
          {
             setTextRect(i);

             // clipping test - don't draw anything outside the visible area
             Rect c = localClipRect;
             c.clipAgainst( _d->currentTextRect );
             if (!c.isValid())
                 continue;

             // get current line%
             if (_d->isPasswordBox)
             {
                 if (_d->brokenText.size() != 1)
                 {
                     _d->brokenText.clear();
                     _d->brokenText.push_back(std::string());
                 }
                 if (_d->brokenText[0].size() != _text.size())
                 {
                     _d->brokenText[0] = _text;
                     for (unsigned int q = 0; q < _text.size(); ++q)
                     {
                         _d->brokenText[0][q] = _d->passwordChar;
                     }
                 }
                 txtLine = &_d->brokenText[0];
                 startPos = 0;
             }
             else
             {
                 txtLine = ml ? &_d->brokenText[i] : &_text;
                 startPos = ml ? _d->brokenTextPositions[i] : 0;
             }

             //font->Draw(txtLine->c_str(), _d->currentTextRect_ + marginOffset, simpleTextColor,	false, true, &localClipRect);
             Rect textureRect( Point( 0, 0 ), _d->currentTextRect.getSize() );
             
             _d->lastBreakFont.draw( *_d->textPicture, *txtLine, 0, 0 );

             // draw mark and marked text
             if( isFocused() && _d->markBegin != _d->markEnd && i >= hlineStart && i < hlineStart + hlineCount)
             {
               int mbegin = 0, mend = 0;
               int lineStartPos = 0, lineEndPos = txtLine->size();

               std::string s;

               if (i == hlineStart)
               {
                   // highlight start is on this line
                   s = txtLine->substr(0, realmbgn - startPos);
                   mbegin = _d->lastBreakFont.getSize( s ).getWidth();

                   // deal with kerning
                   //mbegin += _d->lastBreakFont.getKerningSize( &((*txtLine)[realmbgn - startPos]),
                   //                                      realmbgn - startPos > 0 ? &((*txtLine)[realmbgn - startPos - 1]) : 0);
                   mbegin += 3;     

                   lineStartPos = realmbgn - startPos;
               }

               if( i == hlineStart + hlineCount - 1 )
               {
                   // highlight end is on this line
                   s2 = txtLine->substr( 0, realmend - startPos );
                   mend = _d->lastBreakFont.getSize( s2 ).getWidth();
                   lineEndPos = (int)s2.size();
               }
               else
                   mend = _d->lastBreakFont.getSize( *txtLine ).getWidth();

               _d->markAreaRect = _d->currentTextRect - _d->currentTextRect.UpperLeftCorner;
               _d->markAreaRect.UpperLeftCorner += Point( mbegin, 0 );
               _d->markAreaRect.LowerRightCorner += Point( _d->markAreaRect.UpperLeftCorner.getX() + mend - mbegin, 0 );

               //draw mark
               _d->markAreaRect = _d->markAreaRect /*+ marginOffset */;
               //_d->markAreaRect.UpperLeftCorner += _d->markStyle->GetMargin().getRect().UpperLeftCorner;
               //_d->markAreaRect.LowerRightCorner -= _d->markStyle->GetMargin().getRect().LowerRightCorner;

               // draw marked text
               s = txtLine->substr(lineStartPos, lineEndPos - lineStartPos);

               if( s.size() )
               {
                   //_d->txs4Text.NeedUpdateTextureWithText();
                   //_d->txs4Text.CreateTextureWithText( painter, textureRect, *txtLine, font, simpleTextColor, _textHorzAlign, _textVertAlign, false );
               }
             }
           }
        }
        else
            DrawHolderText_( _d->lastBreakFont, &localClipRect );
      }
  }

  if( _d->cursorPos != _d->oldCursorPos )
  {
      int cursorLine = 0;
      int charcursorpos = 0;
      _d->oldCursorPos = _d->cursorPos;

      std::string* txtLine = &_text;

      if( _d->wordWrapEnabled || _d->multiLine )
      {
          cursorLine = getLineFromPos(_d->cursorPos);
          txtLine = &_d->brokenText[ cursorLine ];
          startPos = _d->brokenTextPositions[ cursorLine ];
      }

      std::string stringBeforeCursor = txtLine->substr(0,_d->cursorPos-startPos);

	    {
            //wchar_t lastChar = _d->cursorPos-startPos > 0 ? txtLine->at( _d->cursorPos-startPos-1 ) : 0;
		    charcursorpos = _d->lastBreakFont.getSize( stringBeforeCursor ).getWidth() + 1/*font.GetKerningWidth(L"_", lastChar ? &lastChar : NULL )*/ ;
	    }

      setTextRect(cursorLine);
      _d->cursorRect = _d->currentTextRect;
      _d->cursorRect.UpperLeftCorner += Point( charcursorpos + 1, 0 );
      _d->cursorRect.LowerRightCorner = _d->cursorRect.UpperLeftCorner + Point( 1, getHeight() );
      //_d->cursorRect.UpperLeftCorner += style.GetMargin().getRect().UpperLeftCorner;
      //_d->cursorRect.LowerRightCorner -= style.GetMargin().getRect().LowerRightCorner;
  }

  Widget::beforeDraw( painter );
}

//! draws the element and its children
void EditBox::draw( GfxEngine& painter )
{
	if (!isVisible())
		return;

	const bool focus = _environment->hasFocus(this);

  //const ElementStyle& style = getStyle().GetState( getActiveState() );
	//const ElementStyle& markStyle = getStyle().GetState( L"Marked" );
  //core::Point marginOffset = style.GetMargin().getRect().UpperLeftCorner;

  if( _d->markAreaRect.isValid() )
  {
      NColor markAreaColor( 0xff0000ff );
      //markAreaColor.setAlpha( _d->resultTransparent );

      //painter.drawRectangle( markAreaColor, convertLocalToScreen( _d->markAreaRect ), &getAbsoluteClippingRectRef() );
  }

	// draw the text
  if( _d->picture )
  {
    painter.drawPicture( *_d->picture, getScreenLeft(), getScreenTop() );
  }

  if( _d->textPicture )
  {
    painter.drawPicture( *_d->textPicture, getScreenLeft(), getScreenTop() );
  }

  if( focus )
	{
		unsigned int t = DateTime::getElapsedTime() % 1000;
    if( t < 500 )
    {
		  painter.drawPicture( *_d->cursorPic, _d->cursorRect.UpperLeftCorner );
    }
	}

	// draw children
	Widget::draw( painter );
}

//! Sets the new caption of this element.
void EditBox::setText(const std::string& text)
{
  _text = text;
  if (static_cast<unsigned int>(_d->cursorPos) > _text.size())
  {
    _d->cursorPos = _text.size();
  }

  _d->horizScrollPos = 0;
  breakText();

  _d->needUpdateTexture = true;
}


//! Enables or disables automatic scrolling with cursor position
//! \param enable: If set to true, the text will move around with the cursor position
void EditBox::setAutoScroll(bool enable)
{
	_d->autoScrollEnabled = enable;
}


//! Checks to see if automatic scrolling is enabled
//! \return true if automatic scrolling is enabled, false if not
bool EditBox::isAutoScrollEnabled() const
{

	return _d->autoScrollEnabled;
}


//! Gets the area of the text in the edit box
//! \return Returns the size in pixels of the text
Size EditBox::getTextDimension()
{
	Rect ret;

	setTextRect(0);
  ret = _d->currentTextRect;

	for (unsigned int i=1; i < _d->brokenText.size(); ++i)
	{
		setTextRect(i);
    ret.addInternalPoint(_d->currentTextRect.UpperLeftCorner);
    ret.addInternalPoint(_d->currentTextRect.LowerRightCorner);
	}

	return ret.getSize();
}


//! Sets the maximum amount of characters which may be entered in the box.
//! \param max: Maximum amount of characters. If 0, the character amount is
//! infinity.
void EditBox::setMax(unsigned int max)
{
	_d->max = max;

	if (_text.size() > _d->max && _d->max != 0)
		_text = _text.substr(0, _d->max);
}


//! Returns maximum amount of characters, previously set by setMax();
unsigned int EditBox::getMax() const
{
	return _d->max;
}


bool EditBox::processMouse(const NEvent& event)
{
	switch(event.MouseEvent.Event)
	{
	case OC3_LMOUSE_LEFT_UP:
		if (_environment->hasFocus(this))
		{
			_d->cursorPos = getCursorPos(event.MouseEvent.X, event.MouseEvent.Y);
			if (_d->mouseMarking)
			{
			    setTextMarkers( _d->markBegin, _d->cursorPos );
			}
			_d->mouseMarking = false;
			calculateScrollPos();
			return true;
		}
		break;
	case OC3_MOUSE_MOVED:
		{
			if (_d->mouseMarking)
			{
				_d->cursorPos = getCursorPos(event.MouseEvent.X, event.MouseEvent.Y);
				setTextMarkers( _d->markBegin, _d->cursorPos );
				calculateScrollPos();
				return true;
			}
		}
		break;
	case OC3_LMOUSE_PRESSED_DOWN:
		if (!_environment->hasFocus(this))
		{
			_d->mouseMarking = true;
			_d->cursorPos = getCursorPos(event.MouseEvent.X, event.MouseEvent.Y);
			setTextMarkers(_d->cursorPos, _d->cursorPos );
			calculateScrollPos();
			return true;
		}
		else
		{
			if( !getAbsoluteClippingRect().isPointInside( event.MouseEvent.getPosition() ) )
			{
				return false;
			}
			else
			{
				// move cursor
				_d->cursorPos = getCursorPos(event.MouseEvent.X, event.MouseEvent.Y);

                int newMarkBegin = _d->markBegin;
				if (!_d->mouseMarking)
					newMarkBegin = _d->cursorPos;

				_d->mouseMarking = true;
				setTextMarkers( newMarkBegin, _d->cursorPos);
				calculateScrollPos();
				return true;
			}
		}
	default:
		break;
	}

	return false;
}


int EditBox::getCursorPos(int x, int y)
{
	Font font = getActiveFont();

	const unsigned int lineCount = (_d->wordWrapEnabled || _d->multiLine) ? _d->brokenText.size() : 1;

  std::string *txtLine=0;
	int startPos=0;
	x+=3;

	for (unsigned int i=0; i < lineCount; ++i)
	{
		setTextRect(i);
    if (i == 0 && y < _d->currentTextRect.UpperLeftCorner.getY() )
    {
      y = _d->currentTextRect.UpperLeftCorner.getY();
    }
 
    if (i == lineCount - 1 && y > _d->currentTextRect.LowerRightCorner.getY() )
    {
      y = _d->currentTextRect.LowerRightCorner.getY();
    }

		// is it inside this region?
    if (y >= _d->currentTextRect.UpperLeftCorner.getY() && y <= _d->currentTextRect.LowerRightCorner.getY() )
		{
			// we've found the clicked line
			txtLine = (_d->wordWrapEnabled || _d->multiLine) ? &_d->brokenText[i] : &_text;
			startPos = (_d->wordWrapEnabled || _d->multiLine) ? _d->brokenTextPositions[i] : 0;
			break;
		}
	}

  if( x < _d->currentTextRect.UpperLeftCorner.getX() )
  {
    x = _d->currentTextRect.UpperLeftCorner.getX();
  }

	if ( !txtLine )
  {
		return 0;
  }

  int idx = font.getCharacterFromPos( *txtLine, x - _d->currentTextRect.UpperLeftCorner.getX() );

	// click was on or left of the line
	if (idx != -1)
		return idx + startPos;

	// click was off the right edge of the line, go to end.
	return txtLine->size() + startPos;
}


//! Breaks the single text line.
void EditBox::breakText()
{
	if ((!_d->wordWrapEnabled && !_d->multiLine))
		return;

	_d->brokenText.clear(); // need to reallocate :/
	_d->brokenTextPositions.clear();

	Font font = getActiveFont();
	if( !font.isValid() )
		return;

  _d->lastBreakFont = font;

  std::string line;
	std::string word;
	std::string whitespace;
	int lastLineStart = 0;
	int size = _text.size();
	int length = 0;
	int elWidth = getWidth() - 6;
	char c;

	for (int i=0; i<size; ++i)
	{
		c = _text[i];
		bool lineBreak = false;

		if (c == L'\r') // Mac or Windows breaks
		{
			lineBreak = true;
			c = 0;
			if (_text[i+1] == L'\n') // Windows breaks
			{
				_text.erase(i+1);
				--size;
			}
		}
		else if (c == L'\n') // Unix breaks
		{
			lineBreak = true;
			c = 0;
		}

		// don't break if we're not a multi-line edit box
		if (!_d->multiLine)
			lineBreak = false;

		if (c == L' ' || c == 0 || i == (size-1))
		{
			// here comes the next whitespace, look if
			// we can break the last word to the next line
			// We also break whitespace, otherwise cursor would vanish beside the right border.
			int whitelgth = font.getSize( whitespace ).getWidth();
			int worldlgth = font.getSize( word ).getWidth();

			if (_d->wordWrapEnabled && length + worldlgth + whitelgth > elWidth)
			{
				// break to next line
				length = worldlgth;
				_d->brokenText.push_back(line);
				_d->brokenTextPositions.push_back(lastLineStart);
				lastLineStart = i - (int)word.size();
				line = word;
			}
			else
			{
				// add word to line
				line += whitespace;
				line += word;
				length += whitelgth + worldlgth;
			}

			word = "";
			whitespace = "";


			if ( c )
				whitespace += c;

			// compute line break
			if (lineBreak)
			{
				line += whitespace;
				line += word;
				_d->brokenText.push_back(line);
				_d->brokenTextPositions.push_back(lastLineStart);
				lastLineStart = i+1;
				line = "";
				word = "";
				whitespace = "";
				length = 0;
			}
		}
		else
		{
			// yippee this is a word..
			word += c;
		}
	}

	line += whitespace;
	line += word;
	_d->brokenText.push_back(line);
	_d->brokenTextPositions.push_back(lastLineStart);
}

void EditBox::setTextRect(int line, const std::string& tempText )
{
	if ( line < 0 )
		return;

	Font font = getActiveFont();
	if( !font.isValid() )
		return;

	Size d;

	// get text dimension
        //const unsigned int lineCount = (WordWrap || MultiLine) ? BrokenText.size() : 1;
	if (_d->wordWrapEnabled || _d->multiLine)
	{
        d = font.getSize( tempText.size() > 0 ? tempText : _d->brokenText[line] );
	}
	else
	{
		d = font.getSize( tempText.size() > 0 ? tempText : _text );
		d.setHeight( getHeight() );
	}
	
  d.setHeight( d.getHeight() + font.getKerningHeight() );

  _d->currentTextRect = getAbsoluteRect();

  _d->currentTextRect.UpperLeftCorner += Point( -_d->horizScrollPos, d.getHeight() *line - _d->vertScrollPos );
  _d->currentTextRect.LowerRightCorner = Point( _d->currentTextRect.getRight() +_d->horizScrollPos, _d->currentTextRect.UpperLeftCorner.getY() + d.getHeight() );
}

int EditBox::getLineFromPos(int pos)
{
	if (!_d->wordWrapEnabled && !_d->multiLine)
		return 0;

	int i=0;
	while (i < (int)_d->brokenTextPositions.size())
	{
		if (_d->brokenTextPositions[i] > pos)
			return i-1;
		++i;
	}
	return (int)_d->brokenTextPositions.size() - 1;
}


void EditBox::inputChar( char c)
{
	if (!isEnabled())
		return;

	if (c != 0)
	{
		if (_text.size() < _d->max || _d->max == 0)
		{
      std::string s;

			if (_d->markBegin != _d->markEnd)
			{
				// replace marked text
				const int realmbgn = _d->markBegin < _d->markEnd ? _d->markBegin : _d->markEnd;
				const int realmend = _d->markBegin < _d->markEnd ? _d->markEnd : _d->markBegin;

				s = _text.substr(0, realmbgn);
        s += c;
				s += _text.substr(realmend, _text.size()-realmend);
				_text = s;
				_d->cursorPos = realmbgn+1;
			}
			else
			{
				// add new character
				s = _text.substr(0, _d->cursorPos);
				s += c;
				s += _text.substr(_d->cursorPos, _text.size()-_d->cursorPos);
				_text = s;
				++_d->cursorPos;
			}

			setTextMarkers(0, 0);
		}
	}

	breakText();
	sendGuiEvent(OC3_EDITBOX_CHANGED);
	calculateScrollPos();
  _d->needUpdateTexture = true;
}


void EditBox::calculateScrollPos()
{
	if (!_d->autoScrollEnabled)
		return;

	// calculate horizontal scroll position
	int cursLine = getLineFromPos(_d->cursorPos);
	if ( cursLine < 0 )
		return;
	setTextRect(cursLine);

	// don't do horizontal scrolling when wordwrap is enabled.
	if (!_d->wordWrapEnabled)
	{
		// get cursor position
		Font font = getActiveFont();
		if( !font.isValid() )
			return;

    std::string *txtLine = _d->multiLine ? &_d->brokenText[cursLine] : &_text;
		int cPos = _d->multiLine ? _d->cursorPos - _d->brokenTextPositions[cursLine] : _d->cursorPos;

    int cStart = _d->currentTextRect.UpperLeftCorner.getX() + _d->horizScrollPos +
		                            font.getSize( txtLine->substr(0, cPos) ).getWidth();

		int cEnd = cStart + font.getSize( "_ " ).getWidth();

		if ( getScreenRight() < cEnd)
			_d->horizScrollPos = cEnd - getScreenRight();
		else if ( getScreenLeft() > cStart)
			_d->horizScrollPos = cStart - getScreenLeft();
		else
			_d->horizScrollPos = 0;

		// todo: adjust scrollbar
	}

	// vertical scroll position
  if( getScreenBottom() < _d->currentTextRect.LowerRightCorner.getY() + _d->vertScrollPos)
  {
    _d->vertScrollPos = _d->currentTextRect.LowerRightCorner.getY() - getScreenBottom() + _d->vertScrollPos;
  }
  else if ( getScreenTop() > _d->currentTextRect.UpperLeftCorner.getY() + _d->vertScrollPos)
  {
    _d->vertScrollPos = _d->currentTextRect.UpperLeftCorner.getY() - getScreenTop() + _d->vertScrollPos;
  }
	else
		_d->vertScrollPos = 0;

	// todo: adjust scrollbar
}

//! set text markers
void EditBox::setTextMarkers(int begin, int end)
{
    if ( begin != _d->markBegin || end != _d->markEnd )
    {
        _d->markBegin = begin;
        _d->markEnd = end;
        sendGuiEvent( OC3_EDITBOX_MARKING_CHANGED);
        _d->needUpdateTexture;
    }
}

//! send some gui event to parent
void EditBox::sendGuiEvent( unsigned int type)
{
    getParent()->onEvent( NEvent::Gui( this, 0, (OC3_GUI_EVENT_TYPE)type ));
}

//! Writes attributes of the element.
//void EditBox::save( core::VariantArray* out ) const
//{
	/*out->addBool  ("Border", 			  Border);
	out->addBool  ("Background", 		  Background);
	out->addBool  ("_d->overrideColorEnabled",_d->overrideColorEnabled );
	//out->addColor ("OverrideColor",       OverrideColor);
	// out->addFont("OverrideFont",OverrideFont);
	out->addInt   ("MaxChars",            Max);
	out->addBool  ("WordWrap",            WordWrap);
	out->addBool  ("MultiLine",           MultiLine);
	out->addBool  ("AutoScroll",          AutoScroll);
	out->addBool  ("PasswordBox",         PasswordBox);
	String ch = L" ";
	ch[0] = PasswordChar;
	out->addString("PasswordChar",        ch.c_str());
	out->addEnum  ("HTextAlign",          _textHorzAlign, NrpAlignmentNames);
	out->addEnum  ("VTextAlign",          _textVertAlign, NrpAlignmentNames);

	INrpElement::serializeAttributes(out,options);
    */
//}


//! Reads attributes of the element
//void EditBox::load( core::VariantArray* in )
//{
//       Widget::load(in);
/*
	setDrawBorder( in->getAttributeAsBool("Border") );
	setDrawBackground( in->getAttributeAsBool("Background") );
	setOverrideColor(in->getAttributeAsColor("OverrideColor"));
	enableOverrideColor(in->getAttributeAsBool("_d->overrideColorEnabled"));
	setMax(in->getAttributeAsInt("MaxChars"));
	setWordWrap(in->getAttributeAsBool("WordWrap"));
	setMultiLine(in->getAttributeAsBool("MultiLine"));
	setAutoScroll(in->getAttributeAsBool("AutoScroll"));
	String ch = in->getAttributeAsStringW("PasswordChar");

	if (!ch.size())
		setPasswordBox(in->getAttributeAsBool("PasswordBox"));
	else
		setPasswordBox(in->getAttributeAsBool("PasswordBox"), ch[0]);

	setTextAlignment( (OC3_ALIGNMENT) in->getAttributeAsEnumeration("HTextAlign", GUIAlignmentNames),
			(OC3_ALIGNMENT) in->getAttributeAsEnumeration("VTextAlign", GUIAlignmentNames));

	// setOverrideFont(in->getAttributeAsFont("OverrideFont"));
    */
//}

NColor EditBox::getOverrideColor() const
{
    return _d->overrideColor;
}

void EditBox::SetDrawBackground( bool enabled )
{
    _d->drawBackground = enabled;
}
