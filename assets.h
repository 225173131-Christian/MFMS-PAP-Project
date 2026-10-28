#ifndef ASSETS_H
#define ASSETS_H

/* Asset Management module - MFMS Project A */

#define MAX_ASSETS 100

void   assetMenu(void);            /* sub-menu: call this from main menu option 4 */
void   addAsset(void);
void   displayAssets(void);
void   searchAsset(void);
int    findAssetById(int id);      /* returns array index, or -1 if not found */
int    getAssetCount(void);
double getTotalAssetValue(void);
void   displayAssetReport(void);   /* Student 5 (Reports) can call this */

#endif
