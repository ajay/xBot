#ifndef __TK_GCUBE_H__
#define __TK_GCUBE_H__

#ifdef __NVCC__

#include <opencv2/core/core.hpp>
#include <string>
#include <vector>

// TODO: allow compatibility with arma commands

namespace gfill {
  const uint8_t none = 0;
  const uint8_t zeros = 1;
  const uint8_t ones = 2;
};

class gcube {
  public:
    gcube(void);
    gcube(size_t n_rows,
          size_t n_cols = 1,
          size_t n_slices = 1,
          uint8_t fill_type = gfill::none);
    gcube(const gcube &gpucube);
    gcube(const std::string &fname);
    //gcube(const std::vector<float> &list); // add later for C++11 usage
    ~gcube(void);

    void create(size_t n_rows,
                size_t n_cols = 1,
                size_t n_slices = 1,
                uint8_t fill_type = gfill::none);
    void create(const std::vector<float> &list);

    gcube &operator=(const gcube &gpucube);

    void load(const std::string &fname);
    void save(const std::string &fname);

    // opencv compatibility
    gcube(cv::Mat &cvMat);
    void create(const cv::Mat &cvMat);
    void create(const cv::Mat &cvMat, int x1, int x2, int y1, int y2);
    cv::Mat cv_mat(void);
    gcube &operator=(const cv::Mat &cvMat);

    float *d_pixels;
    size_t n_rows;
    size_t n_cols;
    size_t n_slices;
    size_t n_elem;
};

#endif
#endif
