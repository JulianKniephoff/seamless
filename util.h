#ifndef SEAMLESS_UTIL_H
#define SEAMLESS_UTIL_H

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) < (b) ? (b) : (a))

#define value_from_rgb(r, g, b) (max (max ((r), (g)), (b)))

#endif
