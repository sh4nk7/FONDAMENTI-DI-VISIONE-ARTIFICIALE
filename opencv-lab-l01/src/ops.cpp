
#include "ops.hpp"
#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <random>
#include <stdexcept>

static inline int clampInt(int x, int lo, int hi){ return std::max(lo, std::min(hi, x)); }

cv::Mat downsample2x(const cv::Mat& src){
    cv::Mat dst(src.rows/2, src.cols/2, src.type());
    const int ch = src.channels();
    for(int r=0;r<dst.rows;++r){
        for(int c=0;c<dst.cols;++c){
            const uchar* s = src.ptr<uchar>(r*2) + (c*2)*ch;
            uchar* d = dst.ptr<uchar>(r) + c*ch;
            for(int k=0;k<ch;++k) d[k]=s[k];
        }
    }
    return dst;
}

cv::Mat downsampleRows2x(const cv::Mat& src){
    cv::Mat dst(src.rows/2, src.cols, src.type());
    const int ch = src.channels();
    for(int r=0;r<dst.rows;++r){
        const uchar* srow = src.ptr<uchar>(r*2);
        uchar* drow = dst.ptr<uchar>(r);
        std::copy(srow, srow + src.cols*ch, drow);
    }
    return dst;
}

cv::Mat downsampleCols2x(const cv::Mat& src){
    cv::Mat dst(src.rows, src.cols/2, src.type());
    const int ch = src.channels();
    for(int r=0;r<dst.rows;++r){
        const uchar* srow = src.ptr<uchar>(r);
        uchar* drow = dst.ptr<uchar>(r);
        for(int c=0;c<dst.cols;++c){
            const uchar* s = srow + (c*2)*ch;
            uchar* d = drow + c*ch;
            for(int k=0;k<ch;++k) d[k]=s[k];
        }
    }
    return dst;
}

cv::Mat flipHorizontal(const cv::Mat& src){
    cv::Mat dst(src.size(), src.type());
    const int ch = src.channels();
    for(int r=0;r<src.rows;++r){
        const uchar* srow = src.ptr<uchar>(r);
        uchar* drow = dst.ptr<uchar>(r);
        for(int c=0;c<src.cols;++c){
            const uchar* s = srow + (src.cols-1-c)*ch;
            uchar* d = drow + c*ch;
            for(int k=0;k<ch;++k) d[k]=s[k];
        }
    }
    return dst;
}

cv::Mat flipVertical(const cv::Mat& src){
    cv::Mat dst(src.size(), src.type());
    const int ch = src.channels();
    for(int r=0;r<src.rows;++r){
        const uchar* srow = src.ptr<uchar>(src.rows-1-r);
        uchar* drow = dst.ptr<uchar>(r);
        std::copy(srow, srow + src.cols*ch, drow);
    }
    return dst;
}

cv::Mat cropRegion(const cv::Mat& src, int u, int v, int w, int h){
    // clamp crop
    int x = clampInt(u, 0, std::max(0, src.cols-1));
    int y = clampInt(v, 0, std::max(0, src.rows-1));
    int cw = clampInt(w, 1, src.cols - x);
    int chh= clampInt(h, 1, src.rows - y);
    cv::Rect roi{x,y,cw,chh};
    return src(roi).clone();
}

cv::Mat addZeroPadding(const cv::Mat& src, int pad){
    cv::Mat dst(src.rows + 2*pad, src.cols + 2*pad, src.type(), cv::Scalar::all(0));
    src.copyTo(dst(cv::Rect(pad, pad, src.cols, src.rows)));
    return dst;
}

cv::Mat shuffleBlocks4(const cv::Mat& src){
    cv::Mat dst(src.size(), src.type());
    int halfR = src.rows/2;
    int halfC = src.cols/2;
    std::vector<cv::Rect> blocks = {
        {0,0,halfC,halfR},
        {halfC,0,halfC,halfR},
        {0,halfR,halfC,halfR},
        {halfC,halfR,halfC,halfR}
    };
    std::random_device rd; std::mt19937 gen(rd());
    std::shuffle(blocks.begin(), blocks.end(), gen);
    // place shuffled in a 2x2 grid
    int idx=0;
    for(int br=0;br<2;++br){
        for(int bc=0;bc<2;++bc){
            cv::Rect dstR(bc*halfC, br*halfR, halfC, halfR);
            src(blocks[idx++]).copyTo(dst(dstR));
        }
    }
    return dst;
}

