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
#define  MYPANEL_EQU_HISTOGRAM            2       /* control type: graph, callback function: (none) */
#define  MYPANEL_ORG_GRAPH                3       /* control type: graph, callback function: (none) */
#define  MYPANEL_QUIT                     4       /* control type: command, callback function: Quit */
#define  MYPANEL_SAVE                     5       /* control type: command, callback function: Save */
#define  MYPANEL_EQUALIZATION             6       /* control type: command, callback function: Equaliztion */
#define  MYPANEL_LOAD_PICTURE             7       /* control type: command, callback function: Load_Picture */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Equaliztion(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Load_Picture(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Save(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
