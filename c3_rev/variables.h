#ifndef __VARIABLES_INCLUDE__
#define __VARIABLES_INCLUDE__

#include "defs.h"

#define __TD_TR 100
#define C3ENG_INDEX_LENGTH 8000
#define C3ENG_HEADER_SIZE 28
#define C3MM_INDEX_SIZE 32024

#define ANTBCODE

static char building_64_house_clinic[__TD_TR]; // weak
static short building_5a_house_theater_amphi[__TD_TR];
static short building_5c_house_amphiGlad_colo[__TD_TR];
static short building_5e_house_coloLion_hippo[__TD_TR];
static short building_60_house_school_library[__TD_TR];

static char building_01_ciid[__TD_TR]; // weak
static char building_02_byte_always0[__TD_TR]; // weak
static char building_03_size[__TD_TR]; // weak
static char building_04_house_isMerged[__TD_TR]; // weak

static unsigned char building__07_y[__TD_TR];
static short building_08_gridOffset[__TD_TR]; // weak

static char building_0e_byte_94BD4E[__TD_TR]; // weak
static short building_10_placedSequenceNumber[__TD_TR]; // weak

static short building_14_word_94BD54[__TD_TR]; // weak

static short building_18_house_roomForPeople[__TD_TR]; // weak
static short building_1a_word_94BD5A[__TD_TR]; // weak
static short building_1c_house_maxPopEver[__TD_TR]; // weak
static short building_0e_word_94BD5E[__TD_TR]; // weak
static char building_20_byte_94BD60[__TD_TR]; // weak
static char building_21_byte_94BD61[__TD_TR]; // weak
static short building_22_walkerId[__TD_TR]; // weak
static short building_24_laborSeekerId[__TD_TR]; // weak
static short building_26_immigrantId[__TD_TR]; // weak
static short building_28_towerBallistaId[__TD_TR]; // weak
static char building_2a_walkerSpawnDelay[__TD_TR]; // weak
static char building_2c_byte_94BD6C[__TD_TR]; // weak
static char building_2d_house_hasFountain[__TD_TR]; // weak
static char building_2e_byte_94BD6E[__TD_TR]; // weak
static short building_30_warehouse_prevStorage[__TD_TR]; // weak
static short building_32_warehouse_nextStorage[__TD_TR]; // weak
static short building_34_industry_unitsStored[__TD_TR]; // weak
static char building_37_house_hasWell[__TD_TR]; // weak
static short building_38_num_workers[__TD_TR]; // weak

static char building_3b_industry_outputGood[__TD_TR]; // weak
static char building_3c_hasRoadAccess[__TD_TR]; // weak
static char building_3d_byte_94BD7D[__TD_TR]; // weak
static short building_3e_damageRisk[__TD_TR]; // weak
static short building_40_fireRisk[__TD_TR]; // weak
static short building_42_word_94BD82[__TD_TR]; // weak
static char building_44_byte_94BD84[__TD_TR]; // weak
static char building_45_byte_94BD85[__TD_TR]; // weak
static char building_46_house_taxcollector[__TD_TR]; // weak
static short building_48_word_94BD88[__TD_TR]; // weak

static short building_4c_granary_capacity[__TD_TR]; // weak
static short building_4e_word_94BD8E[__TD_TR]; // weak
static short building_50_word_94BD90[__TD_TR]; // weak
static short building_52_house_wine[__TD_TR]; // weak
static short building_54_house_oil[__TD_TR]; // weak
static short building_56_house_furniture[__TD_TR]; // weak
static short building_58_house_pottery[__TD_TR]; // weak
static short building_62_house_academy_barber[__TD_TR]; // weak

static int setting_map_camera_x; // weak
static int setting_map_camera_y; // weak

static int dword_8C79EC; // idb
static int setting_map_x; // idb
static int setting_map_y; // idb
static int setting_map_width; // weak
static int setting_map_height; // weak
static int setting_map_startGridOffset; // weak
static int setting_map_gridBorderSize; // weak
static int setting_map_orientation; // idb

