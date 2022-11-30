#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    if (x < 0){
        x = 0;
    }else if (x >= im.w){
        x = im.w - 1;
    } 

    if (y < 0){
        y = 0;
    }else if (y >= im.h){
        y = im.h - 1;
    } 

    if (c >= 0 && c < im.c){
        return im.data[c*im.h*im.w + y*im.w + x];
    }else{
        return 0;
    } 
}

void set_pixel(image im, int x, int y, int c, float v)
{
    if (x >= 0 && x < im.w && y >= 0 && y < im.h && c >= 0 && c < im.c){
        im.data[c*im.h*im.w + y*im.w + x] = v;
    }
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    memcpy(copy.data, im.data, im.c*im.h*im.w*sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    
    float scale[] = {0.299, 0.587, 0.114};
    for (int i = 0; i < im.c; ++i){
      for (int j = 0; j < im.h*im.w; ++j){
        gray.data[j] += scale[i] * im.data[i*im.h*im.w+j];
      }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    for (int i = 0; i < im.h*im.w; ++i){
      im.data[c*im.h*im.w+i] = im.data[c*im.h*im.w+i] + v;
    }
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int i = 0; i < im.c*im.h*im.w; ++i){
        if (im.data[i] > 1){
            im.data[i] = 1;
        }else if (im.data[i] < 0){
            im.data[i] = 0;
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    float r, g, b, h, s, v;
    float m, c;
    
    for (int i = 0; i < im.h*im.w; ++i){
        r = im.data[i];
        g = im.data[i + im.h * im.w];
        b = im.data[i + 2 * im.h * im.w];
        v = three_way_max(r, g, b);
        m = three_way_min(r, g, b);
        c = v - m;

        if (v == 0){
            s = 0;
        }else{
            s = c / v;
        } 
      
        if (v == m){
            h = 0;
        }else{
            if (v == r){
                h = (g - b) / c;
            }else if (v == g){
                h = (b - r) / c + 2;
            }else{
                h = (r - g) / c + 4;
            }
        }

        if (h < 0){
            h = h / 6 + 1;
        }else{
            h /= 6;
        }

        im.data[i] = h;
        im.data[i + im.h*im.w] = s;
        im.data[i + 2*im.h*im.w] = v;
    }

}

void hsv_to_rgb(image im)
{
    
    float h, s, v, r, g, b;
    float dst, mid_pos, mid_neg, m;
    for (int i = 0; i < im.h*im.w; ++i){
        h = im.data[i];
        s = im.data[i + im.h*im.w];
        v = im.data[i + 2*im.h*im.w];  
        h *= 6;
        m = v - s * v;  
        dst = h - floor(h);
        mid_pos = s * v * dst + m;  
        mid_neg = s * v * (1 - dst) + m;  
        if (h >= 0 && h < 1){  
            r = v;
            g = mid_pos;
            b = m;
        }else if (h >= 1 && h < 2){  
            g = v;
            r = mid_neg;
            b = m;
        }else if (h >= 2 && h < 3){  
            g = v;
            b = mid_pos;
            r = m;
        }else if (h >= 3 && h < 4){  
            b = v;
            g = mid_neg;
            r = m;
        }else if (h >= 4 && h < 5){  
            b = v;
            r = mid_pos;
            g = m;
        }else{  
            r = v;
            b = mid_neg;
            g = m;
        }
        im.data[i] = r;
        im.data[i + im.h*im.w] = g;
        im.data[i + 2*im.h*im.w] = b;
    }
}

void scale_image(image im, int c, float v)
{
    for (int i = 0; i < im.h*im.w; ++i){
        im.data[c*im.h*im.w+i] = im.data[c*im.h*im.w+i] * v;
    }
}
