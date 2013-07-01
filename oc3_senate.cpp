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

#include "oc3_senate.hpp"
#include "oc3_scenario.hpp"
#include "oc3_picture.hpp"
#include "oc3_resourcegroup.hpp"

// govt 4  - senate
// govt 9  - advanced senate
// govt 5 ~ 8 - senate flags

Senate::Senate() : ServiceBuilding(S_SENATE, B_SENATE, Size(5) )
{
  setPicture( Picture::load( ResourceGroup::govt, 4) );
}

bool Senate::canBuild( const TilePos& pos ) const
{
  bool mayBuild = ServiceBuilding::canBuild( pos );

  if( mayBuild )
  {
    CityPtr city = Scenario::instance().getCity();
    LandOverlays senate = city->getBuildingList(B_SENATE);
    mayBuild &= !( senate.size() > 0 );
  }

  return mayBuild;
}