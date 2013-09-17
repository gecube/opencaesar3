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

#include "oc3_contextmenuitem.hpp"
#include "oc3_contextmenuitemprivate.h"
#include "oc3_gui_contextmenu.hpp"

ContextMenuItem::ContextMenuItem( ContextMenu* parent, const std::string& text ) 
  : Label( parent, Rect( 0, 0, 1, 1 ), text, false, Label::bgWhite ), _d( new Impl )
{
  _d->isSeparator = false;
  _d->subMenuAlignment = alignAuto;
  _d->checked = false;
  _d->isHovered = false;
  _d->isAutoChecking = false;
  _d->dim = Size( 0, 0 );
  _d->offset = 0;
  _d->subMenu = NULL;
  _d->setFlag( drawSubmenuSprite, false );
  _d->commandId = -1;
  _d->luaFunction = -1;
}

Signal1<bool>& ContextMenuItem::onChecked()
{
  return _d->onCheckedSignal;
}

ContextMenu* ContextMenuItem::addSubMenu( int id )
{
  ContextMenu* sub = new ContextMenu( getParent(), Rect(0,0,100,100), id, false, false);
  setSubMenu( sub );
  getParent()->bringChildToFront( sub );
  return sub;
}

//! Adds a sub menu from an element that already exists.
void ContextMenuItem::setSubMenu( ContextMenu* menu )
{
  if( menu )
    menu->grab();

  if( _d->subMenu )
    _d->subMenu->drop();

  _d->subMenu = menu;
  menu->setVisible(false);

  if( _d->subMenu )
  {
    menu->setAllowFocus( false );
    if( menu->isFocused() )
      getParent()->setFocus();
  }

  if( ContextMenu* parentCntx = safety_cast< ContextMenu* >( getParent() ) )
    parentCntx->updateItems();
}

bool ContextMenuItem::isSeparator() const
{
  return _d->isSeparator;
}

void ContextMenuItem::setCommandId( int cmdId )
{
	_d->commandId = cmdId;
}

int ContextMenuItem::getCommandId() const
{
  return _d->commandId;
}

void ContextMenuItem::toggleCheck()
{
  if( _d->isAutoChecking )
  {
    _d->checked = !_d->checked;
  _d->onCheckedSignal.emit( _d->checked );
  }
}

bool ContextMenuItem::isHovered() const
{
  return _d->isHovered;
}

void ContextMenuItem::setAutoChecking( bool autoChecking )
{
  _d->isAutoChecking = autoChecking;
}

bool ContextMenuItem::isPointInside( const Point& point ) const
{
  return false;
}

int ContextMenuItem::getOffset() const
{
  return _d->offset;
}

void ContextMenuItem::setChecked( bool check )
{
  _d->checked = check;
}

bool ContextMenuItem::isChecked() const
{
  return _d->checked;
}

void ContextMenuItem::setIsSeparator( bool isSepar )
{
  _d->isSeparator = isSepar;
}

void ContextMenuItem::setHovered( bool hover )
{
  _d->isHovered = hover;
}

ContextMenu* ContextMenuItem::getSubMenu() const
{
  return _d->subMenu;
}

void ContextMenuItem::setSubMenuAlignment( SubMenuAlign align )
{
  _d->subMenuAlignment = align;
}

ContextMenuItem::SubMenuAlign ContextMenuItem::getSubMenuAlignment() const
{
  return _d->subMenuAlignment;
}

void ContextMenuItem::setIcon( const Picture& icon )
{

}

void ContextMenuItem::setFlag( DrawFlag flagName, bool set/*=true */ )
{
  _d->setFlag( flagName, set );
}

ContextMenuItem::~ContextMenuItem()
{
  if( _d->subMenu )
    _d->subMenu->drop();
}

void ContextMenuItem::setDim( const Size& size )
{
  _d->dim = size;
}

const Size& ContextMenuItem::getDim() const
{
  return _d->dim;
}

void ContextMenuItem::setOffset( int offset )
{
  _d->offset = offset;
}
