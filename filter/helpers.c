#include <printf.h>
#include <stdlib.h>
#include <math.h>
#include "helpers.h"

// The filter functions below are part of the exercises available in the Harvard CS50 course.
// The exercise requires the implementation of filters for grayscale, reflect, blur and edges.
// To run the program and try the filters, follow the link below and extract the zip file
// to a new project, then copy the 4 functions to the helpers.c file
// https://cs50.harvard.edu/x/2023/psets/4/filter/more/

typedef uint8_t  BYTE;

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.
 */
typedef struct
{
    BYTE  rgbtBlue;
    BYTE  rgbtGreen;
    BYTE  rgbtRed;
} __attribute__((__packed__))
RGBTRIPLE;

int cap(int val)
{
    return val > 255 ? 255 : val;
}

/**
 * Converts an image to grayscale.
 *
 * This function works by iterating over each pixel in the image, calculates the grayscale equivalent
 * of the pixel's RGB values using the formula for luminance, and then replaces the pixel's original
 * color with this grayscale value.
 *
 * The grayscale value for a pixel is calculated as a weighted sum of its red, green, and blue
 * components, with weights of 0.299, 0.587, and 0.114 respectively. These weights reflect the fact
 * that human vision is more sensitive to green light and less sensitive to blue light.
 *
 * @param height The height of the image.
 * @param width The width of the image.
 * @param image The image to which the grayscale operation should be applied.
 */
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i ++) {
        for (int j = 0; j < width; j++) {
            RGBTRIPLE *pixel = &image[i][j];
            BYTE gray = (int) (0.299 * pixel->rgbtRed + 0.587 * pixel->rgbtGreen + 0.114 * pixel->rgbtBlue);

            // Update the pixel's color in the image to the new grayscale value
            pixel->rgbtRed = gray;
            pixel->rgbtGreen = gray;
            pixel->rgbtBlue = gray;
        }
    }
}

/**
 * Reflects an image horizontally.
 *
 * This function works by iterating over each row of pixels in the image and for each row,
 * it swaps each pixel on the left half of the row with the corresponding pixel on the right half.
 *
 * In effect, this produces a mirror image of the original image. The middle pixel (if it exists)
 * in an odd-width row is left unchanged.
 *
 * @param height The height of the image.
 * @param width The width of the image.
 * @param image The image to be reflected.
 */
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i ++) {
        for (int j = 0, k = width - 1; j < (width / 2); j++, k--) {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][k];
            image[i][k] = tmp;
        }
    }
}

/**
 * Applies a simple box blur to an image.
 *
 * This function works by iterating over each pixel in the image and calculating the average
 * RGB values of the pixel's neighborhood (a 3x3 box centered on the pixel). The resulting
 * average RGB value is used to replace the pixel's original value in the output image.
 *
 * Some important aspects of this implementation:
 *
 * - It handles the edge pixels of the image by only considering the valid neighbors within the
 *   image boundaries when calculating the average. This effectively reduces the size of the blur
 *   box for these pixels.
 * - It uses a temporary image to store the blurred image before copying it back to the original
 *   image. This ensures that the blur operation is applied to the original image and not to an image
 *   that's already been partially blurred.
 * - The function uses nested loops to iterate over the 3x3 neighborhood of each pixel. This is a
 *   fundamental part of the box blur operation.
 *
 * This function uses a simple box blur algorithm, which is not as sophisticated as some other
 * blur algorithms but is relatively simple and efficient.
 *
 * @param height The height of the image.
 * @param width The width of the image.
 * @param image The image to which the blur should be applied.
 */
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocate memory for a temporary image
    RGBTRIPLE(*tempImage)[width] = calloc(height, sizeof(RGBTRIPLE[width]));
    if (tempImage == NULL)
    {
        printf("Not enough memory for temporary image.\n");
        return;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // "blur box" -> extracts the rgb values of each one of the
            // neighbouring pixels, calculates the average and replaces
            // the current pixel with the new value
            int red = 0;
            int green = 0;
            int blue = 0;
            int members = 0;

            // This loop is limited to 9 iterations as it will get all
            // pixels that are neighbours of the current one
            for (int h = i - 1; h <= i + 1 ; h++) {
                for (int w = j - 1; w <= j + 1; w++) {
                    if (0 <= h && h < height && 0 <= w && w < width) {
                        members++;
                        red += image[h][w].rgbtRed;
                        green += image[h][w].rgbtGreen;
                        blue += image[h][w].rgbtBlue;
                    }
                }
            }

            tempImage[i][j].rgbtRed = red / members;
            tempImage[i][j].rgbtGreen = green / members;
            tempImage[i][j].rgbtBlue = blue / members;
        }
    }

    // Copy the temporary image to the original image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = tempImage[i][j];
        }
    }

    free(tempImage);
}

/**
 * Applies the Sobel operator to detect edges in an image.
 *
 * This function works by iterating over each pixel in the image and calculating the gradient
 * at that pixel using the Sobel operator. This operator uses two 3x3 kernels to approximate
 * the derivatives of the image in the x and y directions, which are then used to calculate
 * the magnitude and direction of the gradient.
 *
 * Some important aspects of this implementation:
 *
 * - It handles image boundaries by ignoring pixels at the edge of the image,
 *   for which the Sobel operator can't be calculated in the same way as other pixels.
 * - It stores the results in a separate image and then copies this image back to the
 *   original image. This is necessary because the Sobel operator should be applied to the
 *   original image only, not an image that's been partially processed.
 * - Although the Sobel operator requires a nested loop to iterate over the 3x3 neighborhood
 *   of each pixel, this is inherent to the task of applying this operator
 *
 * Possible optimizations for large images could include parallelising the application of the
 * Sobel operator to different parts of the image, but this would add complexity to the code.
 *
 * @param height The height of the image.
 * @param width The width of the image.
 * @param image The image to which the Sobel operator should be applied.
 */
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Allocate memory for a temporary image
    RGBTRIPLE(*tempImage)[width] = calloc(height, sizeof(RGBTRIPLE[width]));
    if (tempImage == NULL)
    {
        printf("Not enough memory for temporary image.\n");
        return;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // "Sobel Operator"
            int red[2] = {0, 0};
            int green[2] = {0, 0};
            int blue[2] = {0, 0};
            int members = 0;

            int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
            int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

            // This loop is limited to 9 iterations as it will get all
            // pixels that are neighbours of the current one
            for (int h = i - 1, h1 = 0; h <= i + 1 ; h++, h1++) {
                for (int w = j - 1, w1 = 0; w <= j + 1; w++, w1++) {
                    if (0 <= h && h < height && 0 <= w && w < width) {
                        members++;
                        red[0] += image[h][w].rgbtRed * gx[h1][w1];
                        red[1] += image[h][w].rgbtRed * gy[h1][w1];
                        green[0] += image[h][w].rgbtGreen * gx[h1][w1];
                        green[1] += image[h][w].rgbtGreen * gy[h1][w1];
                        blue[0] += image[h][w].rgbtBlue * gx[h1][w1];
                        blue[1] += image[h][w].rgbtBlue * gy[h1][w1];
                    }
                }
            }

            tempImage[i][j].rgbtRed = cap((int) sqrt(pow(red[0], 2) + pow(red[1], 2)));
            tempImage[i][j].rgbtGreen = cap((int) sqrt(pow(green[0], 2) + pow(green[1], 2)));
            tempImage[i][j].rgbtBlue = cap((int) sqrt(pow(blue[0], 2) + pow(blue[1], 2)));
        }
    }

    // Copy the temporary image to the original image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = tempImage[i][j];
        }
    }

    free(tempImage);
}