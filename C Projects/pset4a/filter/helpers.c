#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int GxR[height][width];
    int GyR[height][width];
    int GxB[height][width];
    int GyB[height][width];
    int GxG[height][width];
    int GyG[height][width];
    RGBTRIPLE temp[height + 2][width + 2];

    //copy the values of pixels into temp.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i + 1][j + 1].rgbtRed = image[i][j].rgbtRed;
            temp[i + 1][j + 1].rgbtGreen = image[i][j].rgbtGreen;
            temp[i + 1][j + 1].rgbtBlue = image[i][j].rgbtBlue;
        }
    }

    //set all pixels on boundary to 0.
    /*for (int i = 0; i < height + 2; i++)
    {
        temp[i][0].rgbtRed = 0;
        temp[i][0].rgbtGreen = 0;
        temp[i][0].rgbtBlue = 0;

        temp[i][width + 1].rgbtRed = 0;
        temp[i][width + 1].rgbtBlue = 0;
        temp[i][width + 1].rgbtGreen = 0;

    }
    for (int j = 1; j <= width; j++)
    {
        temp[0][j].rgbtRed = 0;
        temp[0][j].rgbtGreen = 0;
        temp[0][j].rgbtBlue = 0;

        temp[height + 1][j].rgbtRed = 0;
        temp[height + 1][j].rgbtGreen = 0;
        temp[height + 1][j].rgbtBlue = 0;
    }*/

    for(int i=0; i< height +2; i++)
    {
        for (int j = 0; j < width+2; j++)
        {
            temp[i][j].rgbtRed = 0;
            temp[i][j].rgbtGreen = 0;
            temp[i][j].rgbtBlue = 0;
            if(j<1 || j> width || i< 1 || i > height)
            {
            }else{
                temp[i][j].rgbtRed = image[i-1][j-1].rgbtRed;
                temp[i][j].rgbtGreen = image[i-1][j-1].rgbtGreen;
                temp[i][j].rgbtBlue = image[i-1][j-1].rgbtBlue;
            }
        }
    }

    for (int i = 1; i < height + 1; i++)
    {
        for (int j = 1; j < width + 1; j++)
        {
            GxR[i - 1][j - 1] = 0-(temp[i - 1][j - 1].rgbtRed)- (2 * temp[i][j - 1].rgbtRed) - (temp[i + 1][j - 1].rgbtRed) + (temp[i - 1][j + 1].rgbtRed) + (2 * temp[i][j + 1].rgbtRed + temp[i + 1][j + 1].rgbtRed);
            GxG[i - 1][j - 1] = 0-(temp[i - 1][j - 1].rgbtGreen) - (2 * temp[i][j - 1].rgbtGreen) - (temp[i + 1][j - 1].rgbtGreen) + (temp[i - 1][j + 1].rgbtGreen) + (2 * temp[i][j + 1].rgbtGreen) + (temp[i + 1][j + 1].rgbtGreen);
            GxB[i - 1][j - 1] = 0-(temp[i - 1][j - 1].rgbtBlue) - (2 * temp[i][j - 1].rgbtBlue) - (temp[i + 1][j - 1].rgbtBlue) + (temp[i - 1][j + 1].rgbtBlue) + (2 * temp[i][j + 1].rgbtBlue) + (temp[i + 1][j + 1].rgbtBlue);

            GyR[i - 1][j - 1] = 0-(temp[i - 1][j - 1].rgbtRed) - (2 * temp[i - 1][j].rgbtRed) - (temp[i - 1][j + 1].rgbtRed) + (temp[i + 1][j - 1].rgbtRed) + (2 * temp[i + 1][j].rgbtRed) + (temp[i + 1][j + 1].rgbtRed);
            GyG[i - 1][j - 1] = 0-(temp[i - 1][j - 1].rgbtGreen) - (2 * temp[i - 1][j].rgbtGreen) - (temp[i - 1][j + 1].rgbtGreen) + (temp[i + 1][j - 1].rgbtGreen) + (2 * temp[i + 1][j].rgbtGreen) + (temp[i + 1][j + 1].rgbtGreen);
            GyB[i - 1][j - 1] = 0-(temp[i - 1][j - 1].rgbtBlue) - (2 * temp[i - 1][j].rgbtBlue) - (temp[i - 1][j + 1].rgbtBlue) + (temp[i + 1][j - 1].rgbtBlue) + (2 * temp[i + 1][j].rgbtBlue) + (temp[i + 1][j + 1].rgbtBlue);

            int val = round(sqrt(pow(GxR[i - 1][j - 1],2) + pow(GyR[i - 1][j - 1],2)));
            if (val > 255)
            {
                image[i - 1][j - 1].rgbtRed = 255;
            }
            else
            {
                image[i - 1][j - 1].rgbtRed = val;
            }
            int val2 = round(sqrt(pow(GxG[i - 1][j - 1],2)+ pow(GyG[i - 1][j - 1],2)));
            if (val2 > 255)
            {
                image[i - 1][j - 1].rgbtGreen = 255;
            }
            else
            {
                image[i - 1][j - 1].rgbtGreen = val2;
            }

            int val3 = round(sqrt(pow(GxB[i - 1][j - 1],2) + pow(GyB[i - 1][j - 1],2)));
            if (val3 > 255)
            {
                image[i - 1][j - 1].rgbtBlue = 255;
            }
            else
            {
                image[i - 1][j - 1].rgbtBlue = val3;
            }
        }
    }
    return;
}