static int ds_dword_995BE0[0xff]; // weak
static int ds_dword_995BE4[0xff]; // weak
static int ds_dword_995BE8[0xff]; // weak
static int ds_dword_995BEC[0xff]; // weak
static int ds_dword_995BF0[0xff]; // idb
static int ds_dword_995C48[0xff]; // weak
static int dword_995C50[10771]; // weak
static int dword_9A049C; // weak
static int dword_9A04A0; // weak
static int dword_9A04A4; // weak
static int dword_9A04A8; // weak
static int dword_9A04AC; // weak
static int iso_tile_height; // weak
static int iso_tile_half_height; // weak
static int dword_9A04B8; // weak
static int dword_9A04BC; // weak
static int iso_viewport_width; // weak
static int cityviewport_yEnd; // weak
static int iso_tile_width; // weak
static int iso_viewport_height; // weak
static int iso_yoffset; // weak
static int dword_9A04D4; // weak
static int dword_9A04D8; // weak
static int iso_tile_half_width; // weak
static int dword_9A04E0; // weak
static int cityviewport_x; // weak
static int dword_9A04E8; // weak
static int dword_9A04EC; // weak
static int dword_9A04F0; // weak
static int minimap_left; // weak
static int minimap_right; // weak
static int minimap_top; // weak
static int minimap_bottom; // weak
static int iso_xoffset; // weak
static int iso_grid_height; // weak
static int dword_9A050C; // weak
static int dword_9A0510; // weak
static int dword_9A0514; // weak
static int iso_grid_width; // weak
static int dword_9A051C; // weak
static int dword_9A0524; // weak
static int cityviewport_xEnd; // weak
static int dword_9A052C; // weak
static int dword_9A0530; // weak
static int dword_9A0534; // weak
static int cityviewport_y; // weak
static int dword_9A053C; // weak
static int dword_9A0540[8]; // weak
static int dword_9A0560[0xff]; // idb
static int dword_9D4B44; // weak
static int dword_9D4B48; // weak
static int dword_9D4B4C; // weak
static int dword_9D4B50; // weak
static int dword_9D4B54; // weak
static int dword_9D4B60[0xff]; // weak
static int dword_9D4B64[0xff]; // weak
static int dword_9D4B68[0xff]; // idb
static int dword_9D4DE4; // weak
static int dword_9D4DE8; // weak
static int dword_9D4DEC; // weak
static int dword_9D4DF0; // weak
static short drawGraphic_colorMask; // idb

static int help_isUrgent[0xff]; // weak
static int help_videoLink[0xff]; // weak
static int help_title[0xff]; // weak
static int help_subtitle[0xff]; // weak
static int help_content[0xff]; // idb
static int dword_6E6AF8; // weak
static int drawGraphic_xOffset; // weak
static int inputtext_lastUsed; // weak
static int drawGraphic_height; // idb
static int drawGraphic_yOffset; // weak
static char drawtext_alwaysZero; // weak
static int draw_setToZero; // weak
static int textmultiline_longestLineLength; // weak
static int drawtext_useSingleShadow; // weak
static int randomcounter_16; // weak
static int randomcounter_24; // weak
static int randomcounter_32; // weak
static int drawGraphic_visiblePixelsClipY; // idb
static int randomcounter_64; // weak
static int draw_clip_xEnd; // weak
static char byte_6E6B50; // weak
static char byte_6E6B51; // weak
static char byte_6E6B52; // weak
static char byte_6E6B53; // weak
static int graphicPrepare_offset; // weak
static char climate_sg2[32]; // weak
static int climate_dataSize[12]; // weak

static int dword_660B90; // weak
static char byte_660B94[__TD_TR]; // weak
static int findfiles_numFiles; // weak
static char byte_660BFC[72]; // weak
static int dword_660C44; // weak
static int shouldScrollMap; // weak
static int button_y; // weak
static int button_x; // weak
static int ddraw_width; // idb
static char window_redrawRequest; // weak
static int font_currentLineHeight[65819]; // weak
static char byte_6A10F8; // weak
static int dword_6A10FC; // weak
static int dword_6A1108; // weak
static char input_allowPunctuation[0xff]; // weak
static int dword_6A1138; // idb
static int dword_6A1148; // idb
static int dword_6A1158; // idb
static struct C3Graphic enemy_sg2[0xff];
static int dword_6AD9C0; // idb
static int drawGraphic_invisibleWidthClipLeft; // weak
static int input_cursorOffset; // weak
static int drawtext_input_cursor_width; // idb
static int dword_6AD9D0; // weak
static int drawline_something; // weak
static int random_7f_2; // weak
static int unused_random_7_2; // weak
static int unused_random_7_1; // weak
static int random_7f_1; // weak
static C3EngHeader c3eng_header;
static int dword_6ADA38; // weak
static int dword_6ADA40; // weak
static int dword_6ADA44; // weak
static int selectAnts_color; // weak
static int drawGraphic_fileOffset; // weak
static int selectAnts_index; // weak
static int random_iv2; // weak
static int input_cursorPosition[0xff]; // idb
static unsigned char drawGraphic_clipYCode;
static int random_iv1; // weak
static int dword_6ADB50; // weak
static int drawGraphic_visibleWidthClipRight; // weak
static int fileDialogSave_from_window; // weak
static int drawtext_useDoubleShadow; // weak
static int distance_y; // weak
static int walkerGridX; // idb
static int walkerGridY; // idb
static int draw_clip_y; // weak
static int drawGraphic_invisibleHeightClipTop; // weak
static int dword_6ADBEC; // weak
static int drawGraphic_copyBytesInBufferForClipX; // weak
static int drawGraphic_visiblePixelsClipX; // weak
static int input_font[0xff]; // idb
static int input_textboxwidth[0xff]; // idb
static int fullscreenImage_width; // weak
static char confirmdialog_okClicked; // weak
static int draw_centeredTextOffset; // weak
static int graphic_xOffset; // idb
static int graphic_yOffset; // idb
static int selectAntsLong_color; // weak
static char byte_6ADD0C; // weak
static int currentButton_parameter;
static int currentButton_parameter2; // idb
static int dword_6ADD18; // weak
static int distance_x; // weak
static int dword_6ADD24; // weak
static char drawGraphic_type; // weak

