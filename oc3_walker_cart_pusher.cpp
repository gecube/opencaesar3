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

#include "oc3_walker_cart_pusher.hpp"

#include "oc3_building_data.hpp"
#include "oc3_exception.hpp"
#include "oc3_city.hpp"
#include "oc3_positioni.hpp"
#include "oc3_granary.hpp"
#include "oc3_building_warehouse.hpp"
#include "oc3_tile.hpp"
#include "oc3_goodhelper.hpp"
#include "oc3_variant.hpp"
#include "oc3_path_finding.hpp"
#include "oc3_picture_bank.hpp"
#include "oc3_building_factory.hpp"
#include "oc3_goodstore.hpp"

class CartPusher::Impl
{
public:
  CityPtr city;
  GoodStock stock;
  BuildingPtr producerBuilding;
  BuildingPtr consumerBuilding;
  Picture cartPicture;
  int maxDistance;
  long reservationID;

  BuildingPtr getWalkerDestination_factory(Propagator& pathPropagator, PathWay &oPathWay);
  BuildingPtr getWalkerDestination_warehouse(Propagator& pathPropagator, PathWay &oPathWay);
  BuildingPtr getWalkerDestination_granary(Propagator& pathPropagator, PathWay &oPathWay);
};

CartPusher::CartPusher( CityPtr city ) : _d( new Impl )
{
   _walkerGraphic = WG_PUSHER;
   _walkerType = WT_CART_PUSHER;
   _d->producerBuilding = NULL;
   _d->consumerBuilding = NULL;
   _d->maxDistance = 25;
   _d->city = city;
   _d->stock._maxQty = 400;
}

void CartPusher::onDestination()
{
  Walker::onDestination();
  _d->cartPicture = Picture();

  if( _d->consumerBuilding != NULL )
  {
    GranaryPtr granary = _d->consumerBuilding.as<Granary>();
    WarehousePtr warehouse = _d->consumerBuilding.as<Warehouse>();
    FactoryPtr factory = _d->consumerBuilding.as<Factory>(); 
    if( granary != NULL )
    {
       granary->getGoodStore().applyStorageReservation(_d->stock, _d->reservationID);
       _d->reservationID = 0;
    }
    else if ( warehouse != NULL )
    {
       warehouse->getGoodStore().applyStorageReservation(_d->stock, _d->reservationID);
       _d->reservationID = 0;
    }
    else if( factory != NULL )
    {
       factory->getGoodStore().applyStorageReservation(_d->stock, _d->reservationID);
       // factory->computePictures();
       _d->reservationID = 0;
    }
  }
  //
  if( !_getPathway().isReverse() )
  {
    _getPathway().toggleDirection();
    _setAction( WA_MOVE );
    computeDirection();
    _d->consumerBuilding = 0;
  }
  else
  {
    deleteLater();
  }
}

GoodStock& CartPusher::getStock()
{
   return _d->stock;
}

void CartPusher::setProducerBuilding(BuildingPtr building)
{
   _d->producerBuilding = building;
}

void CartPusher::setConsumerBuilding(BuildingPtr building)
{
   _d->consumerBuilding = building;
}

BuildingPtr CartPusher::getProducerBuilding()
{
   if( _d->producerBuilding.isNull() ) 
     THROW("ProducerBuilding is not initialized");
   return _d->producerBuilding;
}

BuildingPtr CartPusher::getConsumerBuilding()
{
   if( _d->consumerBuilding.isNull() ) 
     THROW("ConsumerBuilding is not initialized");
   
   return _d->consumerBuilding;
}

Picture& CartPusher::getCartPicture()
{
   if( !_d->cartPicture.isValid() )
   {
     _d->cartPicture = GoodHelper::getCartPicture(_d->stock, getDirection());
   }

   return _d->cartPicture;
}

void CartPusher::onNewDirection()
{
   Walker::onNewDirection();
   _d->cartPicture = Picture();  // need to get the new graphic
}

void CartPusher::getPictureList(std::vector<Picture> &oPics)
{
   oPics.clear();

   // depending on the walker direction, the cart is ahead or behind
   switch (getDirection())
   {
   case D_WEST:
   case D_NORTH_WEST:
   case D_NORTH:
   case D_NORTH_EAST:
      oPics.push_back( getCartPicture() );
      oPics.push_back( getMainPicture() );
      break;
   case D_EAST:
   case D_SOUTH_EAST:
   case D_SOUTH:
   case D_SOUTH_WEST:
      oPics.push_back( getMainPicture() );
      oPics.push_back( getCartPicture() );
      break;
   default:
      break;
   }
}

void CartPusher::computeWalkerDestination()
{
   // get the list of buildings within reach
   PathWay pathWay;
   Propagator pathPropagator( _d->city );
   _d->consumerBuilding = 0;

   _OC3_DEBUG_BREAK_IF( _d->producerBuilding.isNull() && "CartPusher: producerBuilding can't be NULL" );
   if( _d->producerBuilding.isNull() )
   {
     deleteLater();
     return;
   }

   pathPropagator.init( _d->producerBuilding.as<Construction>() );
   pathPropagator.propagate(_d->maxDistance);

   BuildingPtr destBuilding;
   if (destBuilding == NULL)
   {
      // try send that good to a factory
      destBuilding = _d->getWalkerDestination_factory(pathPropagator, pathWay);
   }

   if (destBuilding == NULL)
   {
      // try send that good to a granary
      destBuilding = _d->getWalkerDestination_granary(pathPropagator, pathWay);
   }

   if (destBuilding == NULL)
   {
      // try send that good to a warehouse
      destBuilding = _d->getWalkerDestination_warehouse( pathPropagator, pathWay );
   }

   if( destBuilding != NULL)
   {
      //_isDeleted = true;  // no destination!
     setConsumerBuilding( destBuilding );
     setPathWay( pathWay );
     setIJ( _getPathway().getOrigin().getIJ() );
     setSpeed( 1 );
   }
   else
   {
     _setDirection( D_NORTH );
     setSpeed( 0 );
     setIJ( _d->producerBuilding->getAccessRoads().front()->getIJ() );
     walk();
   }
}

