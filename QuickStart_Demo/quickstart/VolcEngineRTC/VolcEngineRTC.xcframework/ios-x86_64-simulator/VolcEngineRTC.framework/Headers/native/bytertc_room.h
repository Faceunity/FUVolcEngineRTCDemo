/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Room Interface
 */

#pragma once

#include "rtc/bytertc_video_effect_interface.h"  
#include "rtc/bytertc_defines.h"
#include "bytertc_room_event_handler.h"
#include "rtc/bytertc_transcoder_interface.h"
#include "rtc/bytertc_publicstream_interface.h"
#include "rtc/bytertc_range_audio_interface.h"
#include "rtc/bytertc_spatial_audio_interface.h"
#include "rtc/bytertc_panoramic_video_interface.h"
namespace bytertc {

/**
 * 
 * @type api
 * @brief 房间接口
 */

class IRTCRoom {
public:
    /**
     * 
     * @hidden constructor/destructor
     * @brief 析构函数
     */
    
    virtual ~IRTCRoom() {
    }

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 退出并销毁调用 createRTCRoom{@link #IRTCVideo#createRTCRoom} 所创建的房间实例。
     * @list 房间管理
     * @order 1
     */
    
    virtual void destroy() = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 设置用户可见性。未调用该接口前，本地用户默认对他人可见。 <br>
     *        默认情况下，一个 RTC 房间最多同时容纳 50 名可见用户，最多 30 人可同时上麦。更多信息参看[用户和媒体流上限](https:
     * @param enable 设置用户是否对房间内其他用户可见： <br>
     *        - true: 可见，用户可以在房间内发布音视频流，房间中的其他用户将收到用户的行为通知，例如进房、开启视频采集和退房。
     *        - false: 不可见，用户不可以在房间内发布音视频流，房间中的其他用户不会收到用户的行为通知，例如进房、开启视频采集和退房。
     * @return
     *        - 0: 调用成功。
     *        - < 0: 调用失败。参看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明。
     * @note
     *       - 在加入房间前后，用户均可调用此方法设置用户可见性。
     *       - 设置用户可见性，会收到设置成功/失败回调 onUserVisibilityChanged{@link #IRTCRoomEventHandler#onUserVisibilityChanged}。（v3.54 新增）
     *   &#x0020;  • 在加入房间前设置用户可见性，若设置的可见性与默认值不同，将在加入房间时触发本回调。 <br>
     *   &#x0020;  • 在加入房间后设置用户可见性，若可见性前后不同，会触发本回调。 <br>
     *   &#x0020;  • 在断网重连后，若可见性发生改变，会触发本回调。 <br>
     *       - 在房间内，调用此方法成功切换用户可见性后，房间内其他用户会收到相应的回调。
     *   &#x0020;  • 从可见换至不可见时，房间内其他用户会收到 onUserLeave{@link #IRTCRoomEventHandler#onUserLeave}。 <br>
     *   &#x0020;  • 从不可见切换至可见时，房间内其他用户会收到 onUserJoined{@link #IRTCRoomEventHandler#onUserJoined}。 <br>
     *   &#x0020;  • 若调用该方法将可见性设为 `false`，此时尝试发布流会收到 `kWarningCodePublishStreamForbiden` 警告。
     * @list 房间管理
     * @order 6
     */
    
    virtual int setUserVisibility(bool enable) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 通过设置 IRTCRoom{@link #IRTCRoom} 对象的事件句柄，监听此对象对应的回调事件。
     * @param room_event_handler 参见 IRTCRoomEventHandler{@link #IRTCRoomEventHandler}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @list 房间管理
     * @order 2
     */
    
    virtual int setRTCRoomEventHandler(IRTCRoomEventHandler* room_event_handler) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 加入房间。 <br>
     *        调用 createRTCRoom{@link #IRTCVideo#createRTCRoom} 创建房间后，调用此方法加入房间，同房间内其他用户进行音视频通话。
     * @param token 动态密钥，用于对登录用户进行鉴权验证。 <br>
     *        进入房间需要携带 Token。测试时可使用控制台生成临时 Token，正式上线需要使用密钥 SDK 在您的服务端生成并下发 Token。Token 有效期及生成方式参看[使用 Token 完成鉴权](70121)。 <br>
     *       - 使用不同 App ID 的 App 是不能互通的。
     *       - 请务必保证生成 Token 使用的 App ID 和创建引擎时使用的 App ID 相同，否则会导致加入房间失败。
     * @param user_info 用户信息，参看 UserInfo{@link #UserInfo}。
     * @param config 房间参数配置，设置房间模式以及是否自动发布或订阅流。具体配置模式参看 RTCRoomConfig{@link #RTCRoomConfig}。
     * @return
     *        -  0: 成功
     *        - -1：room_id / user_info.uid 包含了无效的参数。
     *        - -2：已经在房间内。接口调用成功后，只要收到返回值为 0 ，且未调用 leaveRoom:{@link #IRTCRoom#leaveRoom} 成功，则再次调用进房接口时，无论填写的房间 ID 和用户 ID 是否重复，均触发此返回值。
     *        调用失败时，具体失败原因会通过 onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} 回调告知。
     * @note
     *       - 同一个 App ID 的同一个房间内，每个用户的用户 ID 必须是唯一的。如果两个用户的用户 ID 相同，则后进房的用户会将先进房的用户踢出房间，并且先进房的用户会收到 onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} 回调通知，错误类型详见 ErrorCode{@link #ErrorCode} 中的 kErrorCodeDuplicateLogin。
     *       - 本地用户调用此方法加入房间成功后，会收到 onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} 回调通知。若本地用户同时为可见用户，加入房间时远端用户会收到 onUserJoined{@link #IRTCRoomEventHandler#onUserJoined} 回调通知。
     *       - 房间内不可见用户的容量远远大于可见用户，而且用户默认可见，因此对于不参与互动的用户，你需要在其进房后调用 setUserVisibility{@link #IRTCRoom#setUserVisibility 更改为不可见用户。从而避免因房间内用户达到数量上限所导致的进房失败。默认情况下，一个 RTC 房间最多同时容纳 50 名可见用户，其中最多 30 人可同时上麦，更多信息参看[用户和媒体流上限](https:
     *       - 用户加入房间成功后，在本地网络状况不佳的情况下，SDK 可能会与服务器失去连接，并触发 onConnectionStateChanged{@link #IRTCVideoEventHandler#onConnectionStateChanged} 回调。此时 SDK 会自动重试，直到成功重连。重连成功后，本地会收到 onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged} 回调通知。
     * @list 房间管理
     * @order 3
     */
    
    virtual int joinRoom(const char* token, const UserInfo& user_info, const RTCRoomConfig& config) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 离开房间。 <br>
     *        用户调用此方法离开房间，结束通话过程，释放所有通话相关的资源。 <br>
     *        加入房间后，必须调用此方法结束通话，否则无法开始下一次通话。无论当前是否在房间内，都可以调用此方法。重复调用此方法没有负面影响。 <br>
     *        此方法是异步操作，调用返回时并没有真正退出房间。真正退出房间后，本地会收到 onLeaveRoom{@link #IRTCRoomEventHandler#onLeaveRoom} 回调通知。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 可见的用户离开房间后，房间内其他用户会收到 onUserLeave{@link #IRTCRoomEventHandler#onUserLeave} 回调通知。
     *       - 如果调用此方法后立即销毁引擎，SDK 将无法触发 onLeaveRoom{@link #IRTCRoomEventHandler#onLeaveRoom} 回调。
     * @list 房间管理
     * @order 5
     */
    
    virtual int leaveRoom() = 0;

    /**
     * 
     * @type api
     * @brief 更新 Token。 <br>
     *        收到 onTokenWillExpire{@link #IRTCRoomEventHandler#onTokenWillExpire}，onPublishPrivilegeTokenWillExpire{@link #IRTCRoomEventHandler#onPublishPrivilegeTokenWillExpire}， 或 onSubscribePrivilegeTokenWillExpire{@link #IRTCRoomEventHandler#onSubscribePrivilegeTokenWillExpire} 时，你必须重新获取 Token，并调用此方法更新 Token，以保证通话的正常进行。
     * @param token 重新获取的有效 Token。 <br>
     *        如果 Token 无效，你会收到 onRoomStateChanged{@link #IRTCRoomEventHandler#onRoomStateChanged}，错误码是 `-1010`。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *      - 3.50（不含）以前的版本中，Token 中的发布和订阅权限为保留参数，无实际控制权限；3.50 及以后版本开放 Token 发布订阅控制权限，如需通过 Token 控制连麦权限，请联系技术支持团队开通白名单后支持。
     *      - 请勿同时调用 updateToken{@link #IRTCRoom#updateToken} 和 joinRoom{@link #IRTCRoom#joinRoom} 方法更新 Token。若因 Token 过期或无效导致加入房间失败或已被移出房间，你应该在获取新的有效 Token 后调用 joinRoom{@link #IRTCRoom#joinRoom} 重新加入房间。
     * @list 房间管理
     * @order 7
     */
    
    virtual int updateToken(const char* token) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 给房间内指定的用户发送点对点文本消息（P2P）。
     * @param uid <br>
     *        消息接收用户的 ID
     * @param message <br>
     *        发送的文本消息内容。 <br>
     *        消息不超过 64 KB。
     * @param config 消息类型，参看 MessageConfig{@link #MessageConfig}。
     * @return 这次发送消息的编号，从 1 开始递增。
     * @note
     *       - 在发送房间内文本消息前，必须先调用 joinRoom{@link #IRTCRoom#joinRoom} 加入房间。
     *       - 调用该函数后会收到一次 onUserMessageSendResult{@link #IRTCRoomEventHandler#onUserMessageSendResult} 回调，通知消息发送方发送成功或失败；
     *       - 若文本消息发送成功，则 uid 所指定的用户会收到 onUserMessageReceived{@link #IRTCRoomEventHandler#onUserMessageReceived} 回调。
     * @list 消息
     * @order 7
     */
    
    virtual int64_t sendUserMessage(const char* uid, const char* message, MessageConfig config = kMessageConfigReliableOrdered) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 给房间内指定的用户发送点对点二进制消息（P2P）。
     * @param uid <br>
     *        消息接收用户的 ID
     * @param length <br>
     *        二进制字符串的长度。
     * @param message <br>
     *        二进制消息的内容。 <br>
     *        消息不超过 46KB。
     * @param config 消息类型，参看 MessageConfig{@link #MessageConfig}。
     * @return 这次发送消息的编号，从 1 开始递增。
     * @note
     *       - 在发送房间内二进制消息前，必须先调用 joinRoom{@link #IRTCRoom#joinRoom} 加入房间。
     *       - 调用该函数后会收到一次 onUserMessageSendResult{@link #IRTCRoomEventHandler#onUserMessageSendResult} 回调，通知消息发送方发送成功或失败；
     *       - 若二进制消息发送成功，则 uid 所指定的用户会收到 onUserBinaryMessageReceived{@link #IRTCRoomEventHandler#onUserBinaryMessageReceived} 回调。
     * @list 消息
     * @order 8
     */
    
    virtual int64_t sendUserBinaryMessage(const char* uid, int length, const uint8_t* message, MessageConfig config = kMessageConfigReliableOrdered) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 给房间内的所有其他用户群发文本消息。
     * @param message <br>
     *        用户发送的广播消息 <br>
     *        消息不超过 64 KB。
     * @note
     *       - 在发送房间内二进制消息前，必须先调用 joinRoom{@link #IRTCRoom#joinRoom} 加入房间。
     *       - 调用该函数后，会收到一次 onRoomMessageSendResult{@link #IRTCRoomEventHandler#onRoomMessageSendResult} 回调。
     *       - 同一房间内的其他用户会收到 onRoomMessageReceived{@link #IRTCRoomEventHandler#onRoomMessageReceived} 回调。
     * @list 消息
     * @order 0
     */
    
    virtual int64_t sendRoomMessage(const char* message) = 0;
    /**
     * 
     * @type api
     * @region 多房间
     * @brief 给房间内的所有其他用户群发二进制消息。
     * @param size <br>
     *        发送的二进制消息长度
     * @param message <br>
     *        用户发送的二进制广播消息 <br>
     *        消息不超过 46KB。
     * @return 这次发送消息的编号，从 1 开始递增。
     * @note
     *       - 在发送房间内二进制消息前，必须先调用 joinRoom{@link #IRTCRoom#joinRoom} 加入房间。
     *       - 调用该函数后，会收到一次 onRoomMessageSendResult{@link #IRTCRoomEventHandler#onRoomMessageSendResult} 回调。
     *       - 同一房间内的其他用户会收到 onRoomBinaryMessageReceived{@link #IRTCRoomEventHandler#onRoomBinaryMessageReceived} 回调。
     * @list 消息
     * @order 1
     */
    
    virtual int64_t sendRoomBinaryMessage(int size, const uint8_t* message) = 0;

    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 在当前所在房间内发布本地通过摄像头/麦克风采集的媒体流
     * @param type 媒体流类型，用于指定发布音频/视频，参看 MediaStreamType{@link #MediaStreamType}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 如果你已经在用户进房时通过调用 joinRoom{@link #IRTCRoom#joinRoom} 成功选择了自动发布，则无需再调用本接口。
     *        - 调用 setUserVisibility{@link #IRTCRoom#setUserVisibility} 方法将自身设置为不可见后无法调用该方法，需将自身切换至可见后方可调用该方法发布摄像头音视频流。
     *        - 如果你需要发布屏幕共享流，调用 publishScreen{@link #IRTCRoom#publishScreen}。(Linux 不适用)
     *        - 如果你需要向多个房间发布流，调用 startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms}。
     *        - 调用此方法后，房间中的所有远端用户会收到 onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream} 回调通知，其中成功收到了音频流的远端用户会收到 onFirstRemoteAudioFrame{@link #IRTCVideoEventHandler#onFirstRemoteAudioFrame} 回调，订阅了视频流的远端用户会收到 onFirstRemoteVideoFrameDecoded{@link #IRTCVideoEventHandler#onFirstRemoteVideoFrameDecoded} 回调。
     *        - 调用 unpublishStream{@link #IRTCRoom#unpublishStream} 取消发布。
     * @list 音视频传输
     * @order 0
     */
    
    virtual int publishStream(MediaStreamType type) = 0;

    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 停止将本地摄像头/麦克风采集的媒体流发布到当前所在房间中
     * @param type 媒体流类型，用于指定停止发布音频/视频，参看 MediaStreamType{@link #MediaStreamType}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用 publishStream{@link #IRTCRoom#publishStream} 手动发布摄像头音视频流后，你需调用此接口停止发布。
     *        - 调用此方法停止发布音视频流后，房间中的其他用户将会收到 onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream} 回调通知。
     * @list 音视频传输
     * @order 1
     */
    
    virtual int unpublishStream(MediaStreamType type) = 0;

    /**
     * 
     * @type api
     * @region 屏幕共享
     * @brief 在当前所在房间内手动发布本地屏幕共享音视频流。 <br>
     *        如果你需要手动向多个房间发布屏幕共享流，可以使用同样的 uid 加入多个房间，并在每个房间调用该方法。同时，你可以在不同的房间发布不同的屏幕共享流类型。
     * @param type 媒体流类型，用于指定发布屏幕音频/视频，参看 MediaStreamType{@link #MediaStreamType}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 即使你已经在用户进房时通过调用 joinRoom{@link #IRTCRoom#joinRoom} 成功选择了自动发布，也需要调用本接口。
     *        - 调用 setUserVisibility{@link #IRTCRoom#setUserVisibility} 方法将自身设置为不可见后无法调用该方法，需将自身切换至可见后方可调用该方法发布屏幕流。
     *        - 调用该方法后，房间中的所有远端用户会收到 onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen} 回调，其中成功收到音频流的远端用户会收到 onFirstRemoteAudioFrame{@link #IRTCVideoEventHandler#onFirstRemoteAudioFrame} 回调，订阅了视频流的远端用户会收到 onFirstRemoteVideoFrameDecoded{@link #IRTCVideoEventHandler#onFirstRemoteVideoFrameDecoded} 回调。
     *        - 调用该方法后，本地用户会收到 onScreenVideoFrameSendStateChanged{@link #IRTCVideoEventHandler#onScreenVideoFrameSendStateChanged}。
     *        - 调用 unpublishScreen{@link #IRTCRoom#unpublishScreen} 取消发布。
     *        - 对 Linux，仅支持发布视频流。
     *        - 查看 [PC 端屏幕共享](https:
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 7
     */
    
    virtual int publishScreen(MediaStreamType type) = 0;

    /**
     * 
     * @type api
     * @region 屏幕共享
     * @brief 停止将本地屏幕共享音视频流发布到当前所在房间中
     * @param type 媒体流类型，用于指定停止发布屏幕音频/视频，参看 MediaStreamType{@link #MediaStreamType}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用 publishScreen{@link #IRTCRoom#publishScreen} 发布屏幕流后，你需调用此接口停止发布。
     *        - 调用此方法停止发布屏幕音视频流后，房间中的其他用户将会收到 onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen} 回调。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 8
     */
    
    virtual int unpublishScreen(MediaStreamType type) = 0;

    /**
     * 
     * @deprecated since 3.36 and will be deleted in 3.51, use subscribeStream{@link #IRTCRoom#subscribeStream}, unsubscribeStream{@link #IRTCRoom#unsubscribeStream}, subscribeScreen{@link #IRTCRoom#subscribeScreen} and unsubscribeScreen{@link #IRTCRoom#unsubscribeScreen} instead.
     * @type api
     * @region 多房间
     * @brief 订阅房间内指定的远端音视频流。 <br>
     *        无论是自动订阅还是手动订阅模式，你都可以调用此方法按需订阅房间中的音视频流。 <br>
     *        该方法也可用于更新已经订阅的流的属性、媒体类型等配置。
     * @param user_id 指定订阅的远端发布音视频流的用户 ID。
     * @param stream_type 流属性，用于指定订阅主流/屏幕流，参看 StreamIndex{@link #StreamIndex}。
     * @param media_type 媒体类型，用于指定订阅音/视频，参看 SubscribeMediaType{@link #SubscribeMediaType}。
     * @param video_config 视频订阅配置，参看 SubscribeVideoConfig{@link #SubscribeVideoConfig}。
     * @note
     *        - 若订阅失败，你会收到 onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} 回调通知，具体失败原因参看 ErrorCode{@link #ErrorCode}。
     *        - 若调用 pauseAllSubscribedStream{@link #IRTCRoom#pauseAllSubscribedStream} 暂停接收远端音视频流，此时仍可使用该方法对暂停接收的流进行设置，你会在调用 resumeAllSubscribedStream{@link #IRTCRoom#resumeAllSubscribedStream} 恢复接收流后收到修改设置后的流。
     */
    
    BYTERTC_DEPRECATED virtual int subscribeUserStream(const char* user_id, StreamIndex stream_type, SubscribeMediaType media_type, const SubscribeVideoConfig& video_config) = 0;
    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 设置期望订阅的远端视频流的参数。
     * @param user_id 期望配置订阅参数的远端视频流发布用户的 ID。
     * @param remote_video_config 期望配置的远端视频流参数，参看 RemoteVideoConfig{@link #RemoteVideoConfig}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *        - 若使用 342 及以前版本的 SDK，调用该方法前请联系技术支持人员开启按需订阅功能。
     *        - 该方法仅在发布端调用 enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode} 开启了发送多路视频流的情况下生效，此时订阅端将收到来自发布端与期望设置的参数最相近的一路流；否则订阅端只会收到一路参数为分辨率 640px × 360px、帧率 15fps 的视频流。
     *        - 若发布端开启了推送多路流功能，但订阅端不对流参数进行设置，则默认接受发送端设置的分辨率最大的一路视频流。
     *        - 该方法需在进房后调用，若想进房前设置，你需调用 joinRoom{@link #IRTCRoom#joinRoom}，并对 `room_config` 中的 `remote_video_config` 进行设置。
     *        - SDK 会根据发布端和所有订阅端的设置灵活调整视频流的参数，具体调整策略详见[推送多路流](https:
     * @list 网络管理
     * @order 1
     */
    
    virtual int setRemoteVideoConfig(const char *user_id,const RemoteVideoConfig &remote_video_config) = 0;

    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 订阅房间内指定的通过摄像头/麦克风采集的媒体流，或更新对指定远端用户的订阅选项
     * @param user_id 指定订阅的远端发布音视频流的用户 ID。
     * @param type 媒体流类型，用于指定订阅音频/视频。参看 MediaStreamType{@link #MediaStreamType}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *        - 若当前用户在调用本接口时已经订阅该远端用户（手动订阅或自动订阅），则将根据本次传入的参数，更新订阅配置。
     *        - 你必须先通过 onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream} 回调获取当前房间里的远端摄像头音视频流信息，然后调用本方法按需订阅。
     *        - 调用该方法后，你会收到 onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed} 通知方法调用结果。
     *        - 成功订阅远端用户的媒体流后，订阅关系将持续到调用 unsubscribeStream{@link #IRTCRoom#unsubscribeStream} 取消订阅或本端用户退房。
     *        - 关于其他调用异常，你会收到 onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} 回调通知，具体异常原因参看 ErrorCode{@link #ErrorCode}。
     * @list 音视频传输
     * @order 3
     */
    
    virtual int subscribeStream(const char *user_id,MediaStreamType type) = 0;
    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 订阅房间内所有通过摄像头/麦克风采集的媒体流，或更新订阅选项。
     * @param type 媒体流类型，用于指定订阅音频/视频。参看 MediaStreamType{@link #MediaStreamType}。
     * @return
     *        0: 方法调用成功 <br>
     *        !0: 方法调用失败
     * @note
     *        - 多次调用订阅接口时，将根据末次调用接口和传入的参数，更新订阅配置。
     *        - 开启音频选路后，如果房间内的媒体流超过上限，建议通过调用 subscribeStream{@link #IRTCRoom#subscribeStream} 逐一指定需要订阅的媒体流。
     *        - 调用该方法后，你会收到 onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed} 通知方法调用结果。
     *        - 成功订阅远端用户的媒体流后，订阅关系将持续到调用 unsubscribeStream{@link #IRTCRoom#unsubscribeStream} 取消订阅或本端用户退房。
     *        - 关于其他调用异常，你会收到 onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} 回调通知，具体异常原因参看 ErrorCode{@link #ErrorCode}。
     * @list 音视频传输
     * @order 5
     */
     
    virtual int subscribeAllStreams(MediaStreamType type) = 0;

    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 取消订阅房间内指定的通过摄像头/麦克风采集的媒体流。 <br>
     *        该方法对自动订阅和手动订阅模式均适用。
     * @param user_id 指定取消订阅的远端发布音视频流的用户 ID。
     * @param type 媒体流类型，用于指定取消订阅音频/视频。参看 MediaStreamType{@link #MediaStreamType}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *        - 调用该方法后，你会收到 onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed} 通知方法调用结果。
     *        - 关于其他调用异常，你会收到 onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} 回调通知，具体失败原因参看 ErrorCode{@link #ErrorCode}。
     * @list 音视频传输
     * @order 4
     */
    
    virtual int unsubscribeStream(const char *user_id,MediaStreamType type) = 0;
    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 取消订阅房间内所有的通过摄像头/麦克风采集的媒体流。 <br>
     *        自动订阅和手动订阅的流都可以通过本方法取消订阅。
     * @param type 媒体流类型，用于指定取消订阅音频/视频。参看 MediaStreamType{@link #MediaStreamType}。
     * @return 方法调用结果： <br>
     *        - 0：成功
     *        - !0：失败
     * @note
     *        - 调用该方法后，你会收到 onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed} 通知方法调用结果。
     *        - 关于其他调用异常，你会收到 onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} 回调通知，具体失败原因参看 ErrorCode{@link #ErrorCode}。
     * @list 音视频传输
     * @order 6
     */
    
    virtual int unsubscribeAllStreams(MediaStreamType type) = 0;
    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 订阅房间内指定的远端屏幕共享音视频流，或更新对指定远端用户的订阅选项
     * @param user_id 指定订阅的远端发布屏幕流的用户 ID。
     * @param type 媒体流类型，用于指定订阅音频/视频。参看 MediaStreamType{@link #MediaStreamType}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *        - 若当前用户在调用本接口时已经订阅该远端用户（手动订阅或自动订阅），则将根据本次传入的参数，更新订阅配置。
     *        - 你必须先通过 onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen}} 回调获取当前房间里的远端屏幕流信息，然后调用本方法按需订阅。
     *        - 调用该方法后，你会收到 onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed} 通知方法调用结果。
     *        - 成功订阅远端用户的媒体流后，订阅关系将持续到调用 unsubscribeScreen{@link #IRTCRoom#unsubscribeScreen} 取消订阅或本端用户退房。
     *        - 关于其他调用异常，你会收到 onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} 回调通知，具体异常原因参看 ErrorCode{@link #ErrorCode}。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 18
     */
    
    virtual int subscribeScreen(const char *user_id,MediaStreamType type) = 0;

    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 取消订阅房间内指定的远端屏幕共享音视频流。 <br>
     *        该方法对自动订阅和手动订阅模式均适用。
     * @param user_id 指定取消订阅的远端发布屏幕流的用户 ID。
     * @param type 媒体流类型，用于指定取消订阅音频/视频。参看 MediaStreamType{@link #MediaStreamType}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *        - 调用该方法后，你会收到 onStreamSubscribed{@link #IRTCRoomEventHandler#onStreamSubscribed} 通知方法调用结果。
     *        - 关于其他调用异常，你会收到 onStreamStateChanged{@link #IRTCRoomEventHandler#onStreamStateChanged} 回调通知，具体失败原因参看 ErrorCode{@link #ErrorCode}。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 19
     */
    
    virtual int unsubscribeScreen(const char *user_id,MediaStreamType type) = 0;

    /**
    * 
     * @hidden for internal use only.
     */
    
    virtual int enableSubscribeLocalStream(bool enable) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 暂停接收来自远端的媒体流。
     * @param media_type 媒体流类型，指定需要暂停接收音频还是视频流，参看 PauseResumeControlMediaType{@link #PauseResumeControlMediaType}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 该方法仅暂停远端流的接收，并不影响远端流的采集和发送；
     *        - 该方法不改变用户的订阅状态以及订阅流的属性。
     *        - 若想恢复接收远端流，需调用 resumeAllSubscribedStream{@link #IRTCRoom#resumeAllSubscribedStream}。
     *        - 多房间场景下，仅暂停接收发布在当前所在房间的流。
     * @list 音视频传输
     * @order 13
     */
    
    virtual int pauseAllSubscribedStream(PauseResumeControlMediaType media_type) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 恢复接收来自远端的媒体流
     * @param media_type 媒体流类型，指定需要暂停接收音频还是视频流，参看 PauseResumeControlMediaType{@link #PauseResumeControlMediaType}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 该方法仅恢复远端流的接收，并不影响远端流的采集和发送；
     *        - 该方法不改变用户的订阅状态以及订阅流的属性。
     * @list 音视频传输
     * @order 14
     */
    
    virtual int resumeAllSubscribedStream(PauseResumeControlMediaType media_type) = 0;


    /**
     * 
     * @type api
     * @region 多房间
     * @brief 设置发流端音画同步。 <br>
     *        当同一用户同时使用两个通话设备分别采集发送音频和视频时，有可能会因两个设备所处的网络环境不一致而导致发布的流不同步，此时你可以在视频发送端调用该接口，SDK 会根据音频流的时间戳自动校准视频流，以保证接收端听到音频和看到视频在时间上的同步性。
     * @param audio_user_id 音频发送端的用户 ID，将该参数设为空则可解除当前音视频的同步关系。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 该方法在进房前后均可调用。
     *        - 进行音画同步的音频发布用户 ID 和视频发布用户 ID 须在同一个 RTC 房间内。
     *        - 调用该接口后音画同步状态发生改变时，你会收到 onAVSyncStateChange{@link #IRTCRoomEventHandler#onAVSyncStateChange} 回调。
     *        - 同一 RTC 房间内允许存在多个音视频同步关系，但需注意单个音频源不支持与多个视频源同时同步。
     *        - 如需更换同步音频源，再次调用该接口传入新的 `audio_user_id` 即可；如需更换同步视频源，需先解除当前的同步关系，后在新视频源端开启同步。
     * @list 音视频传输
     * @order 15
     */
    
    virtual int setMultiDeviceAVSync(const char* audio_user_id) = 0;

    /**
     * 
     * @hidden for internal use only.
     */
    
    virtual int updateCloudRendering(const char* cloudrender_json_string) = 0;

    /**
     * 
     * @hidden for internal use only.
     */
    
    virtual int setCustomUserRole(const char* role) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 开始跨房间转发媒体流。 <br>
     *        在调用 joinRoom{@link #IRTCRoom#joinRoom} 后调用本接口，实现向多个房间转发媒体流，适用于跨房间连麦等场景。
     * @param configuration 跨房间媒体流转发指定房间的信息。参看 ForwardStreamConfiguration{@link #ForwardStreamConfiguration}。
     * @return
     *        0: 方法调用成功 <br>
     *        <0: 方法调用失败
     * @note
     *        - 调用本方法后，将在本端触发 onForwardStreamStateChanged{@link #IRTCRoomEventHandler#onForwardStreamStateChanged} 回调。
     *        - 调用本方法后，你可以通过监听 onForwardStreamEvent{@link #IRTCRoomEventHandler#onForwardStreamEvent} 回调来获取各个目标房间在转发媒体流过程中的相关事件。
     *        - 开始转发后，目标房间中的用户将接收到本地用户进房 onUserJoined{@link #IRTCRoomEventHandler#onUserJoined} 和发流 onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}/onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen} 的回调。
     *        - 调用本方法后，可以调用 updateForwardStreamToRooms{@link #IRTCRoom#updateForwardStreamToRooms} 更新目标房间信息，例如，增加或减少目标房间等。
     *        - 调用本方法后，可以调用 stopForwardStreamToRooms{@link #IRTCRoom#stopForwardStreamToRooms} 停止向所有房间转发媒体流。
     *        - 调用本方法后，可以调用 pauseForwardStreamToAllRooms{@link #IRTCRoom#pauseForwardStreamToAllRooms} 暂停向所有房间转发媒体流。
     * @list 音视频传输
     * @order 8
     */
    
    virtual int startForwardStreamToRooms(const ForwardStreamConfiguration& configuration) = 0;

    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 更新跨房间媒体流转发信息。 <br>
     *        通过 startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms} 发起媒体流转发后，可调用本方法增加或者减少目标房间，或更新房间密钥。 <br>
     *        调用本方法增加或删减房间后，将在本端触发 onForwardStreamStateChanged{@link #IRTCRoomEventHandler#onForwardStreamStateChanged} 回调，包含发生了变动的目标房间中媒体流转发状态。
     * @param configuration 跨房间媒体流转发目标房间信息。参看 ForwardStreamConfiguration{@link #ForwardStreamConfiguration}。
     * @return
     *        - 0: 方法调用成功
     *        - <0: 方法调用失败
     * @note
     *        - 增加目标房间后，新增目标房间中的用户将接收到本地用户进房 onUserJoined{@link #IRTCRoomEventHandler#onUserJoined} 和发布 onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}/onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen} 的回调。
     *        - 删减目标房间后，原目标房间中的用户将接收到本地用户停止发布 onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream}/onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen} 和退房 onUserLeave{@link #IRTCRoomEventHandler#onUserLeave} 的回调。
     * @list 音视频传输
     * @order 10
     */
    
    virtual int updateForwardStreamToRooms(const ForwardStreamConfiguration& configuration) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 停止跨房间媒体流转发。 <br>
     *        通过 startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms} 发起媒体流转发后，可调用本方法停止向所有目标房间转发媒体流。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用本方法后，将在本端触发 onForwardStreamStateChanged{@link #IRTCRoomEventHandler#onForwardStreamStateChanged} 回调。
     *        - 调用本方法后，原目标房间中的用户将接收到本地用户停止发布 onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream}/onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen} 和退房 onUserLeave{@link #IRTCRoomEventHandler#onUserLeave} 的回调。
     *        - 如果需要停止向指定的房间转发媒体流，请调用 updateForwardStreamToRooms{@link #IRTCRoom#updateForwardStreamToRooms} 更新房间信息。
     *        - 如果需要暂停转发，请调用 pauseForwardStreamToAllRooms{@link #IRTCRoom#pauseForwardStreamToAllRooms}，并在之后随时调用 resumeForwardStreamToAllRooms{@link #IRTCRoom#resumeForwardStreamToAllRooms} 快速恢复转发。
     * @list 音视频传输
     * @order 9
     */
    
    virtual int stopForwardStreamToRooms() = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 暂停跨房间媒体流转发。 <br>
     *        通过 startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms} 发起媒体流转发后，可调用本方法暂停向所有目标房间转发媒体流。 <br>
     *        调用本方法暂停向所有目标房间转发后，你可以随时调用 resumeForwardStreamToAllRooms{@link #IRTCRoom#resumeForwardStreamToAllRooms} 快速恢复转发。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 调用本方法后，目标房间中的用户将接收到本地用户停止发布 onUserUnpublishStream{@link #IRTCRoomEventHandler#onUserUnpublishStream}/onUserUnpublishScreen{@link #IRTCRoomEventHandler#onUserUnpublishScreen} 和退房 onUserLeave{@link #IRTCRoomEventHandler#onUserLeave} 的回调。
     * @list 音视频传输
     * @order 11
     */
     
    virtual int pauseForwardStreamToAllRooms() = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 恢复跨房间媒体流转发。 <br>
     *        调用 pauseForwardStreamToAllRooms{@link #IRTCRoom#pauseForwardStreamToAllRooms} 暂停转发之后，调用本方法恢复向所有目标房间转发媒体流。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        目标房间中的用户将接收到本地用户进房 onUserJoined{@link #IRTCRoomEventHandler#onUserJoined} 和发布 onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}/onUserPublishScreen{@link #IRTCRoomEventHandler#onUserPublishScreen} 的回调。
     * @list 音视频传输
     * @order 12
     */
    
    virtual int resumeForwardStreamToAllRooms() = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 范围语音
     * @brief 获取范围语音接口实例。
     * @return 方法调用结果： <br>
     *        - IRangeAudio：成功，返回一个 IRangeAudio{@link #IRangeAudio} 实例。
     *        - nullptr：失败，当前 SDK 不支持范围语音功能。
     * @note 首次调用该方法须在创建房间后、加入房间前。范围语音相关 API 和调用时序详见[范围语音](https:
     * @list 高级功能
     * @order 13
     */
    
    virtual IRangeAudio* getRangeAudio() = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 空间音频
     * @brief 获取空间音频接口实例。
     * @return 方法调用结果： <br>
     *        - ISpatialAudio：成功，返回一个 ISpatialAudio{@link #ISpatialAudio} 实例。
     *        - nullptr：失败，当前 SDK 不支持空间音频功能。
     * @note
     *        - 首次调用该方法须在创建房间后、加入房间前。空间音频相关 API 和调用时序详见[空间音频](https:
     *        - 只有在使用支持真双声道播放的设备时，才能开启空间音频效果；
     *        - 机型性能不足可能会导致音频卡顿，使用低端机时，不建议开启空间音频效果；
     *        - SDK 最多支持 30 个用户同时开启空间音频功能。
     * @list 高级功能
     * @order 6
     */
    
    virtual ISpatialAudio* getSpatialAudio() = 0;
    /**
     * 
     * @type api
     * @region 多房间
     * @brief 调节某个房间内所有远端用户的音频播放音量。
     * @param volume 音频播放音量值和原始音量的比值，范围是 [0, 400]，单位为 %，自带溢出保护。为保证更好的通话质量，建议将 volume 值设为 [0,100]。 <br>
     *              - 0: 静音
     *              - 100: 原始音量，默认值
     *              - 400: 最大可为原始音量的 4 倍(自带溢出保护)
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 假设某远端用户 A 始终在被调节的目标用户范围内， <br>
     *        - 当该方法与 setRemoteAudioPlaybackVolume{@link #IRTCVideo#setRemoteAudioPlaybackVolume} 共同使用时，本地收听用户 A 的音量为后调用的方法设置的音量；
     *        - 当该方法与 setPlaybackVolume{@link #IRTCVideo#setPlaybackVolume} 方法共同使用时，本地收听用户 A 的音量将为两次设置的音量效果的叠加。
     * @list 音频管理
     * @order 25
     */
    
    virtual int setRemoteRoomAudioPlaybackVolume(int volume) = 0;
    /**
     * 
     * @hidden for internal use only on Windows and Android
     * @type api
     * @region 视频管理
     * @brief 接收端调用本接口获取全景视频接口引擎。
     * @return 方法调用结果： <br>
     *        - IPanoramicVideo：成功，返回一个 IPanoramicVideo{@link #IPanoramicVideo} 实例。
     *        - null：失败，当前 SDK 不支持全景视频功能。
     * @note
     *        - 接收端的调用时序为 createRTCVideo{@link #createRTCVideo} >  createRTCRoom{@link #IRTCVideo#createRTCRoom} > 本接口 > joinRoom{@link #IRTCRoom#joinRoom}。
     *        - 获取 IPanoramicVideo 实例后，再调用 updateQuaternionf{@link #IPanoramicVideo#updateQuaternionf} 更新头位姿四元数。
     *        - 你需要使用外部渲染器对接收到的全景视频进行渲染，参见 setRemoteVideoSink{@link #IRTCVideo#setRemoteVideoSink}。
     */
    
    virtual IPanoramicVideo* getPanoramicVideo() = 0;

    /**
     * 
     * @valid since 3.52.
     * @type api
     * @region 房间管理
     * @brief 设置本端发布流在音频选路中的优先级。
     * @param audio_selection_priority 本端发布流在音频选路中的优先级，默认正常参与音频选路。参见 AudioSelectionPriority{@link #AudioSelectionPriority}。
     * @note
     *       - 在控制台上为本 appId 开启音频选路后，调用本接口才会生效。进房前后调用均可生效。更多信息参见[音频选路](https:
     *       - 如果本端用户同时加入不同房间，使用本接口进行的设置相互独立。
     * @list 音频管理
     * @order 40
     */
    
    virtual int setAudioSelectionConfig(AudioSelectionPriority audio_selection_priority) = 0;

    /**
     * 
     * @valid since 3.52.
     * @type api
     * @region 房间管理
     * @brief 设置/更新房间附加信息，可用于标识房间状态或属性，或灵活实现各种业务逻辑。
     * @param key 房间附加信息键值，长度小于 10 字节。 <br>
     *        同一房间内最多可存在 5 个 key，超出则会从第一个 key 起进行替换。
     * @param value 房间附加信息内容，长度小于 128 字节。
     * @return
     *        - 0: 方法调用成功，返回本次调用的任务编号；
     *        - <0: 方法调用失败，具体原因详见 SetRoomExtraInfoResult{@link #SetRoomExtraInfoResult}。
     * @note
     *       - 在设置房间附加信息前，必须先调用 joinRoom{@link #IRTCRoom#joinRoom} 加入房间。
     *       - 调用该方法后，会收到一次 onSetRoomExtraInfoResult{@link #IRTCRoomEventHandler#onSetRoomExtraInfoResult} 回调，提示设置结果。
     *       - 调用该方法成功设置附加信息后，同一房间内的其他用户会收到关于该信息的回调 onRoomExtraInfoUpdate{@link #IRTCRoomEventHandler#onRoomExtraInfoUpdate}。
     *       - 新进房的用户会收到进房前房间内已有的全部附加信息通知。
     * @list 房间管理
     * @order 4
     */
    
    virtual int64_t setRoomExtraInfo(const char*key,const char*value) = 0;

    /**
     * 
     * @valid since 3.52
     * @type api
     * @region 字幕翻译服务
     * @brief 识别或翻译房间内所有用户的语音，形成字幕。 <br>
     *        调用该方法时，可以在 SubtitleMode{@link #SubtitleMode} 中选择语音识别或翻译模式。如果选择识别模式，语音识别文本会通过 onSubtitleMessageReceived{@link #IRTCRoomEventHandler#onSubtitleMessageReceived} 事件回调给你； <br>
     *        如果选择翻译模式，你会同时收到两个 onSubtitleMessageReceived{@link #IRTCRoomEventHandler#onSubtitleMessageReceived} 回调，分别包含字幕原文及字幕译文。 <br>
     *        调用该方法后，用户会收到 onSubtitleStateChanged{@link #IRTCRoomEventHandler#onSubtitleStateChanged} 回调，通知字幕是否开启。
     * @param subtitle_config 字幕配置信息。参看 SubtitleConfig{@link #SubtitleConfig}。
     * @return
     *        -  0: 调用成功。
     *        - !0: 调用失败。
     * @note
     *        - 使用字幕功能前，你需要在 [RTC 控制台](https:
     *        - 如果你需要使用流式语音识别模式，你应在 [语音技术控制台](https:
     *        - 如果你需要使用实时语音翻译模式，你应开通机器翻译服务，参考 [开通服务](https:
     *        - 此方法需要在进房后调用。
     *        - 如需指定源语言，你需要在调用 `joinRoom` 接口进房时，通过 extraInfo 参数传入格式为`"语种英文名": "语种代号"` JSON 字符串，例如设置源语言为英文时，传入 `"source_language": "en"`。如未指定源语言，SDK 会将系统语种设定为源语言。如果你的系统语种不是中文、英文和日文，此时 SDK 会自动将中文设为源语言。
     *          - 识别模式下，你可以传入 [RTC 控制台](https:
     *          - 翻译模式下，你需要传入机器翻译规定的语种英文名和语种代号。翻译模式下支持的语言及对应的代号参看[翻译模式语言支持](https:
     * @list 字幕翻译服务
     * @order 0
     */
    
    virtual int startSubtitle(const SubtitleConfig& subtitle_config) = 0;
    /**
     * 
     * @valid since 3.52
     * @type api
     * @region 字幕翻译服务
     * @brief 关闭字幕。 <br>
     *        调用该方法后，用户会收到 onSubtitleStateChanged{@link #IRTCRoomEventHandler#onSubtitleStateChanged} 回调，通知字幕是否关闭。
     * @return
     *        -  0: 调用成功。
     *        - !0: 调用失败。
     * @list 字幕翻译服务
     * @order 1
     */
    
    virtual int stopSubtitle() = 0;
    /**
     * 
     * @valid since 3.53
     * @type api
     * @brief 获取房间 ID。
     * @return 房间 ID。
     */
    
    virtual const char* getRoomId() = 0;
};

} 