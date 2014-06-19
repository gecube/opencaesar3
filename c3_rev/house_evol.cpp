#include "house_evol.h"
#include "defs.h"
#include "variables.h"

void __cdecl fun_decayService_culture()
{
  signed int i; // [sp+4Ch] [bp-4h]@1

  for ( i = 1; i < 2000; ++i )
  {
    if ( buildings[i].inUse == 1 )
    {
      if ( buildings[i].houseSize )
      {
        if ( (signed int)_LOBYTE(building_5a_house_theater_amphi[64 * i]) <= 1 )
          _LOBYTE(building_5a_house_theater_amphi[64 * i]) = 0;
        else
          --_LOBYTE(building_5a_house_theater_amphi[64 * i]);

        if ( (signed int)_HIBYTE(building_5a_house_theater_amphi[64 * i]) <= 1 )
          _HIBYTE(building_5a_house_theater_amphi[64 * i]) = 0;
        else
          --_HIBYTE(building_5a_house_theater_amphi[64 * i]);

        if ( (signed int)_LOBYTE(building_5c_house_amphiGlad_colo[64 * i]) <= 1 )
          _LOBYTE(building_5c_house_amphiGlad_colo[64 * i]) = 0;
        else
          --_LOBYTE(building_5c_house_amphiGlad_colo[64 * i]);

        if ( (signed int)_HIBYTE(building_5c_house_amphiGlad_colo[64 * i]) <= 1 )
          _HIBYTE(building_5c_house_amphiGlad_colo[64 * i]) = 0;
        else
          --_HIBYTE(building_5c_house_amphiGlad_colo[64 * i]);

        if ( (signed int)_LOBYTE(building_5e_house_coloLion_hippo[64 * i]) <= 1 )
          _LOBYTE(building_5e_house_coloLion_hippo[64 * i]) = 0;
        else
          --_LOBYTE(building_5e_house_coloLion_hippo[64 * i]);

        if ( (signed int)_HIBYTE(building_5e_house_coloLion_hippo[64 * i]) <= 1 )
          _HIBYTE(building_5e_house_coloLion_hippo[64 * i]) = 0;
        else
          --_HIBYTE(building_5e_house_coloLion_hippo[64 * i]);

        if ( (signed int)_LOBYTE(building_60_house_school_library[64 * i]) <= 1 )
          _LOBYTE(building_60_house_school_library[64 * i]) = 0;
        else
          --_LOBYTE(building_60_house_school_library[64 * i]);

        if ( (signed int)_HIBYTE(building_60_house_school_library[64 * i]) <= 1 )
          _HIBYTE(building_60_house_school_library[64 * i]) = 0;
        else
          --_HIBYTE(building_60_house_school_library[64 * i]);

        if ( (signed int)_LOBYTE(building_62_house_academy_barber[64 * i]) <= 1 )
          _LOBYTE(building_62_house_academy_barber[64 * i]) = 0;
        else
          --_LOBYTE(building_62_house_academy_barber[64 * i]);

        if ( (signed int)_HIBYTE(building_62_house_academy_barber[64 * i]) <= 1 )
          _HIBYTE(building_62_house_academy_barber[64 * i]) = 0;
        else
          --_HIBYTE(building_62_house_academy_barber[64 * i]);

        if ( (signed int)(unsigned char)building_64_house_clinic[128 * i] <= 1 )
          building_64_house_clinic[128 * i] = 0;
        else
          --building_64_house_clinic[128 * i];

        if ( (signed int)(unsigned char)building_65_house_bathhouse_dock_numships_entert_days[128 * i] <= 1 )
          building_65_house_bathhouse_dock_numships_entert_days[128 * i] = 0;
        else
          --building_65_house_bathhouse_dock_numships_entert_days[128 * i];
        if ( (signed int)(unsigned char)building_66_house_hospital_entert_days2[128 * i] <= 1 )
          building_66_house_hospital_entert_days2[128 * i] = 0;
        else
          --building_66_house_hospital_entert_days2[128 * i];
        if ( (signed int)(unsigned char)building_67_house_ceres[128 * i] <= 1 )
          building_67_house_ceres[128 * i] = 0;
        else
          --building_67_house_ceres[128 * i];
        if ( (signed int)(unsigned char)building_68_house_neptune[128 * i] <= 1 )
          building_68_house_neptune[128 * i] = 0;
        else
          --building_68_house_neptune[128 * i];
        if ( (signed int)(unsigned char)building_69_house_mercury[128 * i] <= 1 )
          building_69_house_mercury[128 * i] = 0;
        else
          --building_69_house_mercury[128 * i];
        if ( (signed int)(unsigned char)building_6a_house_mars[128 * i] <= 1 )
          building_6a_house_mars[128 * i] = 0;
        else
          --building_6a_house_mars[128 * i];
        if ( (signed int)(unsigned char)building_6b_house_venus[128 * i] <= 1 )
          building_6b_house_venus[128 * i] = 0;
        else
          --building_6b_house_venus[128 * i];
      }
    }
  }
}

