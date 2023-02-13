#ifndef RECT_H
#define RECT_H

struct Rect_st{
	int left;
	int top;
	int right;
	int buttom;
};
typedef struct Rect_st Rect;

struct RectF_st{
	float left;
	float top;
	float right;
	float buttom;
};
typedef struct RectF_st RectF;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif



