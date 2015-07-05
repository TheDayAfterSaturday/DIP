#include <cvirte.h>     
#include <userint.h>
#include <nivision.h>

#include "mean_filter.h"

static int mypanel;
static Image *DestImage;
static Image *SourImage;

int main (int argc, char *argv[])
{
    if (InitCVIRTE (0, argv, 0) == 0)
        return -1;  /* out of memory */
    if ((mypanel = LoadPanel (0, "mean_filter.uir", MYPANEL)) < 0)
        return -1;
    DisplayPanel (mypanel);
    
    SourImage = imaqCreateImage (IMAQ_IMAGE_U8, 2);
    DestImage = imaqCreateImage (IMAQ_IMAGE_U8, 2);
    
    RunUserInterface ();
    DiscardPanel (mypanel);
    return 0;
}

int CVICALLBACK Mean_Filter (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int x = 0;
    int y = 0;
    int i = 0;
    int j = 0;
    
    Point point;
    int width = 0;
    int height = 0;
    float sum = 0.0;
    
    PixelValue pixelvalue;
    PixelValue meanvalue;
    
    imaqGetImageSize (SourImage, &width, &height);
    imaqSetImageSize (DestImage, width, height);
    
    switch (event)
    {
        case EVENT_COMMIT:
            for (y = 1; y < height - 1; y++)
            {
                for (x = 1; x < width - 1; x++)
                {
                    sum = 0;
                    point.x = x;
                    point.y = y;
                    
                    for (j = -1; j <= 1; j++)
                    {
                        for (i = -1; i <= 1; i++)
                        {
                            point.x = x + i;
                            point.y = y + j;
                            imaqGetPixel (SourImage, point, &pixelvalue);
                            sum = sum + pixelvalue.grayscale;
                        }
                    }
                    
                    meanvalue.grayscale = sum / 9;
                    imaqSetPixel (DestImage, point, meanvalue);
                }
            }
                
            imaqSetWindowTitle (1, "result of mean filter");
            imaqMoveWindow (1, imaqMakePoint(150,260));
            imaqDisplayImage (DestImage, 1, TRUE);
            break;
    }
    
    return 0;
}

int CVICALLBACK Load_Picture (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    char filename[512];
    int ret = 0;
    
    switch (event)
    {
        case EVENT_COMMIT:
            ret = FileSelectPopup("", "*.*", "", "select an image file",
                 VAL_LOAD_BUTTON, 0, 0, 1, 0, filename);
                                                                                                
                                                                                                
            if (ret == 1)
            {
                imaqReadFile (SourImage, filename, NULL, NULL);
                imaqMoveWindow (0, imaqMakePoint(50, 260));
                imaqDisplayImage (SourImage, 0, TRUE);
            }
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
