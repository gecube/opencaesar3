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

#ifndef _OPENCAESAR3_BUILDING_EDUCATION_H_INCLUDE_
#define _OPENCAESAR3_BUILDING_EDUCATION_H_INCLUDE_

#include "oc3_building_service.hpp"

class School : public ServiceBuilding
{
public:
  School();

  int getVisitorsNumber() const;

  virtual void deliverService();

  virtual unsigned int getWalkerDistance() const;
};

class Library : public ServiceBuilding
{
public:
   Library();

   int getVisitorsNumber() const;
};

class College : public ServiceBuilding
{
public:
   College();

   int getVisitorsNumber() const;
};


#endif
