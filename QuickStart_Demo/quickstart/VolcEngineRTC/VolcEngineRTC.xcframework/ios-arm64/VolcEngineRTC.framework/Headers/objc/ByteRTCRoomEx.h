#import "ByteRTCRoom.h"
#import "rtc/ByteRTCVideoDefinesEx.h"

@class ByteRTCRoomEx;

/**
 * 
 * @type callback
 * @brief ByteRTCRoomDelegateEx Class<br>
 * 注意：回调函数是在 SDK 内部线程（非 UI 线程）同步抛出来的，请不要做耗时操作或直接操作 UI，否则可能导致 app 崩溃。
 */
@protocol ByteRTCRoomDelegateEx <NSObject>
@optional
/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 房间内新增媒体流的回调。包含由摄像头/麦克风采集的媒体流或屏幕流。
 * @param streamKey 参看 ByteRTCStreamKey{@link #ByteRTCStreamKey}。
 * @param isScreen 是否是屏幕流。
 * @param type 远端媒体流的类型，参看 ByteRTCMediaStreamType{@link #ByteRTCMediaStreamType}。
 * @note 当房间内的远端用户调用 publishStream:mediaStreamType:{@link #ByteRTCRoomEx#publishStream:mediaStreamType:} 成功发布媒体流时，本地用户会收到该回调，此时本地用户可以自行选择是否调用 subscribeStream:streamIndex:mediaStreamType:{@link #ByteRTCRoomEx#subscribeStream:streamIndex:mediaStreamType:} 订阅此流。
 */
- (void)rtcRoom:(ByteRTCRoom* _Nonnull)rtcRoom
        onUserPublishStream:(ByteRTCRemoteStreamKey* _Nonnull)streamKey
                   isScreen:(BOOL)isScreen
            mediaStreamType:(ByteRTCMediaStreamType)type;
/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 房间内媒体流移除的回调。包含由摄像头/麦克风采集的媒体流或屏幕流。
 * @param streamKey 参看 ByteRTCStreamKey{@link #ByteRTCStreamKey}。
 * @param type 移除的远端流类型，参看 ByteRTCMediaStreamType{@link #ByteRTCMediaStreamType}。  
 * @param reason 远端流移除的原因，参看 ByteRTCStreamRemoveReason{@link #ByteRTCStreamRemoveReason}。
 * @note 收到该回调通知后，你可以自行选择是否调用 unsubscribeStream:streamIndex:mediaStreamType:{@link #ByteRTCRoomEx#unsubscribeStream:streamIndex:mediaStreamType:} 取消订阅此流。
 */
- (void)rtcRoom:(ByteRTCRoom* _Nonnull)rtcRoom
        onUserUnpublishStream:(ByteRTCRemoteStreamKey* _Nonnull)streamKey
              mediaStreamType:(ByteRTCMediaStreamType)type
                       reason:(ByteRTCStreamRemoveReason)reason;
/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 流状态改变回调，发生流相关的警告或错误时会收到此回调。
 * @param streamKey 参看 ByteRTCStreamKey{@link #ByteRTCStreamKey}。
 * @param state 流状态码，参看 ByteRTCErrorCode{@link #ByteRTCErrorCode} 和 ByteRTCWarningCode{@link #ByteRTCWarningCode}。
 * @param extraInfo 附加信息，目前为空。
 */
- (void)rtcRoom:(ByteRTCRoom* _Nonnull)rtcRoom
        onStreamStateChanged:(ByteRTCStreamKey* _Nonnull)streamKey
                       state:(NSInteger)state
                   extraInfo:(NSString* _Nonnull)extraInfo;

/**
 * 
 * @type callback
 * @region 房间管理
 * @brief 关于订阅媒体流状态改变的回调
 * @param rtcRoom ByteRTCRoom 对象
 * @param state 订阅媒体流状态，参看 ByteRTCSubscribeState{@link #ByteRTCSubscribeState}
 * @param userId 流发布用户的用户 ID
 * @param streamIndex 流属性 参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex} 
 * @param info 流的属性，参看 ByteRTCSubscribeConfig{@link #ByteRTCSubscribeConfig}
 * @note 本地用户收到该回调的时机包括：  <br>
 *        + 调用 subscribeStream:mediaStreamType:{@link #ByteRTCRoom#subscribeStream:mediaStreamType:} 或 unsubscribeStream:mediaStreamType:{@link #ByteRTCRoom#unsubscribeStream:mediaStreamType:} 订阅/取消订阅指定远端摄像头音视频流后；  <br>
 *        + 调用 subscribeScreen:mediaStreamType:{@link #ByteRTCRoom#subscribeScreen:mediaStreamType:} 或 unsubscribeScreen:mediaStreamType:{@link #ByteRTCRoom#unsubscribeScreen:mediaStreamType:} 订阅/取消订阅指定远端屏幕共享流后。
 */

- (void)rtcRoom:(ByteRTCRoom *_Nonnull)rtcRoom onStreamSubscribed:(ByteRTCSubscribeState)state
                userId:(NSString *_Nonnull)userId
                streamIndex:(ByteRTCStreamIndex)streamIndex
       subscribeConfig:(ByteRTCSubscribeConfig *_Nonnull)info;

@end

/**
 * 
 * @type api
 * @brief IRTCRoomEx Class
 */
BYTERTC_APPLE_EXPORT @interface ByteRTCRoomEx : ByteRTCRoom
/**
 * 
 * @hidden
 */
@property(nonatomic, weak) id<ByteRTCRoomDelegateEx> _Nullable delegateEx;
/**
 * 
 * @type api
 * @region 房间管理
 * @brief 在当前所在房间内发布媒体流
 * @param streamIndex 流索引，参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param type 媒体流类型，用于指定发布音频/视频，参看 ByteRTCMediaStreamType{@link #ByteRTCMediaStreamType}
 * @return  
 *        + 0: 调用成功。<br>
 *        + < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note 
 *        + 如果你已经在用户进房时通过调用 joinRoom:userInfo:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:roomConfig:} 成功选择了自动发布，则无需再调用本接口。<br>
 *        + 调用 setUserVisibility:{@link #ByteRTCRoom#setUserVisibility:} 方法将自身设置为不可见后无法调用该方法，需将自身切换至可见后方可调用该方法发布摄像头音视频流。 <br>
 *        + 如果你需要向多个房间发布流，调用 startForwardStreamToRooms:{@link #ByteRTCRoom#startForwardStreamToRooms:}。  <br>
 *        + 调用此方法后，房间中的所有远端用户会收到 rtcRoom:onUserPublishStream:isScreen:mediaStreamType:{@link #ByteRTCRoomDelegateEx#rtcRoom:onUserPublishStream:isScreen:mediaStreamType:} 回调通知，其中成功收到了音频流的远端用户会收到 rtcEngine:onFirstRemoteAudioFrame:{@link #ByteRTCVideoDelegate#rtcEngine:onFirstRemoteAudioFrame:} 回调，订阅了视频流的远端用户会收到 rtcEngine:onFirstRemoteVideoFrameDecoded:withFrameInfo:{@link #ByteRTCVideoDelegate#rtcEngine:onFirstRemoteVideoFrameDecoded:withFrameInfo:} 回调。<br>
 *        + 调用 unpublishStream:mediaStreamType:{@link #ByteRTCRoomEx#unpublishStream:mediaStreamType:} 取消发布。
 */
- (int)publishStream:(ByteRTCStreamIndex)streamIndex mediaStreamType:(ByteRTCMediaStreamType)type;
/**
 * 
 * @type api
 * @region 房间管理
 * @brief 停止将本地摄像头/麦克风采集的媒体流发布到当前所在房间中
 * @param streamIndex 流索引，参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param type 媒体流类型，用于指定发布音频/视频，参看 ByteRTCMediaStreamType{@link #ByteRTCMediaStreamType}
 * @return  
 *        + 0: 调用成功。<br>
 *        + < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 * @note  
 *        + 调用 publishStream:mediaStreamType:{@link #ByteRTCRoomEx#publishStream:mediaStreamType:} 手动发布摄像头音视频流后，你需调用此接口停止发布。<br>
 *        + 调用此方法停止发布音视频流后，房间中的其他用户将会收到 rtcRoom:onUserUnpublishStream:type:reason:{@link #ByteRTCRoomDelegate#rtcRoom:onUserUnpublishStream:type:reason:} 回调通知。
 */
- (int)unpublishStream:(ByteRTCStreamIndex)streamIndex mediaStreamType:(ByteRTCMediaStreamType)type;
/**
 * 
 * @type api
 * @region 房间管理
 * @brief 订阅房间内指定的媒体流，或更新对指定媒体流的订阅选项
 * @param streamIndex 流索引，参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param userId 指定订阅的远端发布音视频流的用户 ID。
 * @param type 媒体流类型，用于指定发布音频/视频，参看 ByteRTCMediaStreamType{@link #ByteRTCMediaStreamType}
 * @return 方法调用结果：<br>
 *        + 0：成功；<br>
 *        + <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 * @note  
 *        + 若当前用户在调用本接口时已经订阅该远端用户（手动订阅或自动订阅），则将根据本次传入的参数，更新订阅配置。<br>
 *        + 你必须先通过 rtcRoom:onUserPublishStream:isScreen:mediaStreamType:{@link #ByteRTCRoomDelegateEx#rtcRoom:onUserPublishStream:isScreen:mediaStreamType:} 回调获取当前房间里的远端摄像头音视频流信息，然后调用本方法按需订阅。  <br>
 *        + 调用该方法后，你会收到 rtcRoom:onStreamSubscribed:userId:subscribeConfig:{@link #ByteRTCRoomDelegate#rtcRoom:onStreamSubscribed:userId:subscribeConfig:} 通知方法调用结果。  <br>
 *        + 成功订阅远端用户的媒体流后，订阅关系将持续到调用 unsubscribeStream:streamIndex:mediaStreamType:{@link #ByteRTCRoomEx#unsubscribeStream:streamIndex:mediaStreamType:} 取消订阅或本端用户退房。 <br>
 *        + 关于其他调用异常，你会收到 rtcRoom:onStreamStateChanged:state:extraInfo:{@link #ByteRTCRoomDelegateEx#rtcRoom:onStreamStateChanged:state:extraInfo:} 回调通知，具体异常原因参看 ByteRTCErrorCode{@link #ByteRTCErrorCode}。
 */
- (int)subscribeStream:(NSString* _Nonnull)userId
            streamIndex:(ByteRTCStreamIndex)streamIndex
        mediaStreamType:(ByteRTCMediaStreamType)mediaStreamType;
/**
 * 
 * @type api
 * @region 房间管理
 * @brief 取消订阅房间内指定的媒体流。  <br>
 *        该方法对自动订阅和手动订阅模式均适用。
 * @param userId 指定订阅的远端发布音视频流的用户 ID。
 * @param streamIndex 流索引，参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param mediaStreamType 媒体流类型，用于指定发布音频/视频，参看 ByteRTCMediaStreamType{@link #ByteRTCMediaStreamType}
 * @return 方法调用结果：<br>
 *        + 0：成功；<br>
 *        + <0：失败。具体失败原因参看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus}。
 * @note  
 *        + 调用该方法后，你会收到 rtcRoom:onStreamSubscribed:userId:subscribeConfig:{@link #ByteRTCRoomDelegate#rtcRoom:onStreamSubscribed:userId:subscribeConfig:} 通知方法调用结果。  <br>
 *        + 关于其他调用异常，你会收到 rtcRoom:onStreamStateChanged:state:extraInfo:{@link #ByteRTCRoomDelegateEx#rtcRoom:onStreamStateChanged:state:extraInfo:} 回调通知，具体失败原因参看 ByteRTCErrorCode{@link #ByteRTCErrorCode}。
 */
- (int)unsubscribeStream:(NSString* _Nonnull)userId
             streamIndex:(ByteRTCStreamIndex)streamIndex
         mediaStreamType:(ByteRTCMediaStreamType)mediaStreamType;
/**
 * 
 * @type api
 * @region 多房间
 * @brief 通过设置 ByteRTCRoomEx{@link #ByteRTCRoomEx} 对象的事件句柄，监听此对象对应的回调事件。
 * @param roomDelegate 参见 ByteRTCRoomDelegateEx{@link #ByteRTCRoomDelegateEx}
 * @return  
 *        + 0: 调用成功。<br>
 *        + < 0 : 调用失败。查看 ByteRTCReturnStatus{@link #ByteRTCReturnStatus} 获得更多错误说明
 */
- (int)setRTCRoomDelegateEx:(id<ByteRTCRoomDelegateEx> _Nullable)roomDelegate;


/**
 * 
 * @type api
 * @region 视频管理
 * @brief 设置期望订阅的远端视频流的参数。
 * @param userId 期望配置订阅参数的远端视频流发布用户的 ID。
 * @param stream_index 流索引，参看 ByteRTCStreamIndex{@link #ByteRTCStreamIndex}。
 * @param remoteVideoConfig 期望配置的远端视频流参数，参看 ByteRTCRemoteVideoConfig{@link #ByteRTCRemoteVideoConfig}。
 * @return 方法调用结果： <br>
 *        + 0：成功；<br>
 *        + !0：失败。
 * @note 
 *        + 若使用 342 及以前版本的 SDK，调用该方法前请联系技术支持人员开启按需订阅功能。  <br>
 *        + 该方法仅在发布端调用 enableSimulcastMode:{@link #ByteRTCVideo#enableSimulcastMode:} 开启了发送多路视频流的情况下生效，此时订阅端将收到来自发布端与期望设置的参数最相近的一路流；否则订阅端只会收到一路参数为分辨率 640px × 360px、帧率 15fps 的视频流。  <br>
 *        + 若发布端开启了推送多路流功能，但订阅端不对流参数进行设置，则默认接受发送端设置的分辨率最大的一路视频流。  <br>
 *        + 该方法需在进房后调用，若想进房前设置，你需调用 joinRoom:userInfo:roomConfig:{@link #ByteRTCRoom#joinRoom:userInfo:roomConfig:}，并对 `roomConfig` 中的 `remoteVideoConfig` 进行设置。  <br>
 *        + SDK 会根据发布端和所有订阅端的设置灵活调整视频流的参数，具体调整策略详见[推送多路流](https:
 */
- (int)setRemoteVideoConfig:(NSString * _Nonnull) userId streamIndex:(ByteRTCStreamIndex)streamIndex remoteVideoConfig:(ByteRTCRemoteVideoConfig *_Nonnull) remoteVideoConfig;

@end
