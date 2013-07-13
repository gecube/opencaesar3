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
//
// Copyright 2012-2013 Gregoire Athanase, gathanase@gmail.com


#ifndef WALKER_HPP
#define WALKER_HPP

#include <string>
#include <memory>

#include "oc3_building.hpp"
#include "oc3_walker_action.hpp"
#include "oc3_service_building.hpp"
#include "oc3_picture.hpp"
#include "oc3_enums.hpp"
#include "oc3_serializer.hpp"
#include "oc3_referencecounted.hpp"
#include "oc3_smartptr.hpp"
#include "oc3_safetycast.hpp"
#include "oc3_scopedptr.hpp"
#include "oc3_predefinitions.hpp"

typedef unsigned int UniqueId;
class PathWay;

class Walker : public Serializable, public ReferenceCounted
{
public:
   Walker();
   virtual ~Walker();

   virtual void timeStep(const unsigned long time);  // performs one simulation step
   virtual int getType() const;
   // position and movement
   int getI() const;
   int getJ() const;

   TilePos getIJ() const;
   void setIJ( const TilePos& pos );

   Point getPosition() const;

   void setPathWay( const PathWay& pathWay);
   const PathWay& getPathway() const;

   //void setDestinationIJ( const TilePos& pos );
   void setSpeed(const float speed);
   virtual void onNewTile();  // called when the walker is on a new tile
   virtual void onMidTile();  // called when the walker is on the middle of a tile
   virtual void onDestination();  // called when the walker is at his destination
   virtual void onNewDirection(); // called when the walker changes direction
   void computeDirection();
   void walk();
   void setUniqueId( const UniqueId uid );

   DirectionType getDirection();

   void save( VariantMap& stream) const;
   void load( const VariantMap& stream);

   // graphic
   WalkerGraphicType getWalkerGraphic() const;
   virtual void getPictureList(std::vector<Picture*> &oPics);
   Picture& getMainPicture();

   // state
   bool isDeleted() const;  // returns true if the walker should be forgotten
   void deleteLater();

protected:
   WalkerType _walkerType;
   WalkerGraphicType _walkerGraphic;
   bool _isDeleted;

   PathWay& _getPathway();
   Animation& _getAnimation();
   void _setAction( WalkerActionType );
   void _setDirection( DirectionType );

private:
   /* useful method for subtile movement computation
      si   = subtile coordinate in the current tile
      i    = tile coordinate
      amount = amount of the increase, returns remaining movement if any
      midPos = position of the midtile (so that walkers are not all exactly on the middle of the tile)
      newTile = return true if we are now on a new tile
      midTile = return true if we got above the treshold
    */
   void inc(int &ioSI, int &ioI, int &ioAmount, const int iMidPos, bool &oNewTile, bool &oMidTile);
   void dec(int &ioSI, int &ioI, int &ioAmount, const int iMidPos, bool &oNewTile, bool &oMidTile);

private:   
   class Impl;
   ScopedPtr< Impl > _d;
};

/** Soldier, friend or enemy */
class Soldier : public Walker
{
public:
   Soldier();
private:

};

#endif
