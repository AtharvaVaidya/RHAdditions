//
//  NSImage+RHResizableImageAdditions.h
//
//  Created by Richard Heard on 15/04/13.
//  Copyright (c) 2013 Richard Heard. All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//  notice, this list of conditions and the following disclaimer in the
//  documentation and/or other materials provided with the distribution.
//  3. The name of the author may not be used to endorse or promote products
//  derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
//  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
//  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
//  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
//  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Adds a class and category on NSImage that allows for super easy NSDrawNinePartImage drawing.
// Attempts to mirror the iOS5+ resizableImage methods somewhat.

#import <Cocoa/Cocoa.h>

@class RHResizableImage;

typedef struct _RHEdgeInsets {
    CGFloat top, left, bottom, right;  // specify amount to inset (positive) for each of the edges. values can be negative to 'outset'
} RHEdgeInsets;

extern RHEdgeInsets RHEdgeInsetsMake(CGFloat top, CGFloat left, CGFloat bottom, CGFloat right);
extern CGRect RHEdgeInsetsInsetRect(CGRect rect, RHEdgeInsets insets, BOOL flipped); //if flipped origin is top-left otherwise origin is bottom-left (OSX Default is NO)
extern BOOL RHEdgeInsetsEqualToEdgeInsets(RHEdgeInsets insets1, RHEdgeInsets insets2);
extern const RHEdgeInsets RHEdgeInsetsZero;


typedef enum NSInteger {
    RHResizableImageResizingModeTile,
    RHResizableImageResizingModeStretch,
} RHResizableImageResizingMode;

@interface NSImage (RHResizableImageAdditions)

-(RHResizableImage*)stretchableImageWithLeftCapWidth:(CGFloat)leftCapWidth topCapHeight:(CGFloat)topCapHeight; // right cap is calculated as width - leftCapWidth - 1; bottom cap is calculated as height - topCapWidth - 1;

-(RHResizableImage*)resizableImageWithCapInsets:(RHEdgeInsets)capInsets; //create a resizable version of this image. the interior is tiled when drawn.
-(RHResizableImage*)resizableImageWithCapInsets:(RHEdgeInsets)capInsets resizingMode:(RHResizableImageResizingMode)resizingMode; //the interior is resized according to the resizingMode

@end


@interface RHResizableImage : NSImage {
    //ivars are private
    RHEdgeInsets _capInsets;
    RHResizableImageResizingMode _resizingMode;
    
    NSArray *_imagePieces;
    
    NSImage* _cachedImage;
    NSSize _cachedImageSize;
}

-(id)initWithImage:(NSImage*)image leftCapWidth:(CGFloat)leftCapWidth topCapHeight:(CGFloat)topCapHeight; // right cap is calculated as width - leftCapWidth - 1; bottom cap is calculated as height - topCapWidth - 1;

-(id)initWithImage:(NSImage*)image capInsets:(RHEdgeInsets)capInsets;
-(id)initWithImage:(NSImage*)image capInsets:(RHEdgeInsets)capInsets resizingMode:(RHResizableImageResizingMode)resizingMode; //designated initializer

@property(nonatomic,readonly) RHEdgeInsets capInsets; //default is RHEdgeInsetsZero
@property(nonatomic,readonly) RHResizableImageResizingMode resizingMode; //default is UIImageResizingModeTile

-(void)drawInRect:(NSRect)rect;
-(void)drawInRect:(NSRect)rect operation:(NSCompositingOperation)op fraction:(CGFloat)requestedAlpha;
-(void)drawInRect:(NSRect)rect operation:(NSCompositingOperation)op fraction:(CGFloat)requestedAlpha respectFlipped:(BOOL)respectContextIsFlipped hints:(NSDictionary *)hints;
-(void)drawInRect:(NSRect)rect fromRect:(NSRect)fromRect operation:(NSCompositingOperation)op fraction:(CGFloat)requestedAlpha respectFlipped:(BOOL)respectContextIsFlipped hints:(NSDictionary *)hints;

-(void)nonStretchedDrawInRect:(NSRect)rect fromRect:(NSRect)fromRect operation:(NSCompositingOperation)op fraction:(CGFloat)requestedAlpha respectFlipped:(BOOL)respectContextIsFlipped hints:(NSDictionary *)hints; //super passthrough


@end

//utilities
extern NSImage* RHCapturePieceOfImageFromRect(NSImage *image, CGRect rect);
extern NSArray* RHNinePartPiecesFromImageWithInsets(NSImage *image, RHEdgeInsets capInsets);


//nine part
extern void RHDrawNinePartImage(NSRect frame, NSImage *topLeftCorner, NSImage *topEdgeFill, NSImage *topRightCorner, NSImage *leftEdgeFill, NSImage *centerFill, NSImage *rightEdgeFill, NSImage *bottomLeftCorner, NSImage *bottomEdgeFill, NSImage *bottomRightCorner, NSCompositingOperation op, CGFloat alphaFraction, BOOL shouldTile);

extern void RHDrawImageInRect(NSImage* image, NSRect rect, NSCompositingOperation op, CGFloat fraction, BOOL tile);
extern void RHDrawTiledImageInRect(NSImage* image, NSRect rect, NSCompositingOperation op, CGFloat fraction);


