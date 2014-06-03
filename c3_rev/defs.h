#ifndef __C3_REV_INCLUDE__
#define __C3_REV_INCLUDE__

#define __uint32_t unsigned int

struct C3MapPoint
{
  int x;
  int y;
};

struct C3GraphicOffsetID
{
  int xOffset;
  int graphicId;
};


typedef int BuildingType;

enum BuildingTypeT
{
  B_Menu_Farms     = 2,
  B_Menu_RawMaterials  = 3,
 B_Menu_Workshops  = 4,
 B_Road           = 5,
 B_Wall           = 6,
 B_ReservoirSimon  = 7,
 B_Aqueduct       = 8,
 B_ClearLand      = 9,
 B_HouseVacantLot  = 10,
 B_HouseLargeTent  = 11,
 B_HouseSmallShack  = 12,
 B_HouseLargeShack  = 13,
 B_HouseSmallHovel  = 14,
 B_HouseLargeHovel  = 15,
 B_HouseSmallCasa  = 16,
 B_HouseLargeCasa  = 17,
 B_HouseSmallInsula  = 18,
 B_HouseMediumInsula  = 19,
 B_HouseLargeInsula  = 20,
 B_HouseGrandInsula  = 21,
 B_HouseSmallVilla  = 22,
 B_HouseMediumVilla  = 23,
 B_HouseLargeVilla  = 24,
 B_HouseGrandVilla  = 25,
 B_HouseSmallPalace  = 26,
 B_HouseMediumPalace  = 27,
 B_HouseLargePalace  = 28,
 B_HouseLuxuryPalace  = 29,
 B_Amphitheater   = 30,
 B_Theater        = 31,
 B_Hippodrome     = 32,
 B_Colosseum      = 33,
 B_GladiatorSchool  = 34,
 B_LionHouse      = 35,
 B_ActorColony    = 36,
 B_ChariotMaker   = 37,
 B_Plaza          = 38,
 B_Gardens        = 39,
 B_FortLegionaries  = 40,
 B_SmallStatue    = 41,
 B_MediumStatue   = 42,
 B_LargeStatue    = 43,
 B_FortJavelin    = 44,
 B_FortMounted    = 45,
 B_Doctor         = 46,
 B_Hospital       = 47,
 B_Bathhouse      = 48,
 B_Barber         = 49,
 B_DistributionCenter_Unused  = 50,
 B_School         = 51,
 B_Academy        = 52,
 B_Library        = 53,
 B_FortGround     = 54,
 B_Prefecture     = 55,
 B_TriumphalArch  = 56,
 B_FortGround__   = 57,
 B_Gatehouse      = 58,
 B_Tower          = 59,
 B_SmallTempleCeres  = 60,
 B_SmallTempleNeptune  = 61,
 B_SmallTempleMercury  = 62,
 B_SmallTempleMars  = 63,
 B_SmallTempleVenus  = 64,
 B_LargeTempleCeres  = 65,
 B_LargeTempleNeptune  = 66,
 B_LargeTempleMercury  = 67,
 B_LargeTempleMars  = 68,
 B_LargeTempleVenus  = 69,
 B_Market         = 70,
 B_Granary        = 71,
 B_Warehouse      = 72,
 B_WarehouseSpace  = 73,
 B_Shipyard       = 74,
 B_Dock           = 75,
 B_Wharf          = 76,
 B_GovernorsHouse  = 77,
 B_GovernorsVilla  = 78,
 B_GovernorsPalace  = 79,
 B_MissionPost    = 80,
 B_EngineersPost  = 81,
 B_LowBridge      = 82,
 B_ShipBridge     = 83,
 B_Senate         = 84,
 B_SenateUpgraded  = 85,
 B_Forum          = 86,
 B_ForumUpgraded  = 87,
 B_NativeHut      = 88,
 B_NativeMeeting  = 89,
 B_Reservoir      = 90,
 B_Fountain       = 91,
 B_Well           = 92,
 B_NativeCrops    = 93,
 B_MilitaryAcademy  = 94,
 B_Barracks       = 95,
 B_Menu_SmallTemples  = 96,
 B_Menu_LargeTemples  = 97,
 B_Oracle         = 98,
 B_BurningRuin    = 99,
 B_WheatFarm      = 100,
 B_VegetableFarm  = 101,
 B_FruitFarm      = 102,
 B_OliveFarm      = 103,
 B_VinesFarm      = 104,
 B_PigFarm        = 105,
 B_MarbleQuarry   = 106,
 B_IronMine       = 107,
 B_TimberYard     = 108,
 B_ClayPit        = 109,
 B_WineWorkshop   = 110,
 B_OilWorkshop    = 111,
 B_WeaponsWorkshop  = 112,
 B_FurnitureWorkshop  = 113,
 B_PotteryWorkshop  = 114
};

