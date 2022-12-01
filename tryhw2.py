from uwimg import *
im = load_image("data/dog.jpg")
f = make_box_filter(7)
blur = convolve_image(im, f, 1)
save_image(blur, "hasil/dog-box7")


im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
blur = convolve_image(im, f, 1)
save_image(blur, "hasil/dog-gauss2")

im = load_image("data/dog.jpg")
f = make_gaussian_filter(2)
lfreq = convolve_image(im, f, 1)
hfreq = im - lfreq
reconstruct = lfreq + hfreq
save_image(lfreq, "hasil/low-frequency")
save_image(hfreq, "hasil/high-frequency")
save_image(reconstruct, "hasil/reconstruct")

im = load_image("data/dog.jpg")
res = sobel_image(im)
mag = res[0]
feature_normalize(mag)
save_image(mag, "hasil/magnitude")

im = load_image("data/dog.jpg")
colorize_image = colorize_sobel(im)
save_image(colorize_image, "hasil/colorize_dog")