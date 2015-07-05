#include <analysis.h>
#include <Windows.h>
#include <Vfw.h>
#include <stdio.h>
#include <cvirte.h>     
#include <userint.h>
#include <nivision.h>

#include "face_rec.h"

/***** Size and Length macros *****/
#define PROMPTLEN               (128)
#define PATHLEN                 (512) 
#define PXLSUM                  (8192)
#define BORDERLEN               (2)
#define TEMPLEN                 (1024)
#define VALLEN                  (128)

/***** Detection and Recognition constant macros *****/
#define HSTART                  (45)
#define EXTLEN                  (4)
#define FACESDB                  "TESTIMAGES\\FACES.DB"
#define SAVEFLAG                (0)
#define RECOFLAG                (1)

/***** Debugging and Testing macros *****/
#define DEBUG                   (0)
#define TEST                    (0)

/***** Customized Struct Definitions *****/
typedef struct image_s {
    
    Image *src;
    Image *U8;
    Image *RGB;
    
    Image *red;
    Image *green;
    Image *blue;
    
    Image *sepa;
    Image *HrzEdge;
    Image *VtcEdge;
    Image *face;
} image_t;

typedef struct index_s {
    
    int HrzMin;
    int HrzMax;
    int VtcMin;
    int VtcMax;
} index_t;

typedef struct basic_s {
    
    int ret;
    PixelValue RGBvalue;
    PixelValue grayvalue;
    
    char temp[TEMPLEN];
    char prompt[PROMPTLEN];
    char path[PATHLEN];
} basic_t;


typedef struct detection_s {

    int eyes;
    int mouth;
    size_t maxidx;
    size_t minidx;
    double maxval;
    double minval;
    HistogramReport *report;
    
    double ColPxlSum[PXLSUM];
    double RowPxlSum[PXLSUM];  
} detection_t;

typedef struct grid_s {
    
    int width;
    int height;
    int x;
    int y;
} grid_t;

typedef struct recognition_s {
    
    int width;
    int delta;
    int eyesarea;
    int moutharea;
    char path[PATHLEN];
}recognition_t;

typedef struct header_s {

    void *next;
    size_t qlen;
} header_t;

typedef struct face_s {
    
    char code[PATHLEN];
    int width;
    int delta;
    int eyesarea;
    int moutharea;
    struct face_s *next;
}face_t;

/***** Globel Variable Definitions *****/
static int mypanel;
ImageInfo ImageInfos; 

image_t Imgs;
index_t Idxs;
basic_t Basics;
detection_t Infos;
grid_t Grids;
recognition_t Args;
header_t *FaceHd = NULL;


/***** Customized Struct initiatoins and Disposals *****/
int initImages(image_t *imgs)
{
    if(imgs == NULL) {
        printf("initImgs: null pontier: imgs\n");
        return -1;
    }
    
    imgs->src = imaqCreateImage(IMAQ_IMAGE_RGB, BORDERLEN);
    imgs->U8 = imaqCreateImage(IMAQ_IMAGE_U8, BORDERLEN);
    imgs->RGB = imaqCreateImage(IMAQ_IMAGE_RGB, BORDERLEN);
    
    imgs->red = imaqCreateImage(IMAQ_IMAGE_U8, BORDERLEN);
    imgs->green = imaqCreateImage(IMAQ_IMAGE_U8, BORDERLEN);
    imgs->blue = imaqCreateImage(IMAQ_IMAGE_U8, BORDERLEN);
    
    imgs->sepa = imaqCreateImage(IMAQ_IMAGE_U8, BORDERLEN);
    imgs->HrzEdge = imaqCreateImage(IMAQ_IMAGE_U8, BORDERLEN);
    imgs->VtcEdge = imaqCreateImage(IMAQ_IMAGE_U8, BORDERLEN);
    imgs->face = imaqCreateImage(IMAQ_IMAGE_U8, BORDERLEN);
    
    return 0;
} 

int initIndexs(index_t *idxs)
{
    if(idxs == NULL) {
        printf("initIdxs: null pontier: idxs\n");
        return -1;
    }
    
    idxs->HrzMin = 0;
    idxs->HrzMax = 0;
    idxs->VtcMin = 0;
    idxs->VtcMin = 0;
    
    return 0;
}

int initBasics(basic_t *bas){
   
   bas->ret = 0;
   bas->grayvalue.grayscale = 0.0;   
   memset(&(bas->RGBvalue.rgb), '0', sizeof(RGBValue));
   memset(bas->temp, '\0', sizeof(TEMPLEN));
   memset(bas->prompt, '\0', sizeof(PROMPTLEN));
   memset(bas->path, '\0', sizeof(PATHLEN));
   
   return 0;
}

