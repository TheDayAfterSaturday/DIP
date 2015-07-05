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

#define  PANEL                            1
#define  PANEL_HISTOGRAM                  2
#define  PANEL_LOADFILE                   3       /* callback function: Load_file */
#define  PANEL_NUMERICSLIDE               4
#define  PANEL_THRESHOLD                  5       /* callback function: Threshold */
#define  PANEL_MORPH_MOTH0D               6
#define  PANEL_MORPH_PROCESSING           7       /* callback function: Morph_processing */
#define  PANEL_QUIT                       8       /* callback function: Quit */
#define  PANEL_SAVEIMAGE                  9       /* callback function: Save_image */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Load_file(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Morph_processing(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Save_image(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Threshold(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
