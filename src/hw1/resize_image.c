#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    return get_pixel(im, roundf(x), roundf(y), c);
}

image nn_resize(image im, int w, int h)
{
    image im_nn = make_image(w, h, im.c);
    
    float w_scale = (float)im.w / w;
    float h_scale = (float)im.h / h;
    for (int i = 0; i < im.c; ++i){
        for (int j = 0; j < h; ++j){
            for (int k = 0; k < w; ++k){
                float w_mapped = w_scale * (k + 0.5) - 0.5;
                float h_mapped = h_scale * (j + 0.5) - 0.5;
                float v = nn_interpolate(im, w_mapped, h_mapped, i);
                set_pixel(im_nn, k, j, i, v);
            }
        }
    }
    return im_nn;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    float v1 = get_pixel(im, floorf(x), floorf(y), c);
    float v2 = get_pixel(im, ceilf(x), floorf(y), c);
    float v3 = get_pixel(im, floorf(x), ceilf(y), c);
    float v4 = get_pixel(im, ceilf(x), ceilf(y), c);

    float q1, q2, q;

    if (floorf(x) == ceilf(x)){
        q1 = v1;
    }else{
        q1 = v1 * (ceilf(x) - x) + v2 * (x - floorf(x));
    }
    
    if (floorf(x) == ceilf(x)){
        q2 = v3;
    }else{
        q2 = v4 * (x - floorf(x)) + v3 * (ceilf(x) - x);
    }

    if (floorf(y) == ceilf(y)){
        q = q1;
    }else{
        q = q2 * (y - floorf(y)) + q1 * (ceilf(y) - y);
    }
    return q;
}

image bilinear_resize(image im, int w, int h)
{
    image im_bilinear = make_image(w, h, im.c);
    
    float w_scale = (float)im.w / w;
    float h_scale = (float)im.h / h;
    for (int i = 0; i < im.c; ++i){
        for (int j = 0; j < h; ++j){
            for (int k = 0; k < w; ++k){
                float w_mapped = w_scale * (k + 0.5) - 0.5;
                float h_mapped = h_scale * (j + 0.5) - 0.5;
                float v = bilinear_interpolate(im, w_mapped, h_mapped, i);
                set_pixel(im_bilinear, k, j, i, v);
            }
        }
    }
    return im_bilinear;
}

