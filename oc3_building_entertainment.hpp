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

#ifndef __OPENCAESAR3_BUILDING_ENTERTAINMENT_H_INCLUDED__
#define __OPENCAESAR3_BUILDING_ENTERTAINMENT_H_INCLUDED__

#include "oc3_building_service.hpp"

class EntertainmentBuilding : public ServiceBuilding
{
public:
  EntertainmentBuilding( const Service::Type service, const BuildingType type,
                         const Size& size);
  virtual void deliverService();
  virtual int getVisitorsNumber() const;
};

class Theater : public EntertainmentBuilding
{
public:
   Theater();

   virtual int getVisitorsNumber() const;
};

class BuildingAmphiTheater : public EntertainmentBuilding
{
public:
   BuildingAmphiTheater();
};

class Collosseum : public EntertainmentBuilding
{
public:
   Collosseum();
};


class BuildingHippodrome : public EntertainmentBuilding
{
public:
   BuildingHippodrome();
};

#endif //__OPENCAESAR3_BUILDING_ENTERTAINMENT_H_INCLUDED__