struct C3CustomButton
{
  unsigned short xStart;
  unsigned short yStart;
  unsigned short xEnd;
  unsigned short yEnd;
  void (*mouseClickHandler)(C3CustomButton*, unsigned int);
  void (*rightClickHandler)(int, unsigned char);
  unsigned char field_10;
  unsigned char field_11;
  unsigned char field_12;
  unsigned int type;
  unsigned int parameter;
  unsigned int parameter2;
};

struct C3ArrowButton
{
  unsigned short xOffset;
  unsigned short yOffset;
  unsigned short graphicId;
  unsigned short size;
  void (*mouseClickHandler)();
  unsigned char field_C;
  unsigned char field_D;
  unsigned char field_E;
  unsigned char field_F;
  unsigned int parameter;
  unsigned int parameter2;
};

struct C3MenuItem
{
  unsigned short yOffset;
  unsigned short textNumber;
  void (*handler)();
  int field_8;
  int parameter;
};

struct C3Menu
{
  unsigned short xOffset;
  unsigned short xEnd;
  unsigned short yOffset;
  unsigned short textGroup;
  C3MenuItem* items;
  unsigned int numItems;
};

struct C3Graphic
{
  unsigned int offset;
  unsigned int data_length;
  unsigned int uncompressed_length;
  unsigned int unused_0C;
  unsigned int invert_offset;
  unsigned short width;
  unsigned short height;
  unsigned int unused_18;
  unsigned short unused_1C;
  unsigned short unknown_1E;
  unsigned short unused_20;
  unsigned short sprite_top_offset;
  unsigned short sprite_left_offset;
  unsigned short animation[300];
  unsigned char type;
  unsigned char compr_flag;
  unsigned char extern_flag;
  unsigned char has_compressed_part;
  unsigned char unknown_36;
  unsigned char building_size;
  unsigned char bitmap_id;
  unsigned char unused_39;
  unsigned char unknown_3A;
  unsigned char unused_3B;
  unsigned char unknown_3C;
  unsigned char unknown_3D;
  unsigned short unused_3E;
};

struct C3ImageButton
{
  unsigned short xOffset;
  unsigned short yOffset;
  unsigned short width;
  unsigned short height;
  unsigned short field_8;
  unsigned short graphicCollectionId;
  unsigned short graphicIdOffsetInCollection;
  unsigned short field_E;
  void (*mouseClickHandler)();
  unsigned int rightClickHandler;
  unsigned char state_enabled;
  unsigned char field_19;
  unsigned char field_1A;
  unsigned char field_1B;
  unsigned int parameter1;
  unsigned int parameter2;
};

#define __TM_GD 100

C3GraphicOffsetID topmenuGraphics640[__TM_GD];
C3GraphicOffsetID topmenuGraphics800[__TM_GD];
C3GraphicOffsetID topmenuGraphics1024[__TM_GD];

const int Direction_Nort = 0;
const int Direction_East = 2;
const int Direction_South = 4;
const int Direction_West = 6;

