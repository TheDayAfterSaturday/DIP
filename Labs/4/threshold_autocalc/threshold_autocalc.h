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
#define  MYPANEL_SAVE                     2       /* control type: command, callback function: Save */
#define  MYPANEL_QUIT                     3       /* control type: command, callback function: Quit */
#define  MYPANEL_LOAD_PICTURE             4       /* control type: command, callback function: Load_Picture */
#define  MYPANEL_THRESHOLDING             5       /* control type: command, callback function: Thresholding */
#define  MYPANEL_MYGRAPH                  6       /* control type: graph, callback function: (none) */
#define  MYPANEL_MYRING                   7       /* control type: ring, callback function: Threshold_Selection */
#define  MYPANEL_MYNUMERIC                8       /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Load_Picture(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Threshold_Selection(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Thresholding(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
