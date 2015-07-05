#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <nivision.h>

#include "morphology.h"

#define DEBUG (0)

static int mypanel = 0;
static Image *sourimage = NULL;
static Image *destimage = NULL;
static Image *tempimage = NULL;

int main (int argc, char *argv[])
{
    if (InitCVIRTE (0, argv, 0) == 0)
        return -1;  /* out of memory */
    if ((mypanel = LoadPanel (0, "morphology.uir", MYPANEL)) < 0)
        return -1;
    DisplayPanel (mypanel);
    
    sourimage = imaqCreateImage (IMAQ_IMAGE_U8, 2);
    tempimage = imaqCreateImage (IMAQ_IMAGE_U8, 2);
    destimage = imaqCreateImage (IMAQ_IMAGE_U8, 2);
    
    RunUserInterface ();
    DiscardPanel (mypanel);
    return 0;
}

int CVICALLBACK Quit (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event)
    {
        case EVENT_COMMIT:
            QuitUserInterface (0);
            break;
    }
    return 0;
}

int CVICALLBACK Load_Picture (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    char picname[512] = {'\0'};
    int ret = 0;
    HistogramReport *report = NULL;
    
    switch (event)
    {
        case EVENT_COMMIT:

            ret = FileSelectPopup ("", "*.*", "", "Load: select an image file",
                VAL_LOAD_BUTTON, 0, 0, 1, 0, picname);
            
            if (ret == 1)
            {
                imaqReadFile (sourimage, picname, NULL, NULL);
                imaqMoveWindow (0, imaqMakePoint (50, 260));
                imaqDisplayImage (sourimage, 0, TRUE);
                
                report = imaqHistogram (sourimage, 256, 0.0, 255, IMAQ_IMAGE_U8);
                DeleteGraphPlot (mypanel, MYPANEL_MYGRAPH, -1, VAL_IMMEDIATE_DRAW);
                
                PlotY (mypanel, MYPANEL_MYGRAPH, report->histogram, 256, 
                    VAL_UNSIGNED_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            }
            
            break;
    }
    return 0;
}

int CVICALLBACK Thresholding (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int threshold = 0;
    
    switch (event)
    {
        case EVENT_COMMIT:
            
            GetCtrlVal (mypanel, MYPANEL_THRESHOLD_SLIDE, &threshold);
            
            imaqThreshold (tempimage, sourimage, threshold, 255, TRUE, 255);
            imaqSetWindowTitle (1, "Picture after binaryzation");
            
            imaqMoveWindow (1, imaqMakePoint (150, 260));
            imaqDisplayImage (tempimage, 1, TRUE);
            
            break;
    }
    return 0;
}

int CVICALLBACK Morphology (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int method = 0;
    
    switch (event)
    {
        case EVENT_COMMIT:
            
            GetCtrlVal (mypanel, MYPANEL_MYRING, &method);
            // imaqMorphology on the guidebook!!! it corrupts pointer destimage!!!
            imaqGrayMorphology (destimage, tempimage, method, NULL);    
            
            imaqSetWindowPalette (2, IMAQ_PALETTE_BINARY, NULL, 22);
            imaqSetWindowTitle (2, "Picture after morphology processing");
            
            imaqMoveWindow (2, imaqMakePoint(250, 360));
            imaqDisplayImage (destimage, 2, TRUE);
#if DEBUG
            imaqDisplayImage (destimage, 4, TRUE);
#endif
            break;
    }
    return 0;
}

int CVICALLBACK Save (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    char path[512] = {'\0'};
    int ret = 0;
    
    switch (event)
    {
        case EVENT_COMMIT:
            ret = FileSelectPopup ("", "*.bmp", "*.bmp,*.tif,*.jpg", "Save: select a path to save",
                VAL_SAVE_BUTTON, 0, 0, 1, 0, path);
            
            if(ret != 0) imaqWriteFile (destimage, path, NULL);
                break;
    }
    return 0;
}