int initInfos(detection_t *infos)
{
    infos->eyes = 0;
    infos->mouth = 0;
    infos->maxidx = 0;
    infos->minidx = 0;
    
    infos->maxval = 0.0;
    infos->minval = 0.0;
    infos->report = NULL;
    
    memset(infos->ColPxlSum , 0, PXLSUM);
    memset(infos->RowPxlSum , 0, PXLSUM);
    
    return 0;
}

initGrids(grid_t *grd)
{
    grd->width = 0;
    grd->height = 0;
    grd->x = 0;
    grd->y = 0;
    
    return 0;
}

initArgs(recognition_t *args)
{
    args->width = 0.0;
    args->delta = 0;
    args->eyesarea = 0;
    args->moutharea = 0;
    memset(args->path, '\0', PATHLEN);
    
    return 0;
}

int disposeImages(image_t *imgs)
{
   imaqDispose(imgs->src);
   imaqDispose(imgs->U8);
   imaqDispose(imgs->RGB);
   
   imaqDispose(imgs->red);
   imaqDispose(imgs->green);
   imaqDispose(imgs->blue);
   
   imaqDispose(imgs->sepa);
   imaqDispose(imgs->HrzEdge);
   imaqDispose(imgs->VtcEdge);
   imaqDispose(imgs->face);
   
   return 0; 
} 


/***** Face Value Nodes Operation *****/
void *initFace(size_t size)
{
    face_t *face = NULL;
    face = malloc(size);
    if(face) memset(face, 0, size);
    
    return face;
}

int insertFace(header_t *hd, face_t *face)
{
    face_t *p = NULL;

#if DEBUG
     printf("Insert face: width--%d--delta--%d--eyesarea--%d--moutharea--%d--\n", face->width, 
     face->delta,face->eyesarea, face->moutharea); 
#endif

    if(hd->next == NULL) {
        hd->next = face;
        hd->qlen++;
        face->next = NULL;

    } else {
        p = hd->next;
        while(p->next != NULL) p = p->next;
        p->next = face;
        hd->qlen++;
        face->next = NULL;
    }
    
    return 0;
}


int readfacevalue(char *val, char **p)
{
    int i = 0;
    memset(val, '\0', VALLEN);
    char *ch = *p;

    for(i = 0; (i < VALLEN - 1) && (*ch != '\n'); i++, ch++) val[i] = *ch;
#if DEBUG
    printf("Read face value: ----val--%s---\n", val);
#endif

    (*p) += strlen(val) + 1;
    return 0;
}

int loadFaces(header_t *hd)
{
   FILE *fp;
   FILE *fp1;
   face_t *face = NULL; 
   char width[VALLEN] = {'\0'};
   char delta[VALLEN] = {'\0'};
   char eyesarea[VALLEN] = {'\0'};    
   char moutharea[VALLEN] = {'\0'};  
   char *ch = NULL;
   initBasics(&Basics);

   if((face = initFace(sizeof(face_t))) == NULL) return -1;
   
   if((fp = fopen(FACESDB, "a")) == NULL) {             // "w+" cause a bug: clear info.db after reading it
        printf("Load face database: failed to open face data: faces.db\n");
        return -1;
   }
  
    if (fseek(fp, (long)0, SEEK_END) != 0){ 
        printf("Load face database: fseek failed\n") ; 
        return -1;
    }
    if((Basics.ret = (int)ftell(fp)) == 0) return -1;
    fclose(fp);
    
   if((fp = fopen(FACESDB, "r")) == NULL) {             // "w+" cause a bug: clear info.db after reading it
        printf("Load face database: failed to open face data: faces.db\n");
        return -1;
   }
  
   
   fread(Basics.temp, 1, Basics.ret, fp);
#if DEBUG
    printf("Load face: len--%d"
        "----------------temp---------------------\n"
        "%s----------------------------------------", Basics.ret, Basics.temp);
#endif
    
  
   for(ch = Basics.temp; *ch != '\n' && *(ch + 1) != '\0'; ch++) {
       
       if(readfacevalue(face->code, &ch) != 0) return -1;
       if(readfacevalue(width, &ch) != 0) return -1;  
       if(readfacevalue(delta, &ch) != 0) return -1;  
       if(readfacevalue(eyesarea, &ch) != 0) return -1;  
       if(readfacevalue(moutharea, &ch) != 0) return -1;  

       face->delta = atoi(delta);
       face->width = atoi(width);
       face->eyesarea = atoi(eyesarea);
       face->moutharea = atoi(moutharea);
#if DEBUG
        printf("Load face database(string): code:%s--width:%s--delta:%s--eyesarea:%s--moutharea:%s\n",
            face->code, width, delta, eyesarea, moutharea);
        printf("Load face database(number): code:%s--width:%d--delta:%d--eyesarea:%d--moutharea:%d\n",
            face->code, face->width, face->delta, face->eyesarea, face->moutharea);
#endif
       
      
       if(insertFace(FaceHd, face) == -1) {
            printf("Load face database: failed to insert an face node\n");
            return -1;
       }
        
        /***** malloc space for next face_t struct *****/
        if((face = initFace(sizeof(face_t))) == NULL) return -1;
        memset(width, 0, VALLEN);    
        memset(delta, 0, VALLEN);
        memset(eyesarea, 0, VALLEN);
        memset(moutharea, 0, VALLEN);
    }

    fclose(fp);
    return 0;
}

