// 20081467 lim yong seung
// A.I 2th project

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>



void	setNumber(char* tok,int axis,int position, float*** num,int* length){
	int		j = 0;
	
	while (tok != NULL){
		num[axis][position][j] = atof(tok);
		tok = strtok(NULL, "\t");
		j++;
	}

	if (axis == 0)
		length[position] = j;
}

void	contraction(float*** copy, float*** num, int* length, int* lengthCopy){

	int		i,j,k,x;
	int		min = 9999;


//to constrict data for normal value and the size will decrese 
		for (x = 0; x<2054; x++){
			min = 9999;
			for (i = 0; i<3; i++){
				copy[i][x][0] = num[i][x][0];
				copy[i][x][1] = num[i][x][1];
				copy[i][x][2] = num[i][x][2];

				k = 2; j = 3;                                 

				//delete the overlapped value
				while (j<length[x]){
					if (copy[i][x][k] != num[i][x][j])
						copy[i][x][++k] = num[i][x][j];
					j++;
				}

				if (min>k) min = k;
			}
			lengthCopy[x] = min;
		}

}

void	contractionTest(float*** copyTest, float*** test, int* testLength, int* testLengthCopy){


	int		i, j, k, x;
	int		min = 9999;


//to constrict data the size for test case and it will decrese

			for (x = 0; x<446; x++){
				min = 9999;
				for (i = 0; i<3; i++){
					copyTest[i][x][0] = test[i][x][0];
					copyTest[i][x][1] = test[i][x][1];
					copyTest[i][x][2] = test[i][x][2];
					k = 2;


					for (j = 3; j<testLength[x]; j++){
						if (copyTest[i][x][k] != test[i][x][j])
						{
							copyTest[i][x][++k] = test[i][x][j];

						}
					} if (min>k) min = k;
				}
				testLengthCopy[x] = min;

			}
}

int		 median(int length, float *** copyTest, int position, int axis){
	float	tmp = 0;
	int		i = 0;

	for (i = 0; i<length; i++){
		tmp += copyTest[axis][position][i];
	}
	tmp /= length;

	return tmp;
}