cv::Mat shuffleChannels(const cv::Mat& src){
    CV_Assert(src.type() == CV_8UC3);
    std::vector<cv::Mat> chs(3);
    cv::split(src, chs);
    std::vector<int> perm = {0,1,2};
    std::random_device rd; std::mt19937 gen(rd());
    std::shuffle(perm.begin(), perm.end(), gen);
    cv::Mat dst;
    cv::merge(std::vector<cv::Mat>{chs[perm[0]], chs[perm[1]], chs[perm[2]]}, dst);
    return dst;
}

bool hasBayerTag(const std::string& filename){
    auto f = filename;
    auto toUpper = [](std::string& s){ for(auto& c:s) c = (char)std::toupper((unsigned char)c); };
    toUpper(f);
    return f.find("RGGB")!=std::string::npos || f.find("GBRG")!=std::string::npos || f.find("BGGR")!=std::string::npos;
}

Bayer detectBayer(const std::string& filename){
    auto f = filename;
    for(auto& c: f) c = (char)std::toupper((unsigned char)c);
    if(f.find("RGGB")!=std::string::npos) return Bayer::RGGB;
    if(f.find("GBRG")!=std::string::npos) return Bayer::GBRG;
    if(f.find("BGGR")!=std::string::npos) return Bayer::BGGR;
    return Bayer::NONE;
}

cv::Mat debayerDownsampleG(const cv::Mat& bayer, Bayer pattern){
    CV_Assert(bayer.type()==CV_8UC1);
    cv::Mat dst(bayer.rows/2, bayer.cols/2, CV_8UC1);
    for(int r=0;r<dst.rows;++r){
        for(int c=0;c<dst.cols;++c){
            int u = c*2, v = r*2;
            int ul = bayer.data[v*bayer.cols + u];
            int ur = bayer.data[v*bayer.cols + u+1];
            int ll = bayer.data[(v+1)*bayer.cols + u];
            int lr = bayer.data[(v+1)*bayer.cols + u+1];
            int gmean = 0;
            switch(pattern){
                case Bayer::RGGB: // G at ur & ll
                case Bayer::BGGR:
                    gmean = (ur + ll)/2; break;
                case Bayer::GBRG: // G at ul & lr
                    gmean = (ul + lr)/2; break;
                default: gmean = (ul+ur+ll+lr)/4; break;
            }
            dst.data[r*dst.cols + c] = static_cast<uchar>(gmean);
        }
    }
    return dst;
}

cv::Mat debayerLuminance(const cv::Mat& bayer, Bayer pattern){
    CV_Assert(bayer.type()==CV_8UC1);
    cv::Mat dst(bayer.rows, bayer.cols, CV_8UC1);
    auto lum = [](int R,int G1,int G2,int B)->uchar{
        float L = 0.3f*R + 0.59f*( (G1+G2)*0.5f ) + 0.11f*B;
        return static_cast<uchar>(std::clamp((int)std::lround(L),0,255));
    };
    for(int r=0;r<bayer.rows-1;++r){
        for(int c=0;c<bayer.cols-1;++c){
            int ul = bayer.data[r*bayer.cols + c];
            int ur = bayer.data[r*bayer.cols + c+1];
            int ll = bayer.data[(r+1)*bayer.cols + c];
            int lr = bayer.data[(r+1)*bayer.cols + c+1];
            uchar out=0;
            // Determine local 2x2 starting at (r,c) relative to global pattern
            auto emitRGGB = [&](){ out = lum(ul, ur, ll, lr); };
            auto emitGBRG = [&](){ out = lum(ll, ul, lr, ur); };
            auto emitBGGR = [&](){ out = lum(lr, ur, ll, ul); };
            auto emitGRBG = [&](){ out = lum(ur, ul, lr, ll); };
            // choose based on absolute (r,c) parity and base pattern
            bool re = (r%2)==0, ce = (c%2)==0;
            switch(pattern){
                case Bayer::RGGB:
                    if(re&&ce) emitRGGB();
                    else if(re&&!ce) emitGRBG();
                    else if(!re&&ce) emitGBRG();
                    else emitBGGR();
                    break;
                case Bayer::GBRG:
                    if(re&&ce) emitGBRG();
                    else if(re&&!ce) emitBGGR();
                    else if(!re&&ce) emitRGGB();
                    else emitGRBG();
                    break;
                case Bayer::BGGR:
                    if(re&&ce) emitBGGR();
                    else if(re&&!ce) emitGBRG();
                    else if(!re&&ce) emitGRBG();
                    else emitRGGB();
                    break;
                default: out = (uchar)((ul+ur+ll+lr)/4); break;
            }
            dst.data[r*dst.cols + c] = out;
        }
    }
    // last row/col simple copy
    dst.row(dst.rows-1).setTo(0);
    dst.col(dst.cols-1).setTo(0);
    return dst;
}