static int drawtext_hashCharAndWrap; // weak
static char drawWinLoseButtons_isSelected; // weak
static short drawtext_color; // weak
static int drawtext_wrap; // weak
static int fullscreenImage_xOffset; // weak
static int dword_788048; // weak
static char drawtext_hashSomething[0xff]; // idb
static int drawGraphic_width; // idb
static char multiline_something_flag; // weak
static char* c3mm_data; // weak
static struct C3EngIndexEntry c3eng_index[C3ENG_INDEX_LENGTH];
static int randomcounter_4; // weak
static int randomcounter_8; // weak
static char *gametext_result;
static int input_length[0xff]; // idb
static char input_isInsert; // weak
static char byte_7E1EF9; // weak
static int randomcounter_128; // weak
static signed int graphic_currentGraphicId; // idb
static int randomcounter_256; // weak
static char str[0xff]; // idb
static int dword_7E26F4; // weak
static int draw_clip_yEnd; // weak
static int fullscreenImage_yOffset; // weak
static int randomcounter_384; // weak
static int font_wordWidth; // weak
static int dword_7E270C; // weak
static int dword_7E2710; // weak
static int dword_7E2714; // weak
static int dword_7E2718; // weak
static int dword_7E271C; // weak
static int drawGraphic_skipBytesInBufferForClipX; // weak
static int text_xoffset; // weak
static int dword_7E2728; // weak
static int dword_7E272C; // weak
static int dword_7E2730; // weak
static int fullscreenImage_height; // weak
static int input_maxlength[0xff]; // idb
static int dword_7E2794; // weak
static int random_poolIndex; // weak
static int randomcounter_768; // weak
static int input_accepted; // weak
static int text_yoffset; // weak
static unsigned char drawGraphic_clipXCode;
static LPCSTR c3eng_textstring_forMessagebox; // idb

static int dword_607F34[0xff]; // weak
static int dword_607F38[0xff]; // idb
static int dword_607F5C; // idb
static int dword_607F60; // idb
static int dword_607F64; // weak
static volatile LONG Target; // idb
static int empire_selectedCity; // weak
static int trade_selectedCity; // weak
static int debug_houseEvolution; // weak
static int debug_roadEvolution; // weak
static int debug_showPeople; // weak
static int dword_607F8C; // weak
static int dword_607F94; // weak
static int dword_607F98; // weak
static int dword_607F9C; // weak
static int dword_607FA0; // weak
static int dword_607FA4; // weak
static int debugInfo_numFigures; // idb
static int walkerId; // idb
static int dword_607FC8; // weak
static int helpDialog_showVideo; // weak
static LPCVOID dword_607FD4; // idb
static int dword_607FD8; // weak
static char* c3_555;
static int selectAntsLong_index; // weak
static int videotimer_last; // weak
static int dword_607FEC; // weak
static int graphics_format_id; // weak
static int dword_607FF8; // weak

static int ddraw_busy; // weak
static int paintVideo_last; // weak
static int time_current; // weak
static short *ddraw_buffer;
static short *screen_buffer;
static int key_ctrl_down; // weak
static int key_shift_down; // weak
static int arrowkey_right_pressed; // weak
static int arrowkey_left_pressed; // weak
static int arrowkey_down_pressed; // weak
static int arrowkey_up_pressed; // weak
static int c3maps_width; // weak
static int c3maps_height; // weak
static size_t c3maps_imageDataSize; // idb
static int savedgame_version; // weak
static int lastGameTickTime; // weak
static int shouldScrollMap_lastUpdateTime; // weak
static int currentOverlay; // idb
static int previousOverlay; // weak
static int dword_608078; // weak
static int dword_608080; // weak
static int dword_608084; // weak
static int dword_608094; // idb
static int dword_608098; // idb
static int debug_drawNetworkInfo; // weak
static int debug_showHappinessProsperityInfo; // weak
static int sidepanel_collapsed; // weak
static int sidepanel_slide_progress[21]; // weak
static int dword_608100; // weak
static int input_cursorLastChange; // weak
static int input_showCursor[7]; // weak
static int smack_currentVideo; // weak
static int message_useVideo; // weak
static int hasLiteralEncoding; // idb
static int dword_608140; // weak
static int dword_608148; // weak
static int dword_60814C; // weak
static int dword_6081F0; // weak

