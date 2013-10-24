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

#include "oc3_walker_ship.hpp"
#include "oc3_gettext.hpp"
#include "oc3_city.hpp"
#include "oc3_building_wharf.hpp"
#include "oc3_good.hpp"
#include "oc3_fish_place.hpp"
#include "oc3_astarpathfinding.hpp"
#include "oc3_stringhelper.hpp"
#include "oc3_pathway.hpp"

Ship::Ship( CityPtr city )
  : Walker( city )
{
  _setType( WT_NONE );
  _setGraphic( WG_NONE );

  setName( _("##ship##") );
}

Ship::~Ship()
{

}

class FishingBoat::Impl
{
public:
  typedef enum { go2fishplace, catchFish, back2Base, finishCatch, unloadFish, ready2Catch, wait } Mode;
  WharfPtr base;
  TilePos destination;
  GoodStock stock;
  Mode mode;

  PathWay findFishingPlace(CityPtr city, const TilePos& pos);
};

void FishingBoat::save( VariantMap& stream ) const
{
  Ship::save( stream );

  stream[ "destination" ] = _d->destination;
}

void FishingBoat::load( const VariantMap& stream )
{
  Walker::load( stream );
  _d->destination = stream.get( "destination" ).toTilePos();
}

void FishingBoat::timeStep(const unsigned long time)
{
  Ship::timeStep( time );

  if( time % 16 == 1 )
  {
    switch( _d->mode )
    {
    case Impl::ready2Catch:
    {
      _getAnimation().clear();
      _setGraphic( WG_FISHING_BOAT );
      PathWay way = _d->findFishingPlace( _getCity(), getIJ() );
      if( way.isValid() )
      {
        setPathWay( way );
        go();

        _d->mode = Impl::go2fishplace;
      }
    }
    break;

    case Impl::catchFish:
    {
      _getAnimation().clear();
      _setGraphic( WG_FISHING_BOAT_WORK );

      CityHelper helper( _getCity() );
      FishPlacePtr overlay = helper.find<FishPlace>( getIJ() );

      if( overlay != 0 )
      {
        FishPlacePtr fishplace = overlay.as<FishPlace>();
        _d->stock._currentQty = math::clamp( _d->stock._currentQty+10, 0, _d->stock._maxQty );
      }
      else
      {
        _d->mode = Impl::ready2Catch;
      }

      if( _d->stock._currentQty == _d->stock._maxQty )
      {
        _d->mode = Impl::finishCatch;
      }
    }
    break;

    case Impl::finishCatch:
    {
      if( _d->base != 0 )
      {
        bool pathfound = Pathfinder::getInstance().getPath( getIJ(), _d->base->getLandingTile().getIJ(),
                                                            _getPathway(), Pathfinder::waterOnly, Size(0) );

        if( pathfound )
        {
          _d->mode = Impl::back2Base;
          go();
        }

        _getAnimation().clear();
        _setGraphic( WG_FISHING_BOAT );
      }
      else
      {
        StringHelper::debug( 0xff, "Broken fishing boat" );
        deleteLater();
      }
    }
    break;

    case Impl::go2fishplace: break;
    case Impl::back2Base: break;
    case Impl::wait: break;

    case Impl::unloadFish:
      if( _d->stock._currentQty > 0 && _d->base != 0 )
      {
        _d->stock._currentQty = math::clamp( _d->stock._currentQty-10, 0, 100 );
        _d->base->updateProgress( 10 );
      }

      if( _d->stock._currentQty == 0 )
      {
        _d->mode = Impl::ready2Catch;
      }
    break;
    }
  }
}

void FishingBoat::startCatch()
{
  _d->mode = Impl::ready2Catch;
}

bool FishingBoat::isBusy() const
{
  return _d->mode != Impl::wait;
}

FishingBoat::FishingBoat( CityPtr city ) : Ship( city ), _d( new Impl )
{
  _setGraphic( WG_FISHING_BOAT );
  _setType( WT_FISHING_BOAT );
  setName( _("##fishing_boat##") );
  _d->mode = Impl::wait;
  _d->stock.setType( Good::fish );
  _d->stock.setMax( 100 );
}

FishingBoatPtr FishingBoat::create(CityPtr city)
{
  FishingBoatPtr ret( new FishingBoat( city ) );
  ret->drop();

  return ret;
}

void FishingBoat::onDestination()
{
  Walker::onDestination();

  switch( _d->mode )
  {
  case Impl::go2fishplace: _d->mode = Impl::catchFish; break;
  case Impl::back2Base: _d->mode = Impl::unloadFish; break;
  default: break;
  }
}

void FishingBoat::onNewTile()
{
  Walker::onNewTile();
  _getAnimation().setFrameDelay( 3 );
}

PathWay FishingBoat::Impl::findFishingPlace( CityPtr city, const TilePos& pos )
{
  CityHelper helper( city );
  FishPlaceList places = helper.find<FishPlace>( wtrFishPlace );

  int minDistance = 999;
  FishPlacePtr nearest;
  foreach( FishPlacePtr place, places )
  {
    int currentDistance = pos.distanceFrom( place->getTilePos() );
    if( currentDistance < minDistance )
    {
      minDistance = currentDistance;
      nearest = place;
    }
  }

  if( nearest != 0 )
  {
    PathWay way;
    bool pathFound = Pathfinder::getInstance().getPath( pos, nearest->getTilePos(),
                                                        way, Pathfinder::waterOnly, Size(0) );

    if( pathFound )
      return way;
  }

  return PathWay();
}

void FishingBoat::send2City( WharfPtr base, const TilePos &start )
{
  _d->base = base;
  if( !isDeleted() )
  {
    setIJ( start );
    _getCity()->addWalker( this );
  }
}
