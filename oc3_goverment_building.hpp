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

#ifndef __OPENCAESAR3_GOVERMENT_BUILDINGS_H_INCLUDED__
#define __OPENCAESAR3_GOVERMENT_BUILDINGS_H_INCLUDED__

#include "oc3_working_building.hpp"

class GovernorsHouse  : public WorkingBuilding
{
public:
  GovernorsHouse();

  //  void serialize(OutputSerialStream &stream);
  //  void unserialize(InputSerialStream &stream);
};

class GovernorsVilla  : public WorkingBuilding
{
public:
  GovernorsVilla();

  //  void serialize(OutputSerialStream &stream);
  //  void unserialize(InputSerialStream &stream);
};

class GovernorsPalace : public WorkingBuilding
{
public:
  GovernorsPalace();

  //  void serialize(OutputSerialStream &stream);
  //  void unserialize(InputSerialStream &stream);
};

class MissionPost : public WorkingBuilding
{
public:
  MissionPost();
};

#endif //__OPENCAESAR3_GOVERMENT_BUILDINGS_H_INCLUDED__