int findFace(header_t *hd, face_t *face, unsigned char flag)
{
    face_t *p = NULL;
    int i = 0;
    
    if(hd == NULL) {
        printf("Find face: NULL header\n");
        return -1;
    }
    
    if(flag == 1){
        if(hd->next == NULL) {
        printf("Recognize face: find face: no face node on queue\n");
        return -1;
        }                       
    }
    
    p = hd->next;
    for(i = 0; i < hd->qlen; i++) {

        if(abs(p->width - face->width) < 2 && abs(p->delta - face->delta) < 2 &&
            abs(p->eyesarea - face->eyesarea) < 2 && abs(p->moutharea - face->moutharea) < 2) {
            strncpy(face->code, p->code, strlen(p->code));
            printf("----------------\n");
            return 0;
        }
        
        if(p->next == NULL) break;
        p = p->next;
    }
    
    return -1;
}

int disposeFaces(header_t *hd)
{   
    face_t *face;
    if(hd == NULL) return 0;
    if(hd->next == NULL) {
       hd->qlen = 0; 
       free(hd);
       return 0;
    }
    
    while((face = hd->next) != NULL) {
        if(face->next != NULL) hd->next = face->next;
        else hd->next = NULL;
#if DEBUG 
        printf("Quit: Dispose face node: code: %s--width: %d--delta: %d--eyesarea: %d--moutharea: %d--\n",
            face->code, face->width, face->delta, face->eyesarea, face->moutharea);
#endif
       free(face);
    }
    free(hd); 
    
    return 0;
}


/***** Main Function to pre-process *****/
int main (int argc, char *argv[])
{
    if (InitCVIRTE (0, argv, 0) == 0)
        return -1;  /* out of memory */
    if ((mypanel = LoadPanel (0, "face_rec.uir", MYPANEL)) < 0)
        return -1;
    DisplayPanel (mypanel);

    initImages(&Imgs);
    initIndexs(&Idxs);
    initBasics(&Basics);
    initInfos(&Infos);
    initArgs(&Args);
    
    if((FaceHd = malloc(sizeof(header_t))) == NULL) return -1;
    FaceHd->qlen = 0;
    FaceHd->next = NULL;
    loadFaces(FaceHd);
    
    
    RunUserInterface ();
    DiscardPanel (mypanel);                                         
    return 0;
}

/***** Load An Src Image *****/
int CVICALLBACK Load_An_Image (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    initBasics(&Basics);
    
    switch (event) {
        case EVENT_COMMIT:
            
            Basics.ret = FileSelectPopup ("", "*.jpg", "*.*", "Load an image: select an image",
                VAL_LOAD_BUTTON, 0, 0, 1, 0, Basics.path);
            
            
            strncpy(Args.path, Basics.path, strlen(Basics.path));
            
            if(Basics.ret == VAL_EXISTING_FILE_SELECTED) {
                
                imaqReadFile(Imgs.src, Basics.path, NULL, NULL);
                imaqSetWindowTitle(0, "Load an image");
                
                imaqMoveWindow(0, imaqMakePoint(16, 32));
                imaqDisplayImage(Imgs.src, 0, TRUE);
            }
            break;
    }
    return 0;
}


int compesateLight(Image *dest, Image *src, int width, int height)
{
    int x = 0;
    int y = 0;
    
    ImageInfo info; 
    imaqGetImageInfo(src, &info);
    
    return 0;
}

/***** Test for a better separation *****/

