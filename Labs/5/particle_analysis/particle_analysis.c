/* PUZZLE: when there are few particle on the image thresholded,
 * total particle number will be extremely small -- only two on image
 * "Boy__Spice-and-pepper_nosie.bmp" while testing. Worse still, 
 * even if there are enough particles something like 250 or so, the 
 * particle report take the smooth area as "a very big particle consisting
 * of many holes" and give them index numbers. Therefore, sometimes it will
 * show a large area, circumference and hole number value when increase
 * index number one by one.
 * */




#include <cvirte.h>     
#include <userint.h>
#include <nivision.h>
#include <easytab.h>

#include "particle_analysis.h"

#define DEBUG (0)


static int mainpanel = 0;
static Image *sourimage = NULL;
static Image *tempimage = NULL;

static Image *destimage = NULL;
static Image *labelimage = NULL;
HistogramReport *report = NULL;

static int plotter = 0;
static int particle_num = 0;
static int num = 0;

int Area[8192] = {0};
int Circumference[8192] = {0};
int Hole_num[8192] = {0};


int main (int argc, char *argv[])
{
    int tabctrl = 0;
    
    if (InitCVIRTE (0, argv, 0) == 0)
        return -1;  /* out of memory */
    if ((mainpanel = LoadPanel (0, "particle_analysis.uir", MAIN_PANEL)) < 0)
        return -1;
    DisplayPanel (mainpanel);
    
    sourimage = imaqCreateImage(IMAQ_IMAGE_U8, 2);
    tempimage = imaqCreateImage(IMAQ_IMAGE_U8, 2);
    
    destimage = imaqCreateImage(IMAQ_IMAGE_U8, 2);
    labelimage = imaqCreateImage(IMAQ_IMAGE_U8, 2);
    
    tabctrl = EasyTab_ConvertFromCanvas(mainpanel, MAIN_PANEL_CANVAS);
    EasyTab_LoadPanels(mainpanel, tabctrl, 1, "particle_analysis.uir",
        __CVIUserHInst, PROCESSING, 0, ANALYZING, 0, 0);
    
    RunUserInterface ();
    DiscardPanel (mainpanel);
    return 0;
}

int CVICALLBACK Load_Image (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int ret = 0;
    char path[512] = {'\0'};  
    
    switch (event) {
        case EVENT_COMMIT:
            ret = FileSelectPopup("", "*.bmp", "*.*", "Load: select an image",
                VAL_LOAD_BUTTON,0, 0, 1,0, path);
            
            if(ret == 1) {
                
                imaqReadFile(sourimage, path, NULL, NULL);
                imaqSetWindowTitle(0, "Processing: image loaded");
                imaqMoveWindow(0, MakePoint(16, 64));
                imaqDisplayImage(sourimage, 0, TRUE);
                
                report = imaqHistogram(sourimage, 256, 0.0, 255.0, NULL);
                plotter = PlotY(panel, PROCESSING_HISTOGRAM, report->histogram, 256,
                    VAL_UNSIGNED_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
                
                imaqDispose(report);
            }
            break;
    }
    return 0;
}

int CVICALLBACK Thresholding (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    unsigned int value = 0;
    
    switch (event) {
        case EVENT_COMMIT:
            GetCtrlVal(panel, PROCESSING_THRESHOLD_SLIDE, &value);
            imaqThreshold(tempimage, sourimage, value, 255.0, TRUE, 255.0);
            
            imaqSetWindowTitle(0, "Processing: image thresholded");            
            imaqMoveWindow(0, MakePoint(16, 64));    
            imaqDisplayImage(tempimage, 0, TRUE);
            break;
    }
    return 0;
}

int CVICALLBACK Purifying (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) {
        case EVENT_COMMIT:
            imaqLowPass(destimage, tempimage, 4, 4, 32.0, NULL);
            
            imaqSetWindowTitle(1, "Processing: image purified");
            imaqMoveWindow(0, MakePoint(64, 256));    
            imaqDisplayImage(destimage, 1, TRUE);
            break;
    }
    return 0;
}

int CVICALLBACK Label_Image (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) {
        case EVENT_COMMIT:
            imaqLabel(labelimage, destimage, TRUE, &particle_num);
#if DEBUG            
            printf("Label Image: particle num----%d-----\n", particle_num);
#endif          
            imaqSetWindowPalette(2, IMAQ_PALETTE_BINARY, NULL, 0);
            SetCtrlVal(panel, ANALYZING_PARTICLE_NUM, particle_num);
            
            imaqSetWindowTitle(2, "Analyzing: image labelled");
            imaqMoveWindow(0, MakePoint(128, 384));   
            imaqDisplayImage(labelimage, 2, TRUE);
            
            break;
    }
    return 0;
}

int CVICALLBACK Analyze_Arg (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int quantify_element = FALSE;
    
    ParticleReport *particle_report = NULL;
    QuantifyReport *quantify_report = NULL;
    ParticleFilterCriteria *filter_criteria = NULL;
    
    switch (event) {
        case EVENT_COMMIT:
            
#if DEBUG            
            printf("Analyze Arg: before report: particle num----%d-----\n", particle_num);
#endif
/*  function imqaGetParticleInfo will pass a value to argument num */
            particle_report = imaqGetParticleInfo(labelimage, FALSE, IMAQ_ALL_INFO, &num);

/*  error snippets in guidebook

    When num = 0 , maybe stand for whole image,
    index num (num below) ranges from 1 to particle num.
    Loop below get the particle infomation for function Particle_Index

*/
            for(num = 0; num <= particle_num; num++) {              
            
                Area[num] = particle_report[num].area;
                Circumference[num] = particle_report[num].perimeter;
                Hole_num[num] = particle_report[num].numHoles;
            }
           
            break;
    }
    return 0;
}

int CVICALLBACK Particle_Index (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    int idx = 1;
    int index = 0;
    int index_palette[8192] = {0};
    
    if(event == EVENT_VAL_CHANGED) {
     
        GetCtrlVal(panel, ANALYZING_PARTICLE_INDEX, &index);
 #if DEBUG
          printf("Particle Index: index----%d----particle num----%d----\n", index, particle_num);
#endif
        
        if(index < 1){
           
            index = idx;
            SetCtrlVal(panel, ANALYZING_PARTICLE_INDEX, index); 
        }
        
        if(index > particle_num) {

            index = particle_num;         
            SetCtrlVal(panel, ANALYZING_PARTICLE_INDEX, index);
        }
        
        if(index >= 1){
          idx = index;
          
#if DEBUG
          printf("Particle Index; idx----%d------\n", idx);
#endif
          SetCtrlVal(panel, ANALYZING_AREA , Area[idx]);
          SetCtrlVal(panel, ANALYZING_CIRCUMFERENCE, Circumference[idx]);
          SetCtrlVal(panel, ANALYZING_HOLE_NUMBER , Hole_num[idx]);
        } else {
         
          SetCtrlVal(panel, ANALYZING_AREA , 0);
          SetCtrlVal(panel, ANALYZING_CIRCUMFERENCE, 0);
          SetCtrlVal(panel, ANALYZING_HOLE_NUMBER , 0);
        }
        
        for(num = 1; num < idx; num++)  index_palette[num] = VAL_YELLOW;
        for(num = idx + 1; num < 256; num ++) index_palette[num] = VAL_WHITE;
        
        imaqSetWindowPalette(3, IMAQ_PALETTE_BINARY, NULL, IMAQ_PALETTE_USER);
    }

    return 0;
}

int CVICALLBACK Quit (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) {
        case EVENT_COMMIT:
            QuitUserInterface (0);
            break;
    }
    return 0;
}
