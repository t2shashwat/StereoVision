#include<stdio.h>
#include<math.h>
#include <stdlib.h>
#include<time.h>
int main() {
  /* code */
double elapsed;
clock_t start, end;
start = clock();
//srand(time(NULL));
  //input is in greyscale
//parametrizing the code
//=================================
  int maximg_height=500;
  int maximg_width=500;
  int template_size=7;
  int disparity_range=50;
//====================================
  int imgHeight=375;
  int imgWidth=450;

  float l_image[500][500]={0};
  float r_image[500][500]={0};
  float template[7][7]={0};
  float block[7][7]={0};
  float diff_mat[7][7]={0};
  float DbasicSubpixel[375][450]={0};
  float inp;
  int i,j,blk,m,n;
  int minr,minc,maxr,maxc,t_maxc,t_maxr,t_minc,t_minr,blockSize;
  int mind,maxd,temp_row_len,temp_col_len,numBlocks,blockIndex;
  int min_val_index,bestMatchIndex,d;
  float c1,c2,c3;
  FILE *fp;
	fp = fopen("l_image.txt","r");
	fscanf(fp,"%f",&inp);
	//Assigning pixels to the image
	for(i=0;i<imgHeight;i++){
		for(j=0;j<imgWidth;j++){
			l_image[i][j] = inp;
			//printf("%f ",l_image[i][j]);
			fscanf(fp,"%f",&inp);
			if(feof(fp))break;
		}
    //printf("---");
  }
    //FILE *fp;
    fp = fopen("r_image.txt","r");
    fscanf(fp,"%f",&inp);
    //Assigning pixels to the image
    for(i=0;i<imgHeight;i++){
      for(j=0;j<imgWidth;j++){
        r_image[i][j] = inp;
      	//printf("%f ",r_image[i][j]);
        fscanf(fp,"%f",&inp);
        if(feof(fp))break;
      }
		//printf("---");
	}
  //printf("Performing basic block matching......\n");
  //for 450*375 image we are setting disparityRange as:
  int disparityRange = 50;
  int halfBlockSize = 3;
  blockSize=2*halfBlockSize+1;
  for (m = 1; m <= imgHeight; m++) {
    if((m-halfBlockSize)>1){
      minr=(m-halfBlockSize);
      }
    else minr=1;
    if ((m+halfBlockSize)>imgHeight) {
      maxr=imgHeight;
    }
    else maxr=(m+halfBlockSize);
    for (n = 1; n <=imgWidth; n++) {
      if((n-halfBlockSize)>1)
        minc=(n-halfBlockSize);
      else minc=1;
      if ((n+halfBlockSize)>imgWidth) {
        maxc=imgWidth;
      }
      else maxc=(n+halfBlockSize);
      //printf("minr==%d------maxr=%d-------minc=%d-------maxc=%d\n",minr,maxr,minc,maxc);
    mind=0;
    if((imgWidth - maxc)>disparityRange)
      maxd=disparityRange;
    else maxd=imgWidth - maxc;
    temp_row_len=maxr-minr+1;
    temp_col_len=maxc-minc+1;
    t_minr=minr;
    t_maxr=maxr;
    t_minc=minc;
    t_maxc=maxc;
    for (i = 0; i < temp_row_len; i++) {
      for (j = 0; j < temp_col_len; j++) {
      template[i][j]=r_image[t_minr-1][t_minc-1];
      t_minc++;
      }
      t_minr++;
      t_minc=minc;

    }
    //printf("temp-col_len=======================     %d\n",temp_col_len);
    /*for (i = 0; i < temp_row_len; i++) {
      for (j = 0; j < temp_col_len; j++) {
        printf("%f ",template[i][j]);
      }
      printf("--\n");

    }*/
    numBlocks = maxd - mind;
    t_minr=minr;
    t_maxr=maxr;
    t_minc=minc;
    t_maxc=maxc;
    float blockDiffs[51]={0};
    for (blk = mind; blk <= maxd; blk++) {
      t_minc=minc+blk;
      t_maxc=maxc+blk;
      t_minr=minr;
      for (i = 0; i < temp_row_len; i++) {
        for (j = 0; j < temp_col_len; j++) {
        block[i][j]=l_image[t_minr-1][t_minc-1];
        //printf("block====%.2f\n",block[i][j]);
        t_minc++;
        }
        t_minr++;
        t_minc=minc+blk;
      }
     /*for (i = 0; i < temp_row_len; i++) {
        for (j = 0; j < temp_col_len; j++) {
          printf("%f ",block[i][j]);
        }
        printf("--\n");
      }*/

      float sum=0;
      blockIndex = blk - mind;
      for (i = 0; i < temp_row_len; i++) {
        for (j = 0; j < temp_col_len; j++) {
          diff_mat[i][j]=abs(template[i][j]-block[i][j]);
          sum=sum+diff_mat[i][j];

        }
      }
      blockDiffs[blockIndex]=sum;
      //printf("value====%f --------indexx====%d\n", blockDiffs[blockIndex], blockIndex);
    }
    float min_val=10000000;
    for (i = 0; i < numBlocks; i++) {
      if (blockDiffs[i]<min_val) {
        min_val=blockDiffs[i];
        min_val_index=i;
      }
    }
    //printf("value======%f-------bestMatchIndex=======%d\n",min_val,min_val_index );
    bestMatchIndex=min_val_index;
    d=bestMatchIndex+mind;
    //printf("d===========%d\n",d);
    if ((bestMatchIndex==0)||(bestMatchIndex==(numBlocks-1))) {
      DbasicSubpixel[m-1][n-1]=d;
    }
    else{
        c1=blockDiffs[bestMatchIndex-1];
        c2=blockDiffs[bestMatchIndex];
        c3=blockDiffs[bestMatchIndex+1];
        DbasicSubpixel[m-1][n-1]=d-(0.5*(c3-c1)/(c1-(2*c2)+c3));
        //printf("vallllll==============%.5f\n",(0.5*(c3-c1)/(c1-(2*c2)+c3)));
        //printf("DbasicSubpixel==============%.4f--------id1===%d----ind2===%d\n",DbasicSubpixel[m-1][n-1],m-1,n-1 );
      }
      //printf(".................1 iteration fo m completed.......................update template\n");

  }
  if (m%10==0) {
    float val=(m/imgHeight)*100;
  //  printf("Image row %d / %d (%.0f)\n", m, imgHeight,val);
  }

}
end = clock();
elapsed = ((double) (end - start)) /CLOCKS_PER_SEC;
//printf("Calculating disparity map took %.2f min.\n", elapsed/60.0);
//printf("Displaying disparity map...\n");
for (i = 0; i < imgHeight; i++) {
  for (j = 0; j < imgWidth; j++) {
    printf("%.3f\t",DbasicSubpixel[i][j]);
  }
  printf("\n");

}
  return 0;
}
