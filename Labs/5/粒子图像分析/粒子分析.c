#include "nivision.h"
#include "easytab.h"
#include <cvirte.h>		
#include <userint.h>
#include "粒子分析.h"

static int mainpanel;
static Image* source_image;
static Image* dest_image;
static Image* LabelledImage;
static char path[512];
HistogramReport* histogramRpt_Ptr;
static int plotHandle;  

int main (int argc, char *argv[])
{
	int tabctrl;
	
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainpanel = LoadPanel (0, "粒子分析.uir", MAINPANEL)) < 0)
		return -1;
	 source_image = imaqCreateImage (IMAQ_IMAGE_U8, 2);
   	dest_image = imaqCreateImage (IMAQ_IMAGE_U8, 2);
	LabelledImage = imaqCreateImage (IMAQ_IMAGE_U8, 2);   
	
	tabctrl = EasyTab_ConvertFromCanvas (mainpanel, MAINPANEL_CANVAS);
	EasyTab_LoadPanels (mainpanel, tabctrl, 1, "粒子分析.uir", __CVIUserHInst,IMPROCESS,0, MASK_ANANL,0,0);
	
	DisplayPanel (mainpanel);
	RunUserInterface ();
	DiscardPanel (mainpanel);
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
				plotHandle = PlotY (MAINPANEL,IMPROCESS_HISTOGRAM, histogramRpt_Ptr->histogram, 256, VAL_UNSIGNED_INTEGER,
					 VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_BLUE);
			
			/*Clean up the allocated objects from this function*/
				imaqDispose(histogramRpt_Ptr);
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

int CVICALLBACK Thresholding (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned int threshold_value;
	
	switch (event)
	{
		case EVENT_COMMIT:

		GetCtrlVal (MAINPANEL, IMPROCESS_THRESHOLD_SLID, &threshold_value);		    //获取数字滑动控件的数值
		imaqThreshold (dest_image, source_image, threshold_value, 255.0, TRUE, 255);
		imaqDisplayImage(dest_image, 0, TRUE);
		  
			break;
	}
	return 0;	
}

int CVICALLBACK delete_small_region (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
									 
		imaqLowPass (dest_image,dest_image, 3, 3, 40.0, NULL);
		imaqDisplayImage(dest_image, 0, TRUE);

			break;
	}
	return 0;
}

int CVICALLBACK Labeling_image (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	int  count;
	switch (event)
	{
		case EVENT_COMMIT:
			
			imaqLabel (LabelledImage, dest_image, TRUE, &count);
		    SetCtrlVal (MAINPANEL, MASK_ANANL_NUMBER, count);    	    //这里出错了
			imaqSetWindowPalette (2, IMAQ_PALETTE_BINARY, NULL, 0);
			imaqDisplayImage(LabelledImage, 0, TRUE); 
			break;
	}
	return 0;
}

int CVICALLBACK Image_analysis (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK ParticleIndex (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}
