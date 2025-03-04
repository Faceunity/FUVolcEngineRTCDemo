//
//  CustomProcessor.m
//  quickstart
//
//  Created by loco hwang on 2023/7/4.
//

#import "CustomProcessor.h"
#import "FUDemoManager.h"
#import "FUTestRecorder.h"

@implementation CustomProcessor

- (ByteRTCVideoFrame* _Nullable)processVideoFrame:(ByteRTCVideoFrame* _Nonnull)src_frame{
    
    NSLog(@"----%d",src_frame.rotation);
    [[FUDemoManager shared] checkAITrackedResult];
    if (![FUDemoManager shared].shouldRender) {
        return src_frame;
    }
    [FUDemoManager updateBeautyBlurEffect];
    [[FUTestRecorder shareRecorder] processFrameWithLog];
    CVPixelBufferRef srcPixelBuffer = src_frame.textureBuf;
    CVPixelBufferLockBaseAddress(srcPixelBuffer, 0);
    size_t width = (size_t)CVPixelBufferGetWidth(srcPixelBuffer);
    size_t height = (size_t)CVPixelBufferGetHeight(srcPixelBuffer);

    Byte *buffer0 = CVPixelBufferGetBaseAddressOfPlane(srcPixelBuffer, 0);
    Byte *buffer1 = CVPixelBufferGetBaseAddressOfPlane(srcPixelBuffer, 1);
    Byte *buffer2 = CVPixelBufferGetBaseAddressOfPlane(srcPixelBuffer, 2);
    size_t stride0 = CVPixelBufferGetBytesPerRowOfPlane(srcPixelBuffer, 0);
    size_t stride1 = CVPixelBufferGetBytesPerRowOfPlane(srcPixelBuffer, 1);
    size_t stride2 = CVPixelBufferGetBytesPerRowOfPlane(srcPixelBuffer, 2);;
    FUImageBuffer imageBuffer = FUImageBufferMakeI420(buffer0, buffer1, buffer2, width, height, stride0,stride1, stride2);
    FURenderInput *input = [[FURenderInput alloc] init];
    input.imageBuffer = imageBuffer;
    switch (src_frame.rotation) {
        case ByteRTCVideoRotation0:
            input.renderConfig.imageOrientation = FUImageOrientationDown;
            break;
        case ByteRTCVideoRotation90:
            input.renderConfig.imageOrientation = FUImageOrientationLeft;
            break;
        case ByteRTCVideoRotation180:
            input.renderConfig.imageOrientation = FUImageOrientationUP;
            break;
        case ByteRTCVideoRotation270:
            input.renderConfig.imageOrientation = FUImageOrientationRight;
            break;
    }
    //开启重力感应，内部会自动计算正确方向，设置fuSetDefaultRotationMode，无须外面设置
    input.renderConfig.gravityEnable = YES;

    //如果来源相机捕获的图片一定要设置，否则将会导致内部检测异常
    input.renderConfig.isFromFrontCamera = [FUDemoManager shared].stickerH;
    //该属性是指系统相机是否做了镜像: 一般情况前置摄像头出来的帧都是设置过镜像，所以默认需要设置下。如果相机属性未设置镜像，改属性不用设置。
//    input.renderConfig.isFromMirroredCamera = YES;
//    input.renderConfig.readBackToPixelBuffer = YES;
    input.renderConfig.stickerFlipH = [FUDemoManager shared].stickerH;
    FURenderOutput *output = [[FURenderKit shareRenderKit] renderWithInput:input];
    CVPixelBufferUnlockBaseAddress(srcPixelBuffer, 0);
    return src_frame;
}


@end