static int draw_clip_x; // weak
static int random_pool[0xff]; // idb
static char* c3eng_data;
static char byte_6ADEFE[0xff]; // weak
static char byte_6ADEFF[199969]; // weak
static int drawtext_forceColor; // weak
static int drawGraphic_graphicId; // weak
static int input_atEnd; // weak
static int random_7fff_1; // weak
static int unused_random_7fff_2; // weak
static char textmultiline_line[0xff]; // idb
static int drawline_xlen; // weak
static int drawline_ylen; // weak
static int drawline_xmax; // weak
static int drawline_ymax; // weak
static int drawGraphic_visibleHeightClipBottom; // weak
static int drawline_xdir; // weak
static int drawline_ydir; // weak
static int drawline_xmin; // weak
static int drawline_ymin; // weak
static int drawtext_input_position; // weak
static int drawtext_input_cursorSeen; // weak
static char *input_text[0xff];
static char wndproc_closeStatus; // weak
static char c3mm_index[C3MM_INDEX_SIZE]; // weak

static char building_65_house_bathhouse_dock_numships_entert_days[__TD_TR]; // weak
static char building_66_house_hospital_entert_days2[__TD_TR]; // weak
static char building_67_house_ceres[__TD_TR]; // weak
static char building_68_house_neptune[__TD_TR]; // weak
static char building_69_house_mercury[__TD_TR]; // weak
static char building_6a_house_mars[__TD_TR]; // weak
static char building_6b_house_venus[__TD_TR]; // weak

static char building_6e_house_entertainment[__TD_TR]; // weak
static char building_6f_house_education[__TD_TR]; // weak
static char building_70_house_health[__TD_TR]; // weak
static char building_71_house_numGods[__TD_TR]; // weak
static short building_72_wharf_hasBoat_house_evolveStatusDesir[__TD_TR]; // weak
static int building_74_house_taxIncomeThisYear_senateForum_treasureStore[__TD_TR]; // weak

static char building_79_byte_94BDB9[__TD_TR]; // weak
static char building_7a_desirability[__TD_TR]; // weak
static char building_7b_byte_94BDBB[__TD_TR]; // weak
static char building_7c_adjacentToWater[__TD_TR]; // weak
static char building_7f_byte_94BDBF[__TD_TR]; // idb

static char current_fileExtension[32];

static char getextension_result_1; // weak
static char getextension_result_2; // weak
static char getextension_result_3; // weak

static int ciid;
static char intToString_result[__TD_TR];

static char chdir_extension_value[40];
static char setting_cddrive;

static char chdir_home[__TD_TR];
static char getcwd_result[__TD_TR];
static int log_statusTxtExists;
static char log_message[256];
static char log_currentTime_date[40];

static char a0[2]; // weak
static char a9[2]; // weak
static char a8[2]; // weak
static char a7[2]; // weak
static char a6[2]; // weak
static char a5[2]; // weak
static char a4[2]; // weak
static char a3[2]; // weak
static char a2[2]; // weak
static char a2_0[2]; // weak
static char a1[2]; // weak

static int dword_606A14; // weak
static int dword_606AA8; // weak
static int dword_606AAC; // weak
static int dword_606AB0; // weak
static int dword_606AB8; // weak
static int dword_606ABC; // weak
static int dword_606AC0; // weak
static char *off_606D60; // weak
static int dword_606A18;
static int off_606D64[40];
static int dword_606D98[20];
static int dword_606D68[40];
static int dword_606D9C[40];

static short graphic_id_start[0xff]; // weak
static short graphic_terrain_start; // weak
static short graphic_terrain_trees; // weak
static short graphic_terrain_uglygrass; // weak
static short graphic_terrain_shrub; // weak
static short graphic_terrain_water; // weak
static short graphic_terrain_quake; // weak
static short graphic_terrain_grass2; // weak
static short graphic_terrain_rock; // weak
static short graphic_terrain_cliff; // weak
static short graphic_terrain_grass1; // weak
static short graphic_buttons_start; // weak
static short graphic_sidepanel; // weak
static short graphic_mainMenuBg; // weak
static short graphic_buttons; // weak
static short graphic_font; // weak
static short graphic_tower; // weak
static short graphic_aqueduct; // weak
static short graphic_overlaytiles; // weak
static short graphic_fire_almost; // weak
static short graphic_market; // weak
static short graphic_well; // weak
static short graphic_wall_top; // weak
static short graphic_reservoir; // weak
static short graphic_housing_tent; // weak
static short graphic_housing_shack; // weak
static short graphic_housing_hovel; // weak
static short graphic_housing_casa; // weak
static short graphic_housing_insula; // weak
static short graphic_housing_insula2; // weak
static short graphic_housing_villa; // weak
static short graphic_housing_villa2; // weak
static short graphic_housing_palace; // weak
static short graphic_housing_palace2; // weak
static short graphic_housing_vacant; // weak
static short graphic_farm; // weak
static short graphic_quarry; // weak
static short graphic_ironMine; // weak
static short graphic_clayPit; // weak

static int dword_7E2C08; // weak

