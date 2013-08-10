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

#ifndef __OPENCAESAR3_EMPIRECITY_H_INCLUDED__
#define __OPENCAESAR3_EMPIRECITY_H_INCLUDED__

#include "oc3_scopedptr.hpp"
#include "oc3_referencecounted.hpp"
#include "oc3_positioni.hpp"
#include "oc3_predefinitions.hpp"
#include "oc3_serializer.hpp"

class GoodStore;

class EmpireCity : public ReferenceCounted, public Serializable
{
public:
  virtual std::string getName() const = 0;
  virtual Point getLocation() const = 0;
  virtual void setLocation( const Point& location ) = 0;

  virtual void resolveMerchantArrived( EmpireMerchantPtr ) = 0;

  virtual const GoodStore& getSells() const = 0;
  virtual const GoodStore& getBuys() const = 0;
};

#endif //__OPENCAESAR3_EMPIRECITY_H_INCLUDED__