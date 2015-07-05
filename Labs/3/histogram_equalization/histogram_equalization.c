#include <ansi_c.h>
#include <cvirte.h>     
#include <userint.h>
#include <nivision.h>

#include "histogram_equalization.h"
       

static int mypanel;
static Image *sourimage;
static Image *destimage;

int main (int argc, char *argv[])
{
    if (InitCVIRTE (0, argv, 0) == 0)
        return -1;  /* out of memory */
    if ((mypanel = LoadPanel (0, "histogram_equalization.uir", MYPANEL)) < 0)
        return -1;
    DisplayPanel (mypanel);
    
    sourimage = imaqCreateImage (IMAQ_IMAGE_U8, 2);
    destimage = imaqCreateImage (IMAQ_IMAGE_U8, 2);
    
    RunUserInterface ();
    DiscardPanel (mypanel);
    return 0;
}


int CVICALLBACK Load_Picture (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    char picname[512];
    int ret = 0;
    HistogramReport *report = NULL;
    
    switch (event)
    {
        case EVENT_COMMIT:
            ret = FileSelectPopup("", "*.*", "", "select an image file",
                 VAL_LOAD_BUTTON, 0, 0, 1, 0, picname);
                                                                                                
                                                                                                
            if (ret == 1)
            {
                imaqReadFile (sourimage, picname, NULL, NULL);
                imaqMoveWindow (0, imaqMakePoint(50, 260));
                imaqDisplayImage (sourimage, 0, TRUE);
                
                report = imaqHistogram (sourimage, 256, 0, 255, IMAQ_IMAGE_U8) ;
                DeleteGraphPlot (mypanel, MYPANEL_ORG_GRAPH, -1, VAL_IMMEDIATE_DRAW);
                
                PlotY (mypanel, MYPANEL_ORG_GRAPH, (*report).histogram, 256, 
                    VAL_UNSIGNED_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            }
            break;
    }
    
    return 0;
}


int CVICALLBACK Equaliztion (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    HistogramReport *report = NULL;
    int ret = 0;
    
    ret = imaqEqualize (destimage, sourimage, 0, 255, IMAQ_IMAGE_U8);
    
    switch (event)
    {
        case EVENT_COMMIT:
            if (ret != 0)
            {
                imaqSetWindowTitle (1, "After equalization:\n");    
                imaqMoveWindow (1, imaqMakePoint(150, 260));            
                                    
                imaqDisplayImage (destimage, 1, TRUE);
                
                report = imaqHistogram (destimage, 256, 0, 255, IMAQ_IMAGE_U8) ;
                DeleteGraphPlot (mypanel, MYPANEL_EQU_HISTOGRAM, -1, VAL_IMMEDIATE_DRAW);
                
                PlotY (mypanel, MYPANEL_EQU_HISTOGRAM, (*report).histogram, 256, 
                    VAL_UNSIGNED_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            }

            break;
    }
    return 0;
}

int CVICALLBACK Save (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    char path[1024] = {'\0'};
    int ret = 0;
    
    switch (event)
    {
        case EVENT_COMMIT:
            
            ret = FileSelectPopup ("", "*.bmp","*.bmp, *tif, *jpg", "Select a path to save",
            VAL_SAVE_BUTTON, 0, 0, 1, 0, path);                         // specify type list
            
            if(ret != 0) imaqWriteFile (destimage, path, NULL);
            break;
    }
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
