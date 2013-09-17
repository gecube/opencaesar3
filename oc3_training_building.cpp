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

#include "oc3_training_building.hpp"

#include <iostream>

#include "oc3_walker_trainee.hpp"
#include "oc3_exception.hpp"
#include "oc3_gui_info_box.hpp"
#include "oc3_gettext.hpp"
#include "oc3_variant.hpp"
#include "oc3_resourcegroup.hpp"
#include "oc3_city.hpp"

TrainingBuilding::TrainingBuilding( const BuildingType type, const Size& size )
  : WorkingBuilding( type, size )
{
   setMaxWorkers(5);
   setWorkers(0);
   _trainingTimer = 0;
   _trainingDelay = 80;
}

void TrainingBuilding::timeStep(const unsigned long time)
{
   Building::timeStep(time);

   if (_trainingTimer == 0)
   {
      deliverTrainee();
      _trainingTimer = _trainingDelay;
   }
   else if (_trainingTimer > 0)
   {
      _trainingTimer -= 1;
   }

   _getAnimation().update( time );
   const Picture& pic = _getAnimation().getCurrentPicture();
   if( pic.isValid() )
   {
      int level = _fgPictures.size()-1;
      _fgPictures[level] = _getAnimation().getCurrentPicture();
   }
}

// void TrainingBuilding::deliverTrainee()
// {
//    // make a service walker and send him to his wandering
//    ServiceWalker *walker = new ServiceWalker(_service);
//    walker->setServiceBuilding(*this);
//    walker->start();
// }

void TrainingBuilding::save( VariantMap& stream) const
{
  WorkingBuilding::save( stream );
  stream[ "trainingTimer" ] = _trainingTimer;
  stream[ "trainingDelay" ] = _trainingDelay;
}

void TrainingBuilding::load( const VariantMap& stream )
{
//    WorkingBuilding::unserialize(stream);
//    _trainingTimer = stream.read_int(2, 0, 1000);
//    _trainingDelay = stream.read_int(2, 0, 1000);
}


//GuiInfoBox* TrainingBuilding::makeInfoBox()
//{
//   GuiInfoService* box = new GuiInfoService(*this);
//   return box;
//}


ActorColony::ActorColony() : TrainingBuilding( B_ACTOR_COLONY, Size(3) )
{
  setPicture( Picture::load( ResourceGroup::entertaiment, 81));

  _getAnimation().load( ResourceGroup::entertaiment, 82, 9);
  _getAnimation().setOffset( Point( 68, -6 ) );
  _fgPictures.resize(1);
}

void ActorColony::deliverTrainee()
{
   // std::cout << "Deliver trainee!" << std::endl;
  TraineeWalkerPtr trainee = TraineeWalker::create( _getCity(), WT_ACTOR);
  trainee->setOriginBuilding(*this);
  trainee->send2City();
}

GladiatorSchool::GladiatorSchool() : TrainingBuilding( B_GLADIATOR_SCHOOL, Size(3))
{
  setPicture( Picture::load( ResourceGroup::entertaiment, 51));

  _getAnimation().load( ResourceGroup::entertaiment, 52, 10);
  _getAnimation().setOffset( Point( 62, 24 ) );
  _fgPictures.resize(1);
}

void GladiatorSchool::deliverTrainee()
{
   // std::cout << "Deliver trainee!" << std::endl;
  TraineeWalkerPtr trainee = TraineeWalker::create( _getCity(), WT_GLADIATOR);
  trainee->setOriginBuilding(*this);
  trainee->send2City();
}


LionsNursery::LionsNursery() : TrainingBuilding( B_LION_HOUSE, Size(3) )
{
  setPicture( Picture::load( ResourceGroup::entertaiment, 62));

   _getAnimation().load( ResourceGroup::entertaiment, 63, 18);
   _getAnimation().setOffset( Point( 78, 21) );
   _fgPictures.resize(1);
}

void LionsNursery::deliverTrainee()
{
  // std::cout << "Deliver trainee!" << std::endl;
  TraineeWalkerPtr trainee = TraineeWalker::create( _getCity(), WT_TAMER );
  trainee->setOriginBuilding(*this);
  trainee->send2City();
}


WorkshopChariot::WorkshopChariot() : TrainingBuilding( B_CHARIOT_MAKER, Size(3) )
{
  setPicture( Picture::load( ResourceGroup::entertaiment, 91));

  _getAnimation().load( ResourceGroup::entertaiment, 92, 10);
  _getAnimation().setOffset( Point( 54, 23 ));
  _fgPictures.resize(1);
}

void WorkshopChariot::deliverTrainee()
{
   // std::cout << "Deliver trainee!" << std::endl;
  TraineeWalkerPtr trainee = TraineeWalker::create( _getCity(), WT_CHARIOT);
  trainee->setOriginBuilding(*this);
  trainee->send2City();
}