cv::Mat debayerSimple(const cv::Mat& bayer, Bayer pattern){
    CV_Assert(bayer.type()==CV_8UC1);
    cv::Mat dst(bayer.rows, bayer.cols, CV_8UC3);
    auto pack = [&](int r,int c,int R,int G,int B){
        uchar* d = dst.ptr<uchar>(r) + c*3;
        d[0] = (uchar)B; d[1] = (uchar)G; d[2] = (uchar)R; // BGR order
    };
    for(int r=0;r<bayer.rows-1;++r){
        for(int c=0;c<bayer.cols-1;++c){
            int ul = bayer.data[r*bayer.cols + c];
            int ur = bayer.data[r*bayer.cols + c+1];
            int ll = bayer.data[(r+1)*bayer.cols + c];
            int lr = bayer.data[(r+1)*bayer.cols + c+1];
            bool re = (r%2)==0, ce = (c%2)==0;
            // For each 2x2 output pixel at (r,c) just emit at that position
            auto emit = [&](int R,int G1,int G2,int B){
                pack(r,c,R,(G1+G2)/2,B);
            };
            switch(pattern){
                case Bayer::RGGB:
                    if(re&&ce) emit(ul, ur, ll, lr);
                    else if(re&&!ce) emit(ur, ul, lr, ll); // GRBG
                    else if(!re&&ce) emit(ll, lr, ul, ur); // GBRG (R at ll)
                    else emit(lr, ur, ll, ul); // BGGR
                    break;
                case Bayer::GBRG:
                    if(re&&ce) emit(ll, ul, lr, ur);
                    else if(re&&!ce) emit(lr, ur, ll, ul);
                    else if(!re&&ce) emit(ul, ur, ll, lr);
                    else emit(ur, ul, lr, ll);
                    break;
                case Bayer::BGGR:
                    if(re&&ce) emit(lr, ur, ll, ul);
                    else if(re&&!ce) emit(ll, ul, lr, ur);
                    else if(!re&&ce) emit(ur, ul, lr, ll);
                    else emit(ul, ur, ll, lr);
                    break;
                default: pack(r,c,ul, (ur+ll)/2, lr); break;
            }
        }
    }
    return dst;
}

cv::Mat debayerSimpleDown(const cv::Mat& bayer, Bayer pattern){
    CV_Assert(bayer.type()==CV_8UC1);
    cv::Mat dst(bayer.rows/2, bayer.cols/2, CV_8UC3);
    for(int r=0;r<dst.rows;++r){
        for(int c=0;c<dst.cols;++c){
            int u = c*2, v = r*2;
            int ul = bayer.data[v*bayer.cols + u];
            int ur = bayer.data[v*bayer.cols + u+1];
            int ll = bayer.data[(v+1)*bayer.cols + u];
            int lr = bayer.data[(v+1)*bayer.cols + u+1];
            int R=0,G=0,B=0;
            switch(pattern){
                case Bayer::RGGB: R=ul; B=lr; G=(ur+ll)/2; break;
                case Bayer::GBRG: R=ll; B=ur; G=(ul+lr)/2; break;
                case Bayer::BGGR: R=lr; B=ul; G=(ur+ll)/2; break;
                default: R=ul; B=lr; G=(ur+ll)/2; break;
            }
            uchar* d = dst.ptr<uchar>(r) + c*3;
            d[0]=(uchar)B; d[1]=(uchar)G; d[2]=(uchar)R;
        }
    }
    return dst;
}