#if TEST == 1
int CVICALLBACK Separate_Figure (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    float U = 0.0;
    float V = 0.0;
    float I = 0.0;
    float h = 0.0;
    
    initGrids(&Grids);
    initBasics(&Basics);
    
    switch (event) {
        case EVENT_COMMIT:
            
            imaqExtractColorPlanes(Imgs.src, IMAQ_RGB, Imgs.red, Imgs.green, Imgs.blue);
            imaqGetImageSize(Imgs.src, &(Grids.width), &(Grids.height));
            
            //imaqMorphology(Imgs.RGB, Imgs.src, IMAQ_OPEN, NULL);   
            imaqCast (Imgs.U8, Imgs.src, IMAQ_IMAGE_U8, NULL, 8);
#if DEBUG
                printf("height----%d----width-----%d-----\n", Grids.height, Grids.width);
                imaqSetWindowTitle(15, "Separate figure: Imgs.U8 cast");
                
                imaqMoveWindow(15, imaqMakePoint(512, 384));
                imaqDisplayImage(Imgs.U8, 15, TRUE);
#endif           
            for(Grids.x = 1; Grids.x < Grids.width; Grids.x++) {
             
                for(Grids.y = 1; Grids.y < Grids.height; Grids.y++) {
            
                    imaqGetPixel(Imgs.src, imaqMakePoint(Grids.x, Grids.y), &(Basics.RGBvalue));
#if DEBUG
                    printf("R----%d-----G------%d-------B------%d\n", Basics.RGBvalue.rgb.R,
                        Basics.RGBvalue.rgb.G, Basics.RGBvalue.rgb.B);
#endif           
                    
                    U = fabs((-0.148) * Basics.RGBvalue.rgb.R + (-0.289) * Basics.RGBvalue.rgb.G + 0.437 * Basics.RGBvalue.rgb.B);
                    V = fabs(0.615 *Basics.RGBvalue.rgb.R + (-0.515) * Basics.RGBvalue.rgb.G + (-0.1) * Basics.RGBvalue.rgb.B);
                    I = 0.596 * Basics.RGBvalue.rgb.R + (-0.274) * Basics.RGBvalue.rgb.G + (-0.322) * Basics.RGBvalue.rgb.B;
                    h = (180/3.14) * atan(U / V);
                    
                    if((h >= 12) && (h <= 60) && (I >= 15) && (I <= 100)) {
                        Basics.grayvalue.grayscale = 255.0;
                        imaqSetPixel(Imgs.U8, imaqMakePoint(Grids.x, Grids.y), Basics.grayvalue);
                    } else {
                        Basics.grayvalue.grayscale = 0.0;
                        imaqSetPixel(Imgs.U8, imaqMakePoint(Grids.x, Grids.y), Basics.grayvalue);
                    }
                }
            }
            
            imaqNthOrderFilter(Imgs.sepa, Imgs.U8, 3, 3, 5, NULL);
            imaqLowPass (Imgs.sepa, Imgs.sepa, 3, 3, 8.0, NULL);
            
            sprintf(Basics.prompt, "Separate figure from landscape (height x width): %d x %d", Grids.height, Grids.width);
            imaqSetWindowTitle(1, Basics.prompt);
            imaqMoveWindow(1, imaqMakePoint(48, 64));
            imaqDisplayImage(Imgs.sepa, 1, TRUE);
            
            break;
    }
    return 0;
} 

#else /************************************************************************/
int CVICALLBACK Separate_Figure (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    float Y = 0.0;
    float U = 0.0;
    float V = 0.0;
    float h = 0.0;
    
    initGrids(&Grids);
    initBasics(&Basics);
    
    switch (event) {
        case EVENT_COMMIT:
            
            imaqExtractColorPlanes(Imgs.src, IMAQ_RGB, Imgs.red, Imgs.green, Imgs.blue);
            imaqGetImageSize(Imgs.src, &(Grids.width), &(Grids.height));
            
            imaqCast (Imgs.U8, Imgs.src, IMAQ_IMAGE_U8, NULL, 8);
#if DEBUG
                printf("height----%d----width-----%d-----\n", Grids.height, Grids.width);
                imaqSetWindowTitle(15, "Separate figure: Imgs.U8 cast");
                
                imaqMoveWindow(15, imaqMakePoint(512, 384));
                imaqDisplayImage(Imgs.U8, 15, TRUE);
#endif           
            for(Grids.x = 1; Grids.x < Grids.width; Grids.x++) {
             
                for(Grids.y = 1; Grids.y < Grids.height; Grids.y++) {
            
                    imaqGetPixel(Imgs.src, imaqMakePoint(Grids.x, Grids.y), &(Basics.RGBvalue));
#if DEBUG
                    printf("R----%d-----G------%d-------B------%d\n", Basics.RGBvalue.rgb.R,
                        Basics.RGBvalue.rgb.G, Basics.RGBvalue.rgb.B);
#endif           

                    Y = (4899 * Basics.RGBvalue.rgb.R + 9617 * Basics.RGBvalue.rgb.G + 1868 * Basics.RGBvalue.rgb.B) / 16384;
                    U = (9241 * (Basics.RGBvalue.rgb.B - Y) + 8192) / 16384 + 128;
                    V = (9241 * (Basics.RGBvalue.rgb.R - Y) + 8192) / 16384 + 128;

                    if(Y < 108) {
                      Basics.grayvalue.grayscale = 0.0;
                      imaqSetPixel(Imgs.U8, imaqMakePoint(Grids.x, Grids.y), Basics.grayvalue);
                    } else 
                        if((U >= 77) && (U <= 127) && (V >= 133) && (V <= 155)) {
                        Basics.grayvalue.grayscale = 255.0;
                        imaqSetPixel(Imgs.U8, imaqMakePoint(Grids.x, Grids.y), Basics.grayvalue);
                    } else {
                        Basics.grayvalue.grayscale = 0.0;
                        imaqSetPixel(Imgs.U8, imaqMakePoint(Grids.x, Grids.y), Basics.grayvalue);
                    }
                }
            }
            /***** additional choice: find the border after edge detetcion, but it is hard to combine two method *****/
            //imaqEdgeFilter(Imgs.sepa, Imgs.sepa, IMAQ_EDGE_SOBEL, IMAQ_IMAGE_U8);
            
            imaqNthOrderFilter(Imgs.sepa, Imgs.U8, 3, 3, 5, NULL);
            imaqLowPass (Imgs.U8, Imgs.sepa, 3, 3, 8.0, NULL);
            
            sprintf(Basics.prompt, "Separate figure from landscape (height x width): %d x %d", Grids.height, Grids.width);
            imaqSetWindowTitle(1, Basics.prompt);
            imaqMoveWindow(1, imaqMakePoint(48, 64));
            imaqDisplayImage(Imgs.sepa, 1, TRUE);
            
            break;
    }
    return 0;
}
#endif      

