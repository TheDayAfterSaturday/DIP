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

#define  IMPROCESS                        1
#define  IMPROCESS_LOADFILE               2       /* control type: command, callback function: Load_file */
#define  IMPROCESS_THRESHOLD_SLID         3       /* control type: scale, callback function: (none) */
#define  IMPROCESS_QUIT                   4       /* control type: command, callback function: Quit */
#define  IMPROCESS_DELE_SREG              5       /* control type: command, callback function: delete_small_region */
#define  IMPROCESS_THRESHOLDING           6       /* control type: command, callback function: Thresholding */
#define  IMPROCESS_HISTOGRAM              7       /* control type: graph, callback function: (none) */

#define  MAINPANEL                        2
#define  MAINPANEL_CANVAS                 2       /* control type: canvas, callback function: (none) */

#define  MASK_ANANL                       3
#define  MASK_ANANL_IMGANALYSIS           2       /* control type: command, callback function: Image_analysis */
#define  MASK_ANANL_LABELIMG              3       /* control type: command, callback function: Labeling_image */
#define  MASK_ANANL_HOLES                 4       /* control type: string, callback function: (none) */
#define  MASK_ANANL_PERIMETER             5       /* control type: string, callback function: (none) */
#define  MASK_ANANL_AREA                  6       /* control type: string, callback function: (none) */
#define  MASK_ANANL_PARTICLEIDX           7       /* control type: numeric, callback function: ParticleIndex */
#define  MASK_ANANL_NUMBER                8       /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK delete_small_region(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Image_analysis(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Labeling_image(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Load_file(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ParticleIndex(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Thresholding(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