static int buildmenu_xOffsets[0xff]; // weak
static int buildmenu_yOffsets[0xff]; // idb

static int xOffset; // idb
static int yOffset; // idb
static int font_currentFontId; // idb

static short help_type[0xff]; // weak
static short help_messageType[0xff]; // weak
static short help_x[0xff]; // weak
static short help_y[0xff]; // weak
static short help_width[0xff]; // weak
static short help_height[0xff]; // weak
static short help_picture1_id[0xff]; // weak
static short help_picture1_x[0xff]; // weak
static short help_picture1_y[0xff]; // weak
static short help_picture2_id[0xff]; // weak
static short help_picture2_x[0xff]; // weak
static short help_picture2_y[0xff]; // weak
static short help_title_x[0xff]; // weak
static short help_title_y[0xff]; // weak
static short help_subtitle_x[0xff]; // weak
static short help_subtitle_y[0xff]; // weak
static short help_unknown20[0xff]; // weak
static short help_unknown21[0xff]; // weak

static int currentHelpId; // weak
static int message_month; // idb

static char fileDialogSave_filename[180]; // idb
static HCURSOR hCursor; // idb
static int screen_640x480_x; // idb
static int ddraw_scanline; // weak
static HCURSOR cursor_normal;
static int screen_width; // idb
static int system_screen_height; // weak
static char byte_6606BC; // weak
static HCURSOR cursor_sword;
static char video_refreshRequested; // weak
static int windowedMode_yOffset; // weak
static char byte_660728; // weak
static int windowedMode_xOffset; // weak
static char winState; // weak
static char byte_660731[81]; // weak
static short mouseInfo_x; // weak
static short mouseInfo_y; // weak
static char mouseInfo_type; // weak
static char byte_660787; // weak
static HCURSOR cursor_shovel;
static char byte_66078C; // weak
static char mouseInfo_buttonIsDown; // weak

static int dword_7E2838[107]; // weak
static int dword_7E29E4; // weak
static int dword_7E29E8; // weak
static char byte_7E2A00[500]; // idb

static char byte_660795; // weak
static int dword_660798; // weak
static int screen_640x480_y; // idb
static char is_right_click; // weak
static int speed_time_overall; // weak
static int dword_6608AC[13]; // weak
static int graphics_format_hex; // weak
static int dword_6608E8; // weak
static int dword_6608EC; // weak
static int ddraw_height; // idb
static unsigned char window_id;
static char byte_6608FD; // weak
static char byte_660904; // weak
static char imagebuttons_redraw; // weak
static char drawGraphic_555file[96]; // idb
static int dword_660B78; // weak
static int dword_660B7C; // weak
static int byte_605A68[0xff]; // idb

static short pk_literalSequences[255]; // weak
static short pk_literalSequencesEnd; // weak

static int dictionarySize; // idb
static char pk_implode_copyOffsetBits[0xff]; // idb
static char pk_implode_copyOffsetCode[0xff]; // idb
static char pk_implode_copyLengthBaseBits[0xff]; // idb
static char pk_implode_copyLengthExtraBits[0xff]; // idb
static char pk_implode_copyLengthBaseCode[0xff]; // idb
static char pk_literalLengths[0xff]; // idb

static __int16 pk_literalValues[256];
static char aPkwareDataComp[193]; // weak
static char pk_copyOffsetBits[0xff]; // idb
static char pk_copyOffsetCode[64]; // weak
static char pk_copyLengthExtraBits[0xff]; // idb
static unsigned __int16 pk_copyLengthBaseValue[0xff];
static char pk_copyLengthBaseBits[0xff]; // idb
static char pk_copyLengthBaseCode[16]; // weak
static char pk_literalTable[0xff]; // idb

static int dword_64E378;
static WindowsBitmap *c3maps_bitmap;

struct LaborCategoryPriority
{
  int priority;
  int numWorkersNeeded;
  int workersAllocated;
  int relatedToEmployeeAccess;
  int numBuildings;
};

