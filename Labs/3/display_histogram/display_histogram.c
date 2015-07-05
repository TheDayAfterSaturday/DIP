#include <cvirte.h>		
#include <userint.h>
#include <nivision.h>

#include "display_histogram.h"


static int mypanel;
static Image *SourImage;

int main (int argc, char *argv[])				   
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mypanel = LoadPanel (0, "display_histogram.uir", MYPANEL)) < 0)
		return -1;
	DisplayPanel (mypanel);
	
	SourImage = imaqCreateImage (IMAQ_IMAGE_U8, 2);
	
	RunUserInterface ();
	DiscardPanel (mypanel);
	return 0;
}

int CVICALLBACK Load_and_Display (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char picname[512] = {'\0'};
	int ret = 0;
	int histogram[256] = {'0'};
	HistogramReport *report;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			ret = FileSelectPopup("", "*.*", "", "Select an Image File",
				VAL_LOAD_BUTTON, 0, 0, 1, 0, picname);
			
			if(ret == 1)
			{
				 imaqReadFile (SourImage, picname, NULL, NULL);
				 imaqMoveWindow (0, imaqMakePoint(50, 260));
				 
				 imaqDisplayImage (SourImage, 0, TRUE);
				 
				 report = imaqHistogram(SourImage, 256, 0, 
					 255, IMAQ_IMAGE_U8);
				 
				 DeleteGraphPlot(mypanel, MYPANEL_MYGRAPH, -1,
					 VAL_IMMEDIATE_DRAW);
				 
				 PlotY(mypanel, MYPANEL_MYGRAPH, (*report).histogram,
					 256, VAL_UNSIGNED_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE,
					 VAL_SOLID, 1, VAL_RED);
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
