
/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef SKDEBUGCANVAS_H_
#define SKDEBUGCANVAS_H_

#include "SkCanvas.h"
#include "SkDrawCommand.h"
#include "SkPicture.h"
#include "SkTArray.h"
#include "SkString.h"

class SkDebugCanvas : public SkCanvas {
public:
    SkDebugCanvas(int width, int height);
    ~SkDebugCanvas();

    void toggleFilter(bool toggle);

    /**
        Executes all draw calls to the canvas.
        @param canvas  The canvas being drawn to
     */
    void draw(SkCanvas* canvas);

    /**
        Executes the draw calls in the specified range.
        @param canvas  The canvas being drawn to
        @param i  The beginning of the range
        @param j  The end of the range
        TODO(chudy): Implement
     */
    void drawRange(SkCanvas* canvas, int i, int j);

    /**
        Executes the draw calls up to the specified index.
        @param canvas  The canvas being drawn to
        @param index  The index of the final command being executed
     */
    void drawTo(SkCanvas* canvas, int index);

    /**
        Returns the most recently calculated transformation matrix
     */
    const SkMatrix& getCurrentMatrix() {
        return fMatrix;
    }

    /**
        Returns the most recently calculated clip
     */
    const SkIRect& getCurrentClip() {
        return fClip;
    }

    /**
        Returns the index of the last draw command to write to the pixel at (x,y)
     */
    int getCommandAtPoint(int x, int y, int index);

    /**
        Returns the draw command at the given index.
        @param index  The index of the command
     */
    SkDrawCommand* getDrawCommandAt(int index);

    /**
        Returns information about the command at the given index.
        @param index  The index of the command
     */
    SkTDArray<SkString*>* getCommandInfo(int index);

    /**
        Returns the visibility of the command at the given index.
        @param index  The index of the command
     */
    bool getDrawCommandVisibilityAt(int index);

    /**
        Returns the vector of draw commands
     */
    const SkTDArray<SkDrawCommand*>& getDrawCommands() const;

    /**
     * Returns the string vector of draw commands
     */
    SkTArray<SkString>* getDrawCommandsAsStrings() const;

    /**
        Returns length of draw command vector.
     */
    int getSize() {
        return fCommandVector.count();
    }

    /**
        Toggles the visibility / execution of the draw command at index i with
        the value of toggle.
     */
    void toggleCommand(int index, bool toggle);

    void setBounds(int width, int height) {
        fWidth = width;
        fHeight = height;
    }

    void setUserOffset(SkIPoint offset) {
        fUserOffset = offset;
    }

    void setUserScale(float scale) {
        fUserScale = scale;
    }

////////////////////////////////////////////////////////////////////////////////
// Inherited from SkCanvas
////////////////////////////////////////////////////////////////////////////////

    virtual void clear(SkColor) SK_OVERRIDE;

    virtual bool clipPath(const SkPath&, SkRegion::Op, bool) SK_OVERRIDE;

    virtual bool clipRect(const SkRect&, SkRegion::Op, bool) SK_OVERRIDE;

    virtual bool clipRRect(const SkRRect& rrect,
                           SkRegion::Op op = SkRegion::kIntersect_Op,
                           bool doAntiAlias = false) SK_OVERRIDE;

    virtual bool clipRegion(const SkRegion& region, SkRegion::Op op) SK_OVERRIDE;

    virtual bool concat(const SkMatrix& matrix) SK_OVERRIDE;

    virtual void drawBitmap(const SkBitmap&, SkScalar left, SkScalar top,
                            const SkPaint*) SK_OVERRIDE;

    virtual void drawBitmapRectToRect(const SkBitmap&, const SkRect* src,
                                  const SkRect& dst, const SkPaint*) SK_OVERRIDE;

    virtual void drawBitmapMatrix(const SkBitmap&, const SkMatrix&,
                                  const SkPaint*) SK_OVERRIDE;

    virtual void drawBitmapNine(const SkBitmap& bitmap, const SkIRect& center,
                                const SkRect& dst, const SkPaint*) SK_OVERRIDE;

    virtual void drawData(const void*, size_t) SK_OVERRIDE;

    virtual void drawOval(const SkRect& oval, const SkPaint&) SK_OVERRIDE;

    virtual void drawPaint(const SkPaint& paint) SK_OVERRIDE;

    virtual void drawPath(const SkPath& path, const SkPaint&) SK_OVERRIDE;

    virtual void drawPicture(SkPicture& picture) SK_OVERRIDE;

    virtual void drawPoints(PointMode, size_t count, const SkPoint pts[],
                            const SkPaint&) SK_OVERRIDE;

    virtual void drawPosText(const void* text, size_t byteLength,
                             const SkPoint pos[], const SkPaint&) SK_OVERRIDE;

    virtual void drawPosTextH(const void* text, size_t byteLength,
                              const SkScalar xpos[], SkScalar constY,
                              const SkPaint&) SK_OVERRIDE;

    virtual void drawRect(const SkRect& rect, const SkPaint&) SK_OVERRIDE;

    virtual void drawRRect(const SkRRect& rrect, const SkPaint& paint) SK_OVERRIDE;

    virtual void drawSprite(const SkBitmap&, int left, int top,
                            const SkPaint*) SK_OVERRIDE;

    virtual void drawText(const void* text, size_t byteLength, SkScalar x,
                          SkScalar y, const SkPaint&) SK_OVERRIDE;

    virtual void drawTextOnPath(const void* text, size_t byteLength,
                                const SkPath& path, const SkMatrix* matrix,
                                const SkPaint&) SK_OVERRIDE;

    virtual void drawVertices(VertexMode, int vertexCount,
                              const SkPoint vertices[], const SkPoint texs[],
                              const SkColor colors[], SkXfermode*,
                              const uint16_t indices[], int indexCount,
                              const SkPaint&) SK_OVERRIDE;

    virtual void restore() SK_OVERRIDE;

    virtual bool rotate(SkScalar degrees) SK_OVERRIDE;

    virtual int save(SaveFlags) SK_OVERRIDE;

    virtual int saveLayer(const SkRect* bounds, const SkPaint*, SaveFlags) SK_OVERRIDE;

    virtual bool scale(SkScalar sx, SkScalar sy) SK_OVERRIDE;

    virtual void setMatrix(const SkMatrix& matrix) SK_OVERRIDE;

    virtual bool skew(SkScalar sx, SkScalar sy) SK_OVERRIDE;

    virtual bool translate(SkScalar dx, SkScalar dy) SK_OVERRIDE;

private:
    typedef SkCanvas INHERITED;
    SkTDArray<SkDrawCommand*> fCommandVector;
    int fHeight;
    int fWidth;
    SkBitmap fBm;
    bool fFilter;
    int fIndex;
    SkIPoint fUserOffset;
    float fUserScale;
    SkMatrix fMatrix;
    SkIRect fClip;

    /**
        Number of unmatched save() calls at any point during a draw.
        If there are any saveLayer() calls outstanding, we need to resolve
        all of them, which in practice means resolving all save() calls,
        to avoid corruption of our canvas.
    */
    int fOutstandingSaveCount;

    /**
        Adds the command to the classes vector of commands.
        @param command  The draw command for execution
     */
    void addDrawCommand(SkDrawCommand* command);

    /**
        Applies any panning and zooming the user has specified before
        drawing anything else into the canvas.
     */
    void applyUserTransform(SkCanvas* canvas);
};

#endif
