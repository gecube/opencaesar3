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

#include "oc3_goverment_building.hpp"
#include "oc3_resourcegroup.hpp"

// housng1a 46 - governor's house    3 x 3
// housng1a 47 - governor's villa    4 x 4
// housng1a 48 - governor's palace   5 x 5

GovernorsHouse::GovernorsHouse() : WorkingBuilding( B_GOVERNOR_HOUSE, Size(3) )
{
  setMaxWorkers(5);
  setWorkers(0);    
  setPicture(Picture::load( ResourceGroup::housing, 46));
}

GovernorsVilla::GovernorsVilla() : WorkingBuilding(B_GOVERNOR_VILLA, Size(4) )
{
  setMaxWorkers(10);
  setWorkers(0);    
  setPicture(Picture::load( ResourceGroup::housing, 47));
}

GovernorsPalace::GovernorsPalace() : WorkingBuilding(B_GOVERNOR_PALACE, Size( 5 ) )
{
  setMaxWorkers(15);
  setWorkers(0);  
  setPicture(Picture::load(ResourceGroup::housing, 48));
}

MissionPost::MissionPost() : WorkingBuilding(B_MISSION_POST, Size(2) )
{
  setMaxWorkers(20);
  setWorkers(0);  
  setPicture( Picture::load( ResourceGroup::transport, 93));
}

