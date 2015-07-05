/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2015. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MYPANEL                          1
#define  MYPANEL_MEAN_FILTER              2       /* control type: command, callback function: Median_Filter */
#define  MYPANEL_LOAD_PICTURE             3       /* control type: command, callback function: Load_Picture */
#define  MYPANEL_QUIT                     4       /* control type: command, callback function: Quit */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Load_Picture(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Median_Filter(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
