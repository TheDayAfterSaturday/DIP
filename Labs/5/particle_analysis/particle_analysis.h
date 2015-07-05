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

#define  ANALYZING                        1
#define  ANALYZING_ANALYZE_ARG            2       /* control type: command, callback function: Analyze_Arg */
#define  ANALYZING_LABEL_IMAGE            3       /* control type: command, callback function: Label_Image */
#define  ANALYZING_PARTICLE_INDEX         4       /* control type: numeric, callback function: Particle_Index */
#define  ANALYZING_HOLE_NUMBER            5       /* control type: numeric, callback function: (none) */
#define  ANALYZING_CIRCUMFERENCE          6       /* control type: numeric, callback function: (none) */
#define  ANALYZING_AREA                   7       /* control type: numeric, callback function: (none) */
#define  ANALYZING_PARTICLE_NUM           8       /* control type: numeric, callback function: (none) */

#define  MAIN_PANEL                       2
#define  MAIN_PANEL_CANVAS                2       /* control type: canvas, callback function: (none) */
#define  MAIN_PANEL_QUIT                  3       /* control type: command, callback function: Quit */

#define  PROCESSING                       3
#define  PROCESSING_LOAD_IMAGE            2       /* control type: command, callback function: Load_Image */
#define  PROCESSING_THRESHOLD_SLIDE       3       /* control type: scale, callback function: Thresholding */
#define  PROCESSING_PURIFYING             4       /* control type: command, callback function: Purifying */
#define  PROCESSING_HISTOGRAM             5       /* control type: graph, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Analyze_Arg(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Label_Image(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Load_Image(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Particle_Index(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Purifying(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Thresholding(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
