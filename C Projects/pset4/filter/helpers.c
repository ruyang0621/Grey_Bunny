#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int g;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            g = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            image[i][j].rgbtBlue = g;
            image[i][j].rgbtGreen = g;
            image [i][j].rgbtRed = g;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int r, g, b;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            r = round(0.393 * image[i][j].rgbtRed + 0.769 * image[i][j].rgbtGreen + 0.189 * image[i][j].rgbtBlue);
            if (r > 255)
            {
                r = 255;
            }

            g = round(0.349 * image[i][j].rgbtRed + 0.686 * image[i][j].rgbtGreen + 0.168 * image[i][j].rgbtBlue);
            if (g > 255)
            {
                g = 255;
            }

            b = round(0.272 * image[i][j].rgbtRed + 0.534 * image[i][j].rgbtGreen + 0.131 * image[i][j].rgbtBlue);
            if (b > 255)
            {
                b = 255;
            }

            image[i][j].rgbtRed = r;
            image[i][j].rgbtGreen = g;
            image[i][j].rgbtBlue = b;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int m;
    int r, g, b;
    if ((width + 1) % 2 == 0)
    {
        m = (width + 1) / 2;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < m; j++)
            {
                r = image[i][j].rgbtRed;
                image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
                image[i][width - j - 1].rgbtRed = r;

                g = image[i][j].rgbtGreen;
                image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
                image[i][width - j - 1].rgbtGreen = g;

                b = image[i][j].rgbtBlue;
                image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
                image[i][width - j - 1].rgbtBlue = b;
            }
        }
    }
    else
    {
        m = width / 2;
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < m; j++)
            {
                r = image[i][j].rgbtRed;
                image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
                image[i][width - j - 1].rgbtRed = r;

                g = image[i][j].rgbtGreen;
                image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
                image[i][width - j - 1].rgbtGreen = g;

                b = image[i][j].rgbtBlue;
                image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
                image[i][width - j - 1].rgbtBlue = b;
            }
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float counter;
    int sumr;
    int sumg;
    int sumb;
    RGBTRIPLE image2[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sumr = 0;
            sumg = 0;
            sumb = 0;
            counter = 0.0;
            for (int k = -1; k < 2; k++)
            {
                if (j + k < 0 || j + k > width - 1)
                {
                    continue;
                }

                for (int h = -1; h < 2; h++)
                {
                    if (i + h < 0 || i + h > height - 1)
                    {
                        continue;
                    }

                    sumr += image[i + h][j + k].rgbtRed;
                    sumg += image[i + h][j + k].rgbtGreen;
                    sumb += image[i + h][j + k].rgbtBlue;
                    counter++;
                }
            }

            image2[i][j].rgbtRed = round(sumr / counter);
            image2[i][j].rgbtGreen = round(sumg / counter);
            image2[i][j].rgbtBlue = round(sumb / counter);
        }

    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = image2[i][j].rgbtRed;
            image[i][j].rgbtGreen = image2[i][j].rgbtGreen;
            image[i][j].rgbtBlue = image2[i][j].rgbtBlue;
        }
    }

    return;
}