int CVICALLBACK Set_Horizon_Border (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    initBasics(&Basics);
    initGrids(&Grids);
    
    switch (event) {
        case EVENT_COMMIT:
            
            imaqGetImageSize(Imgs.sepa, &Grids.width, &Grids.height);
            imaqDuplicate(Imgs.U8, Imgs.sepa);

            for(Grids.x = 1; Grids.x < Grids.width; Grids.x++) {
                Infos.ColPxlSum[Grids.x] = 0;
                for(Grids.y = 1;  Grids.y < Grids.height; Grids.y++) {
                    imaqGetPixel(Imgs.U8, imaqMakePoint(Grids.x, Grids.y), &Basics.grayvalue);
                    Infos.ColPxlSum[Grids.x] += Basics.grayvalue.grayscale;
                }
            }
            
            for(Grids.x = HSTART; Grids.x < Grids.width - HSTART; Grids.x++) {
                if(Infos.ColPxlSum[Grids.x] > 10000) Idxs.HrzMin = Grids.x;
            }
            
            for(Grids.x = Grids.width - HSTART; Grids.x > HSTART; Grids.x--) {
                if(Infos.ColPxlSum[Grids.x] > 10000) Idxs.HrzMax = Grids.x;
            }
        
            Basics.ret = Idxs.HrzMin - Idxs.HrzMax;
            Idxs.HrzMin -= Basics.ret / 6;
            Idxs.HrzMax += Basics.ret / 6;
            
            DeleteGraphPlot(mypanel, MYPANEL_HORIZONTAL_HISTOGRAM, -1, VAL_IMMEDIATE_DRAW);
            Infos.report = imaqHistogram(Imgs.U8, 256, 0.0, 255.0, IMAQ_IMAGE_U8);
            PlotY(mypanel, MYPANEL_HORIZONTAL_HISTOGRAM , Infos.report->histogram, 256,
                VAL_UNSIGNED_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            
            /***** Set Ctrl Val to Horizontal HistoGram *****/
            SetCtrlVal(mypanel, MYPANEL_LEFT_BORDER, Idxs.HrzMax);
            SetCtrlVal(mypanel, MYPANEL_RIGHT_BORDER, Idxs.HrzMin);
            
            imaqDrawLineOnImage(Imgs.U8, Imgs.U8, IMAQ_DRAW_VALUE, imaqMakePoint(Idxs.HrzMax, 1),
                imaqMakePoint(Idxs.HrzMax, Grids.height), 255.0);
            imaqDrawLineOnImage(Imgs.U8, Imgs.U8, IMAQ_DRAW_VALUE, imaqMakePoint(Idxs.HrzMin, 1),
                imaqMakePoint(Idxs.HrzMin, Grids.height), 255.0);
            
            Grids.width = Idxs.HrzMin - (Idxs.HrzMax + 1);
            imaqScale(Imgs.HrzEdge, Imgs.U8, 1, 1, IMAQ_SCALE_SMALLER, imaqMakeRect(0, Idxs.HrzMax + 1, Grids.height, Grids.width)); 

            sprintf(Basics.prompt, "Set_Horizon_Border (h * w) (min to max): %d * %d, %d - %d",
                Grids.height, (Idxs.HrzMin - Idxs.HrzMax), Idxs.HrzMin, Idxs.HrzMax);
            imaqSetWindowTitle(2, Basics.prompt);
            imaqMoveWindow(2, imaqMakePoint(80, 96));
            imaqDisplayImage(Imgs.HrzEdge, 2, TRUE);
            
            break;
    }
    
    imaqDispose(Infos.report);
    return 0;
}

int CVICALLBACK Set_Vertical_Border (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    initBasics(&Basics);
    initGrids(&Grids);
    int ymax = 0;
    int ymin = 0;
    
    switch (event) {
        case EVENT_COMMIT:
            
            Infos.eyes = 0;
            imaqGetImageSize(Imgs.HrzEdge, &Grids.width, &Grids.height);
            ymax = 50 * Grids.height / 100;
            ymin = 25 * Grids.height / 100;
            imaqDuplicate(Imgs.U8, Imgs.HrzEdge);
            
            for(Grids.y = 1; Grids.y < Grids.height; Grids.y++) {
                Infos.RowPxlSum[Grids.y] = 0.0;
                for(Grids.x = 1; Grids.x < Grids.width; Grids.x++) {
                    imaqGetPixel(Imgs.U8, imaqMakePoint(Grids.x, Grids.y), &Basics.grayvalue);
                    Infos.RowPxlSum[Grids.y] += Basics.grayvalue.grayscale;
                }
            }
            
            ymin = 35 * Grids.height / 100;
            ymax = 43 * Grids.height / 100;
            
            for(Grids.y = ymin; Grids.y < ymax; Grids.y++) {
             if((Infos.RowPxlSum[Grids.y] > Infos.RowPxlSum[Grids.y + 1]) 
                 && (Infos.RowPxlSum[Grids.y + 1] < Infos.RowPxlSum[Grids.y + 2]))
                Infos.eyes = Grids.y + 1; 
            }
            
            ymin = 54 * Grids.height / 100;
            ymax = 64 * Grids.height / 100;

            for(Grids.y = ymin; Grids.y < ymax - 1; Grids.y++) {
             if((Infos.RowPxlSum[Grids.y] > Infos.RowPxlSum[Grids.y + 1]) 
                 && (Infos.RowPxlSum[Grids.y + 1] < Infos.RowPxlSum[Grids.y + 2]))
                Infos.mouth = Grids.y + 1; 
            }

            if(Infos.mouth == 0) {
                for(Grids.y = ymin; Grids.y < ymax - 1; Grids.y++) {
                 if((Infos.RowPxlSum[Grids.y - 1] > Infos.RowPxlSum[Grids.y + 1]) 
                     && (Infos.RowPxlSum[Grids.y + 1] < Infos.RowPxlSum[Grids.y + 2]))
                    Infos.mouth = Grids.y + 1; 
                }
            }

            Basics.ret = Idxs.HrzMin - Idxs.HrzMax;
            Idxs.VtcMax = Infos.eyes - 0.25 * Basics.ret;
            Idxs.VtcMin = Idxs.VtcMax + 1.25 * Basics.ret;
            
            SetCtrlVal(mypanel, MYPANEL_TOP_BORDER, Idxs.VtcMax);
            SetCtrlVal(mypanel, MYPANEL_BASE_BORDER, Idxs.VtcMin);
            
            DeleteGraphPlot(mypanel, MYPANEL_VERTICAL_HISTOGRAM , -1, VAL_IMMEDIATE_DRAW);
            Infos.report = imaqHistogram(Imgs.U8, 256, 0.0, 255.0, IMAQ_IMAGE_U8);
            PlotY(mypanel, MYPANEL_VERTICAL_HISTOGRAM  , Infos.report->histogram, 256,
                VAL_UNSIGNED_INTEGER, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);

            imaqDrawLineOnImage(Imgs.U8, Imgs.U8, IMAQ_DRAW_VALUE, imaqMakePoint(1, Idxs.VtcMax),
                imaqMakePoint(Grids.width, Idxs.VtcMax), 255.0);
            imaqDrawLineOnImage(Imgs.U8, Imgs.U8, IMAQ_DRAW_VALUE, imaqMakePoint(1, Idxs.VtcMin),
                imaqMakePoint(Grids.width, Idxs.VtcMin), 255.0);
            
            Grids.height = Idxs.VtcMin - (Idxs.VtcMax + 1);
            imaqScale(Imgs.VtcEdge, Imgs.U8, 1, 1, IMAQ_SCALE_SMALLER, 
                imaqMakeRect(Idxs.VtcMax + 1, 0, Grids.height, Grids.width)); 

            sprintf(Basics.prompt, "Set_Vertical_Border (min to max): %d - %d", Idxs.VtcMin, Idxs.VtcMax);
            imaqSetWindowTitle(3, Basics.prompt);
            imaqMoveWindow(3, imaqMakePoint(112, 128));
#if DEBUG
            printf("maxv-- %d --maxi-- %d----minv--- %d ---mini---- %d --ymin--- %d "
                "---ymax---- %d ---eye--%d---mouth----%d-----\n", (int)Infos.maxval, 
                Infos.maxidx, (int)Infos.minval, Infos.minidx, ymin, ymax, Infos.eyes, Infos.mouth);
#endif    
            imaqDisplayImage(Imgs.VtcEdge, 3, TRUE);
            
            break;
    }
    
    imaqDispose(Infos.report);
    return 0;
}

int CVICALLBACK Detect_Face (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    initBasics(&Basics);
    initGrids(&Grids);
    
    switch (event) {
        case EVENT_COMMIT:
         
            Grids.height = Idxs.VtcMin - Idxs.VtcMax;
            Grids.width = Idxs.HrzMin - Idxs.HrzMax;
             /***** Draw a rectangle contained face *****/  

            imaqDuplicate(Imgs.U8, Imgs.VtcEdge);
            
            Args.width = Grids.width;
            Args.delta = Infos.mouth - Infos.eyes;
            
            sprintf(Basics.prompt, "Detect Face on rectangle(height x width): %d x %d", Grids.height, Grids.width);
            imaqSetWindowTitle(4, Basics.prompt);
            imaqMoveWindow(4, imaqMakePoint(144, 160));
            
            Basics.ret = Infos.eyes - Idxs.VtcMax;
#if DEBUG
            printf("Detect Face: eyes on rect-----%d-----\n", Basics.ret);
            imaqDrawLineOnImage(Imgs.U8,Imgs.U8, IMAQ_DRAW_VALUE ,
                MakePoint(0, Basics.ret), MakePoint(Grids.width, Basics.ret), 0.0);
#endif
            for(Grids.x = 1; Grids.x < Grids.width - 1; Grids.x++) {
             imaqGetPixel(Imgs.U8, MakePoint(Grids.x, Basics.ret), &Basics.grayvalue);
             if(Basics.grayvalue.grayscale == 0.0) Args.eyesarea++;
            }
            
            
            Basics.ret = Infos.mouth - Idxs.VtcMax; 
#if DEBUG
            printf("Detect Face: mouth on rect-----%d-----\n", Basics.ret);
            imaqDrawLineOnImage(Imgs.U8,Imgs.U8, IMAQ_DRAW_VALUE ,
                MakePoint(0, Basics.ret), MakePoint(Grids.width, Basics.ret), 0.0);
#endif
            for(Grids.x = 1; Grids.x < Grids.width - 1; Grids.x++) {
             imaqGetPixel(Imgs.U8, imaqMakePoint(Grids.x, Basics.ret), &Basics.grayvalue);
             if(Basics.grayvalue.grayscale == 0.0) Args.moutharea++;
            }
#if DEBUG            
            printf("Detect Face(Args): width: %d----delta: %d---eyesarea: %d----moutharea: %d----\n",
                Args.width, Args.delta, Args.eyesarea, Args.moutharea);
#endif
            imaqDisplayImage(Imgs.U8, 4, TRUE);
        break;
    }
    return 0;
}


int CVICALLBACK Save_Facevalue (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    FILE *fp = NULL;
    face_t *face = NULL;
    char *ch = NULL;
    char *p = NULL;
    int i = 0;
    int n = 0;
    
    switch (event) {
        case EVENT_COMMIT:
            face = initFace(sizeof(face_t));

            ch = Args.path;
            n = strlen(Args.path) - 1;
            ch += n;
            p = ch;
            for(i = 0;i <=n; i++){ 
                while(*p != '\\') p--;
            }
            strncpy(face->code, p + 1, ch -p);
            
            face->width = Args.width;
            face->delta = Args.delta;
            face->eyesarea = Args.eyesarea;
            face->moutharea = Args.moutharea;
            face->next = NULL;
            
            if(findFace(FaceHd, face, SAVEFLAG) == 0) {
                printf("Save face value: find face: your face already in face database, your code is %s\n", face->code);
                return 0;
            }
            
            if(insertFace(FaceHd, face) == -1) {
                printf("Save face value: insert face: failed to insert face node\n");
                return -1;
            }
            
           /***** save face values struct into face.db *****/  
            if((face = FaceHd->next) == NULL ) {  
                 printf("Save face value: error: no face node on queue to save\n");
                 return -1;
            }

            if((fp = fopen(FACESDB, "w")) == NULL) return -1;

                while(face != NULL ) {
#if DEBUG
        printf("Save face(value): code--%s--width--%d--delta--%d--eyesarea--%d--moutharea--%d--\n", 
            face->code, face->width, face->delta,face->eyesarea, face->moutharea); 
#endif
                    //if(fprintf(fp, "code:%s,width:%d,delta:%d,eyesarea:%d,moutharea:%d\n",
                      if(fprintf(fp, "%s\n%d\n%d\n%d\n%d\n\n",
                        face->code,face->width, face->delta, face->eyesarea, face->moutharea) == -1) 
                        return -1;

                    memset(face->code, '\0', PATHLEN);
                    face = face->next;
                }
             fclose(fp);
             break;
    }
    return 0;
}


int CVICALLBACK Face_Recognition (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    face_t *face = NULL;
    face = initFace(sizeof(face_t));
    
    switch (event) {
        case EVENT_COMMIT:
            
            face->width = Args.width;
            face->delta = Args.delta;
            face->eyesarea = Args.eyesarea;
            face->moutharea = Args.moutharea;
            face->next = NULL;
            
            if(findFace(FaceHd,face, RECOFLAG) == 0) printf("Recognize face: find face: your code is %s\n", face->code);
            else printf("Recognize face: find face: unrecognized, your face are not recorded\n");
            break;
    }
    return 0;
}


static int handle = 0; 
static HWND ret;
int CVICALLBACK Open_Camera (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    initBasics(&Basics);
    initGrids(&Grids);
    
    switch (event) {
        case EVENT_COMMIT:

         GetPanelAttribute (mypanel, ATTR_SYSTEM_WINDOW_HANDLE, &handle);

         GetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_LEFT, &Grids.x);
         GetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_TOP, &Grids.y);
         GetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_HEIGHT, &Grids.height);
         GetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_WIDTH, &Grids.width);

        ret = capCreateCaptureWindowA("", WS_CHILD | WS_VISIBLE, Grids.x, Grids.y,
            Grids.width, Grids.height, (HWND)handle, 0);
        
        if(ret != NULL) {
        SendMessage(ret, WM_CAP_SET_CALLBACK_VIDEOSTREAM, 0, 0);
        SendMessage(ret, WM_CAP_SET_CALLBACK_ERROR, 0, 0);
        SendMessage(ret, WM_CAP_SET_CALLBACK_STATUSA, 0, 0);
        SendMessage(ret, WM_CAP_DRIVER_CONNECT, 0, 0);
        SendMessage(ret, WM_CAP_SET_SCALE, 1, 0);
        SendMessage(ret, WM_CAP_SET_PREVIEWRATE, 66, 0);
        SendMessage(ret, WM_CAP_SET_OVERLAY, 1, 0);
        SendMessage(ret, WM_CAP_SET_PREVIEW, 1, 0);
        }
        break;
    }
    return 0;
}