template< class T >
BuildingPtr reserveShortestPath( const BuildingType buildingType, 
                                 GoodStock& stock, long& reservationID,
                                 Propagator &pathPropagator, PathWay &oPathWay )
{
  BuildingPtr res;
  Propagator::Routes pathWayList;
  pathPropagator.getRoutes(buildingType, pathWayList);

  //remove factories with improrer storage
  Propagator::Routes::iterator pathWayIt= pathWayList.begin();
  while( pathWayIt != pathWayList.end() )
  {
    // for every factory within range
    SmartPtr<T> building = pathWayIt->first.as<T>();

    if( stock._currentQty >  building->getGoodStore().getMaxStore( stock._goodType ) )
    {
      pathWayList.erase( pathWayIt++ );
    }
    else
    {
      pathWayIt++;
    }
  }

  //find shortest path
  int maxLength = 999;
  PathWay* shortestPath = 0;
  for( Propagator::Routes::iterator pathIt = pathWayList.begin(); 
    pathIt != pathWayList.end(); pathIt++ )
  {
    if( pathIt->second.getLength() < maxLength )
    {
      shortestPath = &pathIt->second;
      maxLength = pathIt->second.getLength();
      res = pathIt->first;
    }
  }

  if( res.isValid() )
  {
    reservationID = res.as<T>()->getGoodStore().reserveStorage( stock );
    if (reservationID != 0)
    {
      oPathWay = *shortestPath;
    }
  }


  return res;
}

BuildingPtr CartPusher::Impl::getWalkerDestination_factory(Propagator &pathPropagator, PathWay &oPathWay)
{
  BuildingPtr res;
  GoodType goodType = stock._goodType;
  BuildingType buildingType = BuildingDataHolder::instance().getBuildingTypeByInGood(goodType);

  if (buildingType == B_NONE)
  {
     // no factory can use this good
     return 0;
  }

  res = reserveShortestPath<Factory>( buildingType, stock, reservationID, pathPropagator, oPathWay );

  return res;
}

BuildingPtr CartPusher::Impl::getWalkerDestination_warehouse(Propagator &pathPropagator, PathWay &oPathWay)
{
  BuildingPtr res;

  res = reserveShortestPath<Warehouse>( B_WAREHOUSE, stock, reservationID, pathPropagator, oPathWay );

  return res;
}

BuildingPtr CartPusher::Impl::getWalkerDestination_granary(Propagator &pathPropagator, PathWay &oPathWay)
{
   BuildingPtr res;

   GoodType goodType = stock._goodType;
   if (!(goodType == G_WHEAT || goodType == G_FISH || goodType == G_MEAT || goodType == G_FRUIT || goodType == G_VEGETABLE))
   {
      // this good cannot be stored in a granary
      return 0;
   }

   res = reserveShortestPath<Granary>( B_GRANARY, stock, reservationID, pathPropagator, oPathWay );

   return res;
}

void CartPusher::send2City( BuildingPtr building, GoodStock& carry )
{
  _d->stock.append( carry );
  setProducerBuilding( building  );

  computeWalkerDestination();

  if( !isDeleted() )
  {
    _d->city->addWalker( this );
  }
}

void CartPusher::timeStep( const unsigned long time )
{
  if( (time % 22 == 1) && (_getPathway().getLength() < 2) )
  {
    computeWalkerDestination();
  }

  Walker::timeStep( time );
}

CartPusherPtr CartPusher::create( CityPtr city )
{
  CartPusherPtr ret( new CartPusher( city ) );
  ret->drop(); //delete automatically

  return ret;
}

void CartPusher::save( VariantMap& stream ) const
{
  Walker::save( stream );
  
  stream[ "stock" ] = _d->stock.save();
  stream[ "producerPos" ] = _d->producerBuilding->getTile().getIJ();
  stream[ "consumerPos" ] = _d->consumerBuilding.isValid() 
                                      ? _d->consumerBuilding->getTile().getIJ()
                                      : TilePos( -1, -1 );

  stream[ "maxDistance" ] = _d->maxDistance;
  stream[ "reservationID" ] = static_cast<int>(_d->reservationID);
}

void CartPusher::load( const VariantMap& stream )
{
  Walker::load( stream );

  _d->stock.load( stream.get( "stock" ).toList() );

  TilePos prPos( stream.get( "producerPos" ).toTilePos() );
  Tile& prTile = _d->city->getTilemap().at( prPos );
  _d->producerBuilding = prTile.getTerrain().getOverlay().as<Building>();
  
  if( _d->producerBuilding.is<WorkingBuilding>() )
  {
    _d->producerBuilding.as<WorkingBuilding>()->addWalker( this );
  }

  TilePos cnsmPos( stream.get( "consumerPos" ).toTilePos() );
  Tile& cnsmTile = _d->city->getTilemap().at( cnsmPos );
  _d->consumerBuilding = cnsmTile.getTerrain().getOverlay().as<Building>();

  _d->maxDistance = stream.get( "maxDistance" ).toInt();
  _d->reservationID = stream.get( "reservationID" ).toInt();
}
