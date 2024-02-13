#ifndef __MATH_H__
#define __MATH_H__

#define DIV_ROUND_UP(x, y) (x + (y - 1)) / y
#define ALIGN_UP(x, y) DIV_ROUND_UP(x, y) * y
#define ALIGN_DOWN(x, y) (x / y) * y

int calculate_number_of_lines(int width_of_one_line, int total_pixels_to_draw) {
    int lines = total_pixels_to_draw / width_of_one_line;
    if (total_pixels_to_draw % width_of_one_line > 0) {
        lines++; // Add one more line if there are remaining pixels
    }
    return lines;
}



#endif // __MATH_H_