int CVICALLBACK Capture (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
     int bmpid = 0;
     initBasics(&Basics);

     switch (event)
      {
        case EVENT_COMMIT:

        Basics.ret = FileSelectPopup ("", "*.jpg", "*.*", "Capture_from_camera: save image", 
            VAL_SAVE_BUTTON, 0, 0, 1, 1, Basics.path);

        if (Basics.ret != VAL_NO_FILE_SELECTED){
            
           SendMessage(ret, WM_CAP_FILE_SAVEDIBA, 0, (LPARAM)Basics.path);
           GetBitmapFromFile(Basics.path, &bmpid);
           CanvasDrawBitmap (mypanel, MYPANEL_MYCANVAS, bmpid, VAL_ENTIRE_OBJECT, VAL_ENTIRE_OBJECT);
           DiscardBitmap (bmpid);
          } 
        break;
    }
    return 0;
}

int CVICALLBACK Close_Camera (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) {
        case EVENT_COMMIT:
            SendMessage(ret, WM_CAP_DRIVER_DISCONNECT, 0, 0);
            break;
    }
    return 0;
}

int CVICALLBACK Resolution (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    initBasics(&Basics);
    initGrids(&Grids);
    
    switch (event) {
        case EVENT_COMMIT:
    
            GetCtrlVal (mypanel, MYPANEL_RESOLUTIONRING , &handle);
            switch (handle) {
              
                 case 0:
                    SetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_WIDTH, 300);
                    SetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_HEIGHT, 200);
                 break;
         
                 case 1:
                    SetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_WIDTH, 360);
                    SetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_HEIGHT, 240);
                 break;
         
                 case 2:
                    SetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_WIDTH, 450);
                    SetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_HEIGHT, 300);
                 break;
         
                 case 3:
                    SetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_WIDTH, 480);
                    SetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_HEIGHT, 320);
                 break;
             }
            
             GetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_LEFT, &Grids.x);
             GetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_TOP, &Grids.y);
             GetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_HEIGHT, &Grids.height);
             GetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_WIDTH, &Grids.width);
             
             SetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_TOP, 24);
             SetCtrlAttribute (mypanel, MYPANEL_MYCANVAS, ATTR_LEFT, 8);
             
             SendMessage(ret, WM_CAP_DRIVER_DISCONNECT, 0, 0);
             
             Open_Camera(panel, MYPANEL_MYCANVAS, EVENT_COMMIT, NULL, 0, 0);
        break;
    }
       
   return 0;
}


int CVICALLBACK Quit (int panel, int control, int event,
        void *callbackData, int eventData1, int eventData2)
{
    switch (event) {
        case EVENT_COMMIT:
            
            disposeImages(&Imgs);
            disposeFaces(FaceHd);
            QuitUserInterface (0);
            break;
    }
    return 0;
}

