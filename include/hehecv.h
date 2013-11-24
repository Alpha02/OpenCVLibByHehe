
#include <cv.h>
#include <highgui.h>
//这个函数用于绘制均等间距的直方图，一维或二维。
#define CVX_1_CHANNEL 1
extern void cvxShowHistogram(char * windows_name,CvHistogram * hist);

/*This Function create an IplImage similar to another IplImage,
if neccessary,you can change its depth and Channels type to new ones;*/
extern IplImage * cvxCreateImageSimilar(IplImage * src,int nChannels_new=-1,int depth_new=-1);

/*This Function try  cvCreateCameraCapture on camera 2~0 ,if failed return NULL*/
extern CvCapture * cvxCreateCameraCaptureAuto();

/*This Function Create multiple Windows,then Move and reshape them to fit the screen*/
extern void cvxArrangeWindows(int number_of_windows,char ** windows_name_list,CvSize &screenSize,CvSize &gridSize=cvSize(0,0));
