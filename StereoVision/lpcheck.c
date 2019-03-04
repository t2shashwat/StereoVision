//Set correct filter values

#include<stdio.h>
#include "exponent_int.c"
#include "remap_gray.c"

int a,b;
int exponent(int base,int exponent);

const int M = 256;
const int N = 256;
int outputf[256][256][10]={0};//output of the gaussian pyramid.
int lap[256][256][10]={0};//output of the gaussian pyramid.

void laplacian_pyramid(int m,int n,int Iremap[][n])
{
	printf("Reached\n");
	int nlev=0;
	int i,j,k,l,temp=0;
	int output[256][256][10]={0};
	int outputl[256][256][10]={0};//Complete Gaussain pyramid of this image is stored in this array
	//Assigning pixels to the image  
	for(i=0;i<m;i++){
		for(j=0;j<n;j++){
			output[i][j][0] = Iremap[i][j];
			outputl[i][j][0] = Iremap[i][j];
		}
	}
	printf("Reached1\n");
	//Scanning and assigning input values to the gaussian filter
	int a=2,b=2;//a,b are the rows and colunms of the gaussian filter
	int filter[a][b];
	for(i=0;i<a;i++){
		for(j=0;j<b;j++){
			filter[i][j] = 1;
		}
	}
	printf("Reached2\n");
	for(nlev = 0; nlev<10 ;nlev++){//nlev<6 for 16*16
		//Multiply and add function(Smoothing function)
		for(l=0;l<(m/expo_int(2,nlev))-a+1;l++){
			for(k=0;k<(n/expo_int(2,nlev))-b+1;k++){
				for(i=0;i<a;i++){
					for(j=0;j<b;j++){
						temp = temp + output[i+l][j+k][nlev]*filter[i][j];
					}
				}
				output[l][k][nlev] = temp;
				temp = 0;
			}
		}
		printf("Reached3\n");
		//Downsampling the output
		for(i=0;i<(m/expo_int(2,nlev+1));i++){
			for(j=0;j<n/expo_int(2,nlev+1);j++){
				output[i][j][nlev+1] = (output[i*2][j*2][nlev]+output[i*2][(j*2)+1][nlev]+output[(i*2)+1][j*2][nlev]+output[(i*2)+1][(j*2)+1][nlev])/4;
				outputl[i][j][nlev+1] = output[i][j][nlev+1];
			}
		}
	}
	printf("GaussianComplete\n");
	//Till here is gaussian pyramid construction which is already done in other code.Now upsample and subtract.
	//Upsampling the gaussian pyamid levels according to how they are to be subtracted
	for(nlev=0;nlev<8;nlev++){//nlev<4 16*16
		for(i=0;i<m/expo_int(2,nlev+1);i++){
			for(j=0;j<n/expo_int(2,nlev+1);j++){
				output[i*2][j*2][nlev+1] = outputl[i][j][nlev+1];		
				output[i*2][j*2+1][nlev+1] = outputl[i][j][nlev+1];		
				output[i*2+1][j*2][nlev+1] = outputl[i][j][nlev+1];		
				output[i*2+1][j*2+1][nlev+1] = outputl[i][j][nlev+1];	
			}
		}
	}
	printf("Reached4\n");
	//Substraction of gaussian levels or construction of laplacian
	for(nlev=0;nlev<9;nlev++){//nlev<5 for 16*16
		for(i=0;i<m/expo_int(2,nlev);i++){
			for(j=0;j<n/expo_int(2,nlev);j++){
				if(nlev<8)lap[i][j][nlev] = output[i][j][nlev+1] - outputl[i][j][nlev];//nlev<4 for 16*16
				if(nlev==8)lap[i][j][nlev] = outputl[i][j][nlev];//nlev==4 for 16*16
			}
		}
	}
	printf("Reached5\n");
}

void gaussian_pyramid(int input_image[M][N]){
int nlev=0;
int i=0,j=0,k,l,temp=0,inp;
int output[256][256][10]={0};
//Assigning pixels to the image  
for(i=0;i<M;i++){
	for(j=0;j<N;j++){
		output[i][j][0] = input_image[i][j];
		outputf[i][j][0] = input_image[i][j];
	}
}
//Assigning input values to the filter
int a=2,b=2;//{a,b=rows,colunms}
float filter[a][b];
for(i=0;i<a;i++){
	for(j=0;j<b;j++){
		filter[i][j] = 1;
	}
}
printf("\n");
for(nlev = 0; nlev<10 ;nlev++){
	//Multiply and add function(Smoothing function)
	for(l=0;l<(M/expo_int(2,nlev))-a+1;l++){
		for(k=0;k<(N/expo_int(2,nlev))-b+1;k++){
			for(i=0;i<a;i++){
				for(j=0;j<b;j++){
					temp = temp + output[i+l][j+k][nlev]*filter[i][j];
				}
			}
			output[l][k][nlev] = temp;
			temp = 0;
		}
	}
	//Downsampling the output
	for(i=0;i<(M/expo_int(2,nlev+1));i++){
		for(j=0;j<N/expo_int(2,nlev+1);j++){
			output[i][j][nlev+1] = (output[i*2][j*2][nlev]+output[i*2][(j*2)+1][nlev]+output[(i*2)+1][j*2][nlev]+output[(i*2)+1][(j*2)+1][nlev])/4;
			outputf[i][j][nlev+1] = output[i][j][nlev+1];
		}
	}
}
}