struct CityInfo
{
  unsigned int population;
  unsigned int births;
  unsigned int maxPopulationEver;
  char hasWonScenario;
  int victoryContinueMonths;
  int salary_rank;
  int salary_amount;
  int tradeCenterId;
  int shipyardBoatsRequested;
  int wages;
  int taxrate;
  int plannedFestival_size;
  int treasury;
  int cultureRating;
  int plannedFestival_god;
  int festivalGod;
  int festivalSize;
  int plannedFestival_monthsToGo;
  int smallFestivalCost;
  int finance_sundries_thisyear;
  int largeFestivalCost;
  int grandFestivalCost;
  int grandFestivalWine;
  int grandFestivalWineUnavailable;
  int ratingAdvisorSelectedRating;
  int cultureRatingExplanation;
  int personalSavings;
  int giftToCaesarSize;
  int lavishGiftCost;
  int generousGiftCost;
  int finance_donated_thisyear;
  int modestGiftCost;
  int missionSavedGameWritten;
  int imperialArmyComing;
  int immigrationPercentageBasedOnHappiness;
  int donateAmount;
  int newcomersThisMonth;
  int enemyDestroyedBuildings;
  int prosperityRatingExplanation;
  int happiness_immigrationDuration;
  int happiness_immigrationAmount;
  int happiness_emigrationDuration;
  int happiness_emigrationValue;
  int peaceRating;
  int peaceRatingExplanation;
  int favorRatingExplanation;
  int favorRating;
  int unemploymentForSenateDrawing;
  int unemploymentPercentage;
  int player_rank;
  int crimeRate;
  int numWineTypesAvailable[10];
  int dword_654220;
  int dword_6543C8;
  int dword_654624;
  int dword_654520;
  int dword_65429C;
  int dword_65451C;
  int dword_6543B8;
  int dword_6543BC;
  int dword_6544F0;
  int finance_exports_thisyear;
  int debtState;
  int monthsSinceStart[100];
  int healthRate;
  int hapinessCity;
  int prosperityRating;
  int resourceStockpile[20];
  int monthsInDebt;
  int dword_654568;
  int numInvasionsFromCaesar;
  int dword_6545B4;
  int dword_6545B0;
  int dword_654570;
  int dword_65456C;
  int dword_654574;
  int ignoredRequestPoints;
  int salaryDifferenceForFavor;
  int tributeNotPaidYears;
  int peopleInVillasAndPalaces;
  int tributeNotPaid;
  int peopleInLargeInsulaAndAbove;
  int wagesRome;
  int maxProsperity[10];
  int peopleInTentsAndShacks;
  int peopleInTents;
  int monthsSinceFestival;
  int dword_654640;
  int dword_654644;
  int dword_654514;
  int evolveFountainRequired;
  int evolveWaterRequired;
  int serviceSchoolRequired;
  int serviceLibraryRequired;
  int educationDemand;
  int evolveReligionRequired;
  int religionDemand;
  int serviceReligionRequired;
  int entertainmentDemand;
  int evolveEntertainmentRequired;
  int evolveMoreReligionRequired;
  int evolveMoreEntertainmentRequired;
  int evolveMoreEducationRequired;
  int serviceBathhouseRequired;
  int evolveEducationRequired;
  int evolveBathhouseRequired;
  int evolveClinicRequired;
  int healthDemand;
  int religion_needsMet;
  int evolveSecondWineRequired;
  int serviceBarberRequired;
  int evolveEvenMoreReligionRequired;
  int evolveRequired_unknown1;
  int evolveRequired_unknown2;
  int evolveRequired_unknown3;
  int evolveRequired_unknown4;
  int evolveRequired_unknown5;
  int evolveBarberRequired;
  int serviceClinicRequired;
  int evolveFoodsRequired;
  int evolveHospitalRequired;
  int entertainment_needsMet;
  int numWorkersNeeded;
  int theaterShows;
  int education_needsMet;
  int health_needsMet;
  int pctReligionCoverage;
  int holdFestival_65444C;
  int holdFestival_654450;
  int amphitheaterShows;
  int theatersWithoutShowsWeighted;
  int entertainmentNeedingShowsMost;
  int amphitheatersWithoutShowsWeighted;
  int colosseumShows;
  int colosseumsWithoutShowsWeighted;
  int hippodromeShows;
  int hippodromeWithoutShowsWeighted;
  int totalFoodStoredInGranaries;
  int foodtypesEaten;
  int foodTypesEaten;
  int stockpile_wine;
  int stockpile_oil;
  int stockpile_furniture;
  int stockpile_pottery;
  int foodSupplyMonths;
  int granaries_3orMoreWorkers;
  int percentagePlebs;
  int numPeopleOfWorkingAgeMen;
  int numAvailableWorkers;
  int numEmployedWorkers;
  int numUnemployedWorkers;
  int wageTotal_lastyear;
  int unitsOfFoodNeededPerMonth;
  int granaries_understaffed;
  int granaries_lessThan3Workers;
  int granaries_lessThan3WorkersWithFoodStored;
  int continueGoverningSomething;
  int wageTotal_thisyear;
  int finance_tribute_thisyear;
  int finance_tribute_lastyear;
  int numHospitalWorkers;
  int dword_6500C0_always3or0;
  int previousPopulation;
  int schoolAgeChildren;
  int academyAgeChildren;
  int populationRoomForMax;
  int populationCanImmigrate;
  int populationPerMonthGraph[2400];
  int populationPerMonthGraph_nextIndex;
  int population_totalYears;
  int populationYearlyBirthsDeathsCalculationNeeded;
  int lastBirthHouseBuildingId;
  short lastHousePeopleRemovedFrom;
  int numImmigrantsThisMonth;
  int numEmigrantsThisMonth;
  int wantedImmigrantsNoHousing;
  int emigrationQueueSize;
  int immigrationQueueSize;
  int x_cityinfo_dword_652798;
  int lastMigrationSize;
  int population_averagePopPerYear;
  int population_totalPopPerYear;
  int entertainmentMessage_hippodromeShown;
  int entertainmentMessage_colosseumShown;
  int emigrationMessageShown;
  int dword_654634;
  int gameWinVideoShown;
  int monthsSinceFestivalCeres[5];
  int monthsSinceFestivalNeptune[5];
  int monthsSinceFestivalMercury[5];
  int monthsSinceFestivalMars[5];
  int monthsSinceFestivalVenus[5];
  int religionAngryGod;
  int immigrationState;
  int finance_wages_sofar_thisyear;
  int finance_wages_thisyear;
  int finance_wages_lastyear;
  int numPlebsTaxed;
  int numPatriciansTaxed;
  int numPlebsNotTaxed;
  int numPatriciansNotTaxed;
  int percentagePlebsRegisteredForTax;
  int percentagePatriciansRegisteredForTax;
  int percentageRegisteredForTax;
  int yearlyTaxFromPlebs;
  int yearlyTaxFromPatricians;
  int yearlyUncollectedTaxFromPlebs;
  int yearlyUncollectedTaxFromPatricians;
  int finance_taxes_thisyear;
  int finance_taxes_lastyear;
  int collectedTaxFromPlebs;
  int uncollectedTaxFromPlebs;
  int collectedTaxFromPatricians;
  int uncollectedTaxFromPatricians;
  int finance_exports_lastyear;
  int finance_imports_thisyear;
  int finance_imports_lastyear;
  int finance_interest_sofar_thisyear;
  int finance_interest_lastyear;
  int finance_interest_thisyear;
  int finance_sundries_lastyear;

