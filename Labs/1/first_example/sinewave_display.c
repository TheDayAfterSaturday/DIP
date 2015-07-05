#include <analysis.h>
#include <cvirte.h>		
#include <userint.h>

#include "sinewave_display.h"

static int mypanel;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mypanel = LoadPanel (0, "sinewave_display.uir", MYPANEL)) < 0)
		return -1;
	DisplayPanel (mypanel);
	RunUserInterface ();
	DiscardPanel (mypanel);
	return 0;
}

int CVICALLBACK Aquire_Display (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double datapoints[256];
	double phase = 0.0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			SineWave (256, 1.0, 7.8125e-3, &phase, datapoints);
			DeleteGraphPlot (MYPANEL, MYPANEL_MYGRAPH, -1, VAL_IMMEDIATE_DRAW);
			PlotY (MYPANEL, MYPANEL_MYGRAPH, datapoints, 256,
				VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, 
				VAL_SOLID, 1, VAL_RED);
			
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