int main()
{
	int dim = 256;//Dimension of input image
	int nlev=0;
	//	printf("Dimensions of input image : %d*%d\n",dim,dim);
	int k = 10;//Max levels in the output laplacian pyramid for 16*16 '6' was written
	int input_image[256][256] = {0};//Instead of this you have to get it to read a text file.
	//	printf("%d%d%d%d\n",input_image[0][0],input_image[0][1],input_image[1][0],input_image[1][1]);
	int lev0;//current level
	int lap_pyr[256][256][10]={0};
	int x0,y0;//Coordinates of the pixel at the current level
	int hw;//%half width of full resolution footprint.
	int coordlimit[9] = {256,128,64,32,16,8,4,2,1};
	int yf,xf;//Coordintes in full resolution image corresponding to (lev0,x0,y0)
	int yrng[2],xrng[2];//Range of the sub image required to evaluate(lev0,x0,y0) in result//Not changed when the value of dim is changed.
	int Isub[256][256] = {0};//Maximum Sub image(size of input image) size needed to evaluate the pixel
	int Iremap[256][256]= {0};//Remapped image after evaluating the subimage
	int i,j,l=0,m=0;
	int count=0;
	int g0;
	int yfc,xfc;
	int yfclev0=0;
	int xfclev0=0;
	int inp;
	FILE *fp;
	fp = fopen("imagenew.txt","r");
	fscanf(fp,"%d",&inp);
	//Assigning pixels to the image  
	for(i=0;i<256;i++){
		for(j=0;j<256;j++){
			input_image[i][j] = inp;
		//	printf("%d ",input_image[i][j]);
			fscanf(fp,"%d",&inp);
			if(feof(fp))break;
		}
	//	printf("\n");
	}
	fclose(fp);
	gaussian_pyramid(input_image);
	for(lev0=0;lev0<10;lev0++){//lev0<k
		hw = 3*expo_int(2,lev0+1)-2;
		count = 0;
		for(y0=0;y0<coordlimit[lev0];y0++){//y0 <= coordlimit[lev0]
			for(x0=0;x0<coordlimit[lev0];x0++){//x0 <= coordlimit[lev0]				
				yf = (y0)*expo_int(2,lev0) + 1;//y0-1,lev0-1
				xf = (x0)*expo_int(2,lev0) + 1;//x0-1,lev0-1
				l=0;m=0;
				yrng[0] = (yf-hw>1)?yf-hw-1:0;
				yrng[1] = (dim<yf+hw-1)?dim-1:yf+hw-1;
				xrng[0] = (xf-hw>1)?xf-hw-1:0;
				xrng[1] = (dim<xf+hw-1)?dim-1:xf+hw-1;
				for(i = xrng[0];i<xrng[1]+1;i++){
					for(j = yrng[0];j<yrng[1]+1;j++){
						Isub[l][m] = input_image[i][j];
						m++;
					}
					l++;
					m=0;
				}
				l=0;m=0;
				g0 = outputf[x0][y0][lev0];
				for(i=0;i<dim;i++){
					for(j=0;j<dim;j++){
						Iremap[i][j] = remap_gray(Isub[i][j],g0);
					}	
				}
				printf("Reached\n");
				laplacian_pyramid(xrng[1]-xrng[0]+1,yrng[1]-yrng[0]+1,Iremap);
				yfc = yf - yrng[0] + 1;
				xfc = xf - xrng[0] + 1;
				yfclev0 = (yfc-1)/expo_int(2,lev0)+1;
				xfclev0 = (xfc-1)/expo_int(2,lev0)+1;
				lap_pyr[x0][y0][lev0]=lap[xfclev0][yfclev0][lev0];
			}
		}
	}
	//Instead of using tempa using lap because after the calculations in the above loop it will become redundant
	for(nlev=0;nlev<9;nlev++){//nlev<5 for 16*16
		for(i=0;i<256/expo_int(2,9-(nlev+1));i++){
			for(j=0;j<256/expo_int(2,9-(nlev+1));j++){
				lap[i*2][j*2][9-(nlev+1)]=lap_pyr[i][j][9-(nlev+1)];
				lap[i*2][j*2+1][9-(nlev+1)]=lap_pyr[i][j][9-(nlev+1)];
				lap[i*2+1][j*2][9-(nlev+1)]=lap_pyr[i][j][9-(nlev+1)];
				lap[i*2+1][j*2+1][9-(nlev+1)]=lap_pyr[i][j][9-(nlev+1)];
			}
		}
		printf("here\n");
		if(nlev<8){//nlev<4 for 16*16
			for(i=0;i<256/expo_int(2,(9-(nlev+2)));i++){
				for(j=0;j<256/expo_int(2,(9-(nlev+2)));j++){
					lap_pyr[i][j][9-(nlev+2)]=lap[i][j][9-(nlev+1)]+lap_pyr[i][j][9-(nlev+2)];
				}
			}
		}
		if(nlev==7)break;
	}
	printf("Final recontructed LP\n");
	for(i=0;i<256;i++){
		for(j=0;j<256;j++){
			printf("%d ",lap_pyr[i][j][0]);
		}
		printf("\n");
	}
	printf("\n");
}