const int Emp_City = 2;
const int Emp_UnusedValue2 = 3;
const int Emp_BattleIcon = 4;
const int Emp_LandTradeRoute =5;
const int Emp_SeaTradeRoute = 6;
const int Emp_DistantBattleRomanArmy = 7;
const int Emp_DistantBattleEnemyArmy = 8;



typedef unsigned int GoodType;

const GoodType G_Wheat = 1;
const GoodType G_Vegetables = 2;
const GoodType G_Fruit = 3;
const GoodType G_Olives = 4;
const GoodType G_Vines = 5;
const GoodType G_MeatFish = 6;
const GoodType G_Clay = 0xb;
const GoodType G_Marble = 0xc;
const GoodType G_Timber = 0xA;
const GoodType G_Wine = 0x7;
const GoodType G_Oil = 0x8;
const GoodType G_Weapons = 0xD;
const GoodType G_Furniture = 0xe;
const GoodType G_Pottery = 0xf;
const GoodType G_Iron  = 0x9;

const int Overlay_None = 0;
const int W_MainMenu = 0;
const int W_SavedGame_LoadSaveDeleteDialog = 0xD;
const int W_DisplayOptionsDialog = 0xA;
const int W_SoundOptionsDialog = 0xB;
const int W_SpeedOptionsDialog = 0xC;
const int W_NumericInputDialog = 0xE;
const int W_SelectListDialog = 0xF;
const int W_Advisors = 0x7;
const int W_OverlaySelectionMenu = 0x8;
const int W_EmpireMap = 0x14;
const int W_EditorEditStartDate = 0x16;
const int W_EditorRequests = 0x17;
const int W_EditorEditRequest = 0x18;
const int W_EditorInvasions = 0x19;
const int W_EditorEditInvasion = 0x1A;
const int W_EditorAllowedBuildings = 0x1B;
const int W_EditorWinCriteria = 0x1C;
const int W_EditorSpecialEvents = 0x1D;
const int W_EditorDemandChanges = 0x1f;
const int W_EditorEditDemandChange = 0x20;
const int W_EditorPriceChanges = 0x21;
const int W_EditorEditPriceChange = 0x22;
const int W_EditorStartingConditions = 0x25;
const int W_ResourceSettingsDialog = 0x28;
const int W_TradePricesDialog = 0x29;
const int W_LaborPriorityDialog = 0x2A;
const int W_HoldFestivalDialog = 0x2B;
const int W_SetSalaryDialog = 0x2C;
const int W_DonateToCityDialog = 0x2D;
const int W_SendGiftDialog = 0x2e;
const int W_MissionBriefing = 0x35;
const int W_ResolutionConfirmDialog = 0x3E;
const int W_DifficultyOptionsDialog = 0x45;

enum GameTextGroup
{
  GAMETEXT_DIALOGS=5,
  GAMETEXT_LOADING=0xB
};

enum OverlayType
{
  Overlay_Fire=8,
  Overlay_Damage=9,
  Overlay_Crime=0xA,
  Overlay_Problems=0x1D,
  Overlay_Desirability=0x1A,
  Overlay_Entertainment=11,
  Overlay_Education=16,
  Overlay_Theater=12,
  Overlay_FoodStocks=25,
  Overlay_Religion=4,
  Overlay_Native =0x1C,
  Overlay_Water=0x2,
  Overlay_Barber=20,
  Overlay_School=17,
  Overlay_Library=18,
  Overlay_Bathhouse=21,
  Overlay_TaxIncome=24,
  Overlay_Amphiheater=13,
  Overlay_Academy  = 19,
  Overlay_Hospital  = 23,
  Overlay_Colosseum=14,
  Overlay_Hippodrome=15,
  Overlay_Clinic=22,
  Overlay_Workers  = 27
};

