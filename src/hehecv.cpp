#include <cv.h>
#include <highgui.h>
//这个函数用于绘制均等间距的直方图，一维或二维。
void cvxShowHistogram(char * windows_name,CvHistogram * hist){
	float v_min,v_max;
	//获取直方图的维数
	int dims=hist->mat.dims;
	int sizes[2];
	//获取直方图的bin数目
	sizes[0]=hist->mat.dim[0].size;
	sizes[1]=hist->mat.dim[1].size;
	//归一化直方图
	cvNormalizeHist(hist,1);
	//寻找直方图的最大值和最小值
	cvGetMinMaxHistValue(hist,&v_min,&v_max);
	//建立窗口
	IplImage * img=cvCreateImage(cvSize(100,100),IPL_DEPTH_8U,1);
	cvSetZero(img);
	int i;
	float scaleX=(img->width)/sizes[0];
	float scaleY;
	float histValue;
	float baseValue;
	if(dims==1){

		scaleY=(img->height);
		baseValue=1/v_max;
		for(i=0;i<sizes[0];i++){
			histValue=cvQueryHistValue_1D(hist,i);
			cvRectangle(img,cvPoint(scaleX*i,img->height-scaleY*histValue*baseValue),cvPoint(scaleX*(i+1),img->height-1),cvScalarAll(255),CV_FILLED);

		}
	}
	if(dims==2){
		baseValue=255/v_max;
		scaleY=(img->height)/sizes[1];
		int j;
		for(i=0;i<sizes[0];i++){
			for(j=0;j<sizes[1];j++)
			{
				histValue=cvQueryHistValue_2D(hist,i,j);
				cvRectangle(img,cvPoint(scaleX*i,img->height-scaleY*(j+1)),cvPoint(scaleX*(i+1),img->height-scaleY*(j)),cvScalarAll(histValue*baseValue),CV_FILLED);
			}
		}
	}
	cvShowImage(windows_name,img);

}

/*This Function create an IplImage similar to another IplImage,
if neccessary,you can change its depth and Channels type to new ones;*/
IplImage * cvxCreateImageSimilar(IplImage * src,int nChannels_new=-1,int depth_new=-1){
	if(depth_new==-1)depth_new=src->depth;
	if(nChannels_new==-1)nChannels_new=src->nChannels;
	IplImage * new_img=cvCreateImage(cvSize(src->width,src->height),depth_new,nChannels_new);
	return new_img;
}

/*This Function try  cvCreateCameraCapture on camera 2~0 ,if failed return NULL*/
CvCapture * cvxCreateCameraCaptureAuto(){
	int read_retry;
	int camera_index;
	IplImage * testFrame;
	for(camera_index=2;camera_index>=0;camera_index--){
		//ID递减，一般ID=0是笔记本自带的摄像头，这个摄像头是最后考虑的。
		std::cout<<"Read Camera ["<<camera_index<<"]";
		CvCapture * capture=cvCreateCameraCapture(camera_index);
		read_retry=0;
		do{
			std::cout<<"...";
			if(read_retry>10){
				//尝试10次读取摄像头
				std::cout<<"Failed.\n";
				break;
			}else{
				cvWaitKey(2);
				testFrame=cvQueryFrame(capture);
				read_retry++;
			}
		}while(!testFrame);
		if(testFrame){
			//直到获取到图像信息，则确认使用该摄像头
			std::cout<<"Successed!\n";
			return capture;
		}
	}
	//如果没有摄像头，返回NULL
	return NULL;
}

/*This Function Create multiple Windows,then Move and reshape them to fit the screen*/
void cvxArrangeWindows(int number_of_windows,char ** windows_name_list,CvSize & screenSize,CvSize & gridSize=cvSize(0,0)){
	int x,y,i=0;
	if(gridSize.width==0){
		//如果没有指定排布方式，则按如下方法自动获取每行和每列的窗口数。
		y=floor(sqrt((float)number_of_windows));
		x=ceil((float)number_of_windows/y);
		gridSize=cvSize(x,y);
	}
	//计算每个窗口的宽和高
	int gridWidth=(screenSize.width)/(gridSize.width);
	int gridHeight=(screenSize.height)/(gridSize.height);
	for(y=0;y<gridSize.height;y++){
		for(x=0;x<gridSize.width;x++){
			//创建，移动和调整窗口大小。
			cvNamedWindow(windows_name_list[i],CV_WINDOW_NORMAL);
			cvMoveWindow(windows_name_list[i],x*gridWidth,y*gridHeight);
			cvResizeWindow(windows_name_list[i],gridWidth,gridHeight);
			i++;
			//直到完成所有窗口
			if(i>=number_of_windows)return;
		}
	}
}

IplImage * cvxGetSubImage(IplImage * src,CvRect rect){
	CvMat * sub_mat=cvCreateMat(rect.height,rect.width,src->depth);
	IplImage * result_img=cvCreateImage(cvSize(rect.width,rect.height),src->depth,src->nChannels);
	cvGetSubRect(src,sub_mat,rect);
	cvCopy(sub_mat,result_img);
	cvReleaseMat(&sub_mat);
	return result_img;
}
void cvxCutImageByMouse_MouseCallback(int event,int x,int y,int flags,void * param){
	int * param_int=(int *)param;
	switch(event){
		case CV_EVENT_LBUTTONDOWN:{
			param_int[0]=CV_EVENT_LBUTTONDOWN;
			param_int[1]=x;
			param_int[2]=y;
		};break;
		case CV_EVENT_LBUTTONUP:{
			param_int[0]=CV_EVENT_LBUTTONUP;
			param_int[3]=x;
			param_int[4]=y;
		};break;
		default:param_int[0]=0;break;
	}
	return;
}
IplImage * cvxCutImageByMouse(IplImage * src,char * window_name="ImageCutter"){
	cvNamedWindow(window_name,CV_WINDOW_NORMAL);
	cvShowImage(window_name,src);
	int callback_param[5]={0,0,0,0,0};
	int x1,y1,x2,y2;
	cvSetMouseCallback(window_name,cvxCutImageByMouse_MouseCallback,callback_param);
    cvWaitKey(0);
	x1=callback_param[1];
	y1=callback_param[2];
	x2=callback_param[3];
 	y2=callback_param[4];
	if(x1>x2){
		int temp=x1;
		x1=x2;
		x2=temp;
	}
	if(y1>y2){
		int temp=y1;
		y1=y2;
		y2=temp;
	}
	if(x1<0)x1=0;
	if(y1<0)y1=0;
	if(x2>=src->width)x2=src->width-1;
	if(y2>=src->height)y2=src->height-1;
	IplImage * result_img=cvxGetSubImage(src,cvRect(x1,y1,x2-x1,y2-y1));
	return result_img;
}
