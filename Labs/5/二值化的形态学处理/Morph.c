#include "nivision.h"
#include <cvirte.h>		
#include <userint.h>
#include "Morph.h"

static int panelHandle;

static int plotHandle;
static Image* source_image;
static Image* dest_image;
static Image* dest1_image;
HistogramReport* histogramRpt_Ptr; 
static char path[512];

									  

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Morph.uir", PANEL)) < 0)
		return -1;
	
	source_image = imaqCreateImage (IMAQ_IMAGE_U8, 2);
	dest_image = imaqCreateImage (IMAQ_IMAGE_U8, 2);
	dest1_image = imaqCreateImage (IMAQ_IMAGE_U8, 2); 
	
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK Load_file (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int status;
	switch (event)
	{
			case EVENT_COMMIT: 
		status = FileSelectPopup ("", "*.bmp", "", "选择原始图像", VAL_LOAD_BUTTON, 0, 0, 1, 0, path);
			 if(status == 1)
			 {
			 	imaqReadFile (source_image, path, NULL, NULL);
				imaqMoveWindow (0, MakePoint(10,240));
				imaqDisplayImage (source_image, 0, TRUE);
				
				histogramRpt_Ptr = imaqHistogram (source_image, 256, 0.0, 255, NULL);
				plotHandle = PlotY (panelHandle,PANEL_HISTOGRAM, histogramRpt_Ptr->histogram, 256, VAL_UNSIGNED_INTEGER,
					 VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_BLUE);
			
			/*Clean up the allocated objects from this function*/
				imaqDispose(histogramRpt_Ptr);
			 }

			break;
	}
	return 0;
}

int CVICALLBACK Threshold (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned int threshold_value;
	
	switch (event)
	{
		case EVENT_COMMIT:

		GetCtrlVal (PANEL, PANEL_NUMERICSLIDE, &threshold_value);		    //获取数字滑动控件的数值
		imaqThreshold (dest_image, source_image, threshold_value, 255.0, TRUE, 255);
		imaqDisplayImage(dest_image, 0, TRUE);
		  
			break;
	}
	return 0;										
}

int CVICALLBACK Morph_processing (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int method_value;
	switch (event)
	{
		case EVENT_COMMIT:

		GetCtrlVal (PANEL, PANEL_MORPH_MOTH0D, &method_value);
		imaqGrayMorphology (dest1_image, dest_image, method_value,NULL);
		imaqSetWindowPalette (2, IMAQ_PALETTE_BINARY, NULL, 0);
		imaqDisplayImage(dest1_image, 0, TRUE);
		
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

int CVICALLBACK Save_image (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char SavePath[260];
													
	int lStatus;
	
	switch (event)
	{
		case EVENT_COMMIT:
				 
		     FileSelectPopup ("", "*.bmp", "", "保存当前图像", VAL_SAVE_BUTTON, 0, 0, 1, 0, SavePath); 
							
			 imaqWriteFile (dest_image, SavePath, NULL);     
			break;
	}
	return 0;
}
