# convert 200x200 picture to c style array for wujian100 vga
# wujian100 uses little end
# ----------------------------------------------------------------------------
# | 31 - 28 | 27 - 24 | 23 - 20 | 19 - 16 | 15 - 12 | 11 - 8  | 7 - 4   |  3 - 0  |
# | PIXEL_7 | PIXEL_6 | PIXEL_5 | PIXEL_4 | PIXEL_3 | PIXEL_2 | PIXEL_1 | PIXEL_0 |
# ----------------------------------------------------------------------------
# |  3     2       1      0  |
# | red green_1 green_0 blue |
# bits with XXX_0 contain 4-bit color value for pixel (N) and XXX_1 for (N + 1)

import cv2
import numpy as np

IMAGE_PATH = "school_badge_200x200.bmp"

if __name__ == "__main__":
    img = cv2.imread(IMAGE_PATH)
    # use lut table to quantize
    table = np.zeros((256, 1, 3), dtype=np.uint8)
    # blue
    table[128:256, 0, 0] = 255
    # green
    table[32:96, 0, 1] = 64
    table[96:160, 0, 1] = 128
    table[160:224, 0, 1] = 192
    table[224:, 0, 1] = 255
    # red
    table[128:, 0, 2] = 255

    img_array = np.array(img, dtype=np.uint8)
    myimg_array = np.zeros((200, 200, 3), dtype=np.uint8)

    cv2.LUT(img_array, table, myimg_array)
    cv2.imshow("", myimg_array)  # show result
    cv2.waitKey(0)

    myimg_array[:, :, 0] = myimg_array[:, :, 0] >> 7
    myimg_array[:, :, 1] = myimg_array[:, :, 1] >> 6
    myimg_array[:, :, 2] = myimg_array[:, :, 2] >> 7
    myimg_array = myimg_array.reshape((-1, 3))

    # convert to c style array
    hex_str_list = []
    c_array = []
    for i in range(myimg_array.shape[0]):
        pixel_data = myimg_array[i]
        pixel_pack = pixel_data[0] + pixel_data[1] * 2 + pixel_data[2] * 8
        hex_str_list.append(hex(pixel_pack)[2:])
        if i % 8 == 7:
            hex_str_list.reverse()
            c_array.append('0x' + ''.join(hex_str_list))
            hex_str_list = []

    c_array_str = "// array size is " + str(len(c_array)) + "\nstatic const uint32_t picture_data[]={" + ', '.join(
        c_array) + "};"
    print(c_array_str)
