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

#include "oc3_service_building.hpp"

#include <cstdlib>
#include <ctime>

#include "oc3_tile.hpp"
#include "oc3_scenario.hpp"
#include "oc3_walker_service.hpp"
#include "oc3_exception.hpp"
#include "oc3_gui_info_box.hpp"
#include "oc3_gettext.hpp"
#include "oc3_variant.hpp"
#include "oc3_resourcegroup.hpp"

class ServiceBuilding::Impl
{
public:
  int serviceDelay;
  ServiceType service;
  int serviceTimer;
  int serviceRange;
};

ServiceBuilding::ServiceBuilding(const ServiceType service,
                                 const BuildingType type, const Size& size)
                                 : WorkingBuilding( type, size ), _d( new Impl )
{
   _d->service = service;
   setMaxWorkers(5);
   setWorkers(0);
   setServiceDelay( 80 );
   _d->serviceTimer = 0;
   _d->serviceRange = 30;
}

void ServiceBuilding::setServiceDelay( const int delay )
{
  _d->serviceDelay = delay;
}

ServiceType ServiceBuilding::getService() const
{
   return _d->service;
}

void ServiceBuilding::timeStep(const unsigned long time)
{
   WorkingBuilding::timeStep(time);   

   if (_d->serviceTimer == 0)
   {
      deliverService();
      _d->serviceTimer = getServiceDelay();
   }
   else if (_d->serviceTimer > 0)
   {
      _d->serviceTimer -= 1;
   }

   _getAnimation().update( time );
   Picture *pic = _getAnimation().getCurrentPicture();
   if (pic != NULL)
   {
      int level = _fgPictures.size()-1;
      _fgPictures[level] = _getAnimation().getCurrentPicture();
   }
}

void ServiceBuilding::destroy()
{
   WorkingBuilding::destroy();
}

void ServiceBuilding::deliverService()
{
  // make a service walker and send him to his wandering
  ServiceWalkerPtr serviceman = ServiceWalker::create( Scenario::instance().getCity(), getService() );
  serviceman->setMaxDistance( getWalkerDistance() );
  serviceman->send2City( BuildingPtr( this ) );

  if( !serviceman->isDeleted() )
  {
      addWalker( serviceman.as<Walker>() );
  }
}

int ServiceBuilding::getServiceRange() const
{
   return _d->serviceRange;
}

void ServiceBuilding::save( VariantMap& stream ) const 
{
  WorkingBuilding::save( stream );
  stream[ "timer" ] = _d->serviceTimer;
  stream[ "delay" ] = _d->serviceDelay;
  stream[ "range" ] = _d->serviceRange;
}

void ServiceBuilding::load( const VariantMap& stream )
{
  WorkingBuilding::load( stream );
  _d->serviceTimer = (int)stream.get( "timer" );
  _d->serviceDelay = (int)stream.get( "delay" );
  _d->serviceRange = (int)stream.get( "range" );
}

int ServiceBuilding::getServiceDelay() const
{
  return _d->serviceDelay;
}

ServiceBuilding::~ServiceBuilding()
{

}

unsigned int ServiceBuilding::getWalkerDistance() const
{
  return 5;
}

EntertainmentBuilding::EntertainmentBuilding(const ServiceType service, 
                                             const BuildingType type,
                                             const Size& size ) 
  : ServiceBuilding(service, type, size)
{
   switch (service)
   {
   case S_THEATER:
      _traineeMap[WTT_ACTOR] = 0;
      break;
   case S_AMPHITHEATER:
      _traineeMap[WTT_ACTOR] = 0;
      _traineeMap[WTT_GLADIATOR] = 0;
      break;
   case S_COLLOSSEUM:
      _traineeMap[WTT_GLADIATOR] = 0;
      _traineeMap[WTT_TAMER] = 0;
      break;
   default:
      break;
   }
}

void EntertainmentBuilding::deliverService()
{
   // we need all trainees types for the show
   int minLevel = 100;
   for (std::map<WalkerTraineeType, int>::iterator itLevel = _traineeMap.begin(); itLevel != _traineeMap.end(); ++itLevel)
   {
      minLevel = std::min(minLevel, itLevel->second);
   }

   if (minLevel > 10)
   {
      // all trainees are there for the show!
      for (std::map<WalkerTraineeType, int>::iterator itLevel = _traineeMap.begin(); itLevel != _traineeMap.end(); ++itLevel)
      {
         itLevel->second = itLevel->second - 10;
      }
      ServiceBuilding::deliverService();
   }
}

int EntertainmentBuilding::getVisitorsNumber() const
{
  return 0;
}

Theater::Theater() : EntertainmentBuilding(S_THEATER, B_THEATER, Size(2))
{
  setPicture( Picture::load( "entertainment", 13));

   _getAnimation().load("entertainment", 14, 21);
   _getAnimation().setOffset( Point( 60, 36 ) );
  
   _fgPictures.resize(2);
   _fgPictures[0] = &Picture::load("entertainment", 35);
}

int Theater::getVisitorsNumber() const
{
  return 500;
}

BuildingAmphiTheater::BuildingAmphiTheater() : EntertainmentBuilding(S_AMPHITHEATER, B_AMPHITHEATER, Size(3))
{
  setPicture( Picture::load("entertainment", 1));

  _getAnimation().load("entertainment", 2, 10);
  _getAnimation().setOffset( Point( 100, 49 ) );
  _fgPictures.resize(2);
  _fgPictures[0] = &Picture::load("entertainment", 12);
}

BuildingCollosseum::BuildingCollosseum() : EntertainmentBuilding(S_COLLOSSEUM, B_COLLOSSEUM, Size(5) )
{
  setPicture( Picture::load("entertainment", 36));

  _getAnimation().load("entertainment", 37, 13);
  _getAnimation().setOffset( Point( 122, 81 ) );
  _fgPictures.resize(2);
  _fgPictures[0] = &Picture::load("entertainment", 50);
}

//------------

BuildingHippodrome::BuildingHippodrome() : EntertainmentBuilding(S_HIPPODROME, B_HIPPODROME, Size(5) )
{
  setPicture( Picture::load("circus", 5));
  getPicture().setOffset(0,106);
  Picture* logo = &Picture::load("circus", 3);
  Picture* logo1 = &Picture::load("circus", 1);
  logo -> setOffset(150,181);
  logo1 -> setOffset(300,310);
  _fgPictures.resize(5);
  _fgPictures.at(0) = logo;
  _fgPictures.at(1) = logo1;
}

//-----------


School::School() : ServiceBuilding(S_SCHOOL, B_SCHOOL, Size(2))
{
  setPicture( Picture::load( ResourceGroup::commerce, 83));
}

int School::getVisitorsNumber() const
{
  return 75;
}

Library::Library() : ServiceBuilding(S_LIBRARY, B_LIBRARY, Size(2))
{
  setPicture( Picture::load( ResourceGroup::commerce, 84));
}

int Library::getVisitorsNumber() const
{
  return 800;
}

College::College() : ServiceBuilding(S_COLLEGE, B_COLLEGE, Size(3))
{
  setPicture( Picture::load( ResourceGroup::commerce, 85));
}

int College::getVisitorsNumber() const
{
  return 100;
}
