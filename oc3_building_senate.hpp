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

#ifndef __OPENCAESAR3_SENATE_H_INCLUDED_
#define __OPENCAESAR3_SENATE_H_INCLUDED_

#include "oc3_building_service.hpp"
#include "oc3_scopedptr.hpp"

class Senate : public ServiceBuilding
{
public:
  Senate();
  unsigned int getFunds() const;

  int collectTaxes();
  int getPeoplesReached() const;

  void deliverService();

  bool canBuild( const TilePos& pos )const;
private:
  class Impl;
  ScopedPtr< Impl > _d;
};

#endif