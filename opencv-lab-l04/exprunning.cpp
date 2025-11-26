// OpenCV
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// std
#include <iostream>
#include <map>
#include <unistd.h>

void binarize(const cv::Mat &src, cv::Mat &dst, unsigned int th);
void dilateImg(const cv::Mat &, const cv::Mat &, const cv::Point &, cv::Mat &);
void erodeImg(const cv::Mat &, const cv::Mat &, const cv::Point &, cv::Mat &);
void opening(const cv::Mat &, const cv::Mat &, const cv::Point &, cv::Mat &);
void labelImage(const cv::Mat &, cv::Mat &);

struct Args {
  std::string pattern;
  int wait = 0;
  int k = 50;
  float alpha = 0.5f;
};

bool parseArgs(Args &args, int argc, char **argv)
{
  int opt;
  while ((opt = getopt(argc, argv, "i:t:k:a:h")) != -1) {
    switch(opt) {
      case 'i': args.pattern = optarg; break;
      case 't': args.wait = atoi(optarg); break;
      case 'k': args.k = atoi(optarg); break;
      case 'a': args.alpha = atof(optarg); break;
      default:
        std::cerr << "Usage: -i seq -t ms -k thr -a alpha\n";
        return false;
    }
  }
  return !args.pattern.empty();
}

int main(int argc, char **argv)
{
  Args args;
  if(!parseArgs(args, argc, argv))
    return 1;

  cv::Mat bg;
  int f = 0;
  char fname[256];

  while(true)
  {
    sprintf(fname, args.pattern.c_str(), f);
    cv::Mat frame = cv::imread(fname, cv::IMREAD_GRAYSCALE);

    if(frame.empty()) {
      std::cerr << "End of sequence\n";
      break;
    }

    if(bg.empty()) {
      frame.copyTo(bg);
    } else {
      cv::Mat diff16, bg16;
      frame.convertTo(diff16, CV_16S);
      bg.convertTo(bg16, CV_16S);

      cv::Mat absdiff, binary;
      cv::absdiff(diff16, bg16).convertTo(absdiff, CV_8U);

      binarize(absdiff, binary, args.k);

      cv::Mat se = cv::Mat::ones(3,3,CV_8U);
      cv::Mat opened;
      opening(binary, se, cv::Point(1,1), opened);

      cv::Mat lbl;
      labelImage(opened, lbl);

      cv::Mat dbg;
      lbl.convertTo(dbg, CV_8U);
      cv::applyColorMap(dbg, dbg, cv::COLORMAP_HSV);

      cv::imshow("background", bg);
      cv::imshow("binary", binary);
      cv::imshow("opened", opened);
      cv::imshow("labels", dbg);

      bg = args.alpha * bg + (1 - args.alpha) * frame;
    }

    cv::imshow("input", frame);

    if(cv::waitKey(args.wait) == 'q')
      break;

    f++;
  }

  return 0;
}

void binarize(const cv::Mat &src, cv::Mat &dst, unsigned int th)
{
  dst.create(src.size(), CV_8U);
  for(int r = 0; r < src.rows; ++r) {
    const uchar *sp = src.ptr<uchar>(r);
    uchar *dp = dst.ptr<uchar>(r);
    for(int c = 0; c < src.cols; ++c)
      dp[c] = (sp[c] < th ? 0 : 255);
  }
}

void dilateImg(const cv::Mat &src, const cv::Mat &se, const cv::Point &org, cv::Mat &dst)
{
  dst = src.clone();
  for(int r = 0; r < src.rows; ++r)
    for(int c = 0; c < src.cols; ++c)
      for(int kr = 0; kr < se.rows; ++kr)
        for(int kc = 0; kc < se.cols; ++kc)
          if(se.at<uchar>(kr,kc)) {
            int rr = r + kr - org.y;
            int cc = c + kc - org.x;
            if(rr>=0 && rr<src.rows && cc>=0 && cc<src.cols)
              if(src.at<uchar>(rr,cc)) {
                dst.at<uchar>(r,c) = 255;
                kr = se.rows; kc = se.cols;
              }
          }
}

void erodeImg(const cv::Mat &src, const cv::Mat &se, const cv::Point &org, cv::Mat &dst)
{
  dst = src.clone();
  for(int r = 0; r < src.rows; ++r)
    for(int c = 0; c < src.cols; ++c)
      for(int kr = 0; kr < se.rows; ++kr)
        for(int kc = 0; kc < se.cols; ++kc)
          if(se.at<uchar>(kr,kc)) {
            int rr = r + kr - org.y;
            int cc = c + kc - org.x;
            if(rr<0 || rr>=src.rows || cc<0 || cc>=src.cols || !src.at<uchar>(rr,cc)) {
              dst.at<uchar>(r,c) = 0;
              kr = se.rows; kc = se.cols;
            }
          }
}

void opening(const cv::Mat &src, const cv::Mat &se, const cv::Point &org, cv::Mat &dst)
{
  cv::Mat tmp;
  erodeImg(src, se, org, tmp);
  dilateImg(tmp, se, org, dst);
}

void labelImage(const cv::Mat &src, cv::Mat &dst)
{
  dst = cv::Mat::zeros(src.size(), CV_16U);
  uint16_t lab = 1;
  std::map<uint16_t,uint16_t> eq;

  for(int r = 0; r < src.rows; ++r)
    for(int c = 0; c < src.cols; ++c) {
      if(src.at<uchar>(r,c) == 0) continue;

      uint16_t up = (r>0) ? dst.at<uint16_t>(r-1,c) : 0;
      uint16_t lf = (c>0) ? dst.at<uint16_t>(r,c-1) : 0;

      if(up && lf) {
        dst.at<uint16_t>(r,c) = std::min(up,lf);
        if(up != lf) eq[std::max(up,lf)] = std::min(up,lf);
      } else if(up || lf)
        dst.at<uint16_t>(r,c) = up ? up : lf;
      else
        dst.at<uint16_t>(r,c) = ++lab;
    }

  for(int r = 0; r < src.rows; ++r)
    for(int c = 0; c < src.cols; ++c) {
      uint16_t &v = dst.at<uint16_t>(r,c);
      while(eq.count(v)) v = eq[v];
    }
}

