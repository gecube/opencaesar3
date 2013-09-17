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

#ifndef _OPENCAESAR3_SCROLLBAR_INCLUDE_
#define _OPENCAESAR3_SCROLLBAR_INCLUDE_

#include "oc3_gui_widget.hpp"
#include "oc3_predefinitions.hpp"
#include "oc3_signals.hpp"
#include "oc3_scopedptr.hpp"

class AbstractSkin;
class PushButton;
class Picture;

class ScrollBar : public Widget
{
public:
	enum { Vertical=false, Horizontal=true };
	//! constructor
	ScrollBar( Widget* parent, const Rect& rectangle, bool horizontal=false, int id=-1,
	           bool noclip=false);

	//! destructor
	virtual ~ScrollBar();

  virtual void beforeDraw( GfxEngine& painter );

	//! called if an event happened.
	virtual bool onEvent(const NEvent& event);

	//! draws the element and its children
	virtual void draw( GfxEngine& painter );

	virtual void afterPaint(unsigned int timeMs);

	//! gets the maximum value of the scrollbar.
	virtual int getMax() const;

	//! sets the maximum value of the scrollbar.
	virtual void setMax(int max);

	//! gets the minimum value of the scrollbar.
	virtual int getMin() const;

	//! sets the minimum value of the scrollbar.
	virtual void setMin(int max);

	//! gets the small step value
	virtual int getSmallStep() const;

	//! sets the small step value
	virtual void setSmallStep(int step);

	//! gets the large step value
	virtual int getLargeStep() const;

	//! sets the large step value
	virtual void setLargeStep(int step);

	//! gets the current position of the scrollbar
	virtual int getPos() const;

	//! sets the position of the scrollbar
	virtual void setPos(int pos);

	//! sets the texture which draw
  virtual void setBackgroundImage( const Picture& pixmap );

	//! gets the UpButton
	virtual PushButton* getUpButton();

	virtual PushButton* getDownButton();

	virtual void setSliderImage( const Picture& pixmap, const ElementState state );

	//! Writes attributes of the element.
	virtual void save( VariantMap& out ) const;

	//! Reads attributes of the element
	virtual void load( const VariantMap& in );

  virtual void setHorizontal( bool horizontal );
  virtual void setVisibleFilledArea( bool vis );

	float getRange () const;

oc3_signals public:
    Signal1<int>& onPositionChanged();

protected:
	void _resizeEvent();
  void refreshControls_();

  virtual int _getPosFromMousePos(const Point& p) const;

  bool _dragging;
  bool _isSliderHovered, _lastSliderHovered;
  bool _horizontal;
  bool _draggedBySlider;
  bool _trayClick;
  bool _overrideBgColorEnabled, _visibleFilledArea;

  unsigned int _lastStateNameHash;

  int _overrideBgColor;
  int _value;
  int _sliderPos, _lastSliderPos;
	int _drawLenght;
  int _minValue;
  int _maxVallue;
  int _smallStep;
  int _largeStep;
  int _desiredPos;
  unsigned int _lastTimeChange;

	class Impl;
	ScopedPtr< Impl > _d;

  void _resolvePositionChanged();
  std::string _GetFullStyleName();
  PushButton* _CreateButton( const Rect& rectangle,
                             TypeAlign left, TypeAlign rigth, TypeAlign top, TypeAlign bottom, int type );
};

#endif