  int finance_construction_lastyear;
  int finance_construction_thisyear;
  int finance_salary_lastyear;
  int finance_salary_thisyear;

  int finance_salary_sofar_thisyear;
  int finance_totalIncome_lastyear;
  int finance_totalIncome_thisyear;
  int finance_totalExpenses_lastyear;
  int finance_totalExpenses_thisyear;
  int finance_netInOut_lastyear;
  int finance_netInOut_thisyear;
  int finance_balance_lastyear;
  int finance_balance_thisyear;

  __int16 resourceAmountExported[16];
  __int16 resourceInStock[16];
  __int16 stock_olives;
  __int16 stock_vines;
  __int16 stock_wine;
  __int16 stock_oil;
  __int16 stock_iron;
  __int16 stock_timber;
  __int16 stock_clay;
  __int16 stock_marble;
  __int16 stock_weapons;
  __int16 stock_furniture;
  __int16 stock_pottery;
  __int16 resourceTradeStatus[16];
  __int16 resourceTradeStatus_olives;
  __int16 resourceTradeStatus_vines;
  __int16 resourceTradeStatus_wine;
  __int16 resourceTradeStatus_iron;
  __int16 resourceTradeStatus_timber;
  __int16 resourceTradeStatus_clay;
  __int16 exportOver[16];
  __int16 industryMothballed[16];
  __int16 industryMonthballed_wheat;
  __int16 industryMonthballed_vegetables;
  __int16 industryMonthballed_fruit;
  __int16 industryMonthballed_olives;
  __int16 industryMonthballed_vines;
  __int16 industryMonthballed_meat;
  __int16 industryMonthballed_wine;
  __int16 industryMonthballed_oil;
  __int16 industryMonthballed_iron;
  __int16 industryMonthballed_timber;
  __int16 industryMonthballed_clay;
  __int16 industryMonthballed_marble;
  __int16 industryMonthballed_weapons;
  __int16 industryMonthballed_furniture;
  __int16 industryMonthballed_pottery;

  int labor_categoryWater_numWorkersNeeded;
  int labor_categoryWater_workersAllocated;
  int labor_categoryWater_priority;

  int treasury_lastyear_prosperity;
  int culture_theater;
  int culture_religion;
  int culture_school;
  int culture_library;
  int culture_academy;

  int foodInGranaries_wheat[8];
  int x_cityinfo_dword_6528E8[7];
  int x_cityinfo_dword_652900[7];
  int dword_652AF8;

  __int16 censusGraph[100];
  __int16 censusGraph_peopleAged100;
  int societyGraph[20];

  int dword_6544F4;
  int dword_6544FC;
  int dword_654500;
  int dword_654504;
  int dword_654508;
  int dword_65450C;
  int estimatedTaxIncome;

  char pctReligionCoverageGod[5];
  //char pctReligionCoverageNeptune;
  //char pctReligionCoverageMercury;
  //char pctReligionCoverageMars;
  //char pctReligionCoverageVenus;
  char happinessGod[5];
  //char happinessNeptune[5];
  //char happinessMercury[5];
  //char happinessMars[5];
  //char happinessVenus[5];
  char numBoltsGod[5];
  //char numBoltsNeptune[5];
  //char numBoltsMercury[5];
  //char numBoltsMars[5];
  //char numBoltsVenus[5];

