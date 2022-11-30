#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    float sum = 0;
    for (int i = 0; i < im.c * im.h * im.w; ++i){
        sum += im.data[i];
    }
    if (!sum)
        return;
    for (int i = 0; i < im.c * im.h * im.w; ++i){
        im.data[i] /= sum;
    }
}

image make_box_filter(int w)
{
    image box_filter = make_image(w, w, 1);
    for (int i = 0; i < w * w; ++i){
        box_filter.data[i] = 1.0 / (w * w);
    }
    return box_filter;
}

image convolve_image(image im, image filter, int preserve)
{
    assert(filter.c == 1 || filter.c == im.c);

    image im_out;
    if (preserve == 1){
        im_out = make_image(im.w, im.h, im.c);
    }
    else{
        im_out = make_image(im.w, im.h, 1);
    }

    int h_offset = filter.h / 2;
    int w_offset = filter.w / 2;

    for (int i = 0; i < im.c; ++i){
        for (int j = 0 - h_offset; j < im.h - h_offset; ++j){
            for (int k = 0 - w_offset; k < im.w - w_offset; ++k){
                
                float v; 
                if (preserve == 1){
                    v = 0;
                }else{
                    v = get_pixel(im_out, k + w_offset, j + h_offset, 0);
                }
                
                for (int m = 0; m < filter.h; ++m){
                    for (int n = 0; n < filter.w; ++n){
                        if (filter.c == 1){
                            v += get_pixel(im, k + n, j + m, i) * 
                                 get_pixel(filter, n, m, 0);
                        }else{
                            v += get_pixel(im, k + n, j + m, i) *
                                 get_pixel(filter, n, m, i);
                        }
                    }
                }
                
                if (preserve == 1){
                    set_pixel(im_out, k + w_offset, j + h_offset, i, v);
                }else{
                    set_pixel(im_out, k + w_offset, j + h_offset, 0, v);
                }
            }
        }
    }
    return im_out;
}

image make_highpass_filter()
{
    image highpass_filter = make_image(3, 3, 1);
    float weight[9] = {0, -1, 0,
                       -1, 4, -1,
                       0, -1, 0};
    memcpy(highpass_filter.data, weight, sizeof(weight));
    return highpass_filter;
}

image make_sharpen_filter()
{
    image sharpen_filter = make_image(3, 3, 1);
    float weight[9] = {0, -1, 0,
                       -1, 5, -1,
                       0, -1, 0};
    memcpy(sharpen_filter.data, weight, sizeof(weight));
    return sharpen_filter;
}

image make_emboss_filter()
{
    image emboss_filter = make_image(3, 3, 1);
    float weight[9] = {-2, -1, 0,
                       -1, 1, 1,
                       0, 1, 2};
    memcpy(emboss_filter.data, weight, sizeof(weight));
    return emboss_filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: TODO

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: TODO

image make_gaussian_filter(float sigma)
{
    int filter_size = (int)((int)ceil(sigma * 6) % 2 == 0) ? (int)ceil(sigma * 6) + 1 : (int)ceil(sigma * 6);
    image gs_filter = make_image(filter_size, filter_size, 1);
    for (int j = 0; j < gs_filter.h; j++){
        for (int i = 0; i < gs_filter.w; i++){
            int x = i - (filter_size - 1) / 2;
            int y = j - (filter_size - 1) / 2;
            float val = exp(-(pow(x, 2) + pow(y, 2)) / (2 * pow(sigma, 2))) / (TWOPI * pow(sigma, 2));
            set_pixel(gs_filter, i, j, 0, val);
        }
    }
    l1_normalize(gs_filter);
    return gs_filter;
}

image add_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image im_out = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.c * a.h * a.w; ++i){
        im_out.data[i] = a.data[i] + b.data[i];
    }
    return im_out;
}

image sub_image(image a, image b)
{
    assert(a.w == b.w && a.h == b.h && a.c == b.c);
    image im_out = make_image(a.w, a.h, a.c);
    for (int i = 0; i < a.c * a.h * a.w; ++i){
        im_out.data[i] = a.data[i] - b.data[i];
    }
    return im_out;
}

image make_gx_filter()
{
    image gx_filter = make_image(3, 3, 1);
    float weight[9] = {-1, 0, 1,
                       -2, 0, 2,
                       -1, 0, 1};
    memcpy(gx_filter.data, weight, sizeof(weight));
    return gx_filter;
}

image make_gy_filter()
{
    image gy_filter = make_image(3, 3, 1);
    float weight[9] = {-1, -2, -1,
                       0, 0, 0,
                       1, 2, 1};
    memcpy(gy_filter.data, weight, sizeof(weight));
    return gy_filter;
}

void feature_normalize(image im)
{
    float min = 1, max = 0;
    
    for (int i = 0; i < im.c * im.h * im.w; ++i){
        if (im.data[i] > max)
            max = im.data[i];
        if (im.data[i] < min)
            min = im.data[i];
    }

    float range = max - min;
    if (!range){
        for (int i = 0; i < im.c * im.h * im.w; ++i){
            im.data[i] = 0;
        }
    }
    else{
        for (int i = 0; i < im.c * im.h * im.w; ++i){
            im.data[i] = (im.data[i] - min) / range;
        }
    }
}

image *sobel_image(image im)
{
    image *res = calloc(2, sizeof(image));
    image gx_filter = make_gx_filter();
    image gy_filter = make_gy_filter();
    image gx = convolve_image(im, gx_filter, 0);
    image gy = convolve_image(im, gy_filter, 0);
    image mag = make_image(gx.w, gx.h, 1);
    image theta = make_image(gx.w, gx.h, 1);
    for (int i = 0; i < im.h * im.w; ++i){
        mag.data[i] = sqrtf(gx.data[i] * gx.data[i] + gy.data[i] * gy.data[i]);
        theta.data[i] = atan2f(gy.data[i], gx.data[i]);
    }
    res[0] = mag;
    res[1] = theta;
    free_image(gx_filter);
    free_image(gy_filter);
    free_image(gx);
    free_image(gy);

    return res;
}

image colorize_sobel(image im)
{
    image colorized = make_image(im.w, im.h, 3);
    image *res = sobel_image(im);
    feature_normalize(res[0]);
    feature_normalize(res[1]);
    memcpy(colorized.data, res[1].data, im.h * im.w * sizeof(float));
    memcpy(colorized.data + im.h * im.w, res[0].data, im.h * im.w * sizeof(float));
    memcpy(colorized.data + 2 * im.h * im.w, res[0].data, im.h * im.w * sizeof(float));
    hsv_to_rgb(colorized);
    free_image(res[0]);
    free_image(res[1]);
    return colorized;
}
