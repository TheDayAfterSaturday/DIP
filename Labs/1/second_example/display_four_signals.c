#include <ansi_c.h>
#include <analysis.h>
#include <cvirte.h>		
#include <userint.h>
#include "display_four_signals.h"

static int mypanel;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mypanel = LoadPanel (0, "display_four_signals.uir", MYPANEL)) < 0)
		return -1;
	DisplayPanel (mypanel);
	RunUserInterface ();
	DiscardPanel (mypanel);
	return 0;
}

int CVICALLBACK Aquire_Display (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double phase = 0.0;
	double datapoints[256];
	
	int i = 0;
	int index = 0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlIndex (MYPANEL, MYPANEL_MYRING, &index);
			switch (index)
			{
			case 0:
			SineWave (256, 1.0, 7.8125e-3, &phase, datapoints);
			break;
			
			case 1:
				SquareWave (256, 1.0, 7.8125e-3, &phase, 50.0, datapoints);
			break;
				
			case 2:
				TriangleWave (256, 1.0, 7.8125e-3, &phase, datapoints);
			break;
					
			case 3:
			for (i = 0; i < 256; i++)
				datapoints[i] = rand() /32767.0;
			break;
			}
			
			DeleteGraphPlot (MYPANEL, MYPANEL_MYGRAPH, -1, VAL_IMMEDIATE_DRAW);
			
			PlotY (MYPANEL, MYPANEL_MYGRAPH, datapoints, 256, VAL_DOUBLE,
			VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
			
			break;
	}
	return 0;
}

int CVICALLBACK Qiut (int panel, int control, int event,
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
