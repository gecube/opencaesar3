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

#ifndef _OPENCAESAR3_WINDOW_PLAYERNAME_H_INCLUDE_
#define _OPENCAESAR3_WINDOW_PLAYERNAME_H_INCLUDE_

#include "oc3_gui_widget.hpp"
#include "oc3_signals.hpp"
#include "oc3_scopedptr.hpp"

class WindowPlayerName : public Widget
{
public:
  WindowPlayerName( Widget* parent );
  ~WindowPlayerName();

public oc3_signals:
  Signal0<>& onClose();
  Signal1<std::string>& onNameChange();

private:
  class Impl;
  ScopedPtr< Impl > _d;
};

#endif //_OPENCAESAR3_WINDOW_PLAYERNAME_H_INCLUDE_
