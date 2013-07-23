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

#ifndef _OPENCAESAR3_CONTEXMENUITEM_INCLUDE_H_
#define _OPENCAESAR3_CONTEXMENUITEM_INCLUDE_H_

#include "oc3_gui_label.hpp"
#include "oc3_scopedptr.hpp"

class ContextMenu;

class ContextMenuItem : public Label
{
public:    
  typedef enum { hoverIndex=3 } OpacityIndex;
  typedef enum { alignRigth=0x1, alignLeft=0x2, alignHorizCenter=0x4, 
		alignTop=0x10, alignBottom=0x20, alignVertCenter=0x40, 
		alignAuto=0xff } SubMenuAlign;

  typedef enum 
  {
    drawSubmenuSprite = 0,
    drawCount
  } DrawFlag;


  ContextMenuItem( ContextMenu* parent, const std::string& text );

  ~ContextMenuItem();

  virtual bool isSeparator() const;

  virtual void setCommandId( int cmdId);   

  virtual int getCommandId() const;

  virtual ContextMenu* getSubMenu() const;

  virtual void setIsSeparator( bool isSepar );

  virtual void toggleCheck();

  virtual void setFlag( DrawFlag flagName, bool set=true );

  virtual void setIcon( const Picture& icon );

  virtual bool isHovered() const;

  virtual void setHovered( bool hover );

  virtual void setAutoChecking( bool autoChecking );

  virtual bool isPointInside(const Point& point) const;

  virtual void setChecked( bool check );

  virtual bool isChecked() const;

  virtual int getOffset() const;

  virtual void setOffset( int offset );

  virtual void setDim( const Size& size );

  virtual const Size& getDim() const;

  //! Adds a sub menu from an element that already exists.
  virtual void setSubMenu( ContextMenu* menu );

  virtual ContextMenu* addSubMenu( int id = -1 );

  virtual void setSubMenuAlignment( SubMenuAlign align );

  virtual SubMenuAlign getSubMenuAlignment() const;

oc3_signals public:
  Signal1<bool>& onChecked();

private:
  class Impl;
  ScopedPtr< Impl > _d;
};

#endif //_OPENCAESAR3_CONTEXMENUITEM_INCLUDE_H_