enum AdvisorType {
  Adv_Labor=1,
  Adv_Military=2,
  Adv_Imperial=3,
  Adv_Ratings=4,
  Adv_Trade=5,
  Adv_Population=6,
  Adv_Health=7,
  Adv_Education=8,
  Adv_Entertainment=9,
  Adv_Religion=0xA,
  Adv_Financial=0xB,
  Adv_Chief=0xC
};

enum FontType
{
  F_NormalBlack=0x86,
  F_LargeBlack=0x29e,
  F_SmallBrown=0x430

};

enum TradeStatusType {
  Trade_None=0,
  Trade_Import=1,
  Trade_Export=2
};

enum TerrainType {
  T_Tree           = 1,
 T_Rock           = 2,
 T_Water          = 4,
 T_Building       = 8,
 T_Scrub          = 0x10,
 T_Garden         = 0x20,
 T_Road           = 0x40,
 T_ReservoirRange  = 0x80,
 T_Aqueduct       = 0x100,
 T_Elevation      = 0x200,
 T_AccessRamp     = 0x400,
 T_Meadow         = 0x800,
 T_FountainRange  = 0x2000,
 T_Wall           = 0x4000,
 T_Gatehouse      = 0x8000

};

enum WalkerType
{
Walker_Immigrant  = 1,
Walker_Emigrant  = 2,
Walker_Homeless  = 3,
Walker_CartPusher  = 4,
Walker_LaborSeeker  = 5,
Walker_Explosion  = 6,
Walker_TaxCollector  = 7,
Walker_Engineer  = 8,
Walker_Warehouseman  = 9,
Walker_Prefect   = 10,
Walker_FortJavelin  = 11,
Walker_FortMounted  = 12,
Walker_FortLegionary  = 13,
Walker_FortStandard  = 14,
Walker_Actor     = 15,
Walker_Gladiator  = 16,
Walker_LionTamer  = 17,
Walker_Charioteer  = 18,
Walker_TradeCaravan  = 19,
Walker_TradeShip  = 20,
Walker_TradeCaravanDonkey  = 21,
Walker_Protestor  = 22,
Walker_Criminal  = 23,
Walker_Rioter    = 24,
Walker_FishingBoat  = 25,
Walker_MarketTrader  = 26,
Walker_Priest    = 27,
Walker_SchoolChild  = 28,
Walker_Teacher   = 29,
Walker_Librarian  = 30,
Walker_Barber    = 31,
Walker_BathhouseWorker  = 32,
Walker_Doctor    = 33,
Walker_Surgeon   = 34,
Walker_MapFlag   = 36,
Walker_Flotsam   = 37,
Walker_Dockman   = 38,
Walker_MarketBuyer  = 39,
Walker_Patrician  = 40,
Walker_IndigenousNative  = 41,
Walker_TowerSentry  = 42,
Walker_Enemy43   = 43,
Walker_Enemy44   = 44,
Walker_Enemy45   = 45,
Walker_Enemy46   = 46,
Walker_Enemy47   = 47,
Walker_Enemy48   = 48,
Walker_Enemy49   = 49,
Walker_Enemy50   = 50,
Walker_Enemy51   = 51,
Walker_Enemy52   = 52,
Walker_Enemy53   = 53,
Walker_Enemy54   = 54,
Walker_Enemy55   = 55,
Walker_Enemy56   = 56,
Walker_Enemy57   = 57,
Walker_NativeTrader  = 58,
Walker_Arrow     = 59,
Walker_Javelin   = 60,
Walker_Bolt      = 61,
Walker_Ballista  = 62,
Walker_Creature  = 63,
Walker_Missionary  = 64,
Walker_FishGulls  = 65,
Walker_DeliveryBoy  = 66,
Walker_Shipwreck  = 67,
Walker_Sheep     = 68,
Walker_Wolf      = 69,
Walker_Zebra     = 70,
Walker_Spear     = 71,
Walker_HippodromeMiniHorses  = 72
}


#endif