  int finance_donated_lastyear;

  __int16 word_654388[10];
  __int16 word_6543A2;
  __int16 numOpenSeaTradeRoutes;
  __int16 numOpenLandTradeRoutes;
  __int16 seaTradeProblemDuration;
  __int16 landTradeProblemDuration;
  __int16 numWorkingDocks;
  __int16 word_6543AE;
  __int16 numWorkingWharfs;
  __int16 moneyStolenThisYear;
  __int16 moneyStolenLastYear;

  int numLegionaryForts;
  int estimatedWageCost;

  int rioting;

  char byte_6543CC;
  char byte_6543CD;
  __int16 word_6543CE;
  int barracksBuildingId;
  int dword_6543D4;
  int dword_6543EC;

  int dword_654224;
  int dword_654260;
  int dword_654264;


  char byte_654303[5];
  char byte_654308[5];
  char byte_65430D[5];
  char byte_654312[5];
  char byte_654317[5];

  int dword_65433C;

  int dword_654458;
  int dword_65445C;
  int dword_654460;
  int dword_654464;
  int dword_654468;
  int dword_65446C;
  int dword_654470;
  int dword_654474;
  int dword_654478;

  int numDestroyedBuildingsByEnemies;

  int favor_monthsSinceGift;
  int favor_giftPenalty;
  int modestGiftType;
  int generousGiftType;
  int lavishGiftType;

  int milestoneMissed;

  int favorRatingLastYear;
  int favorRatingIncrease;
  int dword_6544B8;
  int dword_6544BC;
  int dword_6544C0;
  int dword_6544C4;
  int dword_6544C8;

  int yearsSinceStartOfPeace;
  char distantBattleCity;
  char byte_654581;
  char byte_654582;
  char byte_654583;
  char byte_654584;
  char byte_654585;
  char byte_654586;
  char availableTriumphalArches;
  char byte_654588;
  char byte_654589;
  char byte_65458A;
  char byte_65458B;
  char byte_65458C;
  char byte_65458D;
  char byte_65458F;
  char placedTriumphalArches;
  char byte_654591;
  char byte_654592;
  char byte_654593;
  int dword_654598;
  char byte_65459C;
  char byte_65459D;
  char byte_65459E;
  char byte_65459F;
  char byte_6545A0;
  char byte_6545A1;
  char byte_6545A2;
  char byte_6545A3;
  char byte_6545A4;
  char byte_6545AB;

  int dword_6545CC[10];
  int dword_6545D0[10];


  int relatedToImmigration_always0;
  __int16 word_6527E4[20];
  __int16 warehouse_nextTakeGoods;
  char entry_x;
  char entry_y;
  __int16 entry_gridOffset;
  char exit_x;
  char exit_y;
  __int16 exit_gridOffset;
  char byte_652820;
  char byte_652821;
  __int16 word_652822;
  int dword_652824;
  __int16 word_652828;

  char byte_654518;
  char byte_654519;
  __int16 word_65451A;
  int dword_654558;

  int dword_654620;

  char byte_6500A2_always1;
  char byte_6500A3_always1;
  int calculatedTargetHealthRate;

  int deaths;
  int dword_652784;
  int dword_6544E8;
  int dword_654648;
  short byte_6500A5_always0;


  LaborCategoryPriority labor_category_priority[10];

  CityInfo()
  {
    memset( this, 0, sizeof(CityInfo) );
  }
};

struct Walker
{
  int gridOffset;
  char inUse;
  short nextIdOnSameTile;
  unsigned char actionState;
  int tradeCityId;
  int direction;
  int buildingId;

  unsigned char y;
  unsigned char x;

  int progressOnTile;

  int tilePosition_y;
  int tilePosition_x;

  int destination_x;

  int type;
  int word_7FA344;
  char byte_7FA34C;
  char byte_7FA358;
  char byte_7FA3A6;

  int state;
};

static Walker walkers[1000];

struct Building
{
 int type;
 int d7d_storageId;
 int x;

 unsigned char inUse;
 int house_crimeRisk;
 int houseSize;
 int house_population;
 int walkerServiceAccess;
 int laborCategory;
 int word_94BDAC[2];
 char byte_94BDB8;
 int level_resourceId;
 int grow_value_house_foodstocks[8];
};

static Building buildings[2000];

struct Storage
{
char inUse;
char emptyAll;
char resourceState[16];
char stateWheat;
char stateVegetables;
char stateFruit;
char stateMeatFish;
};

struct ModelHouse
{
int des_devolve[20];
int des_evolve;
int entertainment;
int water;
int religion;
int education;
int barber;
int bathhouse;
int health;
int foodtypes;
int pottery;
int oil;
int furniture;
int wine;
int prosperity;
int numPeople;
int tax;
};

static ModelHouse model_houses[20];

static Storage storages[200];

static char relatedToCityInfoAlwaysZero[8];

static CityInfo city_inform[8];

#endif