int main(){
	clock_t start = clock();

	int		i = 0, j = 0, k = 0, x = 0, y = 0, buffer = 0, bound = 0, outK = 0;
	int		length[2054];
	int		testLength[446];
	int		testLengthCopy[446];
	int	    boundCount = 0;
	int		jump = 0;
	int		d, e;
	int		lengthCopy[2054];

	bool	jumpDtw, outKK;
	
	float	dtw[160][160];
	float	cost = 0;
	float	min = 9999;
	float	such = 0;
	float	bb = 0;
	float	avg = 0;
	float	ccppAvg = 0;
	float	avgY = 0;
	float	ccppAvgY = 0;
	float	avgX = 0;
	float	ccppAvgX = 0;
	float	***num = (float ***)malloc(sizeof(float**) * 3);
	float	***test = (float ***)malloc(sizeof(float**) * 3);
	float	***copy = (float ***)malloc(sizeof(float**) * 3);
	float	***copyTest = (float ***)malloc(sizeof(float**) * 3);


	char	*tok;
	char	tmp[3000];
	char	val[2054];


	//file open basic files and chracter
	FILE	*in3 = fopen("aa3.txt", "rb");
	FILE	*in4 = fopen("aa4.txt", "rb");
	FILE	*in5 = fopen("aa5.txt", "rb");
	FILE	*fp  = fopen("aa6.txt", "rb");

	//file open for undecisioned chracter
	FILE	*test3 = fopen("test3.txt", "rb");
	FILE	*test4 = fopen("test4.txt", "rb");
	FILE	*test5 = fopen("test5.txt", "rb");

	//write the result
	FILE	*out = fopen("result3.txt", "w");

	if (!in3 || !in4 || !in5 || !test3 || !test4 || !test5) {
		fputs("File not found!\n", stderr);
		return 1;
	}


	//allocate the space for save the value from files and initialize other values
	for (k = 0; k<3; k++){
		num[k] = (float**)malloc(sizeof(float*) * 2054);
		for (i = 0; i<2054; i++)
			num[k][i] = (float*)malloc(sizeof(float) * 160);
	}

	for (k = 0; k<3; k++){
		test[k] = (float**)malloc(sizeof(float*) * 446);
		for (i = 0; i<446; i++)
			test[k][i] = (float*)malloc(sizeof(float) * 160);
	}

	for (i = 0; i<160; i++){
		dtw[i][0] = 9999;
	}

	for (i = 0; i<160; i++){
		dtw[0][i] = 9999;
	}
	for (i = 0; i < 2054; i++)
		length[i] = 0;

	dtw[0][0]	= 0;

	for (k = 0; k<3; k++){
		copy[k] = (float**)malloc(sizeof(float*) * 2054);
		for (i = 0; i<2054; i++)
			copy[k][i] = (float*)malloc(sizeof(float) * 160);
	}


	for (k = 0; k<3; k++)
	{
		copyTest[k] = (float**)malloc(sizeof(float*) * 446);
		for (i = 0; i<446; i++)
			copyTest[k][i] = (float*)malloc(sizeof(float) * 160);

	}

	//set value from x, y, z file to array	
	for (i = 0; i<2054; i++){

		fgets(tmp, 3000, in3);
		if (feof(in3))  { break; }
		tok = strtok(tmp, "\t");
		setNumber(tok, 0, i, num, length);
	}

	for (i = 0; i<2054; i++){
		fgets(tmp, 3000, in4);
		if (feof(in4))  { break; }
		tok = strtok(tmp, "\t");
		setNumber(tok, 1, i, num, length);
	}

	for (i = 0; i<2054; i++){
		fgets(tmp, 3000, in5);
		if (feof(in5))  { break; }
		tok = strtok(tmp, "\t");	
		setNumber(tok, 2, i, num, length);
	}

	//set value from unknown x,y,z file to array	
	for (i = 0; i<446; i++){
		fgets(tmp, 3000, test3);
		if (feof(test3))  { break; }
		tok = strtok(tmp, "\t");
		setNumber(tok, 0, i, test, testLength);
	}

	for (i = 0; i<446; i++){
		fgets(tmp, 3000, test4);
		if (feof(test4))  { break; }
		tok = strtok(tmp, "\t");
		setNumber(tok, 1, i, test, testLength);
	}

	for (i = 0; i<446; i++){
		fgets(tmp, 3000, test5);
		if (feof(test5))  { break; }
		tok = strtok(tmp, "\t");
		setNumber(tok, 2, i, test, testLength);
	}


	// set the chracter
	for (i = 0; i<2054; i++){
		fscanf(fp, "%c\n", &val[i]);
	}


	//to improve the capacity
	contraction(copy,num,length,lengthCopy);
	contractionTest(copyTest, test, testLength, testLengthCopy);


	//core algorithm with Dynamic Time Wrapping and Brench & Bound
	for (x = 0; x<446; x++){

		jump	 = 0;
		e		 = testLengthCopy[x];

		//average of contracted value of test value
		ccppAvgX = median(e, copyTest, x, 0);
		ccppAvgY = median(e, copyTest, x, 1);
		ccppAvg  = median(e, copyTest, x, 2);
		such	 = 160;

		// k is the number of useful sample 
		for (k = 0; k<1970; k++){             
			outKK = true;
			d	 = lengthCopy[k];
		
		//average of contracted value of original value
			avgX = median(d, copy, k, 0);
			avgY = median(d, copy, k, 1);
			avg  = median(d, copy, k, 2);

			//condition of experienced best option
			if (abs(ccppAvg - avg) < 0.0007 || abs(ccppAvgY - avgY) < 0.0294 || abs(ccppAvgX - avgX) < 0.0143){

				for (i = 0; i<d; i++){
					for (j = 0; j<e; j++){
						//distance of uclide within DTW
						cost = (copyTest[0][x][j] - copy[0][k][i])*(copyTest[0][x][j] - copy[0][k][i])
							+ (copyTest[1][x][j] - copy[1][k][i])*(copyTest[1][x][j] - copy[1][k][i]) + (copyTest[2][x][j] - copy[2][k][i])*(copyTest[2][x][j] - copy[2][k][i]);


						min = dtw[i][j + 1];
						if (min>dtw[i + 1][j])	min = dtw[i + 1][j];
						if (min>dtw[i][j])		min = dtw[i][j];
						dtw[i + 1][j + 1] = cost + min;;
					}

					//check each 4th count to break or not
					if (i % 4 == 3) {
						jumpDtw = true;
						for (j = 0; j<e; j++){
							if (such > dtw[i + 1][j + 1]){ jumpDtw = false; break; }
						}
						//if the remained value will bigger than now, go to next chracter
						if (jumpDtw) { outKK = false; if (such<4.95)jump++; break; }
					}
				}

				//if find the best fit value, save the position of ogiginal value
				if (outKK && such>dtw[d][e]){
					such = dtw[d][e];       //x 하나에 대해서
					buffer = k;
				}

				if (jump > 1180) break;

			}
		}			//if(x<100)
		//printf(" x val is %d min is %f,원래값 %c, 검색했던 값 %c \n",x+1,such,val[x] ,val[buffer]);
		fprintf(out, "%c\n", val[buffer]);

	}


	//free dynamic allocate
	free(num);
	free(test);
	free(copy);
	free(copyTest);

	//printf("boundk is %d",boundCount);
	//close all file pointer
	fclose(in3);
	fclose(in4);
	fclose(in5);
	fclose(test3);
	fclose(test4);
	fclose(test5);

	printf("총 프로그램 실행 시간 : %d ms", (int)difftime(clock(), start));
	return 0;
}