void fun_determineHouseEvolveText(int buildingId)
{
  int numFoods; // [sp+4Ch] [bp-50h]@6
  signed int i; // [sp+50h] [bp-4Ch]@6
  bool desirabilityMet; // [sp+54h] [bp-48h]@2
  signed int reqWine; // [sp+58h] [bp-44h]@6
  signed int v5; // [sp+58h] [bp-44h]@80
  int reqFoodTypes; // [sp+68h] [bp-34h]@6
  int v7; // [sp+68h] [bp-34h]@80
  signed int reqHealth; // [sp+6Ch] [bp-30h]@6
  signed int v9; // [sp+6Ch] [bp-30h]@80
  signed int reqReligion; // [sp+7Ch] [bp-20h]@6
  signed int v11; // [sp+7Ch] [bp-20h]@80
  signed int reqEducation; // [sp+80h] [bp-1Ch]@6
  signed int v13; // [sp+80h] [bp-1Ch]@80
  signed int reqEntertainment; // [sp+84h] [bp-18h]@6
  signed int v15; // [sp+84h] [bp-18h]@80
  int reqWater; // [sp+88h] [bp-14h]@6
  int v17; // [sp+88h] [bp-14h]@80
  int desirability; // [sp+8Ch] [bp-10h]@1
  signed int v19; // [sp+98h] [bp-4h]@1
  int v20; // [sp+98h] [bp-4h]@80

  v19 = building_0c_level_resourceId[64 * buildingId];
  desirability = building_7a_desirability[128 * buildingId];
  if ( desirability > model_houses_des_devolve[20 * v19] )
    desirabilityMet = desirability >= model_houses_des_evolve[20 * v19];
  else
    desirabilityMet = -1;
  reqWater = model_houses_water[20 * v19];
  reqEntertainment = model_houses_entertainment[20 * v19];
  reqEducation = model_houses_education[20 * v19];
  reqReligion = model_houses_religion[20 * v19];
  reqHealth = model_houses_health[20 * v19];
  reqFoodTypes = model_houses_foodtypes[20 * v19];
  reqWine = model_houses_wine[20 * v19];
  numFoods = 0;
  for ( i = 0; i < 4; ++i )
  {
    if ( *(short*)((char *)&building_4a_grow_value_house_foodstocks[64 * buildingId] + 2 * i) )
      ++numFoods;
  }
  if ( desirabilityMet == -1 )
  {
    _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 0;
    return;
  }
  if ( reqWater == 1 && !building_2d_house_hasFountain[128 * buildingId] && !building_37_house_hasWell[128 * buildingId] )
  {
    _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 1;
    return;
  }
  if ( reqWater == 2 && !building_2d_house_hasFountain[128 * buildingId] )
  {
    _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 2;
    return;
  }
  if ( reqEntertainment <= building_6e_house_entertainment[128 * buildingId] )
  {
    if ( reqFoodTypes > numFoods )
    {
      if ( reqFoodTypes == 1 )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 9;
        return;
      }
      if ( reqFoodTypes == 2 )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 10;
        return;
      }
      if ( reqFoodTypes == 3 )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 11;
        return;
      }
    }
    if ( reqEducation > building_6f_house_education[128 * buildingId] )
    {
      if ( reqEducation == 1 )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 14;
        return;
      }
      if ( reqEducation == 2 )
      {
        if ( _LOBYTE(building_60_house_school_library[64 * buildingId]) )
        {
          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 15;
          return;
        }
        if ( _HIBYTE(building_60_house_school_library[64 * buildingId]) )
        {
          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 16;
          return;
        }
      }
      else
      {
        if ( reqEducation == 3 )
        {
          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 17;
          return;
        }
      }
    }
    if ( model_houses_bathhouse[20 * v19] > (unsigned char)building_65_house_bathhouse_dock_numships_entert_days[128 * buildingId] )
    {
      _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 18;
      return;
    }
    if ( model_houses_pottery[20 * v19] > building_58_house_pottery[64 * buildingId] )
    {
      _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 19;
      return;
    }
    if ( reqReligion > building_71_house_numGods[128 * buildingId] )
    {
      if ( reqReligion == 1 )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 20;
        return;
      }
      if ( reqReligion == 2 )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 21;
        return;
      }
      if ( reqReligion == 3 )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 22;
        return;
      }
    }
    if ( model_houses_barber[20 * v19] > _HIBYTE(building_62_house_academy_barber[64 * buildingId]) )
    {
      _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 23;
      return;
    }
    if ( reqHealth <= building_70_house_health[128 * buildingId] )
    {
      if ( model_houses_oil[20 * v19] > building_54_house_oil[64 * buildingId] )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 27;
        return;
      }
      if ( model_houses_furniture[20 * v19] > building_56_house_furniture[64 * buildingId] )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 28;
        return;
      }
      if ( reqWine > building_52_house_wine[64 * buildingId] )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 29;
        return;
      }
      if ( reqWine > 1 && city_inform[ciid].numWineTypesAvailable[0] < 2 )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 65;
        return;
      }
      if ( v19 >= 19 )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 60;
        return;
      }
      v20 = v19 + 1;
      v17 = model_houses_water[20 * v20];
      v15 = model_houses_entertainment[20 * v20];
      v13 = model_houses_education[20 * v20];
      v11 = model_houses_religion[20 * v20];
      v9 = model_houses_health[20 * v20];
      v7 = model_houses_foodtypes[20 * v20];
      v5 = model_houses_wine[20 * v20];
      if ( desirabilityMet )
      {
        if ( v17 == 1
          && !building_2d_house_hasFountain[128 * buildingId]
          && !building_37_house_hasWell[128 * buildingId] )
        {
          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 31;
          return;
        }
        if ( v17 == 2 && !building_2d_house_hasFountain[128 * buildingId] )
        {
          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 32;
          return;
        }
        if ( v15 <= building_6e_house_entertainment[128 * buildingId] )
        {
          if ( v7 > numFoods )
          {
            if ( v7 == 1 )
            {
              _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 39;
              return;
            }
            if ( v7 == 2 )
            {
              _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 40;
              return;
            }
            if ( v7 == 3 )
            {
              _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 41;
              return;
            }
          }
          if ( v13 > building_6f_house_education[128 * buildingId] )
          {
            if ( v13 == 1 )
            {
              _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 44;
              return;
            }
            if ( v13 == 2 )
            {
              if ( _LOBYTE(building_60_house_school_library[64 * buildingId]) )
              {
                _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 45;
                return;
              }
              if ( _HIBYTE(building_60_house_school_library[64 * buildingId]) )
              {
                _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 46;
                return;
              }
            }
            else
            {
              if ( v13 == 3 )
              {
                _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 47;
                return;
              }
            }
          }
          if ( model_houses_bathhouse[20 * v20] > (unsigned char)building_65_house_bathhouse_dock_numships_entert_days[128 * buildingId] )
          {
            _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 48;
            return;
          }
          if ( model_houses_pottery[20 * v20] > building_58_house_pottery[64 * buildingId] )
          {
            _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 49;
            return;
          }
          if ( v11 <= building_71_house_numGods[128 * buildingId] )
            goto LABEL_154;
          if ( v11 == 1 )
          {
            _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 50;
            return;
          }
          if ( v11 == 2 )
          {
            _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 51;
            return;
          }
          if ( v11 != 3 )
          {
LABEL_154:
            if ( model_houses_barber[20 * v20] <= _HIBYTE(building_62_house_academy_barber[64 * buildingId]) )
            {
              if ( v9 <= building_70_house_health[128 * buildingId] )
              {
                if ( model_houses_oil[20 * v20] <= building_54_house_oil[64 * buildingId] )
                {
                  if ( model_houses_furniture[20 * v20] <= building_56_house_furniture[64 * buildingId] )
                  {
                    if ( v5 <= building_52_house_wine[64 * buildingId] )
                    {
                      if ( v5 <= 1 || city_inform[ciid].numWineTypesAvailable[0] >= 2 )
                      {
                        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 61;
                        if ( _LOBYTE(building_6c_word_94BDAC[64 * buildingId]) == 1 )
                        {
                          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 64;
                        }
                      }
                      else
                      {
                        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 66;
                      }
                    }
                    else
                    {
                      _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 59;
                    }
                  }
                  else
                  {
                    _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 58;
                  }
                }
                else
                {
                  _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 57;
                }
              }
              else
              {
                if ( v9 == 1 )
                {
                  _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 54;
                }
                else
                {
                  if ( building_64_house_clinic[128 * buildingId] )
                    _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 55;
                  else
                    _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 56;
                }
              }
            }
            else
            {
              _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 53;
            }
          }
          else
          {
            _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 52;
          }
        }
        else
        {
          if ( building_6e_house_entertainment[128 * buildingId] )
          {
            if ( v15 >= 10 )
            {
              if ( v15 >= 25 )
              {
                if ( v15 >= 50 )
                {
                  if ( v15 >= 80 )
                    _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 38;
                  else
                    _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 37;
                }
                else
                {
                  _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 36;
                }
              }
              else
              {
                _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 35;
              }
            }
            else
            {
              _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 34;
            }
          }
          else
          {
            _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 33;
          }
        }
      }
      else
      {
        if ( dword_64E378 )
          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 62;
        else
          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 30;
      }
    }
    else
    {
      if ( reqHealth == 1 )
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 24;
      }
      else
      {
        if ( building_64_house_clinic[128 * buildingId] )
          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 25;
        else
          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 26;
      }
    }
  }
  else
  {
    if ( building_6e_house_entertainment[128 * buildingId] )
    {
      if ( reqEntertainment >= 10 )
      {
        if ( reqEntertainment >= 25 )
        {
          if ( reqEntertainment >= 50 )
          {
            if ( reqEntertainment >= 80 )
              _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 8;
            else
              _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 7;
          }
          else
          {
            _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 6;
          }
        }
        else
        {
          _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 5;
        }
      }
      else
      {
        _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 4;
      }
    }
    else
    {
      _HIBYTE(building_72_wharf_hasBoat_house_evolveStatusDesir[64 * buildingId]) = 3;
    }
  }
}
