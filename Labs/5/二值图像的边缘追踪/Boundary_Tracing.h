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
#define  PANEL_LOADFILE                   2       /* callback function: Load_file */
#define  PANEL_THRESHOLD_SLID             3
#define  PANEL_QUIT                       4       /* callback function: Quit */
#define  PANEL_BOUNDARY_TRACING           5       /* callback function: Boundary_Tracing */
#define  PANEL_THRESHOLDING               6       /* callback function: Thresholding */
#define  PANEL_HISTOGRAM                  7


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Boundary_Tracing(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Load_file(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Thresholding(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
