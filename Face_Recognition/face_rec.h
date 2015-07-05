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
#define  MYPANEL_LOAD_AN_IMAGE            2       /* control type: command, callback function: Load_An_Image */
#define  MYPANEL_QUIT                     3       /* control type: command, callback function: Quit */
#define  MYPANEL_SET_HORIZONTAL_BORDER    4       /* control type: command, callback function: Set_Horizon_Border */
#define  MYPANEL_BASE_BORDER              5       /* control type: numeric, callback function: (none) */
#define  MYPANEL_TOP_BORDER               6       /* control type: numeric, callback function: (none) */
#define  MYPANEL_LEFT_BORDER              7       /* control type: numeric, callback function: (none) */
#define  MYPANEL_RIGHT_BORDER             8       /* control type: numeric, callback function: (none) */
#define  MYPANEL_HORIZONTAL_HISTOGRAM     9       /* control type: graph, callback function: (none) */
#define  MYPANEL_VERTICAL_HISTOGRAM       10      /* control type: graph, callback function: (none) */
#define  MYPANEL_SET_VERTICAL_BORDER      11      /* control type: command, callback function: Set_Vertical_Border */
#define  MYPANEL_CAPTURE                  12      /* control type: command, callback function: Capture */
#define  MYPANEL_CLOSE_CAMERA             13      /* control type: command, callback function: Close_Camera */
#define  MYPANEL_OPEN_CAMERA              14      /* control type: command, callback function: Open_Camera */
#define  MYPANEL_SEPARATE_FIGURE          15      /* control type: command, callback function: Separate_Figure */
#define  MYPANEL_RECOGNIZE_FACE           16      /* control type: command, callback function: Face_Recognition */
#define  MYPANEL_SAVE_FACEVALUE           17      /* control type: command, callback function: Save_Facevalue */
#define  MYPANEL_DETECT_FACE              18      /* control type: command, callback function: Detect_Face */
#define  MYPANEL_MYCANVAS                 19      /* control type: canvas, callback function: (none) */
#define  MYPANEL_RESOLUTIONRING           20      /* control type: ring, callback function: Resolution */
#define  MYPANEL_TEXTMSG                  21      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Capture(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Close_Camera(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Detect_Face(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Face_Recognition(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Load_An_Image(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Open_Camera(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Resolution(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Save_Facevalue(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Separate_Figure(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_Horizon_Border(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Set_Vertical_Border(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
