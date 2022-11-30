from uwimg import *
im = load_image("data/dogsmall.jpg")
a = nn_resize(im, im.w*4, im.h*4)
save_image(a, "dog4x-nn")

im = load_image("data/dogsmall.jpg")
b = bilinear_resize(im, im.w*4, im.h*4)
save_image(b, "dog4x-bl")