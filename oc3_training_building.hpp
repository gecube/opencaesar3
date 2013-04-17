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


#ifndef TRAINING_BUILDING_HPP
#define TRAINING_BUILDING_HPP

#include "oc3_building.hpp"


class TrainingBuilding : public WorkingBuilding
{
public:
   TrainingBuilding();

   void timeStep(const unsigned long time);

   // called when a trainee is created
   virtual void deliverTrainee() = 0;

   // virtual GuiInfoBox* makeInfoBox();

   void serialize(OutputSerialStream &stream);
   void unserialize(InputSerialStream &stream);

private:
   int _trainingTimer;
   int _trainingDelay;
};


class BuildingActor : public TrainingBuilding
{
public:
   BuildingActor();
   BuildingActor* clone() const;

   void deliverTrainee();
};

class BuildingGladiator : public TrainingBuilding
{
public:
   BuildingGladiator();
   BuildingGladiator* clone() const;

   void deliverTrainee();
};

class BuildingLion : public TrainingBuilding
{
public:
   BuildingLion();
   BuildingLion* clone() const;

   void deliverTrainee();
};

class BuildingChariot : public TrainingBuilding
{
public:
   BuildingChariot();
   BuildingChariot* clone() const;

   void deliverTrainee();
};


#endif
