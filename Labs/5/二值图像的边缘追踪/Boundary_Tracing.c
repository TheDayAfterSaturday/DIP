#include "nivision.h"
#include <cvirte.h>		
#include <userint.h>
#include "Boundary_Tracing.h"

static int panelHandle;
static Image *DestImage;
static Image *SourceImage;
static Image *Dest1Image;
static int plotHandle;
static char path[512];
static HistogramReport* histogramRpt_Ptr; 

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "Boundary_Tracing.uir", PANEL)) < 0)
		return -1;
	
	SourceImage = imaqCreateImage (IMAQ_IMAGE_U8, 2);
    DestImage = imaqCreateImage (IMAQ_IMAGE_U8, 2); 
	 Dest1Image = imaqCreateImage (IMAQ_IMAGE_U8, 2); 
	
	
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK Load_file (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			FileSelectPopup ("", "*.bmp", "*.bmp;*.tif", "ԭʼͼ��",  VAL_LOAD_BUTTON, 0, 0, 1, 0, path); 
 	        imaqReadFile (SourceImage, path, NULL, NULL);
            imaqMoveWindow(0, MakePoint (10,240));
            imaqDisplayImage(SourceImage, 0, TRUE);
			histogramRpt_Ptr = imaqHistogram (SourceImage, 256, 0, 255, NULL);
		plotHandle = PlotY (panelHandle, PANEL_HISTOGRAM, histogramRpt_Ptr->histogram, 256, VAL_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_BLUE);
		imaqDispose(histogramRpt_Ptr) ;

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

		GetCtrlVal (PANEL, PANEL_THRESHOLD_SLID, &threshold_value);		    //��ȡ���ֻ����ؼ�����ֵ
		imaqThreshold (DestImage, SourceImage, threshold_value, 255, TRUE, 255);
		imaqDisplayImage(DestImage, 0, TRUE);
		  
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

int CVICALLBACK Boundary_Tracing (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	Point point ;  
		
    int width,height; //������ͼ��Ŀ�͸�
    int x,y,  //��ǰ��������꣬�˵�ΪĿ������߽�����ڲ�
        now_x,now_y, //��(x,y) ��Χ8�����е�ĳһ��
        direction; //��ǰ����
    PixelValue pixelvalue; //ĳ������ֵ  
	
	imaqGetImageSize (SourceImage, &width, &height);
	imaqSetImageSize (DestImage, width, height);
	
    struct code_direction {   int dx;int dy;}
                    direction_code[8]=   //����ṹ�����飬��ʾ��x,y)����ܱ�
                       {
                         {1,0},   //��
                         {1,-1},  //����
                         {0,-1},  //��
                         {-1,-1},  //����
                         {-1,0},   //��
                         {-1,1},   //����
                         {0,1},   //��
                         {1,1}   //����
                     };    
    switch (event)
{
      case EVENT_COMMIT:
  
       for(y=1;y<height-1;y=y+1)  
        for(x=1;x<width-1;x=x+1) 
          {
			  	point.x= x;
				point.y = y;
            	imaqGetPixel (SourceImage, point, &pixelvalue);
             if(pixelvalue.grayscale==150.0)  //��ֵ��ʱ����ͼ��Ϊ��0,150)��������
        for(direction=0;direction<8;direction++)
         {
           point.x=point.x+direction_code[direction].dx;
           point.y=point.y+direction_code[direction].dy;
           imaqGetPixel (DestImage,point, &pixelvalue);
           if(pixelvalue.grayscale==0) 
		   {
			   pixelvalue.grayscale = 250;
                imaqSetPixel (DestImage, point, pixelvalue);
		   }
         }//���ñ߽�����Ϊ250
      }  
	   
	   	imaqDisplayImage(DestImage, 0, TRUE);   	  //��ʾ�߽�׷�ٺ��ͼ��
   }
         return 0;
}
