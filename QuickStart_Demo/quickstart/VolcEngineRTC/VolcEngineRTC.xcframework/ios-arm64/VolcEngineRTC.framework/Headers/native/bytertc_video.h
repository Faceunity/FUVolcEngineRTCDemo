/*
 * Copyright (c) 2020 The VolcEngineRTC project authors. All Rights Reserved.
 * @brief VolcEngineRTC Interface Lite
 */

#pragma once

#include "bytertc_room.h"
#include "rtc/bytertc_video_device_manager.h"
#include "rtc/bytertc_audio_frame.h"
#include "rtc/byte_rtc_asr_engine_event_handler.h"
#include "rtc/bytertc_audio_mixing_manager.h"
#include "rtc/bytertc_audio_effect_player.h"
#include "rtc/bytertc_media_player.h"
#include "rtc/bytertc_video_processor_interface.h"
#include "rtc/bytertc_camera_control_interface.h"
#include "bytertc_video_event_handler.h"
#include "rtc/bytertc_sing_scoring_interface.h"
#include "rtc/bytertc_ktv_manager_interface.h"

namespace bytertc {

/**
 * 
 * @type api
 * @brief 引擎 API
 */

class IRTCVideo {
public:
    /**
     * 
     * @valid since 3.58.1
     * @type api
     * @region 音量管理
     * @brief 设置是否将录音信号静音（不改变本端硬件）。
     * @param index 流索引，指定调节主流/屏幕流音量，参看 StreamIndex{@link #StreamIndex}。
     * @param mute 是否静音音频采集。 <br>
     *        - True：静音（关闭麦克风）
     *        - False：（默认）开启麦克风
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
              + 该方法支持选择静音或取消静音麦克风采集，而不影响 SDK 音频流发布状态。
              + 静音后通过 setCaptureVolume{@link #IRTCVideo#setCaptureVolume} 调整音量不会取消静音状态，音量状态会保存至取消静音。
              + 调用 startAudioCapture{@link #IRTCVideo#startAudioCapture} 开启音频采集前后，都可以使用此接口设置采集音量。
     */
    
    virtual int muteAudioCapture(StreamIndex index, bool mute) = 0;

    /**
     * 
     * @type api
     * @region 音量管理
     * @brief 调节音频采集音量
     * @param index 流索引，指定调节主流还是调节屏幕流的音量，参看 StreamIndex{@link #StreamIndex}
     * @param volume 采集的音量值和原始音量的比值，范围是 [0, 400]，单位为 %，自带溢出保护。 <br>
     *                    只改变音频数据的音量信息，不涉及本端硬件的音量调节。 <br>
     *        为保证更好的通话质量，建议将 volume 值设为 [0,100]。 <br>
     *       - 0：静音
     *       - 100：原始音量
     *       - 400: 最大可为原始音量的 4 倍(自带溢出保护)
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 在开启音频采集前后，你都可以使用此接口设定采集音量。
     * @list 音频管理
     * @order 22
     */
    
    virtual int setCaptureVolume(StreamIndex index, int volume) = 0;
    /**
     * 
     * @type api
     * @region 音量管理
     * @brief 调节本地播放的所有远端用户混音后的音量。 <br>
     *        播放音频前或播放音频时，你都可以使用此接口设定播放音量。
     * @param volume 音频播放音量值和原始音量的比值，范围是 [0, 400]，单位为 %，自带溢出保护。 <br>
     *                    只改变音频数据的音量信息，不涉及本端硬件的音量调节。 <br>
     *        为保证更好的通话质量，建议将 volume 值设为 [0,100]。 <br>
     *       - 0: 静音
     *       - 100: 原始音量
     *       - 400: 最大可为原始音量的 4 倍(自带溢出保护)
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 假设某远端用户 A 始终在被调节的目标用户范围内，当该方法与 setRemoteAudioPlaybackVolume{@link #IRTCVideo#setRemoteAudioPlaybackVolume} 或 setRemoteRoomAudioPlaybackVolume{@link #IRTCRoom#setRemoteRoomAudioPlaybackVolume} 共同使用时，本地收听用户 A 的音量将为两次设置的音量效果的叠加。
     * @list 音频管理
     * @order 23
     */
    
    virtual int setPlaybackVolume(const int volume) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 音频管理
     * @brief 开启/关闭耳返功能
     * @param mode 是否开启耳返功能，参看 EarMonitorMode{@link #EarMonitorMode}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 耳返功能仅支持设备通过 3.5mm 接口、USB 接口、或蓝牙方式直连耳机时可以使用。对于通过 HDMI 或 USB-C 接口连接显示器，再连接，或通过连接 OTG 外接声卡再连接的耳机，不支持耳返功能。
     * @list 高级功能
     * @order 4
     */
    
    virtual int setEarMonitorMode(EarMonitorMode mode) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 音频管理
     * @brief 设置耳返的音量
     * @param volume 耳返的音量相对原始音量的比值，取值范围：[0,100]，单位：%
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 设置耳返音量前，你必须先调用 setEarMonitorMode{@link #IRTCVideo#setEarMonitorMode} 打开耳返功能。
     * @list 高级功能
     * @order 5
     */
    
    virtual int setEarMonitorVolume(const int volume) = 0;

    /**
     * 
     * @hidden(macOS,Windows,Linux,Android)
     * @type api
     * @region 蓝牙模式化设置
     * @brief 设置蓝牙模式
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 仅在媒体场景下生效。
     */
    
    virtual int setBluetoothMode(BluetoothMode mode) = 0;

    /**
     * 
     * @type api
     * @region 音频管理
     * @brief 开启内部音频采集。默认为关闭状态。 <br>
     *        内部采集是指：使用 RTC SDK 内置的音频采集机制进行音频采集。 <br>
     *        调用该方法开启后，本地用户会收到 onAudioDeviceStateChanged{@link #IRTCVideoEventHandler#onAudioDeviceStateChanged} 的回调。 <br>
     *        非隐身用户进房后调用该方法，房间中的其他用户会收到 onUserStartAudioCapture{@link #IRTCVideoEventHandler#onUserStartAudioCapture} 的回调。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 若未取得当前设备的麦克风权限，调用该方法后会触发 onAudioDeviceStateChanged{@link #IRTCVideoEventHandler#onAudioDeviceStateChanged} 回调，对应的错误码为 `MediaDeviceError.kMediaDeviceErrorDeviceNoPermission = 1`。
     *       - 调用 stopAudioCapture{@link #IRTCVideo#stopAudioCapture} 可以关闭音频采集设备，否则，SDK 只会在销毁引擎的时候自动关闭设备。
     *       - 由于不同硬件设备初始化响应时间不同，频繁调用 stopAudioCapture{@link #IRTCVideo#stopAudioCapture} 和本接口闭麦/开麦可能出现短暂无声问题，建议使用 publishStream{@link #IRTCRoom#publishStream}/unpublishStream{@link #IRTCRoom#unpublishStream} 实现临时闭麦和重新开麦。
     *       - 创建引擎后，无论是否发布音频数据，你都可以调用该方法开启音频采集，并且调用后方可发布音频。
     *       - 如果需要从自定义音频采集切换为内部音频采集，你必须先停止发布流，调用 setAudioSourceType{@link #IRTCVideo#setAudioSourceType} 关闭自定义采集，再调用此方法手动开启内部采集。
     * @list 音频管理
     * @order 27
     */
    
    virtual int startAudioCapture() = 0;

    /**
     * 
     * @type api
     * @region 音频管理
     * @brief 立即关闭内部音频采集。默认为关闭状态。 <br>
     *        内部采集是指：使用 RTC SDK 内置的音频采集机制进行音频采集。 <br>
     *        调用该方法，本地用户会收到 onAudioDeviceStateChanged{@link #IRTCVideoEventHandler#onAudioDeviceStateChanged} 的回调。 <br>
     *        非隐身用户进房后调用该方法后，房间中的其他用户会收到 onUserStopAudioCapture{@link #IRTCVideoEventHandler#onUserStopAudioCapture} 的回调。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 调用 startAudioCapture{@link #startAudioCapture} 可以开启音频采集设备。
     *       - 如果不调用本方法停止内部视频采集，则只有当销毁引擎实例时，内部音频采集才会停止。
     * @list 音频管理
     * @order 28
     */
    
    virtual int stopAudioCapture() = 0;
    /**
     * 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region 音频管理
     * @brief 设置音频场景类型。 <br>
     *        你可以根据你的应用所在场景，选择合适的音频场景类型。 <br>
     *        选择音频场景后，SDK 会自动根据客户端音频采集播放设备和状态，适用通话音量/媒体音量，并同步变更对应音频相关的算法配置和采集配置。
     * @param scenario 音频场景类型，参看 AudioScenarioType{@link #AudioScenarioType}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 建议在加入房间和调用音频相关接口之前，调用此接口设置音频场景类型。如果在此之后调用此接口，可能会引入音频卡顿。
     *        - 通话音量更适合通话、会议等对信息准确度更高的场景。通话音量会激活系统硬件信号处理，使通话声音更清晰。同时，音量无法降低到 0。
     *        - 媒体音量更适合娱乐场景，因其声音的表现力会更强。媒体音量下，最低音量可以为 0。
     */
    
    virtual int setAudioScenario(AudioScenarioType scenario) = 0;
    /**
     * 
     * @hidden internal use only
     * @valid since 3.55
     * @type api
     * @brief 设置音频场景类型。 <br>
     *        选择音频场景后，SDK 会自动根据场景切换对应的音量模式（通话音量/媒体音量）和改场景下的最佳音频配置。和 `setAudioScenario` 不同的是，`audioScenario` 只修改了音量模式，不会修改音频相关的算法配置。 <br>
     *        该接口不应与 `setAudioScenario` 混用。如混用，可能会出现未知错误，如音频算法失效等。
     * @param scenario 音频场景类型，参看 AudioSceneType{@link #AudioSceneType}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 此接口在进房前后调用都有效。
     *        - 通话音量更适合通话、会议等对信息准确度更高的场景。通话音量会激活系统硬件信号处理，使通话声音更清晰。同时，音量无法降低到 0。
     *        - 媒体音量更适合娱乐场景，因其声音的表现力会更强。媒体音量下，最低音量可以为 0。
     */
    
    virtual int setAudioScene(AudioSceneType scene) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 美声特效管理
     * @brief 设置变声特效类型
     * @param voice_changer 变声特效类型，参看 VoiceChangerType{@link #VoiceChangerType}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - !0：失败。
     * @note
     *        - 如需使用该功能，需集成 SAMI 动态库，详情参看[按需集成插件](1108726)文档。
     *        - 在进房前后都可设置。
     *        - 对 RTC SDK 内部采集的音频和自定义采集的音频都生效。
     *        - 只对单声道音频生效。
     *        - 与 setVoiceReverbType{@link #setVoiceReverbType} 互斥，后设置的特效会覆盖先设置的特效。
     *        - 在 Windows 端使用此功能前，必须额外接入 [音频技术](https:
     * @list 音频处理
     * @order 0
     */
    
    virtual int setVoiceChangerType(VoiceChangerType voice_changer) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 美声特效管理
     * @brief 设置混响特效类型
     * @param voice_reverb 混响特效类型，参看 VoiceReverbType{@link #VoiceReverbType}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - !0：失败。
     * @note
     *        - 在进房前后都可设置。
     *        - 对 RTC SDK 内部采集的音频和自定义采集的音频都生效。
     *        - 只对单声道音频生效。
     *        - 只在包含美声特效能力的 SDK 中有效。
     *        - 与 setVoiceChangerType{@link #setVoiceChangerType} 互斥，后设置的特效会覆盖先设置的特效。
     * @list 音频处理
     * @order 1
     */
    
    virtual int setVoiceReverbType(VoiceReverbType voice_reverb) = 0;
    /**
     * 
     * @hidden(Linux)
     * @type api
     * @brief 设置本地采集语音的均衡效果。包含内部采集和外部采集，但不包含混音音频文件。
     * @param config 语音均衡效果，参看 VoiceEqualizationConfig{@link #VoiceEqualizationConfig}
     * @return
     *        - 0： 成功。
     *        - < 0： 失败。
     * @note 根据奈奎斯特采样率，音频采样率必须大于等于设置的中心频率的两倍，否则，设置不生效。
     * @list 音频处理
     * @order 3
     */
    
    virtual int setLocalVoiceEqualization(VoiceEqualizationConfig config) = 0;
    /**
     * 
     * @type api
     * @hidden(Linux)
     * @brief 设置本地采集音频的混响效果。包含内部采集和外部采集，但不包含混音音频文件。
     * @param param 混响效果，参看 VoiceReverbConfig{@link #VoiceReverbConfig}
     * @return
     *        - 0： 成功。
     *        - < 0： 失败。
     * @note 调用 enableLocalVoiceReverb{@link #IRTCVideo#enableLocalVoiceReverb} 开启混响效果。
     * @list 音频处理
     * @order 4
     */
    
    virtual int setLocalVoiceReverbParam(VoiceReverbConfig param) = 0;
    /**
     * 
     * @type api
     * @hidden(Linux)
     * @brief 开启本地音效混响效果
     * @param enable 是否开启
     * @return
     *        - 0： 成功。
     *        - < 0： 失败。
     * @note 调用 setLocalVoiceReverbParam{@link #IRTCVideo#setLocalVoiceReverbParam} 设置混响效果。
     * @list 音频处理
     * @order 5
     */
    
    virtual int enableLocalVoiceReverb(bool enable) = 0;

    /**
     * 
     * @type api
     * @region 音频管理
     * @brief 设置音质档位。 <br>
     *        当所选的 RoomProfileType{@link #RoomProfileType} 中的音频参数无法满足你的场景需求时，调用本接口切换的音质档位。
     * @param audio_profile 音质档位，参看 AudioProfileType{@link #AudioProfileType}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 该方法在进房前后均可调用；
     *        - 支持通话过程中动态切换音质档位。
     * @list 音频管理
     * @order 31
     */
    
    virtual int setAudioProfile(AudioProfileType audio_profile) = 0;

    /**
     * 
     * @type api
     * @region 音频管理
     * @brief 支持根据业务场景，设置通话中的音频降噪模式。
     * @param ans_mode 降噪模式。具体参见 AnsMode{@link #AnsMode}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 该接口进房前后均可调用，可重复调用，仅最后一次调用生效。
     *        - 降噪算法包含传统降噪和 AI 降噪。传统降噪主要是抑制平稳噪声，比如空调声、风扇声等。而 AI 降噪主要是抑制非平稳噪声，比如键盘敲击声、桌椅碰撞声等。
     *        - 在 V3.57.1 之前的版本，只有以下 RoomProfileType{@link #RoomProfileType} 场景时，调用本接口可以开启 AI 降噪。其余场景的 AI 降噪不会生效。
     *                 -  游戏语音模式：kRoomProfileTypeGame
     *                 -  高音质游戏模式：kRoomProfileTypeGameHD
     *                 -  云游戏模式：kRoomProfileTypeCloudGame
     *                 -  1 vs 1 音视频通话：kRoomProfileTypeChat
     *                 -  多端同步播放音视频：kRoomProfileTypeLwTogether
     *                 -  云端会议中的个人设备：kRoomProfileTypeMeeting
     *                 -  课堂互动模式：kRoomProfileTypeClassroom
     *                 -  云端会议中的会议室终端：kRoomProfileTypeMeetingRoom
     * @list 高级功能
     * @order 36
     */
    
    virtual int setAnsMode(AnsMode ans_mode) = 0;

    /**
     * 
     * @type api
     * @valid since 3.51
     * @region 音频设备管理
     * @brief 打开/关闭 AGC(Automatic Gain Control) 自动增益控制功能。 <br>
     *        开启该功能后，SDK 会自动调节麦克风的采集音量，确保音量稳定。
     * @param enable 是否打开 AGC 功能: <br>
     *        - true: 打开 AGC 功能。
     *        - false: 关闭 AGC 功能。
     * @return
     *        -  0: 调用成功。
     *        - -1: 调用失败。
     * @note
     *         该方法在进房前后均可调用。如果你需要在进房前使用 AGC 功能，请联系技术支持获得私有参数，传入对应 RoomProfileType{@link #RoomProfileType} 。 <br>
     *         要想在进房后开启 AGC 功能，你需要把 RoomProfileType{@link #RoomProfileType} 设置为 `kRoomProfileTypeMeeting` 、`kRoomProfileTypeMeetingRoom` 或 `kRoomProfileTypeClassroom` 。 <br>
     *         AGC 功能生效后，不建议再调用 setAudioCaptureDeviceVolume{@link #IAudioDeviceManager#setAudioCaptureDeviceVolume} 来调节设备麦克风的采集音量。
     * @list 音频管理
     * @order 37
     */
    
    virtual int enableAGC(bool enable) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 自定义音频采集渲染
     * @brief 切换音频采集方式
     * @param type 音频数据源，详见 AudioSourceType{@link #AudioSourceType}。 <br>
     *             默认使用内部音频采集。音频采集和渲染方式无需对应。
     * @return 方法调用结果： <br>
     *        - >0: 切换成功。
     *        - -1：切换失败。
     * @note
     *      - 进房前后调用此方法均有效。
     *      - 如果你调用此方法由内部采集切换至自定义采集，SDK 会自动关闭内部采集。然后，调用 pushExternalAudioFrame{@link #IRTCVideo#pushExternalAudioFrame} 推送自定义采集的音频数据到 RTC SDK 用于传输。
     *      - 如果你调用此方法由自定义采集切换至内部采集，你必须再调用 startAudioCapture{@link #IRTCVideo#startAudioCapture} 手动开启内部采集。
     * @list 自定义流处理
     * @order 6
     */
    
    virtual int setAudioSourceType (AudioSourceType type) = 0;
    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 自定义音频采集渲染
     * @brief 切换音频渲染方式
     * @param type 音频输出类型，详见 AudioRenderType{@link #AudioRenderType} <br>
     *             默认使用内部音频渲染。音频采集和渲染方式无需对应。
     * @return 方法调用结果： <br>
     *        - >0: 切换成功。
     *        - -1：切换失败。
     * @note
     *      - 进房前后调用此方法均有效。
     *      - 如果你调用此方法切换至自定义渲染，调用 pullExternalAudioFrame{@link #IRTCVideo#pullExternalAudioFrame} 获取音频数据。
     * @list 自定义流处理
     * @order 7
     */
    
    virtual int setAudioRenderType (AudioRenderType type) = 0;
    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 自定义音频采集渲染
     * @brief 推送自定义采集的音频数据到 RTC SDK。
     * @param audio_frame 10 ms 对应的音频数据。详见 IAudioFrame{@link #IAudioFrame}。 <br>
     *        - 音频采样格式必须为 S16。音频缓冲区内的数据格式必须为 PCM，其容量大小应该为 audioFrame.samples × audioFrame.channel × 2。
     *        - 必须指定具体的采样率和声道数，不支持设置为自动。
     * @return 方法调用结果 <br>
     *        - 0：方法调用成功
     *        - < 0：方法调用失败
     * @note
     *       - 推送自定义采集的音频数据前，必须先调用 setAudioSourceType{@link #IRTCVideo#setAudioSourceType} 开启自定义采集。
     *       - 你必须每 10 ms 推送一次数据。
     * @list 自定义流处理
     * @order 8
     */
    
    virtual int pushExternalAudioFrame(IAudioFrame* audio_frame) = 0;
    /**
     * 
     * @hidden(Linux)
     * @region 自定义音频采集渲染
     * @brief 拉取下行音频数据用于自定义音频渲染。 <br>
     *        调用该方法后，SDK 会主动拉取待播放的音频数据，包括远端已解码和混音后的音频数据，用于外部播放。
     * @param audio_frame 获取的 10 ms 内的音频数据。详见 IAudioFrame{@link #IAudioFrame}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *       - 获取音频数据用于自定义渲染前，必须先调用 setAudioRenderType{@link #IRTCVideo#setAudioRenderType} 开启自定义渲染。
     *       - 由于 RTC SDK 的帧长为 10 毫秒，你应当每隔 10 毫秒拉取一次音频数据。确保音频采样点数（sample）x 拉取频率等于 audio_frame 的采样率 （sampleRate）。如设置采样率为 48000 时，每 10 毫秒调用本接口拉取数据，每次应拉取 480 个采样点。
     *       - 该函数运行在用户调用线程内，是一个同步函数。
     * @list 自定义流处理
     * @order 9
     */
    
    virtual int pullExternalAudioFrame(IAudioFrame* audio_frame) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 立即开启内部视频采集。默认为关闭状态。 <br>
     *        内部视频采集指：使用 RTC SDK 内置视频采集模块，进行采集。 <br>
     *        调用该方法后，本地用户会收到 onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} 的回调。 <br>
     *        本地用户在非隐身状态下调用该方法后，房间中的其他用户会收到 onUserStartVideoCapture{@link #IRTCVideoEventHandler#onUserStartVideoCapture} 的回调。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 调用 stopVideoCapture{@link #IRTCVideo#stopVideoCapture} 可以停止内部视频采集。否则，只有当销毁引擎实例时，内部视频采集才会停止。
     *       - 创建引擎后，无论是否发布视频数据，你都可以调用该方法开启内部视频采集。只有当（内部或外部）视频采集开始以后视频流才会发布。
     *       - 如果需要从自定义视频采集切换为内部视频采集，你必须先关闭自定义采集，再调用此方法手动开启内部采集。
     * @list 视频管理
     * @order 0
     */
    
    virtual int startVideoCapture() = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 立即关闭内部视频采集。默认为关闭状态。 <br>
     *        内部视频采集指：使用 RTC SDK 内置视频采集模块，进行采集。 <br>
     *        调用该方法后，本地用户会收到 onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} 的回调。 <br>
     *        非隐身用户进房后调用该方法，房间中的其他用户会收到 onUserStopVideoCapture{@link #IRTCVideoEventHandler#onUserStopVideoCapture} 的回调。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 调用 startVideoCapture{@link #IRTCVideo#startVideoCapture} 可以开启内部视频采集。
     *       - 如果不调用本方法停止内部视频采集，则只有当销毁引擎实例时，内部视频采集才会停止。
     * @list 视频管理
     * @order 1
     */
    
    virtual int stopVideoCapture() = 0;

   /**
    * 
    * @type api
    * @region 视频管理
    * @brief 设置 RTC SDK 内部采集时的视频采集参数。 <br>
    *        如果你的项目使用了 SDK 内部采集模块，可以通过本接口指定视频采集参数，包括模式、分辨率、帧率。
    * @param video_capture_config 视频采集参数。参看: VideoCaptureConfig{@link #VideoCaptureConfig}。
    * @return
    *        - 0: 成功；
    *        - < 0: 失败；
    * @note
    * - 本接口在引擎创建后可调用，调用后立即生效。建议在调用 startVideoCapture{@link #IRTCVideo#startVideoCapture} 前调用本接口。
    * - 建议同一设备上的不同引擎使用相同的视频采集参数。
    * - 如果调用本接口前使用内部模块开始视频采集，采集参数默认为 Auto 模式。
    * @list 视频管理
    * @order 2
    */
   
    virtual int setVideoCaptureConfig(const VideoCaptureConfig& video_capture_config) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 设置本端采集的视频帧的旋转角度。 <br>
     *        当摄像头倒置或者倾斜安装时，可调用本接口进行调整。
     * @param rotation 相机朝向角度，默认为 `VIDEO_ROTATION_0(0)`，无旋转角度。详见 VideoRotation{@link #VideoRotation}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用本接口也将对自定义采集视频画面生效，在原有的旋转角度基础上叠加本次设置。
     *        - 对于 Windows SDK，视频贴纸特效或通过 enableVirtualBackground{@link #IVideoEffect#enableVirtualBackground} 增加的虚拟背景，也会跟随本接口的设置进行旋转。
     *        - 本地渲染视频和发送到远端的视频都会相应旋转，但不会应用到单流转推中。如果希望在单流转推的视频中应用旋转，调用 setVideoOrientation{@link #IRTCVideo#setVideoOrientation}。
     * @list 视频处理
     * @order 21
     */
    
    virtual int setVideoCaptureRotation(VideoRotation rotation) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 该方法设置视频流发布端是否开启发布多路编码参数不同的视频流的模式。
     * @param enabled 是否开启推送多路视频流模式： <br>
     *        - true：开启
     *        - false：关闭（默认）
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 你应在进房前或进房后但未发布流时，调用此方法。
     *        - 开启推送多路流后，不能动态关闭，也不能更新多路流的路数和编码参数。
     *        - 开启推送多路视频流模式后，你可以在发布流前调用 [setVideoEncoderConfig](#IRTCVideo-setvideoencoderconfig-1) 为多路视频流分别设置编码参数。
     *        - 该功能关闭时，或该功能开启但未设置多路流参数时，默认只发一路视频流，该流的编码参数为：分辨率 640px × 360px，帧率 15fps。
     * @list 网络管理
     * @order 0
     */
    
    virtual int enableSimulcastMode(bool enabled) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief <span id="IRTCVideo-setvideoencoderconfig-1"></span> 视频发布端设置期望发布的最大分辨率视频流参数，包括分辨率、帧率、码率、网络不佳时的回退策略等。 <br>
     *        该接口支持设置一路视频流参数，设置多路参数请使用重载 API：[setVideoEncoderConfig](#IRTCVideo-setvideoencoderconfig-2)。
     * @param max_solution 期望发布的最大分辨率视频流参数。参看 VideoEncoderConfig{@link #VideoEncoderConfig}。
     * @return 方法调用结果： <br>
     *        - 0：成功
     *        - !0：失败
     * @note
     *        - 你可以同时使用 enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode} 方法来发布多路分辨率不同的流。具体而言，若期望发布多路不同分辨率的流，你需要在发布流之前调用本方法以及 enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode} 方法开启多路流模式，SDK 会根据订阅端的设置智能调整发布的流数（最多发布 4 条）以及各路流的参数。其中，调用本方法设置的分辨率为各路流中的最大分辨率。具体规则参看[推送多路流](https:
     *        - 调用该方法前，SDK 默认仅发布一条分辨率为 640px × 360px，帧率为 15fps 的视频流。
     *        - 使用自定义采集时，必须调用该方法设置编码参数，以保证远端收到画面的完整性。
     *        - 该方法适用于摄像头采集的视频流，设置屏幕共享视频流参数参看 setScreenVideoEncoderConfig{@link #IRTCVideo#setScreenVideoEncoderConfig}（Linux 不适用）。
     * @list 视频管理
     * @order 11
     */
    
    virtual int setVideoEncoderConfig(const VideoEncoderConfig& max_solution) = 0;

    /**
     * 
     * @hidden for internal use only on Windows and Android
     * @type api
     * @region 视频管理
     * @brief <span id="RTCVideo-setvideoencoderconfig-3"></span> 发布端设置全景视频，包括分辨率、高清视野和低清背景分辨率、Tile 大小，以及其他常规编码参数。
     * @param encoder_config 期望发布的最大分辨率视频流参数。参看 VideoEncoderConfig{@link #VideoEncoderConfig}。 <br>
     *                      支持 8K 和 4K 两种分辨率的全景视频。
     * @param parameters 全景视频的编码参数，JSON 字符串格式 <br>
     *                  - 8K: HD: 7680x3840, LD: 2560x1280, Tile: 640x640
     *                  - 4K: HD: 3840x1920, LD: 1280x640, Tile: 320x320
     * { <br>
     *  "rtc.fov_config":{ <br>
     *      "mode":0,   
     *      "hd_width":3840,    
     *      "hd_height":1920,   
     *      "ld_width":1280,    
     *      "ld_height":640,    
     *      "tile_width":320,   
     *      "tile_height":320,  
     *      "framerate":30, 
     *      "max_kbps":40000} 
     *}
     * @return 方法调用结果： <br>
     *        - 0：成功
     *        - !0：失败
     * @note
     *        - 发布全景视频前，绑定自定义采集器，必须调用该方法设置编码参数。支持的视频格式包括 YUV 或者 Texture 纹理。
     *        - 通过 onFrame{@link #IVideoSink#onFrame} ,接收端获取到视频帧和解码需要的信息，传给自定义渲染器进行渲染。
     */
    
    virtual int setVideoEncoderConfig(const VideoEncoderConfig& encoder_config, const char* parameters) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief <span id="IRTCVideo-setvideoencoderconfig-2"></span> 视频发布端设置推送多路流时各路流的参数，包括分辨率、帧率、码率、网络不佳时的回退策略等。
     * @param channel_solutions 要推送的多路视频流的参数需注意，所设置的分辨率是各路流的最大分辨率。参看 VideoEncoderConfig{@link #VideoEncoderConfig}。
     * @param solution_num 视频参数数组长度，最多支持设置 3 路参数，超过 3 路时默认取前 3 路的值。当设置了多路参数时，分辨率和帧率必须是从大到小排列。需注意，所设置的分辨率是各路流的最大分辨率。
     * @return 方法调用结果： <br>
     *        - 0：成功
     *        - !0：失败
     * @note
     *        - 该方法设置的多路参数是否均生效，取决于是否同时调用了 enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode} 开启发布多路参数不同的视频流模式。若未开启推送多路流模式，但调用本方法设置了多个分辨率，SDK 则默认发布设置的第一条流，多个分辨率的设置会在开启推送多路流模式之后生效。
     *        - 若期望推送多路不同分辨率的流，你需要在发布流之前调用本方法以及 enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode} 方法。
     *        - 开启推送多路流后，不能动态关闭，也不能更新多路流的路数和编码参数。
     *        - 调用该方法设置多路视频流参数前，SDK 默认仅发布一条分辨率为 640px × 360px，帧率为 15fps 的视频流。
     *        - 调用该方法设置分辨率不同的多条流后，SDK 会根据订阅端设置的期望订阅参数自动匹配发送的流，具体规则参看[推送多路流](https:
     *        - 该方法适用于摄像头采集的视频流，设置屏幕共享视频流参数参看 setScreenVideoEncoderConfig{@link #IRTCVideo#setScreenVideoEncoderConfig}（Linux 不适用）。
     */
    
    virtual int setVideoEncoderConfig(const VideoEncoderConfig* channel_solutions, int solution_num) = 0;

    /**
     * 
     * @type api
     * @region 屏幕共享
     * @brief 为发布的屏幕共享视频流设置期望的编码参数，包括分辨率、帧率、码率、网络不佳时的回退策略等。
     * @param screen_solution 屏幕共享视频流参数。参看 ScreenVideoEncoderConfig{@link #ScreenVideoEncoderConfig}。
     * @return
     *         0：成功。 <br>
     *         !0：失败。
     * @note
     *        - 建议在采集视频前设置编码参数。若采集前未设置编码参数，则使用默认编码参数: 分辨率 1920px × 1080px，帧率 15fps。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 17
     */
    
    virtual int setScreenVideoEncoderConfig(const ScreenVideoEncoderConfig& screen_solution) = 0;

    /**
     * 
     * @deprecated since 3.36 and will be deleted in 3.51, use [setVideoEncoderConfig](#IRTCVideo-setvideoencoderconfig-2) instead.
     * @type api
     * @region 视频管理
     * @brief 设置推送多路流时的各路视频参数，包括分辨率、帧率、码率、缩放模式、网络不佳时的回退策略等。
     * @param index 视频流属性，参看 StreamIndex{@link #StreamIndex}。
     * @param solutions 要推送的多路视频流参数，参看 VideoSolution{@link #VideoSolution}。 <br>
     *                       最大分辨率为 4096px × 4096px，超过或设置的分辨率无法编码时，会导致编码推流失败。
     * @param solution_num 视频参数数组长度。 <br>
     *                          最多支持 4 路参数。当设置了多路参数时，分辨率必须是从大到小排列。
     * @return
     *        - 0：成功
     *        - !0：失败
     * @note
     *       - 该接口已废弃，请使用同名新接口代替；若仍需使用该旧接口，请注意无需先调用 `enableSimulcastMode` 开启推送多路流模式。
     *       - 当使用内部采集时，视频采集的分辨率、帧率会根据最大的编码分辨率、帧率进行适配
     *       - 默认的视频编码参数为：分辨率 640px × 360px，帧率 15fps。
     *       - 变更编码分辨率后马上生效，可能会引发相机重启。
     *       - 屏幕流只取视频参数数组的第一组数据。
     */
    
    BYTERTC_DEPRECATED virtual int setVideoEncoderConfig(StreamIndex index, const VideoSolution* solutions, int solution_num) = 0;

    /**
     * 
     * @hidden(Linux, macOS)
     * @valid since 3.58
     * @type api
     * @region 视频管理
     * @brief 开启自定义采集视频帧的 Alpha 通道编码功能。 <br>
     *        适用于需要分离推流端视频主体与背景，且在拉流端可自定义渲染背景的场景。
     * @param streamIndex 需开启该功能的视频流类型，当前仅支持对 StreamIndex.kStreamIndexMain 即主流开启。
     * @param alphaLayout 分离后的 Alpha 通道相对于 RGB 通道信息的排列位置。当前仅支持 AlphaLayout.kAlphaLayoutTop，即置于 RGB 通道信息上方。
     * @return 方法调用结果： <br>
     *         - 0：成功；
     *         - !0：失败。
     * @note
     *        - 该接口仅作用于自定义采集的、并且使用 RGBA 色彩模型的视频帧，包括：
     *            - Windows：VideoPixelFormat.kVideoPixelFormatRGBA；
     *            - Android：VideoPixelFormat.TEXTURE_2D、VideoPixelFormat.TEXTURE_OES、VideoPixelFormat.RGBA；
     *            - iOS：ByteRTCVideoPixelFormat.ByteRTCVideoPixelFormatCVPixelBuffer。
     *        - 该接口须在发布视频流之前调用。
     *        - 调用本接口开启 Alpha 通道编码后，你需调用 pushExternalVideoFrame{@link #IRTCVideo#pushExternalVideoFrame} 把自定义采集的视频帧推送至 RTC SDK。若推送了不支持的视频帧格式，则调用 pushExternalVideoFrame{@link #IRTCVideo#pushExternalVideoFrame} 时会返回错误码 ReturnStatus.kReturnStatusParameterErr。
     */
    
    virtual int enableAlphaChannelVideoEncode(StreamIndex streamIndex, AlphaLayout alphaLayout) = 0;

    /**
     * 
     * @valid since 3.58
     * @type api
     * @region 视频管理
     * @brief 关闭外部采集视频帧的 Alpha 通道编码功能。
     * @param streamIndex 需关闭该功能的视频流类型，当前仅支持设置为 StreamIndex.kStreamIndexMain 即主流。
     * @return 方法调用结果： <br>
     *         - 0：成功；
     *         - !0：失败。
     * @note 该接口须在停止发布视频流之后调用。
     */
     
    virtual int disableAlphaChannelVideoEncode(StreamIndex streamIndex) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 设置本地视频渲染时使用的视图，并设置渲染模式。
     * @param index 视频流属性, 参看 StreamIndex{@link #StreamIndex}
     * @param canvas 视图信息和渲染模式，参看：VideoCanvas{@link #VideoCanvas}
     * @return
     *        - 0：成功
     *        - !0：失败
     * @note
     *        - 你应在加入房间前，绑定本地视图。退出房间后，此设置仍然有效。
     *        - 如果需要解除绑定，你可以调用本方法传入空视图。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 视频管理
     * @order 3
     */
    
    virtual int setLocalVideoCanvas(StreamIndex index, const VideoCanvas& canvas) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 修改本地视频渲染模式和背景色。
     * @param index 视频流属性, 参看 StreamIndex{@link #StreamIndex}
     * @param render_mode 渲染模式，参看 RenderMode{@link #RenderMode}
     * @param background_color 背景颜色，参看 VideoCanvas{@link #VideoCanvas}.background_color
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 你可以在本地视频渲染过程中，调用此接口。调用结果会实时生效。
     * @list 视频管理
     * @order 4
     */
    
    virtual int updateLocalVideoCanvas(StreamIndex index, const enum RenderMode render_mode, const uint32_t background_color) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 设置渲染指定视频流 `stream_key` 时，使用的视图和渲染模式。 <br>
     *        如果需要解除某个用户的绑定视图，你可以把 `canvas` 设置为空。
     * @param stream_key 参看 RemoteStreamKey{@link #RemoteStreamKey}
     * @param canvas 视图信息和渲染模式，参看：VideoCanvas{@link #VideoCanvas}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 本地用户离开房间时，会解除调用此 API 建立的绑定关系；远端用户离开房间则不会影响。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 视频管理
     * @order 5
     */
    
    virtual int setRemoteVideoCanvas(RemoteStreamKey stream_key, const VideoCanvas& canvas) = 0;

    /**
     * 
     * @deprecated since 3.56 on iOS and Android, and will be deleted in 3.62. Use [updateRemoteStreamVideoCanvas](70095#updateremotestreamvideocanvas-2) instead.
     * @type api
     * @region 视频管理
     * @brief 修改远端视频帧的渲染设置，包括渲染模式和背景颜色。
     * @param stream_key 远端流信息。参看 RemoteStreamKey{@link #RemoteStreamKey}。
     * @param render_mode 渲染模式，参看 RenderMode{@link #RenderMode}。
     * @param background_color 背景颜色，参看 VideoCanvas{@link #VideoCanvas}.background_color
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 你可以在远端视频渲染过程中，调用此接口。调用结果会实时生效。
     */
    
    virtual int updateRemoteStreamVideoCanvas(RemoteStreamKey stream_key, const enum RenderMode render_mode, const uint32_t background_color) = 0;

    /**
     * 
     * @hidden(Windows, macOS, Linux)
     * @valid since 3.56
     * @type api
     * @region 视频管理
     * @brief 使用 SDK 内部渲染时，修改远端视频帧的渲染设置，包括渲染模式、背景颜色和旋转角度。
     * @param stream_key 远端流信息。参看 RemoteStreamKey{@link #RemoteStreamKey}。
     * @param remote_video_render_config 视频帧渲染设置。具体参看 RemoteVideoRenderConfig{@link #RemoteVideoRenderConfig}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用 setRemoteVideoCanvas{@link #IRTCVideo#setRemoteVideoCanvas} 设置了远端视频渲染模式后，你可以调用此接口更新渲染模式、背景颜色、旋转角度的设置。
     *        - 该接口可以在远端视频渲染过程中调用，调用结果会实时生效。
     */
    
    virtual int updateRemoteStreamVideoCanvas(RemoteStreamKey stream_key, const RemoteVideoRenderConfig& remote_video_render_config) = 0;

    /**
     * 
     * @type api
     * @deprecated since 3.57, use setLocalVideoRender{@link #IRTCVideo#setLocalVideoRender} instead.
     * @region 自定义视频采集渲染
     * @brief 将本地视频流与自定义渲染器绑定。
     * @param index 视频流属性。采集的视频流/屏幕视频流，参看 StreamIndex{@link #StreamIndex}
     * @param video_sink 自定义视频渲染器，参看 IVideoSink{@link #IVideoSink}。
     * @param required_format video_sink 适用的视频帧编码格式，参看 PixelFormat{@link #PixelFormat}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - RTC SDK 默认使用 RTC SDK 自带的渲染器（内部渲染器）进行视频渲染。
     *        - 如果需要解除绑定，必须将 video_sink 设置为 null。退房时将清除绑定状态。
     *        - 一般在收到 onFirstLocalVideoFrameCaptured{@link #IRTCVideoEventHandler#onFirstLocalVideoFrameCaptured} 回调通知完成本地视频首帧采集后，调用此方法为视频流绑定自定义渲染器；然后加入房间。
     *        - 本方法获取的是前处理后的视频帧，如需获取其他位置的视频帧（如采集后的视频帧），请调用 setLocalVideoRender{@link #IRTCVideo#setLocalVideoRender}。
     */
    
    virtual int setLocalVideoSink(StreamIndex index, IVideoSink* video_sink, IVideoSink::PixelFormat required_format) = 0;

    /**
     * 
     * @valid since 3.57
     * @type api
     * @region 自定义视频帧回调
     * @brief 将本地视频流与自定义渲染器绑定。你可以通过参数设置返回指定位置和格式的视频帧数据。
     * @param index 视频流属性。采集的视频流/屏幕视频流，参看 StreamIndex{@link #StreamIndex}。
     * @param video_sink 自定义视频渲染器，参看 IVideoSink{@link #IVideoSink}。
     * @param render_config 本地视频帧回调配置，参看 LocalVideoSinkConfig{@link #LocalVideoSinkConfig}。
     * @return
     *        - 0: 调用成功。
     *        - < 0: 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明。
     * @note
     *        - RTC SDK 默认使用自带的渲染器（内部渲染器）进行视频渲染。
     *        - 退房时将清除绑定状态。
     *        - 如果需要解除绑定，你必须将 video_sink 设置为 null。
     *        - 一般在收到 onFirstLocalVideoFrameCaptured{@link #IRTCVideoEventHandler#onFirstLocalVideoFrameCaptured} 回调通知完成本地视频首帧采集后，调用此方法为视频流绑定自定义渲染器；然后加入房间。
     * @list 自定义流处理
     * @order 2
     */
    
    virtual int setLocalVideoRender(StreamIndex index, IVideoSink* video_sink, LocalVideoSinkConfig& render_config) = 0;

    /**
     * 
     * @type api
     * @deprecated since 3.57, use setRemoteVideoRender{@link #IRTCVideo#setRemoteVideoRender} instead.
     * @region 自定义视频采集渲染
     * @brief 将远端视频流与自定义渲染器绑定。
     * @param stream_key 远端流信息，用于指定需要渲染的视频流来源及属性，参看 RemoteStreamKey{@link #RemoteStreamKey}。
     * @param video_sink 自定义视频渲染器，参看 IVideoSink{@link #IVideoSink}。
     * @param required_format video_sink 适用的视频帧编码格式，参看 PixelFormat{@link #PixelFormat}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - RTC SDK 默认使用 RTC SDK 自带的渲染器（内部渲染器）进行视频渲染。
     *        - 该方法进房前后均可以调用。若想在进房前调用，你需要在加入房间前获取远端流信息；若无法预先获取远端流信息，你可以在加入房间并通过 onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream} 回调获取到远端流信息之后，再调用该方法。
     *        - 如果需要解除绑定，必须将 video_sink 设置为 null。退房时将清除绑定状态。
     *        - 本方法获取的是后处理后的视频帧，如需获取其他位置的视频帧（如解码后的视频帧），请调用 setRemoteVideoRender{@link #IRTCVideo#setRemoteVideoRender}。
     */
    
    virtual int setRemoteVideoSink(RemoteStreamKey stream_key, IVideoSink* video_sink, IVideoSink::PixelFormat required_format) = 0;
    
    /**
     * 
     * @valid since 3.57
     * @type api
     * @region 自定义视频帧回调
     * @brief 将远端视频流与自定义渲染器绑定。你可以通过参数设置返回指定位置和格式的视频帧数据。
     * @param stream_key 远端流信息，用于指定需要渲染的视频流来源及属性，参看 RemoteStreamKey{@link #RemoteStreamKey}。
     * @param video_sink 自定义视频渲染器，参看 IVideoSink{@link #IVideoSink}。
     * @param config 远端视频帧回调配置，参看 RemoteVideoSinkConfig{@link #RemoteVideoSinkConfig}。
     * @return
     *        - 0: 调用成功。
     *        - < 0: 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明。
     * @note
     *        - RTC SDK 默认使用自带的渲染器（内部渲染器）进行视频渲染。
     *        - 该方法进房前后均可以调用。若想在进房前调用，你需要在加入房间前获取远端流信息；若无法预先获取远端流信息，你可以在加入房间并通过 onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream} 回调获取到远端流信息之后，再调用该方法。
     *        - 退房时将清除绑定状态。
     *        - 如果需要解除绑定，你必须将 video_sink 设置为 null。
     * @list 自定义流处理
     * @order 3
     */
     
    virtual int setRemoteVideoRender(RemoteStreamKey stream_key, IVideoSink* video_sink, RemoteVideoSinkConfig& config) = 0;

    /**
     * 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region 视频管理
     * @brief 切换视频内部采集时使用的前置/后置摄像头 <br>
     *        调用此接口后，在本地会触发 onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} 回调。
     * @param camera_id 参看 CameraID {@link #CameraID}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 默认使用前置摄像头。
     *       - 如果你正在使用相机进行视频采集，切换操作当即生效；如果相机未启动，后续开启内部采集时，会打开设定的摄像头。
     *       - 如果本地有多个摄像头且想选择特定工作摄像头可通过 IVideoDeviceManager{@link #IVideoDeviceManager} 来控制。
     */
    
    virtual int switchCamera(CameraID camera_id) = 0;

    /**
     * 
     * @type api
     * @region 屏幕共享
     * @brief 推送屏幕视频帧
     * @param frame 设置屏幕视频帧，详见：IVideoFrame{@link #IVideoFrame}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *       - 暂时只支持 YUV420P 格式的视频帧。
     *       - 该函数运行在用户调用线程内，即将销毁 IRTCVideo 实例前，请停止调用该函数推送屏幕共享数据
     *       - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 15
     */
    
    virtual int pushScreenVideoFrame(IVideoFrame* frame) = 0;

    /**
     * 
     * @hidden(Linux,iOS,Android)
     * @type api
     * @region 屏幕共享
     * @brief 外部采集时，当屏幕或待采集窗口大小发生改变，为了使 RTC 更好地决策合适的帧率和分辨率积，调用此接口设置改变前的分辨率。
     * @param original_capture_width 首次采集屏幕流的宽度。
     * @param original_capture_height 首次采集屏幕流的高度。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用此接口之前，建议调用 setScreenVideoEncoderConfig{@link #IRTCVideo#setScreenVideoEncoderConfig} 设置屏幕流编码相关参数：编码模式设置为智能模式，屏幕帧宽高设置为 0，最大码率设置为-1，最小码率设置为 0。
     *        - 调用此接口后，你将收到回调 onExternalScreenFrameUpdate{@link #IRTCVideoEventHandler#onExternalScreenFrameUpdate}，根据推荐的帧率和分辨率积重新采集。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 16
     */
    
    virtual int setOriginalScreenVideoInfo(int original_capture_width, int original_capture_height) = 0;

    /**
     * 
     * @hidden(Android,iOS)
     * @type api
     * @region 屏幕共享
     * @brief 通过 RTC SDK 提供的采集模块采集屏幕视频流时，更新采集区域。仅用于采集源为显示器屏幕时。
     * @param region_rect 采集区域。参见 Rectangle{@link #Rectangle} <br>
     *                         此参数描述了调用此接口后的采集区域，和 startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture} 中 `source_info` 设定区域的相对关系。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用此接口前，必须已通过调用 startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture} 开启了内部屏幕流采集。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 5
     */
    
    virtual int updateScreenCaptureRegion(const Rectangle& region_rect) = 0;

    /**
     * 
     * @hidden(Android,iOS)
     * @type api
     * @region 屏幕共享
     * @brief 采集屏幕视频流，用于共享。屏幕视频流包括：屏幕上显示的内容，应用窗口中显示的内容，或虚拟屏幕中显示的内容。 <br>
     *        其中，虚拟屏幕中显示的内容仅在 Windows 平台上支持。
     * @param source_info 待共享的屏幕源，参看 ScreenCaptureSourceInfo{@link #ScreenCaptureSourceInfo}。 <br>
     *                         你可以调用 getScreenCaptureSourceList{@link #IRTCVideo#getScreenCaptureSourceList} 获得所有可以共享的屏幕源。
     * @param capture_params 共享参数。参看 ScreenCaptureParameters{@link #ScreenCaptureParameters}。
     * @return
     *        - 0: 成功
     *        - -1: 失败
     * @note
     *       - 调用本接口时，采集模式应为内部模式。在外部采集模式下调用无效，并将触发 onVideoDeviceWarning{@link #IRTCVideoEventHandler#onVideoDeviceWarning} 回调。
     *       - 调用此方法仅开启屏幕流视频采集，不会发布采集到的视频。发布屏幕流视频需要调用 publishScreen{@link #IRTCRoom#publishScreen}。
     *       - 调用 stopScreenVideoCapture{@link #IRTCVideo#stopScreenVideoCapture} 关闭屏幕视频源采集。
     *       - 本地用户通过 onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} 的回调获取屏幕采集状态，包括开始、暂停、恢复、错误等。
     *       - 调用成功后，本端会收到 onFirstLocalVideoFrameCaptured{@link #IRTCVideoEventHandler#onFirstLocalVideoFrameCaptured} 回调。
     *       - 调用此接口前，你可以调用 setScreenVideoEncoderConfig{@link #IRTCVideo#setScreenVideoEncoderConfig} 设置屏幕视频流的采集帧率和编码分辨率。
     *       - 在收到 onFirstLocalVideoFrameCaptured{@link #IRTCVideoEventHandler#onFirstLocalVideoFrameCaptured} 回调后通过调用 setLocalVideoCanvas{@link #IRTCVideo#setLocalVideoCanvas} 或 setLocalVideoSink{@link #setLocalVideoSink} 函数设置本地屏幕共享视图。
     *       - 可以调用 setLocalVideoSink{@link #IRTCVideo#setLocalVideoSink} 将本地视频流与自定义渲染器绑定，通过回调 onFrame{@link #IVideoSink#onFrame} 获取采集成功的本地视频帧。
     *       - 对于 Windows SDK，再开启采集屏幕视频流后，你可以调用 updateScreenCaptureHighlightConfig{@link #IRTCVideo#updateScreenCaptureHighlightConfig} 更新边框高亮设置，调用 updateScreenCaptureMouseCursor{@link #IRTCVideo#updateScreenCaptureMouseCursor} 更新对鼠标的处理设置，调用 updateScreenCaptureFilterConfig{@link #IRTCVideo#updateScreenCaptureFilterConfig} 设置需要过滤的窗口。
     *       - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 1
     */
    
    virtual int startScreenVideoCapture(const ScreenCaptureSourceInfo& source_info, const ScreenCaptureParameters& capture_params) = 0;

    /**
     * 
     * @hidden(Android,iOS)
     * @type api
     * @region 屏幕共享
     * @brief 停止屏幕视频流采集。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 调用本接口时，采集模式应为内部模式。在外部采集模式下调用无效，并将触发 onVideoDeviceWarning{@link #IRTCVideoEventHandler#onVideoDeviceWarning} 回调。
     *       - 要开启屏幕视频流采集，调用 startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture}。
     *       - 调用后，本地用户会收到 onVideoDeviceStateChanged{@link #IRTCVideoEventHandler#onVideoDeviceStateChanged} 的回调。
     *       - 调用此接口不影响屏幕视频流发布。
     *       - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 6
     */
    
    virtual int stopScreenVideoCapture() = 0;

    /**
     * 
     * @hidden(Android,iOS)
     * @type api
     * @region 屏幕共享
     * @brief 通过 RTC SDK 提供的采集模块采集屏幕视频流时，更新边框高亮设置。默认展示表框。
     * @param highlight_config 边框高亮设置。参见 HighlightConfig{@link #HighlightConfig}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用此接口前，必须已通过调用 startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture} 开启了内部屏幕流采集。
     *        - 对 Linux 系统，采用 X11 协议时可用；采用 Wayland 协议时不可用。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 2
     */
    
    virtual int updateScreenCaptureHighlightConfig(const HighlightConfig& highlight_config) = 0;

    /**
     * 
     * @hidden(Android,iOS)
     * @type api
     * @region 屏幕共享
     * @brief 通过 RTC SDK 提供的采集模块采集屏幕视频流时，更新对鼠标的处理设置。默认采集鼠标。
     * @param capture_mouse_cursor 参看 MouseCursorCaptureState{@link #MouseCursorCaptureState}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用此接口前，必须已通过调用 startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture} 开启了内部屏幕流采集。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 3
     */
    
    virtual int updateScreenCaptureMouseCursor(MouseCursorCaptureState capture_mouse_cursor) = 0;

    /**
     * 
     * @hidden(Linux,iOS,Android)
     * @type api
     * @region 屏幕共享
     * @brief 通过 RTC SDK 提供的采集模块采集屏幕视频流时，设置需要过滤的窗口。
     * @param filter_config 窗口过滤设置，参看 ScreenFilterConfig{@link #ScreenFilterConfig}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 调用此接口前，必须已通过调用 startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture} 开启了内部屏幕流采集。
     *       - 本函数在屏幕源类别是屏幕而非应用窗体时才起作用。详见：ScreenCaptureSourceType{@link #ScreenCaptureSourceType}。
     *       - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 4
     */
    
    virtual int updateScreenCaptureFilterConfig(const ScreenFilterConfig& filter_config) = 0;

    /**
     * 
     * @hidden(Android,iOS)
     * @brief 获取共享对象列表。
     * @return 共享对象列表，包括应用窗口和屏幕。详见 IScreenCaptureSourceList{@link #IScreenCaptureSourceList}。用户选择后，可调用 startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture} 进行共享。
     * @note
     *        - 对 Linux 系统，采用 X11 协议时可用；采用 Wayland 协议时不可用。采用 Wayland 协议，调用 startScreenVideoCapture{@link #IRTCVideo#startScreenVideoCapture} 时，系统会通过弹窗供用户选择共享对象。
     *        - 使用完之后需要调用 release{@link #IScreenCaptureSourceList#release} 接口释放。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 0
     */
    
    virtual IScreenCaptureSourceList* getScreenCaptureSourceList() = 0;

    /**
     * 
     * @hidden(Android,iOS)
     * @brief 获取共享对象缩略图
     * @region 屏幕共享
     * @param type 屏幕采集对象的类型。详见 ScreenCaptureSourceType{@link #ScreenCaptureSourceType}。
     * @param source_id 屏幕共享对象的 ID，可通过 getScreenCaptureSourceList{@link #IRTCVideo#getScreenCaptureSourceList} 枚举共享对象列表中获取。详见 view_t{@link #view-t}。
     * @param max_width 最大宽度。保持采集对象本身的宽高比不变，将缩略图缩放到指定范围内的最大宽高。如果给出的尺寸与共享对象比例不同，得到的缩略图会有黑边。
     * @param max_height 最大高度。参见 max_width 的说明。
     * @return 共享对象缩略图，详见 IVideoFrame{@link #IVideoFrame}。
     * @note
     *        - 对 Linux 系统，采用 X11 协议时可用；采用 Wayland 协议时不可用。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 9
     */
    
    virtual IVideoFrame* getThumbnail(
            ScreenCaptureSourceType type, view_t source_id, int max_width, int max_height) = 0;
    /**
     * 
     * @hidden(Android,iOS)
     * @region 屏幕共享
     * @brief 获取应用窗体所属应用的图标。
     * @param source_id 屏幕共享对象的 ID，可通过 getScreenCaptureSourceList{@link #IRTCVideo#getScreenCaptureSourceList} 枚举共享对象列表中获取。详见 view_t{@link #view-t}。
     * @param max_width 最大宽度。返回的图标将是宽高相等的，输入宽高不等时，取二者较小值。宽高范围为 [32,256]，超出该范围将返回 nullptr，默认输出 100 x 100 的图像。
     * @param max_height 最大高度。参见 max_width 的说明。
     * @return 详见 IVideoFrame{@link #IVideoFrame}。当屏幕共享对象为应用窗体，且有图标时有效，否则返回 nullptr。
     * @note
     *        - 对 Linux 系统，采用 X11 协议时可用；采用 Wayland 协议时不可用。
     *        - 对 Linux SDK：仅在 Client 版本有，在 Server 版本上没有。
     * @list 屏幕分享
     * @order 10
     */
    
    virtual IVideoFrame* getWindowAppIcon(view_t source_id, int max_width = 100, int max_height = 100) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 设置向 SDK 输入的视频源，包括屏幕流 <br>
     *        默认使用内部采集。内部采集指：使用 RTC SDK 内置的视频采集机制进行视频采集。
     * @param stream_index 视频流的属性，参看 StreamIndex{@link #StreamIndex}
     * @param type 视频输入源类型，参看 VideoSourceType{@link #VideoSourceType}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 该方法进房前后均可调用。
     *        - 当你已调用 startVideoCapture{@link #IRTCVideo#startVideoCapture} 开启内部采集后，再调用此方法切换至自定义采集时，SDK 会自动关闭内部采集。
     *        - 当你调用此方法开启自定义采集后，想要切换至内部采集，你必须先调用此方法关闭自定义采集，然后调用 startVideoCapture{@link #IRTCVideo#startVideoCapture} 手动开启内部采集。
     *        - 当你需要向 SDK 推送自定义编码后的视频帧，你需调用该方法将视频源切换至自定义编码视频源。
     * @list 自定义流处理
     * @order 0
     */
    
    virtual int setVideoSourceType(StreamIndex stream_index, VideoSourceType type) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 推送外部视频帧。
     * @param frame 设置视频帧，参看 IVideoFrame{@link #IVideoFrame}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *       - 支持格式：I420, NV12, RGBA, BGRA, ARGB
     *       - 该函数运行在用户调用线程内
     *       - 推送外部视频帧前，必须调用 setVideoSourceType{@link #setVideoSourceType} 开启外部视频源采集。
     * @list 自定义流处理
     * @order 1
     */
    
    virtual int pushExternalVideoFrame(IVideoFrame* frame) = 0;

    /**
     * 
     * @hidden(Windows,Linux,macOS)
     * @deprecated since 3.38 and will be deleted in 3.51, use setDefaultAudioRoute{@link #IRTCVideo#setDefaultAudioRoute} instead on Android and iOS.
     * @type api
     * @region 音频设备管理
     * @brief 设置音频播放设备，默认使用扬声器。
     * @param device 音频播放设备。参看 AudioPlaybackDevice{@link #AudioPlaybackDevice}
     * @return 方法调用结果 <br>
     *        - 0: 方法调用成功
     *        - < 0: 方法调用失败
     * @note
     *       - 该接口仅适用于移动设备。
     *       - 该方法只支持将音视频播放设备设置为听筒或者扬声器。当 App 连接有线或蓝牙音频播放设备时，SDK 会自动切换到有线或蓝牙音频播放设备。主动设置为有线或蓝牙音频播放设备，会返回调用失败。
     *       - 若连接有线或者蓝牙音频播放设备时，将音频播放设备设置为扬声器或听筒将调用成功，但不会立马切换到扬声器或听筒，会在有线或者蓝牙音频播放设备移除后，根据设置自动切换到听筒或者扬声器。
     *       - 通话前和通话中都可以调用该方法。
     */
    
    BYTERTC_DEPRECATED virtual int setAudioPlaybackDevice(AudioPlaybackDevice device) = 0;

    /**
     * 
     * @hidden(Windows,Linux,macOS)
     * @type api
     * @region 音频设备管理
     * @brief 设置音频播放设备。默认使用通过 setDefaultAudioRoute{@link #IRTCVideo#setDefaultAudioRoute} 设置的默认音频路由。 <br>
     *        音频播放设备发生变化时，会收到 onAudioRouteChanged{@link #IRTCVideoEventHandler#onAudioRouteChanged} 回调。
     * @param device 音频播放设备。参看 AudioRoute{@link #AudioRoute}。仅支持听筒或者扬声器。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 对于绝大多数音频场景，使用 setDefaultAudioRoute{@link #IRTCVideo#setDefaultAudioRoute} 设置默认音频路由，并借助 RTC SDK 的音频路由自动切换逻辑即可完成。切换逻辑参见[移动端设置音频路由](https:
     *       - 本接口仅支持在 `AUDIO_SCENARIO_COMMUNICATION` 音频场景下使用。你可以通过调用 setAudioScenario{@link #IRTCVideo#setAudioScenario} 切换音频场景。
     */
    
    virtual int setAudioRoute(AudioRoute route) = 0;

    /**
     * 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region 音频设备管理
     * @brief 将默认的音频播放设备设置为听筒或扬声器。
     * @param route 音频播放设备。参看 AudioRoute{@link #AudioRoute}
     * @return 方法调用结果 <br>
     *        - 0: 方法调用成功。立即生效。当所有音频外设移除后，音频路由将被切换到默认设备。
     *        - < 0: 方法调用失败。指定除扬声器和听筒以外的设备将会失败。
     * @note 对于音频路由切换逻辑，参见[移动端设置音频路由](https:
     */
    
    virtual int setDefaultAudioRoute(AudioRoute route) = 0;

    /**
     * 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region 音频设备管理
     * @brief 获取当前音频播放设备 <br>
     *        音频播放设备发生变化时，会收到 onAudioRouteChanged{@link #IRTCVideoEventHandler#onAudioRouteChanged} 回调。
     * @return device 当前音频播放设备。参看 AudioRoute{@link #AudioRoute}
     * @note
     *       - 1. 该接口仅适用于移动设备。
     *       - 2. 通话前和通话中都可以调用该方法。
     */
    
    virtual AudioRoute getAudioRoute() = 0;
    /**
     * 
     * @type api
     * @region 房间管理
     * @brief 创建房间实例。 <br>
     *        调用此方法仅返回一个房间实例，你仍需调用 joinRoom{@link #IRTCRoom#joinRoom} 才能真正地创建/加入房间。 <br>
     *        多次调用此方法以创建多个 IRTCRoom{@link #IRTCRoom} 实例。分别调用各 IRTCRoom 实例中的 joinRoom{@link #IRTCRoom#joinRoom} 方法，同时加入多个房间。 <br>
     *        多房间模式下，用户可以同时订阅各房间的音视频流。
     * @param room_id 标识通话房间的房间 ID。该字符串符合正则表达式：`[a-zA-Z0-9_@\-\.]{1,128}`。
     * @return 创建的 IRTCRoom{@link #IRTCRoom} 房间实例。
     * @note
     *        - 如果需要加入的房间已存在，你仍需先调用本方法来获取 IRTCRoom 实例，再调用 joinRoom{@link #IRTCRoom#joinRoom} 加入房间。
     *        - 请勿使用同样的 roomId 创建多个房间，否则后创建的房间实例会替换先创建的房间实例。
     *        - 如果你需要在多个房间发布音视频流，无须创建多房间，直接调用 startForwardStreamToRooms{@link #IRTCRoom#startForwardStreamToRooms} 开始跨房间转发媒体流。
     * @list 房间管理
     * @order 0
     */
    
    virtual IRTCRoom* createRTCRoom(const char* room_id) = 0;

    /**
     * 
     * @type api
     * @region 音视频回退
     * @brief 设置发布的音视频流的回退选项。 <br>
     *        你可以调用该接口设置网络不佳或设备性能不足时从大流起进行降级处理，以保证通话质量。
     * @param option 本地发布的音视频流回退选项，参看 PublishFallbackOption{@link #PublishFallbackOption}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 该方法仅在调用 enableSimulcastMode{@link #IRTCVideo#enableSimulcastMode} 开启了发送多路视频流的情况下生效。
     *        - 该方法必须在进房前设置，进房后设置或更改设置无效。
     *        - 调用该方法后，如因性能或网络不佳产生发布性能回退或恢复，本端会提前收到 onPerformanceAlarms{@link #IRTCVideoEventHandler#onPerformanceAlarms} 回调发出的告警，以便采集设备配合调整。
     *        - 设置回退后，本地发布的音视频流发生回退或从回退中恢复后，远端会收到 onSimulcastSubscribeFallback{@link #IRTCVideoEventHandler#onSimulcastSubscribeFallback} 回调，通知该情况。
     *        - 你可以调用客户端 API 或者在服务端下发策略设置回退。当使用服务端下发配置实现时，下发配置优先级高于在客户端使用 API 设定的配置。
     * @list 网络管理
     * @order 2
     */
    
    virtual int setPublishFallbackOption(PublishFallbackOption option) = 0;
    /**
     * 
     * @type api
     * @region 音视频回退
     * @brief 设置订阅的音视频流的回退选项。 <br>
     *        你可调用该接口设置网络不佳或设备性能不足时允许订阅流进行降级或只订阅音频流，以保证通话流畅。
     * @param option 订阅的音视频流回退选项，参看 SubscribeFallbackOption{@link #SubscribeFallbackOption}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 你必须在进房前设置，进房后设置或更改设置无效。
     *        - 设置回退选项后，订阅的音视频流发生回退或从回退中恢复时，会收到 onSimulcastSubscribeFallback{@link #IRTCVideoEventHandler#onSimulcastSubscribeFallback} 和 onRemoteVideoSizeChanged{@link #IRTCVideoEventHandler#onRemoteVideoSizeChanged} 回调通知。
     *        - 你可以调用 API 或者在服务端下发策略设置回退。当使用服务端下发配置实现时，下发配置优先级高于在客户端使用 API 设定的配置。
     * @list 网络管理
     * @order 3
     */
    
    virtual int setSubscribeFallbackOption(SubscribeFallbackOption option) = 0;

    /**
     * 
     * @type api
     * @region 音视频回退
     * @brief 设置用户优先级。
     * @param room_id 房间 ID
     * @param user_id 远端用户的 ID
     * @param priority 远端用户的需求优先级，详见枚举类型 RemoteUserPriority{@link #RemoteUserPriority}
     * @return 0: 方法调用成功 <br>
     *        - < 0: 方法调用失败
     * @note
     *        - 1. 该方法与 setSubscribeFallbackOption{@link #IRTCVideo#setSubscribeFallbackOption} 搭配使用。
     *        - 2. 如果开启了订阅流回退选项，弱网或性能不足时会优先保证收到的高优先级用户的流的质量。
     *        - 3. 该方法在进房前后都可以使用，可以修改远端用户的优先级。
     * @list 网络管理
     * @order 4
     */
    
    virtual int setRemoteUserPriority(const char* room_id, const char* user_id, RemoteUserPriority priority) = 0;

    /**
     * 
     * @type api
     * @region 引擎管理
     * @brief 设置业务标识参数 <br>
     *        可通过 businessId 区分不同的业务场景。businessId 由客户自定义，相当于一个“标签”，可以分担和细化现在 AppId 的逻辑划分的功能，但不需要鉴权。
     * @param business_id <br>
     *        用户设置的自己的 businessId 值 <br>
     *        businessId 只是一个标签，颗粒度需要用户自定义。
     * @return
     *        - 0： 成功
     *        - < 0： 失败，具体原因参照 BusinessCheckCode{@link #BusinessCheckCode} 。
     *        - -6001： 用户已经在房间中。
     *        - -6002： 输入非法，合法字符包括所有小写字母、大写字母和数字，除此外还包括四个独立字符，分别是：英文句号，短横线，下划线和 @ 。
     * @note
     *        - 需要在调用 joinRoom{@link #IRTCRoom#joinRoom} 之前调用，joinRoom{@link #IRTCRoom#joinRoom} 之后调用该方法无效。
     * @list 引擎管理
     * @order 20
     */
    
    virtual int setBusinessId(const char* business_id) = 0;

    /**
     * 
     * @type api
     * @hidden(Windows,macOS,Linux)
     * @brief 设置采集视频的旋转模式。默认以 App 方向为旋转参考系。 <br>
     *        接收端渲染视频时，将按照和发送端相同的方式进行旋转。
     * @param rotation_mode 视频旋转参考系为 App 方向或重力方向，参看 VideoRotationMode{@link #VideoRotationMode}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 旋转仅对内部视频采集生效，不适用于外部视频源和屏幕源。
     *        - 调用该接口时已开启视频采集，将立即生效；调用该接口时未开启视频采集，则将在采集开启后生效。
     *        - 更多信息请参考[视频采集方向](https:
     */
    
    virtual int setVideoRotationMode(VideoRotationMode rotation_mode) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 视频管理
     * @brief 为采集到的视频流开启镜像
     * @param mirror_type 镜像类型，参看 MirrorType{@link #MirrorType}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 切换视频源不影响镜像设置。
     *        - 屏幕视频流始终不受镜像设置影响。
     *        - 使用外部渲染器时，`mirrorType` 支持设置为 `0`（无镜像）和 `3`（本地预览和编码传输镜像），不支持设置为 `1`（本地预览镜像）。
     *        - 该接口调用前，各视频源的初始状态如下：
     *        <table>
     *           <tr><th></th><th>前置摄像头</th><th>后置摄像头</th><th>自定义采集视频源</th> <th>桌面端摄像头</th> </tr>
     *           <tr><td>移动端</td><td>本地预览镜像，编码传输不镜像</td><td> 本地预览不镜像，编码传输不镜像 </td><td> 本地预览不镜像，编码传输不镜像 </td><td>/</td></tr>
     *           <tr><td>桌面端</td><td>/</td><td>/</td><td> 本地预览不镜像，编码传输不镜像 </td><td> 本地预览镜像，编码传输不镜像 </td></tr>
     *        </table>
     * @list 视频处理
     * @order 0
     */
    
    virtual int setLocalVideoMirrorType(MirrorType mirror_type) = 0;

    /**
     * 
     * @type api
     * @valid since 3.57
     * @region 视频管理
     * @brief 使用内部渲染时，为远端流开启镜像。
     * @param stream_key 远端流信息，用于指定需要镜像的视频流来源及属性，参看 RemoteStreamKey{@link #RemoteStreamKey}。
     * @param mirror_type 远端流的镜像类型，参看 RemoteMirrorType{@link #RemoteMirrorType}。
     * @return
     *        - 0: 调用成功。
     *        - < 0: 调用失败，参看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明。
     * @list 视频处理
     * @order 1
     */
    
    virtual int setRemoteVideoMirrorType(RemoteStreamKey stream_key, RemoteMirrorType mirror_type) = 0;

    /**
     * 
     * @type api
     * @region 音视频处理
     * @brief 获取视频特效接口。
     * @return 视频特效接口，参看 IVideoEffect{@link #IVideoEffect}。
     * @list 视频处理
     * @order 7
     */
    
    virtual IVideoEffect* getVideoEffectInterface() = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 音视频处理
     * @brief 开启/关闭基础美颜。
     * @param enable 基础美颜开关 <br>
     *        - true: 开启基础美颜
     *        - false: 关闭基础美颜（默认）
     * @return
     *        - 0: 调用成功。
     *        - –1000: 未集成特效 SDK。
     *        - –1001: RTC SDK 版本不支持此功能。
     *        - –1002: 特效 SDK 当前版本不支持此功能，建议使用特效 SDK v4.4.2+ 版本。
     *        - –1003: 联系技术支持人员。
     *        - –1004: 正在下载相关资源，下载完成后生效。
     *        - <0: 调用失败，特效 SDK 内部错误，具体错误码请参考[错误码表](https:
     * @note
     *        - 本方法不能与高级视频特效接口共用。如已购买高级视频特效，建议参看[集成指南](https:
     *        - 使用此功能需要集成特效 SDK，建议使用特效 SDK v4.4.2+ 版本。更多信息参看 [Native 端基础美颜](https:
     *        - 调用 setBeautyIntensity{@link #IRTCVideo#setBeautyIntensity} 设置基础美颜强度。若在调用本方法前没有设置美颜强度，则使用默认强度。各基础美颜模式的强度默认值分别为：美白 0.7，磨皮 0.8，锐化 0.5，清晰 0.7。
     *        - 本方法仅适用于视频源，不适用于屏幕源。
     * @list 视频处理
     * @order 5
     */
    
    virtual int enableEffectBeauty(bool enable) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 音视频处理
     * @brief 调整基础美颜强度。
     * @param beauty_mode 基础美颜模式，参看 EffectBeautyMode{@link #EffectBeautyMode}。
     * @param intensity 美颜强度，取值范围为 [0,1]。强度为 0 表示关闭。 <br>
     *                       各基础美颜模式的强度默认值分别为：美白 0.7，磨皮 0.8，锐化 0.5，清晰 0.7。
     * @return
     *        - 0: 调用成功。
     *        - –1000: 未集成特效 SDK。
     *        - –1001: RTC SDK 版本不支持此功能。
     *        - <0: 调用失败，特效 SDK 内部错误，具体错误码请参考[错误码表](https:
     * @note
     *        - 若在调用 enableEffectBeauty{@link #IRTCVideo#enableEffectBeauty} 前设置美颜强度，则对应美颜功能的强度初始值会根据设置更新。
     *        - 销毁引擎后，美颜功能强度恢复默认值。
     * @list 视频处理
     * @order 6
     */
    
    virtual int setBeautyIntensity(EffectBeautyMode beauty_mode, float intensity) = 0;

    /**
     * 
     * @valid since 3.54
     * @hidden(Windows, macOS, Linux)
     * @type api
     * @region 音视频处理
     * @brief 设置远端视频超分模式。
     * @param stream_key 远端流信息，用于指定需要设置超分的视频流来源及属性，参看 RemoteStreamKey{@link #RemoteStreamKey}。
     * @param mode 超分模式，参看 VideoSuperResolutionMode{@link #VideoSuperResolutionMode}。
     * @return
     *        - 0: kReturnStatusSuccess，SDK 调用成功，并不代表超分模式实际状态，需要根据回调 onRemoteVideoSuperResolutionModeChanged{@link #IRTCVideoEventHandler#onRemoteVideoSuperResolutionModeChanged} 判断实际状态。
     *        - -1: kReturnStatusNativeInvalid，native library 未加载。
     *        - -2: kReturnStatusParameterErr，参数非法，指针为空或字符串为空。
     *        - -9: kReturnStatusScreenNotSupport，不支持对屏幕流开启超分。
     * @note
     *        - 该方法须进房后调用。
     *        - 远端用户视频流的原始分辨率不能超过 640 × 360 px。
     *        - 支持对一路远端流开启超分，不支持对多路流开启超分。
     */
    
    virtual int setRemoteVideoSuperResolution(RemoteStreamKey stream_key, VideoSuperResolutionMode mode) = 0;

    /**
     * 
     * @valid since 3.54
     * @hidden(Windows, macOS, Linux)
     * @type api
     * @region 音视频处理
     * @brief 设置视频降噪模式。
     * @param mode 视频降噪模式，启用后能够增强视频画质，但同时会增加性能负载。参看 VideoDenoiseMode{@link #VideoDenoiseMode}。
     * @return
     *        - 0: API 调用成功。 用户可以根据回调函数 onVideoDenoiseModeChanged{@link #IRTCVideoEventHandler#onVideoDenoiseModeChanged} 判断视频降噪是否开启。
     *        - < 0：API 调用失败。
     */
    
    virtual int setVideoDenoiser(VideoDenoiseMode mode) = 0;

    /**
     * 
     * @hidden(Windows, Linux, macOS)
     * @type api
     * @region 自定义流处理
     * @brief 在自定义视频前处理及编码前，设置 RTC 链路中的视频帧朝向，默认为 Adaptive 模式。 <br>
     *        移动端开启视频特效贴纸，或使用自定义视频前处理时，建议固定视频帧朝向为 Portrait 模式。单流转推场景下，建议根据业务需要固定视频帧朝向为 Portrait 或 Landscape 模式。不同模式的具体显示效果参看[视频帧朝向](https:
     * @param orientation 视频帧朝向，参看 VideoOrientation{@link #VideoOrientation}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 视频帧朝向设置仅适用于内部采集视频源。对于自定义采集视频源，设置视频帧朝向可能会导致错误，例如宽高对调。屏幕源不支持设置视频帧朝向。
     *        - 编码分辨率的更新与视频帧处理是异步操作，进房后切换视频帧朝向可能导致画面出现短暂的裁切异常，因此建议在进房前设置视频帧朝向，且不在进房后进行切换。
     */
    
    virtual int setVideoOrientation(VideoOrientation orientation) = 0;

    /**
     * 
     * @hidden(macOS, Windows,Linux)
     * @type api
     * @region 视频管理
     * @brief 获取相机控制接口
     * @return 相机控制接口指针，参看 ICameraControl{@link #ICameraControl}
     */
    
    virtual ICameraControl* getCameraControl() = 0;

    /**
     * 
     * @type api
     * @region 加密
     * @brief 设置传输时使用内置加密的方式。
     * @param encrypt_type 内置加密算法，详见 EncryptType{@link #EncryptType}
     * @param encrypt_type 加密类型，详见 EncryptType{@link #EncryptType}
     * @param key 加密密钥，长度限制为 36 位，超出部分将会被截断
     * @param key_size 参数 key 的长度
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 使用传输时内置加密时，使用此方法；如果需要使用传输时自定义加密，参看 onEncryptData{@link #IEncryptHandler#onEncryptData}。 内置加密和自定义加密互斥，根据最后一个调用的方法确定传输是加密的方案。
     *       - 该方法必须在进房之前调用，可重复调用，以最后调用的参数作为生效参数。
     * @list 安全与加密
     * @order 3
     */
    
    virtual int setEncryptInfo(EncryptType encrypt_type, const char* key, int key_size) = 0;

    /**
     * 
     * @type api
     * @region 加密
     * @brief 设置自定义加密和解密方式。
     * @param handler 自定义加密 handler，需要实现 handler 的加密和解密方法。参看 IEncryptHandler{@link #IEncryptHandler}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 该方法与 setEncryptInfo{@link #IRTCVideo#setEncryptInfo} 为互斥关系，只能选择自定义加密方式或者默认加密方式。最终生效的加密方式取决于最后一个调用的方法。
     *       - 该方法必须在进房之前调用，可重复调用，以最后调用的参数作为生效参数。
     *       - 无论加密或者解密，其对原始数据的长度修改，需要控制在 180% 之间，即如果输入数据为 100 字节，则处理完成后的数据必须不超过 180 字节，如果加密或解密结果超出该长度限制，则该音视频帧可能会被丢弃。
     *       - 数据加密/解密为串行执行，因而视实现方式不同，可能会影响到最终渲染效率。是否使用该方法，需要由使用方谨慎评估。
     *
     * @list 安全与加密
     * @order 2
     */
    
    virtual int setCustomizeEncryptHandler(IEncryptHandler* handler) = 0;

    /**
     * 
     * @type api
     * @region 音频数据回调
     * @brief 设置并开启指定的音频数据帧回调
     * @param method 音频回调方法，参看 AudioFrameCallbackMethod{@link #AudioFrameCallbackMethod}。 <br>
     *               当音频回调方法设置为 `kRecord`、`kPlayback`、`kMixed`、`kRecordScreen`时，你需要在参数 `format` 中指定准确的采样率和声道，暂不支持设置为自动。 <br>
     *               当音频回调方法设置为 `kRemoteUser`时，将 `format` 中的各个字段设置为默认值。
     * @param format 音频参数格式，参看 AudioFormat{@link #AudioFormat}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 开启音频回调并调用 registerAudioFrameObserver{@link #IRTCVideo#registerAudioFrameObserver} 后，IAudioFrameObserver{@link #IAudioFrameObserver} 会收到对应的音频回调。两者调用顺序没有限制且相互独立。
     * @list 自定义流处理
     * @order 16
     */
    
    virtual int enableAudioFrameCallback(AudioFrameCallbackMethod method, AudioFormat format) = 0;

    /**
     * 
     * @type api
     * @region 音频数据回调
     * @brief 关闭音频回调
     * @param method 音频回调方法，参看 AudioFrameCallbackMethod{@link #AudioFrameCallbackMethod}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 该方法需要在调用 enableAudioFrameCallback{@link #IRTCVideo#enableAudioFrameCallback} 之后调用。
     * @list 自定义流处理
     * @order 17
     */
    
    virtual int disableAudioFrameCallback(AudioFrameCallbackMethod method) = 0;

    /**
     * 
     * @type api
     * @region 音频数据回调
     * @brief 注册音频数据回调观察者。
     * @param observer 音频数据观察者，参看 IAudioFrameObserver{@link #IAudioFrameObserver}。如果传入 null，则取消注册。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 注册音频数据回调观察者并调用 enableAudioFrameCallback{@link #IRTCVideo#enableAudioFrameCallback} 后，IAudioFrameObserver{@link #IAudioFrameObserver} 会收到对应的音频回调。对回调中收到的音频数据进行处理，不会影响 RTC 的编码发送或渲染。
     * @list 自定义流处理
     * @order 15
     */
    
    virtual int registerAudioFrameObserver(IAudioFrameObserver* observer) = 0;

    /**
     * 
     * @type api
     * @brief 注册自定义音频处理器。 <br>
     *        注册完成后，你可以调用 enableAudioProcessor{@link #IRTCVideo#enableAudioProcessor}，对本地采集到的音频进行处理，RTC SDK 将对处理后的音频进行编码和发送。也可以对接收到的远端音频进行自定义处理，RTC SDK 将对处理后的音频进行渲染。
     * @param processor 自定义音频处理器，详见 IAudioFrameProcessor{@link #IAudioFrameProcessor}。 <br>
     *        SDK 只持有 processor 的弱引用，你应保证其生命周期。需要取消注册时，设置此参数为 nullptr。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     * - 重复调用此接口时，仅最后一次调用生效。
     * - 更多相关信息，详见[音频自定义处理](https:
     * @list 自定义流处理
     * @order 12
     */
    
    virtual int registerAudioProcessor(IAudioFrameProcessor* processor) = 0;

    /**
     * 
     * @type api
     * @brief 设置并开启指定的音频帧回调，进行自定义处理。
     * @param method 音频帧类型，参看 AudioProcessorMethod{@link #AudioProcessorMethod}。可多次调用此接口，处理不同类型的音频帧。 <br>
     *        选择不同类型的音频帧将收到对应的回调： <br>
     *        - 选择本地采集的音频时，会收到 onProcessRecordAudioFrame{@link #IAudioFrameProcessor#onProcessRecordAudioFrame}。
     *        - 选择远端音频流的混音音频时，会收到 onProcessPlayBackAudioFrame{@link #IAudioFrameProcessor#onProcessPlayBackAudioFrame}。
     *        - 选择远端音频流时，会收到 onProcessRemoteUserAudioFrame{@link #IAudioFrameProcessor#onProcessRemoteUserAudioFrame}。
     *        - 选择屏幕共享音频流时，会收到 onProcessScreenAudioFrame{@link #IAudioFrameProcessor#onProcessScreenAudioFrame}。（Linux 不适用）
     * @param format 设定自定义处理时获取的音频帧格式，参看 AudioFormat{@link #AudioFormat}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 在调用此接口前，你需要调用 registerAudioProcessor{@link #IRTCVideo#registerAudioProcessor} 注册自定义音频处理器。
     *        - 要关闭音频自定义处理，调用 disableAudioProcessor{@link #IRTCVideo#disableAudioProcessor}。
     * @list 自定义流处理
     * @order 13
     */
    
    virtual int enableAudioProcessor(AudioProcessorMethod method, AudioFormat format) = 0;

    /**
     * 
     * @type api
     * @brief 关闭自定义音频处理。
     * @param method 音频帧类型，参看 AudioProcessorMethod{@link #AudioProcessorMethod}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @list 自定义流处理
     * @order 14
     */
    
    virtual int disableAudioProcessor(AudioProcessorMethod method) = 0;

    /**
     * 
     * @hidden
     * @deprecated since 3.50 and will be deleted in 3.55, use setLocalVideoSink{@link #IRTCVideo#setLocalVideoSink} instead.
     * @type api
     * @region 视频数据回调
     * @brief 注册视频数据回调观察者
     * @param observer 数据回调函数，详见 IVideoFrameObserver{@link #IVideoFrameObserver}，将参数设置为 nullptr 则取消注册。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 该方法可以在任意时间调用，建议在 joinRoom{@link #IRTCRoom#joinRoom} 前。
     */
    
    BYTERTC_DEPRECATED virtual int registerVideoFrameObserver(IVideoFrameObserver* observer) = 0;

    /**
     * 
     * @type api
     * @region 视频处理
     * @brief 设置自定义视频前处理器。 <br>
     *        使用这个视频前处理器，你可以调用 processVideoFrame{@link #IVideoProcessor#processVideoFrame} 对 RTC SDK 采集得到的视频帧进行前处理，并将处理后的视频帧用于 RTC 音视频通信。
     * @param processor 自定义视频处理器，详见 IVideoProcessor{@link #IVideoProcessor}。如果传入 null，则不对 RTC SDK 采集得到的视频帧进行前处理。 <br>
     *        SDK 只持有 processor 的弱引用，你应保证其生命周期。
     * @param config 自定义视频前处理器适用的设置，详见 VideoPreprocessorConfig{@link #VideoPreprocessorConfig}。 <br>
     *               当前，`config` 中的 `required_pixel_format` 仅支持：`kVideoPixelFormatI420` 和 `kVideoPixelFormatUnknown`： <br>
     *               - 设置为 `kVideoPixelFormatUnknown` 时，RTC SDK 给出供 processor 处理的视频帧格式即采集的格式。
     *                 你可以通过 VideoFrameType{@link #VideoFrameType} 和 VideoPixelFormat{@link #VideoPixelFormat} 获取实际采集的视频帧格式和像素类型。 <br>
     *               - 设置为 `kVideoPixelFormatI420` 时，RTC SDK 会将采集得到的视频帧转变为对应的格式，供前处理使用。
     *               - 设置为其他值时，此方法调用失败。
     * @return
     *         - 0：方法调用成功
     *         - !0：方法调用失败
     * @note
     *        - 对于 Windows 平台，经前处理返回的视频帧格式仅支持 `kVideoPixelFormatI420`
     *        - 对于 Windows 平台，将 `config` 中的 required_pixel_format 设置为 `kVideoPixelFormatI420`，可以通过避免格式转换带来一些性能优化。
     *        - 重复调用此接口时，仅最后一次调用生效。效果不会叠加。
     * @list 自定义流处理
     * @order 10
     */
    
    virtual int registerLocalVideoProcessor(IVideoProcessor* processor, VideoPreprocessorConfig config) = 0;
    /**
     * 
     * @type api
     * @valid since 3.51
     * @brief 设置本地摄像头数码变焦参数，包括缩放倍数，移动步长。
     * @param type 数码变焦参数类型，缩放系数或移动步长。参看 ZoomConfigType{@link #ZoomConfigType}。必填。
     * @param size 缩放系数或移动步长，保留到小数点后三位。默认值为 0。必填。 <br>
     *                  选择不同 `type` 时有不同的取值范围。当计算后的结果超过缩放和移动边界时，取临界值。 <br>
     *                  - `kZoomConfigTypeFocusOffset`：缩放系数增量，范围为 [0, 7]。例如，设置为 0.5 时，如果调用 setVideoDigitalZoomControl{@link #IRTCVideo#setVideoDigitalZoomControl} 选择 Zoom in，则缩放系数增加 0.5。缩放系数范围 [1，8]，默认为 `1`，原始大小。
     *                  - `kZoomConfigTypeMoveOffset`：移动百分比，范围为 [0, 0.5]，默认为 0，不移动。如果调用 setVideoDigitalZoomControl{@link #IRTCVideo#setVideoDigitalZoomControl} 选择的是左右移动，则移动距离为 size x 原始视频宽度；如果选择的是上下移动，则移动距离为 size x 原始视频高度。例如，视频帧边长为 1080 px，设置为 0.5 时，实际移动距离为 0.5 x 1080 px = 540 px。
     * @return
     *        - 0：成功。
     *        - !0：失败。
     * @note
     *        - 每次调用本接口只能设置一种参数。如果缩放系数和移动步长都需要设置，分别调用本接口传入相应参数。
     *        - 由于移动步长的默认值为 `0` ，在调用 setVideoDigitalZoomControl{@link #IRTCVideo#setVideoDigitalZoomControl} 或 startVideoDigitalZoomControl{@link #IRTCVideo#startVideoDigitalZoomControl} 进行数码变焦操作前，应先调用本接口。
     * @list 视频处理
     * @order 22
     */
    
    virtual int setVideoDigitalZoomConfig(ZoomConfigType type, float size) = 0;

    /**
     * 
     * @type api
     * @valid since 3.51
     * @brief 控制本地摄像头数码变焦，缩放或移动一次。设置对本地预览画面和发布到远端的视频都生效。
     * @param direction 数码变焦操作类型，参看 ZoomDirectionType{@link #ZoomDirectionType}。
     * @return
     *        - 0：成功。
     *        - !0：失败。
     * @note
     *        - 由于默认步长为 `0`，调用该方法前需通过 setVideoDigitalZoomConfig{@link #IRTCVideo#setVideoDigitalZoomConfig} 设置参数。
     *        - 调用该方法进行移动前，应先使用本方法或 startVideoDigitalZoomControl{@link #IRTCVideo#startVideoDigitalZoomControl} 进行放大，否则无法移动。
     *        - 当数码变焦操作超出范围时，将置为临界值。例如，移动到了图片边界、放大到了 8 倍、缩小到原图大小。
     *        - 如果你希望实现持续数码变焦操作，调用 startVideoDigitalZoomControl{@link #IRTCVideo#startVideoDigitalZoomControl}。
     *        - 移动端可对摄像头进行光学变焦控制，参看 `setCameraZoomRatio`。
     * @list 视频处理
     * @order 23
     */
    
    virtual int setVideoDigitalZoomControl(ZoomDirectionType direction) = 0;

    /**
     * 
     * @type api
     * @valid since 3.51
     * @brief 开启本地摄像头持续数码变焦，缩放或移动。设置对本地预览画面和发布到远端的视频都生效。
     * @param direction 数码变焦操作类型，参看 ZoomDirectionType{@link #ZoomDirectionType}。
     * @return
     *        - 0：成功。
     *        - !0：失败。
     * @note
     *        - 由于默认步长为 `0`，调用该方法前需通过 setVideoDigitalZoomConfig{@link #IRTCVideo#setVideoDigitalZoomConfig} 设置参数。
     *        - 调用该方法进行移动前，应先使用本方法或 setVideoDigitalZoomControl{@link #IRTCVideo#setVideoDigitalZoomControl} 进行放大，否则无法移动。
     *        - 当数码变焦操作超出范围时，将置为临界值并停止操作。例如，移动到了图片边界、放大到了 8 倍、缩小到原图大小。
     *        - 你也可以调用 stopVideoDigitalZoomControl{@link #IRTCVideo#stopVideoDigitalZoomControl} 手动停止控制。
     *        - 如果你希望实现单次数码变焦操作，调用 setVideoDigitalZoomControl{@link #IRTCVideo#setVideoDigitalZoomControl}。
     *        - 移动端可对摄像头进行光学变焦控制，参看 `setCameraZoomRatio`。
     * @list 视频处理
     * @order 24
     */
    
    virtual int startVideoDigitalZoomControl(ZoomDirectionType direction) = 0;

    /**
     * 
     * @type api
     * @valid since 3.51
     * @brief 停止本地摄像头持续数码变焦。
     * @return
     *        - 0：成功。
     *        - !0：失败。
     * @note 关于开始数码变焦，参看 startVideoDigitalZoomControl{@link #IRTCVideo#startVideoDigitalZoomControl}。
     * @list 视频处理
     * @order 25
     */
    
    virtual int stopVideoDigitalZoomControl() = 0;

    /**
     * 
     * @hidden
     * @deprecated since 3.37 and will be deleted in 3.51, use registerAudioFrameObserver{@link #IRTCVideo#registerAudioFrameObserver} instead.
     * @type api
     * @region 音频数据回调
     * @brief 注册音频数据回调观察者。
     * @param observer 音频数据回调观察者，详见 IRemoteAudioFrameObserver{@link #IRemoteAudioFrameObserver}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 注册该回调，可以收到单个远端用户的 PCM 数据。
     */
    
    BYTERTC_DEPRECATED virtual int registerRemoteAudioFrameObserver(IRemoteAudioFrameObserver* observer) = 0;

    /**
     * 
     * @deprecated since 3.50 and will be deleted in 3.55, use [sendSEIMessage](#IRTCVideo-sendseimessage-2) instead.
     * @type api
     * @region 消息
     * @brief <span id="IRTCVideo-sendseimessage-1"></span> 通过视频帧发送 SEI 数据。 <br>
     *        在视频通话场景下，SEI 数据会随视频帧发送；在语音通话场景下，SDK 会自动生成一路 16px × 16px 的黑帧视频流用来发送 SEI 数据。
     * @param stream_index 指定携带 SEI 数据的媒体流类型，参看 StreamIndex{@link #StreamIndex}。 <br>
     *        语音通话场景下，该值需设为 `kStreamIndexMain`，否则 SEI 数据会被丢弃从而无法送达远端。
     * @param message SEI 消息。
     * @param length SEI 消息长度，不超过 4KB。
     * @param repeat_count 消息发送重复次数。取值范围是 [0, 30]。 <br>
     *                    调用此接口后，SEI 数据会添加到从当前视频帧开始的连续 `repeat_count+1` 个视频帧中。
     * @return
     *        - >=0: 将被添加到视频帧中的 SEI 的数量
     *        - <0: 发送失败
     * @note
     *        - 语音通话场景中，仅支持在内部采集模式下调用该接口发送 SEI 数据，且调用频率需为 15/repeat_count FPS。
     *        - 视频帧仅携带前后 2s 内收到的 SEI 数据；语音通话场景下，若调用此接口后 1min 内未有 SEI 数据发送，则 SDK 会自动取消发布视频黑帧。
     *        - 消息发送成功后，远端会收到 onSEIMessageReceived{@link #IRTCVideoEventHandler#onSEIMessageReceived} 回调。
     *        - 语音通话切换至视频通话时，会停止使用黑帧发送 SEI 数据，自动转为用采集到的正常视频帧发送 SEI 数据。
     * @list 消息
     * @order 14
     */
    
    BYTERTC_DEPRECATED virtual int sendSEIMessage(StreamIndex stream_index, const uint8_t* message, int length, int repeat_count) = 0;

    /**
     * 
     * @type api
     * @region 消息
     * @brief <span id="IRTCVideo-sendseimessage-2"></span> 通过视频帧发送 SEI 数据。 <br>
     *        在视频通话场景下，SEI 数据会随视频帧发送；在语音通话场景下，SDK 会自动生成一路 16px × 16px 的黑帧视频流用来发送 SEI 数据。
     * @param stream_index 指定携带 SEI 数据的媒体流类型，参看 StreamIndex{@link #StreamIndex}。 <br>
     *        语音通话场景下，该值需设为 `kStreamIndexMain`，否则 SEI 数据会被丢弃从而无法送达远端。
     * @param message SEI 消息。
     * @param length SEI 消息长度，建议每帧 SEI 数据总长度长度不超过 4 KB。
     * @param repeat_count 消息发送重复次数。取值范围是 [0, max{29, %{视频帧率}-1}]。推荐范围 [2,4]。 <br>
     *                    调用此接口后，SEI 数据会添加到从当前视频帧开始的连续 `repeat_count+1` 个视频帧中。
     * @param mode SEI 发送模式，参看 SEICountPerFrame{@link #SEICountPerFrame}。
     * @return
     *        - >= 0: 将被添加到视频帧中的 SEI 的数量。
     *        - < 0: 发送失败。
     * @note
     *        - 每秒发送的 SEI 消息数量建议不超过当前的视频帧率。在语音通话场景下，黑帧帧率为 15 fps。
     *        - 语音通话场景中，仅支持在内部采集模式下调用该接口发送 SEI 数据。
     *        - 视频通话场景中，使用自定义采集并通过 pushExternalVideoFrame{@link #IRTCVideo#pushExternalVideoFrame} 推送至 SDK 的视频帧，若本身未携带 SEI 数据，也可通过本接口发送 SEI 数据；若原视频帧中已添加了 SEI 数据，则调用此方法不生效。
     *        - 视频帧仅携带前后 2s 内收到的 SEI 数据；语音通话场景下，若调用此接口后 1min 内未有 SEI 数据发送，则 SDK 会自动取消发布视频黑帧。
     *        - 消息发送成功后，远端会收到 onSEIMessageReceived{@link #IRTCVideoEventHandler#onSEIMessageReceived} 回调。
     *        - 语音通话切换至视频通话时，会停止使用黑帧发送 SEI 数据，自动转为用采集到的正常视频帧发送 SEI 数据。
     */
    
    virtual int sendSEIMessage(StreamIndex stream_index, const uint8_t* message, int length, int repeat_count, SEICountPerFrame mode) = 0;

    /**
     * 
     * @valid since 3.56
     * @hidden for internal use only
     * @type api
     * @region 消息
     * @brief <span id="IRTCVideo-sendseimessage-2"></span> 公共流视频帧发送 SEI 数据。
     * @param stream_index 指定携带 SEI 数据的媒体流类型，参看 StreamIndex{@link #StreamIndex}。
     * @param channel_id SEI 消息传输通道，取值范围 [0 - 255]。通过此参数，你可以为不同接受方设置不同的 ChannelID，这样不同接收方可以根据回调中的 ChannelID 选择应关注的 SEI 信息。
     * @param message SEI 消息。
     * @param length SEI 消息长度，建议每帧 SEI 数据总长度长度不超过 4 KB。
     * @param repeat_count 消息发送重复次数。取值范围是 [0, max{29, %{视频帧率}-1}]。推荐范围 [2,4]。 <br>
     *                    调用此接口后，SEI 数据会添加到从当前视频帧开始的连续 `repeat_count+1` 个视频帧中。
     * @param mode SEI 发送模式，参看 SEICountPerFrame{@link #SEICountPerFrame}。
     * @return
     *        - < 0：说明调用失败
     *        - = 0：说明当前发送队列已满，无法发送
     *        - > 0: 说明调用成功，该数值为已经发送 SEI 的数量
     * @note
     *        - 每秒发送的 SEI 消息数量建议不超过当前的视频帧率
     *        - 视频通话场景中，使用自定义采集并通过 pushExternalVideoFrame{@link #IRTCVideo#pushExternalVideoFrame} 推送至 SDK 的视频帧，若本身未携带 SEI 数据，也可通过本接口发送 SEI 数据；若原视频帧中已添加了 SEI 数据，则调用此方法不生效。
     *        - 视频帧仅携带前后 2s 内收到的 SEI 数据
     *        - 消息发送成功后，远端会收到 onPublicStreamSEIMessageReceivedWithChannel{@link #IRTCVideoEventHandler#onPublicStreamSEIMessageReceivedWithChannel} 回调。
     *        - 调用失败时，本地及远端都不会收到回调。
     */
    
    virtual int sendPublicStreamSEIMessage(StreamIndex stream_index, int channel_id, const uint8_t* message, int length,
                                           int repeat_count, SEICountPerFrame mode) = 0;

    /**
     * 
     * @type api
     * @region 视频设备管理
     * @brief 创建视频设备管理实例
     * @return 视频设备管理实例，详见 IVideoDeviceManager{@link #IVideoDeviceManager}
     * @list 视频管理
     * @order 8
     */
    
    virtual IVideoDeviceManager* getVideoDeviceManager() = 0;

    /**
     * 
     * @hidden(Android,iOS)
     * @type api
     * @region 音频管理
     * @brief 设备音频管理接口创建
     * @return 音频设备管理接口
     * @list 音频管理
     * @order 32
     */
    
    virtual IAudioDeviceManager* getAudioDeviceManager() = 0;

    /**
     * 
     * @type api
     * @region 本地录制
     * @brief 录制通话过程中的音视频数据到本地的文件中。
     * @param type 流属性，指定录制主流还是屏幕流，参看 StreamIndex{@link #StreamIndex}
     * @param config 本地录制参数配置，参看 RecordingConfig{@link #RecordingConfig}
     * @param recording_type 本地录制的媒体类型，参看 RecordingType{@link #RecordingType}
     * @return
     *        - 0: 正常
     *        - -1: 参数设置异常
     *        - -2: 当前版本 SDK 不支持该特性，请联系技术支持人员
     * @note
     *        - 调用该方法后，你会收到 onRecordingStateUpdate{@link #IRTCVideoEventHandler#onRecordingStateUpdate} 回调。
     *        - 如果录制正常，系统每秒钟会通过 onRecordingProgressUpdate{@link #IRTCVideoEventHandler#onRecordingProgressUpdate} 回调通知录制进度。
     * @list 高级功能
     * @order 0
     */
    
    virtual int startFileRecording(StreamIndex type, RecordingConfig config, RecordingType recording_type) = 0;

    /**
     * 
     * @type api
     * @region 本地录制
     * @brief 停止本地录制
     * @param type 流属性，指定停止主流或者屏幕流录制，参看 StreamIndex{@link #StreamIndex}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用 startFileRecording{@link #IRTCVideo#startFileRecording} 开启本地录制后，你必须调用该方法停止录制。
     *        - 调用该方法后，你会收到 onRecordingStateUpdate{@link #IRTCVideoEventHandler#onRecordingStateUpdate} 回调提示录制结果。
     * @list 高级功能
     * @order 1
     */
    
    virtual int stopFileRecording(StreamIndex type) = 0;

    /**
     * 
     * @type api
     * @brief 开启录制语音通话，生成本地文件。 <br>
     *        在进房前后开启录制，如果未打开麦克风采集，录制任务正常进行，只是不会将数据写入生成的本地文件；只有调用 startAudioCapture{@link #IRTCVideo#startAudioCapture} 接口打开麦克风采集后，才会将录制数据写入本地文件。
     * @param config 参看 AudioRecordingConfig{@link #AudioRecordingConfig}
     * @return
     *        - 0: 正常
     *        - -2: 参数设置异常
     *        - -3: 当前版本 SDK 不支持该特性，请联系技术支持人员
     * @note
     *        - 录制包含各种音频效果。但不包含背景音乐。
     *        - 调用 stopAudioRecording{@link #IRTCVideo#stopAudioRecording} 关闭录制。
     *        - 加入房间前后均可调用。在进房前调用该方法，退房之后，录制任务不会自动停止，需调用 stopAudioRecording{@link #IRTCVideo#stopAudioRecording} 关闭录制。在进房后调用该方法，退房之后，录制任务会自动被停止。如果加入了多个房间，录制的文件中会包含各个房间的音频。
     *        - 调用该方法后，你会收到 onAudioRecordingStateUpdate{@link #IRTCVideoEventHandler#onAudioRecordingStateUpdate} 回调。
     * @list 高级功能
     * @order 2
     */
    
    virtual int startAudioRecording(AudioRecordingConfig& config) = 0;

    /**
     * 
     * @type api
     * @brief 停止音频文件录制
     * @return
     *         - 0: 正常
     *         - -3: 当前版本 SDK 不支持该特性，请联系技术支持人员
     * @note 调用 startAudioRecording{@link #IRTCVideo#startAudioRecording} 开启本地录制。
     * @list 高级功能
     * @order 3
     */
     
    virtual int stopAudioRecording() = 0;

    /**
     * 
     * @type api
     * @brief 启用匹配外置声卡的音频处理模式
     * @param enable <br>
     *        - true: 开启
     *        - false: 不开启(默认)
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 当采用外接声卡进行音频采集时，建议开启此模式，以获得更好的音质。
     *        - 开启此模式时，仅支持耳机播放。如果需要使用扬声器或者外置音箱播放，关闭此模式。
     * @list 音频管理
     * @order 36
     */
    
    virtual int enableExternalSoundCard(bool enable) = 0;

    /**
     * 
     * @type api
     * @region 引擎管理
     * @brief 设置运行时的参数
     * @param json_string  json 序列化之后的字符串
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 该接口需在 joinRoom{@link #IRTCRoom#joinRoom} 和 startAudioCapture{@link #IRTCVideo#startAudioCapture} 之前调用。
     * @list 引擎管理
     * @order 25
     */
    
    virtual int setRuntimeParameters(const char * json_string) = 0;

    /**
     * 
     * @hidden(Windows,Linux,macOS)
     * @type api
     * @region 语音识别服务
     * @brief 开启自动语音识别服务。 <br>
     *        该方法将识别后的用户语音转化成文字，并通过 onMessage{@link #IRTCASREngineEventHandler#onMessage} 事件回调给用户。
     * @param asr_config 校验信息，参看 RTCASRConfig{@link #RTCASRConfig}
     * @param handler 语音识别服务使用状态回调，参看 IRTCASREngineEventHandler{@link #IRTCASREngineEventHandler}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    
    virtual int startASR(const RTCASRConfig& asr_config, IRTCASREngineEventHandler* handler) = 0;

    /**
     * 
     * @hidden(Windows,Linux,macOS)
     * @type api
     * @region 语音识别服务
     * @brief 关闭语音识别服务
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    
    virtual int stopASR() = 0;
    /**
     * 
     * @type api
     * @region 引擎管理
     * @brief 将用户反馈的问题上报到 RTC。
     * @param type 预设问题列表，参看 ProblemFeedbackOption{@link #ProblemFeedbackOption}
     * @param info 预设问题以外的其他问题的具体描述，房间信息。 参看 ProblemFeedbackInfo{@link #ProblemFeedbackInfo}
     * @return
     *         - 0: 上报成功
     *         - -1: 上报失败，还没加入过房间
     *         - -2: 上报失败，数据解析错误
     *         - -3: 上报失败，字段缺失
     * @note
     *         - 你可以在 [RTC 控制台](https:
     *         - 如果用户上报时在房间内，那么问题会定位到用户当前所在的一个或多个房间；如果用户上报时不在房间内，那么问题会定位到引擎此前退出的房间。
     * @list 引擎管理
     * @order 3
     */
    
    virtual int feedback(uint64_t type, const ProblemFeedbackInfo* info) = 0;
    /**
     * 
     * @type api
     * @deprecated since 353.1, will be deleted in 359, use getAudioEffectPlayer{@link #IRTCVideo#getAudioEffectPlayer} or getMediaPlayer{@link #IRTCVideo#getMediaPlayer} instead
     * @region 混音
     * @brief 混音管理接口创建
     * @return 混音管理实例，详见 IAudioMixingManager{@link #IAudioMixingManager}
     */
    
    virtual IAudioMixingManager* getAudioMixingManager() = 0;
    /**
     * 
     * @type api
     * @valid since 3.53
     * @brief 创建音效播放器实例。
     * @return 音效播放器。详见 IAudioEffectPlayer{@link #IAudioEffectPlayer}。
     * @list 混音
     * @order 0
     */
    
    virtual IAudioEffectPlayer* getAudioEffectPlayer() = 0;
    /**
     * 
     * @type api
     * @valid since 3.53
     * @brief 创建音乐播放器实例。
     * @param player_id 音乐播放器实例 id。取值范围为 `[0, 3]`。最多同时存在 4 个实例，超出取值范围时返回 nullptr。
     * @return 音乐播放器实例，详见 IMediaPlayer{@link #IMediaPlayer}
     * @list 混音
     * @order 17
     */
    
    virtual IMediaPlayer* getMediaPlayer(int player_id) = 0;
    
    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 登陆 RTS 服务器。 <br>
     *        必须先登录，才能调用 sendUserMessageOutsideRoom{@link #IRTCVideo#sendUserMessageOutsideRoom} 和 sendServerMessage{@link #IRTCVideo#sendServerMessage} 发送房间外点对点消息和向应用服务器发送消息 <br>
     *        在调用本接口登录后，如果想要登出，需要调用 logout{@link #IRTCVideo#logout}。
     * @param token 用户登录必须携带的 Token，用于鉴权验证。 <br>
     *               测试时可使用[控制台](https:
     *               正式上线需要使用密钥 SDK 在你的服务端生成并下发 Token，`roomId` 置空，Token 有效期及生成方式参看[使用 Token 完成鉴权](70121)。
     * @param uid <br>
     *        用户 ID <br>
     *        用户 ID 在 appid 的维度下是唯一的。
     * @return
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note 本地用户调用此方法登录后，会收到 onLoginResult{@link #IRTCVideoEventHandler#onLoginResult} 回调通知登录结果，远端用户不会收到通知。
     * @list 消息
     * @order 2
     */
    
    virtual int login(const char* token, const char* uid) = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 登出 RTS 服务器。 <br>
     *        调用本接口登出后，无法调用房间外消息以及端到服务器消息相关的方法或收到相关回调。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 调用本接口登出前，必须先调用 login{@link #IRTCVideo#login} 登录。
     *       - 本地用户调用此方法登出后，会收到 onLogout{@link #IRTCVideoEventHandler#onLogout} 回调通知结果，远端用户不会收到通知。
     * @list 消息
     * @order 3
     */
    
    virtual int logout() = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 更新用户用于登录的 Token <br>
     *        Token 有一定的有效期，当 Token 过期时，需调用此方法更新登录的 Token 信息。 <br>
     *        调用 login{@link #IRTCVideo#login} 方法登录时，如果使用了过期的 Token 将导致登录失败，并会收到 onLoginResult{@link #IRTCVideoEventHandler#onLoginResult} 回调通知，错误码为 kLoginErrorCodeInvalidToken。此时需要重新获取 Token，并调用此方法更新 Token。
     * @param token <br>
     *        更新的动态密钥
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 如果 Token 无效导致登录失败，则调用此方法更新 Token 后，SDK 会自动重新登录，而用户不需要自己调用 login{@link #IRTCVideo#login} 方法。
     *       - Token 过期时，如果已经成功登录，则不会受到影响。Token 过期的错误会在下一次使用过期 Token 登录时，或因本地网络状况不佳导致断网重新登录时通知给用户。
     * @list 消息
     * @order 4
     */
    
    virtual int updateLoginToken(const char* token) = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 设置应用服务器参数 <br>
     *        客户端调用 sendServerMessage{@link #IRTCVideo#sendServerMessage} 或 sendServerBinaryMessage{@link #IRTCVideo#sendServerBinaryMessage} 发送消息给应用服务器之前，必须设置有效签名和应用服务器地址。
     * @param signature 动态签名，应用服务器可使用该签名验证消息来源。 <br>
     *        签名需自行定义，可传入任意非空字符串，建议将 uid 等信息编码为签名。 <br>
     *        设置的签名会以 post 形式发送至通过本方法中 url 参数设置的应用服务器地址。
     * @param url 应用服务器的地址
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 用户必须调用 login{@link #IRTCVideo#login} 登录后，才能调用本接口。
     *       - 调用本接口后，SDK 会使用 onServerParamsSetResult{@link #IRTCVideoEventHandler#onServerParamsSetResult} 返回相应结果。
     * @list 消息
     * @order 5
     */
    
    virtual int setServerParams(const char* signature, const char* url) = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 查询对端用户或本端用户的登录状态
     * @param peer_user_id <br>
     *        需要查询的用户 ID
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 必须调用 login{@link #IRTCVideo#login} 登录后，才能调用本接口。
     *       - 调用本接口后，SDK 会使用 onGetPeerOnlineStatus{@link #IRTCVideoEventHandler#onGetPeerOnlineStatus} 回调通知查询结果。
     *       - 在发送房间外消息之前，用户可以通过本接口了解对端用户是否登录，从而决定是否发送消息。也可以通过本接口查询自己查看自己的登录状态。
     * @list 消息
     * @order 6
     */
    
    virtual int getPeerOnlineStatus(const char* peer_user_id) = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 给房间外指定的用户发送文本消息（P2P）
     * @param uid <br>
     *        消息接收用户的 ID
     * @param message <br>
     *        发送的文本消息内容 <br>
     *        消息不超过 64 KB。
     * @param config 消息类型，参看 MessageConfig{@link #MessageConfig}。
     * @return
     *        - >0：发送成功，返回这次发送消息的编号，从 1 开始递增
     *        - -1：发送失败，IRTCVideo 实例未创建
     *        - -2：发送失败，uid 为空
     * @note
     *       - 在发送房间外文本消息前，必须先调用 login{@link #IRTCVideo#login} 完成登录。
     *       - 用户调用本接口发送文本信息后，会收到一次 onUserMessageSendResultOutsideRoom{@link #IRTCVideoEventHandler#onUserMessageSendResultOutsideRoom} 回调，得知消息是否成功发送。
     *       - 若文本消息发送成功，则 uid 所指定的用户会通过 onUserMessageReceivedOutsideRoom{@link #IRTCVideoEventHandler#onUserMessageReceivedOutsideRoom} 回调收到该消息。
     * @list 消息
     * @order 9
     */
    
    virtual int64_t sendUserMessageOutsideRoom(const char* uid, const char* message, MessageConfig config = kMessageConfigReliableOrdered) = 0;

    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 给房间外指定的用户发送二进制消息（P2P）
     * @param uid <br>
     *        消息接收用户的 ID
     * @param length <br>
     *        二进制字符串的长度
     * @param message <br>
     *        发送的二进制消息内容 <br>
     *        消息不超过 46KB。
     * @param config 消息类型，参看 MessageConfig{@link #MessageConfig}。
     * @return
     *        - >0：发送成功，返回这次发送消息的编号，从 1 开始递增
     *        - -1：发送失败，IRTCVideo 实例未创建
     *        - -2：发送失败，uid 为空
     * @note
     *       - 在发送房间外二进制消息前，必须先调用 login{@link #IRTCVideo#login} 完成登录。
     *       - 用户调用本接口发送二进制消息后，会收到一次 onUserMessageSendResultOutsideRoom{@link #IRTCVideoEventHandler#onUserMessageSendResultOutsideRoom} 回调，通知消息是否发送成功。
     *       - 若二进制消息发送成功，则 uid 所指定的用户会通过 onUserBinaryMessageReceivedOutsideRoom{@link #IRTCVideoEventHandler#onUserBinaryMessageReceivedOutsideRoom} 回调收到该条消息。
     * @list 消息
     * @order 10
     */
    
    virtual int64_t sendUserBinaryMessageOutsideRoom(const char* uid, int length, const uint8_t* message, MessageConfig config = kMessageConfigReliableOrdered) = 0;
    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 客户端给应用服务器发送文本消息（P2Server）
     * @param message <br>
     *        发送的文本消息内容 <br>
     *        消息不超过 64 KB。
     * @return
     *        - >0：发送成功，返回这次发送消息的编号，从 1 开始递增
     *        - -1：发送失败，IRTCVideo 实例未创建
     * @note
     *       - 在向应用服务器发送文本消息前，必须先调用 login{@link #IRTCVideo#login} 完成登录，随后调用 setServerParams{@link #IRTCVideo#setServerParams} 设置应用服务器。
     *       - 调用本接口后，会收到一次 onServerMessageSendResult{@link #IRTCVideoEventHandler#onServerMessageSendResult} 回调，通知消息发送方是否发送成功。
     *       - 若文本消息发送成功，则之前调用 setServerParams{@link #IRTCVideo#setServerParams} 设置的应用服务器会收到该条消息。
     * @list 消息
     * @order 11
     */
    
    virtual int64_t sendServerMessage(const char* message) = 0;
    /**
     * 
     * @type api
     * @region 实时消息通信
     * @brief 客户端给应用服务器发送二进制消息（P2Server）
     * @param length <br>
     *        二进制字符串的长度
     * @param message <br>
     *        发送的二进制消息内容 <br>
     *        消息不超过 46KB。
     * @return
     *        - >0：发送成功，返回这次发送消息的编号，从 1 开始递增
     *        - -1：发送失败，IRTCVideo 实例未创建
     * @note
     *       - 在向应用服务器发送二进制消息前，必须先调用 login{@link #IRTCVideo#login} 完成登录，随后调用 setServerParams{@link #IRTCVideo#setServerParams} 设置应用服务器。
     *       - 调用本接口后，会收到一次 onServerMessageSendResult{@link #IRTCVideoEventHandler#onServerMessageSendResult} 回调，通知消息发送方发送成功或失败。
     *       - 若二进制消息发送成功，则之前调用 setServerParams{@link #IRTCVideo#setServerParams} 设置的应用服务器会收到该条消息。
     * @list 消息
     * @order 12
     */
    
    virtual int64_t sendServerBinaryMessage(int length, const uint8_t* message) = 0;

    /**
     * 
     * @type api
     * @region 通话前网络探测
     * @brief 开启通话前网络探测
     * @param is_test_uplink 是否探测上行带宽
     * @param expected_uplink_bitrate 期望上行带宽，单位：kbps<br>范围为 {0, [100-10000]}，其中， `0` 表示由 SDK 指定最高码率。
     * @param is_test_downlink 是否探测下行带宽
     * @param expected_downlink_biterate 期望下行带宽，单位：kbps<br>范围为 {0, [100-10000]}，其中， `0` 表示由 SDK 指定最高码率。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 成功调用本接口后，会在 3s 内收到一次 onNetworkDetectionResult{@link #IRTCVideoEventHandler#onNetworkDetectionResult} 回调，此后每 2s 会收到一次该回调，通知探测结果；
     *       - 若探测停止，则会收到一次 onNetworkDetectionStopped{@link #IRTCVideoEventHandler#onNetworkDetectionStopped} 通知探测停止。
     * @list 网络管理
     * @order 5
     */
    
    virtual int startNetworkDetection(bool is_test_uplink, int expected_uplink_bitrate, bool is_test_downlink, int expected_downlink_biterate) = 0;

    /**
     * 
     * @type api
     * @region 通话前网络探测
     * @brief 停止通话前网络探测
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 调用本接口后，会收到一次 onNetworkDetectionStopped{@link #IRTCVideoEventHandler#onNetworkDetectionStopped} 回调通知探测停止。
     * @list 网络管理
     * @order 6
     */
    
    virtual int stopNetworkDetection() = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 屏幕共享
     * @brief 在屏幕共享时，设置屏幕音频的采集方式（内部采集/自定义采集）
     * @param source_type 屏幕音频输入源类型, 参看 AudioSourceType{@link #AudioSourceType}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *      - 默认采集方式是 RTC SDK 内部采集。
     *      - 你应该在 publishScreen{@link #IRTCRoom#publishScreen} 前，调用此方法。否则，你将收到 onWarning{@link #IRTCVideoEventHandler#onWarning} 的报错：`kWarningCodeSetScreenAudioSourceTypeFailed`
     *      - 如果设定为内部采集，你必须再调用 startScreenAudioCapture{@link #IRTCVideo#startScreenAudioCapture} 开始采集；
     *      - 如果设定为自定义采集，你必须再调用 pushScreenAudioFrame{@link #IRTCVideo#pushScreenAudioFrame} 将自定义采集到的屏幕音频帧推送到 RTC SDK。
     *      - 无论是内部采集还是自定义采集，你都必须调用 publishScreen{@link #IRTCRoom#publishScreen} 将采集到的屏幕音频推送到远端。
     * @list 屏幕分享
     * @order 11
     */
    
    virtual int setScreenAudioSourceType(AudioSourceType source_type) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 屏幕共享
     * @brief 在屏幕共享时，设置屏幕音频流和麦克风采集到的音频流的混流方式
     * @param index 混流方式，参看 StreamIndex{@link #StreamIndex} <br>
     *        - `kStreamIndexMain`: 将屏幕音频流和麦克风采集到的音频流混流
     *        - `kStreamIndexScreen`: 默认值， 将屏幕音频流和麦克风采集到的音频流分为两路音频流
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 你应该在 publishScreen{@link #IRTCRoom#publishScreen} 之前，调用此方法。否则，你将收到 onWarning{@link #IRTCVideoEventHandler#onWarning} `的报错：kWarningCodeSetScreenAudioStreamIndexFailed`
     * @list 屏幕分享
     * @order 12
     */
    
    virtual int setScreenAudioStreamIndex(StreamIndex index) = 0;

    /**
     * 
     * @hidden(iOS,Android,Linux)
     * @type api
     * @region 屏幕共享
     * @brief 在屏幕共享时，设置屏幕音频流的声道数
     * @param channel 声道数，参看 AudioChannel{@link #AudioChannel}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 当你调用 setScreenAudioStreamIndex{@link #IRTCVideo#setScreenAudioStreamIndex} 并设置屏幕音频流和麦克风音频流混流时，此接口不生效，音频通道数由 setAudioProfile{@link #IRTCVideo#setAudioProfile} 控制。
     * @list 屏幕分享
     * @order 13
     */
    
    virtual int setScreenAudioChannel(AudioChannel channel) = 0;

    /**
     * 
     * @hidden(Android,iOS,macOS)
     * @type api
     * @region 屏幕共享
     * @brief 在屏幕共享时，开始使用 RTC SDK 内部采集方式，采集屏幕音频
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用本接口时，采集模式应为内部模式。在外部采集模式下调用无效，并将触发 onAudioDeviceWarning{@link #IRTCVideoEventHandler#onAudioDeviceWarning} 回调。
     *        - 采集后，你还需要调用 publishScreen{@link #IRTCRoom#publishScreen} 将采集到的屏幕音频推送到远端。
     *        - 要关闭屏幕音频内部采集，调用 stopScreenAudioCapture{@link #IRTCVideo#stopScreenAudioCapture}。
     * @list 音频管理
     * @order 29
     */
    
    virtual int startScreenAudioCapture() = 0;

    /**
     * 
     * @hidden(Windows,Linux,Android,iOS)
     * @type api
     * @region 屏幕共享
     * @brief 在屏幕共享时，开始使用 RTC SDK 内部采集方式，采集屏幕音频
     * @param device_id 虚拟设备 ID
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用本接口时，采集模式应为内部模式。在外部采集模式下调用无效，并将触发 onAudioDeviceWarning{@link #IRTCVideoEventHandler#onAudioDeviceWarning} 回调。
     *        - 采集后，你还需要调用 publishScreen{@link #IRTCRoom#publishScreen} 将采集到的屏幕音频推送到远端。
     *        - 要关闭屏幕音频内部采集，调用 stopScreenAudioCapture{@link #IRTCVideo#stopScreenAudioCapture}。
     */
    
    virtual int startScreenAudioCapture(const char device_id[MAX_DEVICE_ID_LENGTH]) = 0;

    /**
     * 
     * @hidden(Android,iOS)
     * @type api
     * @region 屏幕共享
     * @brief 在屏幕共享时，停止使用 RTC SDK 内部采集方式，采集屏幕音频。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用本接口时，采集模式应为内部模式。在外部采集模式下调用无效，并将触发 onAudioDeviceWarning{@link #IRTCVideoEventHandler#onAudioDeviceWarning} 回调。
     *        - 本接口仅对内部采集生效。如果为外部采集模式，调用本接口失败。你需要在外部采集器中实现停止采集的逻辑。
     *        - 要开始屏幕音频内部采集，调用 startScreenAudioCapture{@link #IRTCVideo#startScreenAudioCapture}。
     * @list 音频管理
     * @order 30
     */
    
    virtual int stopScreenAudioCapture() = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 屏幕共享
     * @brief 使用自定义采集方式，采集屏幕共享时的屏幕音频时，将音频帧推送至 RTC SDK 处进行编码等处理。
     * @param frame 音频数据帧，参见 IAudioFrame{@link #IAudioFrame} <br>
     *                   - 音频采样格式为 S16。音频缓冲区内的数据格式必须为 PCM 数据，其容量大小应该为 samples × frame.channel × 2。
     *                   - 必须指定具体的采样率和声道数，不支持设置为自动。
     * @return 方法调用结果 <br>
     *        - 0: 设置成功
     *        - < 0: 设置失败
     * @note
     *        - 调用此接口推送屏幕共享时的自定义采集的音频数据前，必须调用 setScreenAudioSourceType{@link #IRTCVideo#setScreenAudioSourceType} 开启屏幕音频自定义采集。
     *        - 你应每隔 10 毫秒，调用一次此方法推送一次自定义采集的音频帧。一次推送的音频帧中应包含 frame.sample_rate / 100 个音频采样点。比如，假如采样率为 48000Hz，则每次应该推送 480 个采样点。
     *        - 调用此接口将自定义采集的音频帧推送到 RTC SDK 后，你必须调用 publishScreen{@link #IRTCRoom#publishScreen} 将采集到的屏幕音频推送到远端。在调用 publishScreen{@link #IRTCRoom#publishScreen} 前，推送到 RTC SDK 的音频帧信息会丢失。
     * @list 屏幕分享
     * @order 14
     */
    
    virtual int pushScreenAudioFrame(IAudioFrame* frame) = 0;

    /**
     * 
     * @type api
     * @hidden internal use only
     * @brief 在听众端，设置订阅的所有远端音频流精准对齐后播放。
     * @param stream_key 作为对齐基准的远端音频流。参看 RemoteStreamKey{@link #RemoteStreamKey}。 <br>
     *                  一般选择主唱的音频流。 <br>
     *                  你必须在收到 onUserPublishStream{@link #IRTCRoomEventHandler#onUserPublishStream}，确认此音频流已发布后，调用此 API。
     * @param mode 是否对齐，默认不对齐。参看 AudioAlignmentMode{@link #AudioAlignmentMode}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 你必须在实时合唱场景下使用此功能。在加入房间时，所有人应设置 RoomProfileType{@link #RoomProfileType} 为 `kRoomProfileTypeChorus`。
     *        - 订阅的所有远端流必须通过 startAudioMixing{@link #IAudioMixingManager#startAudioMixing} 开启了背景音乐混音，并将 AudioMixingConfig{@link #AudioMixingConfig} 中的 `sync_progress_to_record_frame` 设置为 `true`。
     *        - 如果订阅的某个音频流延迟过大，可能无法实现精准对齐。
     *        - 合唱的参与者不应调用此 API，因为调用此 API 会增加延迟。如果希望从听众变为合唱参与者，应关闭对齐功能。
     * @list 在线 KTV
     * @order 34
     */
    
    virtual int setAudioAlignmentProperty(const RemoteStreamKey& stream_key, AudioAlignmentMode mode) = 0;

    /**
     * 
     * @hidden(macOS,Windows,Android,Linux)
     * @type api
     * @region 屏幕共享
     * @brief 设置 Extension 配置项
     * @param group_id App 和 Extension 应该归属于同一个 App Group，此处需要传入 Group Id
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 该函数必须在 sharedEngineWithAppId 函数之后立即调用。如果不调用或者调用时机比较晚，本地用户会收到 onMediaDeviceStateChanged{@link #IRTCVideoEventHandler#onMediaDeviceStateChanged} 的回调。 <br>
     *        参数 device_state 值为 ByteRTCMediaDeviceStateStopped，device_error 值为 `ByteRTCMediaDeviceErrorNotFindGroupId`
     */
    
    virtual int setExtensionConfig(const char* group_id) = 0;

    /**
     * 
     * @hidden(macOS,Windows,Android,Linux)
     * @type api
     * @region 屏幕共享
     * @brief 发送屏幕共享 Extension 程序消息
     * @param message :发送给 Extension 程序的消息内容
     * @param size :message 大小
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 该函数必须在 startScreenCapture 函数之后调用
     */
    
    virtual int sendScreenCaptureExtensionMessage(const char* message, size_t size) = 0;

    /**
     * 
     * @hidden(macOS,Windows,Android,Linux)
     * @type api
     * @region 屏幕共享
     * @brief 开启本地屏幕共享数据采集
     * @param type 屏幕共享数据采集类型
     * @param bundle_id 传入 Broadcast Upload Extension 的 Bundle Id，用于在设置中优先展示 Extension
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 当从 ios 控制中心异常启动 Extension 时可以不用调用本函数 <br>
     *       本地用户会收到 onMediaDeviceStateChanged{@link #IRTCVideoEventHandler#onMediaDeviceStateChanged} 的回调。 <br>
     *       参数 device_state 值为 ByteRTCMediaDeviceStateStarted，device_error 值为 ByteRTCMediaDeviceErrorOK
     */
    
    virtual int startScreenCapture(ScreenMediaType type, const char* bundle_id) = 0;

    /**
     * 
     * @hidden(macOS,Windows,iOS,Linux)
     * @type api
     * @region 屏幕共享
     * @brief 通过传入的 Type 和 Context 开启屏幕音视频内部采集。
     * @param type <br>
     *        指定的屏幕媒体采集类型，参看 ScreenMediaType{@link #ScreenMediaType}
     * @param context <br>
     *        Android 平台传入 Intent 对象，由业务方申请系统录屏权限后获得。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 采集后，你还需要调用 publishScreen {@link #IRtcRoom#publishScreen} 将采集到的屏幕音视频推送到远端。
     *        - 开启屏幕音视频内部采集，Android 专用接口。
     */
    
    virtual int startScreenCapture(ScreenMediaType type, void* context) = 0;

    /**
     * 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region 屏幕共享
     * @brief 停止本地屏幕共享数据采集
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    
    virtual int stopScreenCapture() = 0;

    /**
     * 
     * @deprecated since 3.52, will be deleted in 3.58, using startPushMixedStreamToCDN{@link #IRTCVideo#startPushMixedStreamToCDN} instead.
     * @hidden(Linux) not available
     * @type api
     * @region 多房间
     * @brief 新增转推直播任务，并设置合流的图片、视频视图布局和音频属性。 <br>
     *        同一个任务中转推多路直播流时，SDK 会先将多路流合成一路流，然后再进行转推。
     * @param task_id 转推直播任务 ID，长度不超过 126 字节。 <br>
     *               你可以在同一房间内发起多个转推直播任务，并用不同的任务 ID 加以区分。当你需要发起多个转推直播任务时，应使用多个 ID；当你仅需发起一个转推直播任务时，建议使用空字符串。
     * @param param 转推直播配置参数。参看 ITranscoderParam{@link #ITranscoderParam}。
     * @param observer 端云一体转推直播观察者。参看 ITranscoderObserver{@link #ITranscoderObserver}。 <br>
     *        通过注册 observer 接收转推直播相关的回调。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 在调用该接口前，你需要在[控制台](https:
     *       - 调用该方法后，关于启动结果和推流过程中的错误，会收到 onStreamMixingEvent{@link #ITranscoderObserver#onStreamMixingEvent} 回调。
     *       - 调用 stopLiveTranscoding{@link #IRTCVideo#stopLiveTranscoding} 停止转推直播。
     */
    
    BYTERTC_DEPRECATED virtual int startLiveTranscoding(const char* task_id, ITranscoderParam* param, ITranscoderObserver* observer) = 0;

    /**
     * 
     * @deprecated since 3.52, will be deleted in 3.58, using stopPushStreamToCDN{@link #IRTCVideo#stopPushStreamToCDN} instead.
     * @hidden(Linux) not available
     * @type api
     * @region 多房间
     * @brief 停止转推直播，会收到 onStreamMixingEvent{@link #ITranscoderObserver#onStreamMixingEvent} 回调。 <br>
     *        关于启动转推直播，参看 startLiveTranscoding{@link #IRTCVideo#startLiveTranscoding}。
     * @param task_id 转推直播任务 ID。可以指定想要停止的转推直播任务。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    
    BYTERTC_DEPRECATED virtual int stopLiveTranscoding(const char* task_id) = 0;

    /**
     * 
     * @deprecated since 3.52, will be deleted in 3.58, using updatePushMixedStreamToCDN{@link #IRTCVideo#updatePushMixedStreamToCDN} instead.
     * @hidden(Linux) not available
     * @type api
     * @region 多房间
     * @brief 更新转推直播参数，会收到 onStreamMixingEvent{@link #ITranscoderObserver#onStreamMixingEvent} 回调。 <br>
     *        使用 startLiveTranscoding{@link #IRTCVideo#startLiveTranscoding} 启用转推直播功能后，使用此方法更新功能配置参数。
     * @param task_id 转推直播任务 ID。指定想要更新参数设置的转推直播任务。
     * @param param 转推直播配置参数，参看 ITranscoderParam{@link #ITranscoderParam}。除特殊说明外，均支持过程中更新。 <br>
     *                   调用时，结构体中没有传入值的属性，会被更新为默认值。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    
    BYTERTC_DEPRECATED virtual int updateLiveTranscoding(const char* task_id, ITranscoderParam* param) = 0;

    /**
     * 
     * @valid since 3.52
     * @hidden(Linux) not available
     * @type api
     * @region 多房间
     * @brief 新增合流转推直播任务，并设置合流的图片、视频视图布局和音频属性。 <br>
     *        同一个任务中转推多路直播流时，SDK 会先将多路流合成一路流，然后再进行转推。
     * @param task_id 转推直播任务 ID，长度不超过 126 字节。 <br>
     *        你可以在同一房间内发起多个转推直播任务，并用不同的任务 ID 加以区分。当你需要发起多个转推直播任务时，应使用多个 ID；当你仅需发起一个转推直播任务时，建议使用空字符串。
     * @param config 转推直播配置参数。参看 IMixedStreamConfig{@link #IMixedStreamConfig}。
     * @param observer 无效参数。请忽略。
     * @return
     *        - 0: 成功
     *        - !0: 失败
     * @note
     *       - 在调用该接口前，你需要在[控制台](https:
     *       - 调用该方法后，关于启动结果和推流过程中的错误，会收到 onMixingEvent{@link #IMixedStreamObserver#onMixingEvent} 回调。
     *       - 如果已在[控制台](https:
     *       - 调用 stopPushStreamToCDN{@link #IRTCVideo#stopPushStreamToCDN} 停止转推直播。
     * @list CDN 推流
     * @order 0
     */
    
    virtual int startPushMixedStreamToCDN(const char* task_id, IMixedStreamConfig* config, IMixedStreamObserver* observer) = 0;
    /**
     * 
     * @valid since 3.52
     * @hidden(Linux) not available
     * @type api
     * @region 多房间
     * @brief 更新合流转推直播参数，会收到 onMixingEvent{@link #IMixedStreamObserver#onMixingEvent} 回调。 <br>
     *        使用 startPushMixedStreamToCDN{@link #IRTCVideo#startPushMixedStreamToCDN} 启用转推直播功能后，使用此方法更新功能配置参数。
     * @param task_id 转推直播任务 ID。指定想要更新参数设置的转推直播任务。
     * @param config 转推直播配置参数，参看 IMixedStreamConfig{@link #IMixedStreamConfig}。除特殊说明外，均支持过程中更新。 <br>
     *        调用时，结构体中没有传入值的属性，会被更新为默认值。
     * @return
     *        - 0: 成功
     *        - !0: 失败
     * @list CDN 推流
     * @order 1
     */
    
    virtual int updatePushMixedStreamToCDN(const char* task_id, IMixedStreamConfig* config) = 0;
    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 转推直播
     * @brief 新增单流转推直播任务。
     * @param task_id 任务 ID。 <br>
     *               你可以发起多个转推直播任务，并用不同的任务 ID 加以区分。当你需要发起多个转推直播任务时，应使用多个 ID；当你仅需发起一个转推直播任务时，建议使用空字符串。
     * @param param 转推直播配置参数。详见 PushSingleStreamParam{@link #PushSingleStreamParam}。
     * @param observer 单流转推直播观察者。详见 IPushSingleStreamToCDNObserver{@link #IPushSingleStreamToCDNObserver}。 <br>
     *        通过注册 observer 接收单流转推直播相关的回调。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 在调用该接口前，你需要在[控制台](https:
     *       - 调用该方法后，关于启动结果和推流过程中的错误，会收到 onStreamPushEvent{@link #IPushSingleStreamToCDNObserver#onStreamPushEvent} 回调。
     *       - 调用 stopPushStreamToCDN{@link #IRTCVideo#stopPushStreamToCDN} 停止任务。
     *       - 由于本功能不进行编解码，所以推到 RTMP 的视频流会根据推流端的分辨率、编码方式、关闭摄像头等变化而变化。
     * @list CDN 推流
     * @order 2
     */
    
    virtual int startPushSingleStreamToCDN(const char* task_id, PushSingleStreamParam& param, IPushSingleStreamToCDNObserver* observer) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 转推直播
     * @brief 停止转推直播。 <br>
     *        该方法可用于停止单流转推直播或停止合流转推直播，通过 taskId 区分需要停止的任务。
     * @param task_id 任务 ID。可以指定想要停止的单流转推直播或合流转推直播任务。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 关于启动单流转推直播，参看 startPushSingleStreamToCDN{@link #IRTCVideo#startPushSingleStreamToCDN}。
     *        - 关于启动合流转推直播，参看 startPushMixedStreamToCDN{@link #IRTCVideo#startPushMixedStreamToCDN}。
     * @list CDN 推流
     * @order 3
     */
    
    virtual int stopPushStreamToCDN(const char* task_id) = 0;

    /**
      * 
      * @hidden internal use only
      * @type api
      * @brief 开启缓存同步功能。开启后，会缓存收到的实时音视频数据，并对齐不同数据中的时间戳完成同步。此功能会影响音视频数据消费的实时性。
      * @param config 参看 ChorusCacheSyncConfig{@link #ChorusCacheSyncConfig}。
      * @param observer 事件和数据观察者，参看 IChorusCacheSyncObserver{@link #IChorusCacheSyncObserver}。
      * @return 查看 ReturnStatus{@link #ReturnStatus}。
      * @note 要关闭缓存同步功能，调用 stopChorusCacheSync{@link #IRTCVideo#stopChorusCacheSync}。
      */
     
    virtual int startChorusCacheSync(ChorusCacheSyncConfig * config, IChorusCacheSyncObserver* observer) = 0;
    /**
      * 
      * @hidden internal use only
      * @type api
      * @brief 关闭缓存同步功能。
      * @return 查看 ReturnStatus{@link #ReturnStatus}。
      */
     
    virtual int stopChorusCacheSync() = 0;
  
    /**
     * 
     * @type api
     * @brief 发布一路公共流 <br>
     *        用户可以指定房间内多个用户发布的媒体流合成一路公共流。使用同一 `appID` 的用户，可以调用 startPlayPublicStream{@link #IRTCVideo#startPlayPublicStream} 获取和播放指定的公共流。
     * @param public_stream_id 公共流 ID
     * @param param 公共流参数。详见 IPublicStreamParam{@link #IPublicStreamParam}。 <br>
     *              一路公共流可以包含多路房间内的媒体流，按照指定的布局方式进行聚合。 <br>
     *              如果指定的媒体流还未发布，则公共流将在指定流开始发布后实时更新。
     * @return
     *        - 0: 成功。同时将收到 onPushPublicStreamResult{@link #IRTCVideoEventHandler#onPushPublicStreamResult} 回调。
     *        - !0: 失败。当参数不合法或参数为空，调用失败。
     * @note
     *        - 同一用户使用同一公共流 ID 多次调用本接口无效。如果你希望更新公共流参数，调用 updatePublicStreamParam{@link #IRTCVideo#updatePublicStreamParam} 接口。
     *        - 不同用户使用同一公共流 ID 多次调用本接口时，RTC 将使用最后一次调用时传入的参数更新公共流。
     *        - 使用不同的 ID 多次调用本接口可以发布多路公共流。
     *        - 调用 stopPushPublicStream{@link #IRTCVideo#stopPushPublicStream} 停止发布公共流。
     *        - 关于公共流功能的介绍，详见[发布和订阅公共流](https:
     * @list 音视频传输
     * @order 16
     */
    
    virtual int startPushPublicStream(const char* public_stream_id, IPublicStreamParam* param) = 0;
    /**
     * 
     * @type api
     * @brief 停止发布当前用户发布的公共流 <br>
     *        关于发布公共流，查看 startPushPublicStream{@link #IRTCVideo#startPushPublicStream}。
     * @param public_stream_id 公共流 ID <br>
     *                  指定的流必须为当前用户所发布。
     * @return
     *        - 0: 成功
     *        - !0: 失败
     * @list 音视频传输
     * @order 17
     */
    
    virtual int stopPushPublicStream(const char* public_stream_id) = 0;

    /**
     * 
     * @type api
     * @brief 更新公共流参数 <br>
     *        关于发布公共流，查看 startPushPublicStream{@link #IRTCVideo#startPushPublicStream}。
     * @param public_stream_id 公共流 ID
     * @param param 公共流参数。详见 IPublicStreamParam{@link #IPublicStreamParam}。 <br>
     *              指定的流必须为当前用户所发布的。
     * @return
     *        - 0: 成功
     *        - !0: 失败
     * @note 调用本接口前需要通过 onPushPublicStreamResult{@link #IRTCVideoEventHandler#onPushPublicStreamResult} 确认公共流是否已经成功启动。
     * @list 音视频传输
     * @order 18
     */
    
    virtual int updatePublicStreamParam(const char* public_stream_id, IPublicStreamParam* param) = 0;


    /**
     * 
     * @hidden(macOS,Windows,Linux)
     * @type api
     * @region 屏幕共享
     * @brief 更新屏幕采集数据类型
     * @param type 屏幕采集数据类型
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 该函数必须在 startScreenCapture 函数之后调用。本地用户会收到 onMediaDeviceStateChanged{@link #IRTCVideoEventHandler#onMediaDeviceStateChanged} 的回调。 <br>
     *        参数 device_state 值为 ByteRTCMediaDeviceStateStarted 或 ByteRTCMediaDeviceStateStopped，device_error 值为 ByteRTCMediaDeviceErrorOK。
     */
    
    virtual int updateScreenCapture(ScreenMediaType type) = 0;

     /**
      * 
      * @type api
      * @region 音频管理
      * @brief 启用音频信息提示。开启提示后，你会收到 onLocalAudioPropertiesReport{@link #IRTCVideoEventHandler#onLocalAudioPropertiesReport}，onRemoteAudioPropertiesReport{@link #IRTCVideoEventHandler#onRemoteAudioPropertiesReport} 和 onActiveSpeaker{@link #IRTCVideoEventHandler#onActiveSpeaker}。
      * @param config 详见 AudioPropertiesConfig{@link #AudioPropertiesConfig}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
      * @list 音频管理
      * @order 26
      */
     
    virtual int enableAudioPropertiesReport(const AudioPropertiesConfig& config) = 0;

    /**
     * 
     * @type api
     * @region 多房间
     * @brief 调节来自指定远端用户的音频播放音量。
     * @param room_id 音频来源用户所在的房间 ID
     * @param user_id 音频来源的远端用户 ID
     * @param volume 音频播放音量值和原始音量的比值，范围是 [0, 400]，单位为 %，自带溢出保护。为保证更好的通话质量，建议将 volume 值设为 [0,100]。 <br>
     *                    只改变音频数据的音量信息，不涉及本端硬件的音量调节。 <br>
     *              - 0: 静音
     *              - 100: 原始音量，默认值
     *              - 400: 最大可为原始音量的 4 倍(自带溢出保护)
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note 假设某远端用户 A 始终在被调节的目标用户范围内， <br>
     *        - 当该方法与 setRemoteRoomAudioPlaybackVolume{@link #IRTCRoom#setRemoteRoomAudioPlaybackVolume} 共同使用时，本地收听用户 A 的音量为后调用的方法设置的音量；
     *        - 当该方法与 setPlaybackVolume{@link #IRTCVideo#setPlaybackVolume} 方法共同使用时，本地收听用户 A 的音量将为两次设置的音量效果的叠加。
     * @list 音频管理
     * @order 24
     */
    
    virtual int setRemoteAudioPlaybackVolume(const char* room_id,const char* user_id, int volume) = 0;

    /**
     * 
     * @type api
     * @region 音频管理
     * @brief 开启/关闭音量均衡功能。 <br>
     *        开启音量均衡功能后，人声的响度会调整为 -16lufs。如果已调用 setAudioMixingLoudness{@link #IAudioMixingManager#setAudioMixingLoudness} 传入了混音音乐的原始响度，此音乐播放时，响度会调整为 -20lufs。（Linux 不支持）
     * @param enable 是否开启音量均衡功能： <br>
     *       - true: 是
     *       - false: 否
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 该接口须在调用 startAudioMixing{@link #IAudioMixingManager#startAudioMixing} 开始播放音频文件之前调用。
     */
    
    virtual int enableVocalInstrumentBalance(bool enable) = 0;

    /**
     * 
     * @type api
     * @region 音频管理
     * @brief 打开/关闭音量闪避功能，适用于在 RTC 通话过程中会同时播放短视频或音乐的场景，如“一起看”、“在线 KTV”等。 <br>
     *        开启该功能后，当检测到远端人声时，本地的媒体播放音量会自动减弱，从而保证远端人声的清晰可辨；当远端人声消失时，本地媒体音量会恢复到闪避前的音量水平。
     * @param enable 是否开启音量闪避： <br>
     *        - true: 是
     *        - false: 否
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     */
    
    virtual int enablePlaybackDucking(bool enable) = 0;

    /**
     * 
     * @type api
     * @region 视频数据回调
     * @brief 注册本地视频帧监测器。 <br>
     *        无论使用内部采集还是自定义采集，调用该方法后，SDK 每监测到一帧本地视频帧时，都会将视频帧信息通过 onLocalEncodedVideoFrame{@link #ILocalEncodedVideoFrameObserver#onLocalEncodedVideoFrame} 回调给用户
     * @param observer 本地视频帧监测器，参看 ILocalEncodedVideoFrameObserver{@link #ILocalEncodedVideoFrameObserver}。将参数设置为 nullptr 则取消注册。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 该方法可在进房前后的任意时间调用，在进房前调用可保证尽可能早地监测视频帧并触发回调
     * @list 自定义流处理
     * @order 18
     */
    
    virtual int registerLocalEncodedVideoFrameObserver(ILocalEncodedVideoFrameObserver* observer) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 注册远端编码后视频数据回调。 <br>
     *        完成注册后，当 SDK 监测到远端编码后视频帧时，会触发 onRemoteEncodedVideoFrame{@link #IRemoteEncodedVideoFrameObserver#onRemoteEncodedVideoFrame} 回调
     * @param observer 远端编码后视频数据监测器，参看 IRemoteEncodedVideoFrameObserver{@link #IRemoteEncodedVideoFrameObserver}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 更多自定义解码功能说明参看 [自定义视频编解码](https:
     *       - 该方法适用于手动订阅，并且进房前后均可调用，建议在进房前调用。
     *       - 引擎销毁前需取消注册，调用该方法将参数设置为 nullptr 即可。
     * @list 自定义流处理
     * @order 4
     */
    
    virtual int registerRemoteEncodedVideoFrameObserver(IRemoteEncodedVideoFrameObserver* observer) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 注册自定义编码帧推送事件回调
     * @param encoder_handler 自定义编码帧回调类，参看 IExternalVideoEncoderEventHandler{@link #IExternalVideoEncoderEventHandler}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *       - 该方法需在进房前调用。
     *       - 引擎销毁前需取消注册，调用该方法将参数设置为 nullptr 即可。
     * @list 自定义流处理
     * @order 19
     */
    
    virtual int setExternalVideoEncoderEventHandler(IExternalVideoEncoderEventHandler* encoder_handler) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 推送自定义编码后的视频流
     * @param index 需要推送的编码流的属性，参看 StreamIndex{@link #StreamIndex}
     * @param video_index 对应的编码流下标，从 0 开始，如果调用 setVideoEncoderConfig{@link #IRTCVideo#setVideoEncoderConfig} 设置了多路流，此处数量须与之保持一致
     * @param video_stream 编码流视频帧信息，参看 IEncodedVideoFrame{@link #IEncodedVideoFrame}。
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @note
     *        - 目前仅支持推送 H264 和 ByteVC1 格式的视频帧，且视频流协议格式须为 Annex B 格式。
     *        - 该函数运行在用户调用线程内
     *        - 推送自定义编码视频帧前，必须调用 setVideoSourceType{@link #IRTCVideo#setVideoSourceType} 将视频输入源切换至自定义编码视频源。
     * @list 自定义流处理
     * @order 20
     */
    
    virtual int pushExternalEncodedVideoFrame(StreamIndex index, int video_index, IEncodedVideoFrame* video_stream) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 在订阅远端视频流之前，设置远端视频数据解码方式
     * @param key 远端流信息，即对哪一路视频流进行解码方式设置，参看 RemoteStreamKey{@link #RemoteStreamKey}。
     * @param config 视频解码方式，参看 VideoDecoderConfig{@link #VideoDecoderConfig}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 当你想要对远端流进行自定义解码时，你需要先调用 registerRemoteEncodedVideoFrameObserver{@link #IRTCVideo#registerRemoteEncodedVideoFrameObserver} 注册远端视频流监测器，然后再调用该接口将解码方式设置为自定义解码。监测到的视频数据会通过 onRemoteEncodedVideoFrame{@link #IRemoteEncodedVideoFrameObserver#onRemoteEncodedVideoFrame} 回调出来。
     *        - 自 3.56 起，要用于自动订阅场景下，你可以设置 `key` 中的 `RoomId` 和 `UserId` 为 `nullptr`，此时，通过此接口设置的解码方式根据 `key` 中的 `StreamIndex` 值，适用于所有的远端主流或屏幕流的解码方式。
     * @list 自定义流处理
     * @order 5
     */
    
    virtual int setVideoDecoderConfig(RemoteStreamKey key, VideoDecoderConfig config) = 0;

    /**
     * 
     * @type api
     * @region 视频管理
     * @brief 在订阅远端视频流之后，向远端请求关键帧
     * @param stream_info 远端流信息，参看 RemoteStreamKey{@link #RemoteStreamKey}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 该方法仅适用于手动订阅模式，并且在成功订阅远端流之后使用。
     *        - 该方法适用于调用 setVideoDecoderConfig{@link #IRTCVideo#setVideoDecoderConfig} 开启自定义解码功能后，并且自定义解码失败的情况下使用
     * @list 音视频传输
     * @order 7
     */
    
    virtual int requestRemoteVideoKeyFrame(const RemoteStreamKey& stream_info) = 0;

    /**
     * 
     * @type api
     * @region 音频管理
     * @brief 发送音频流同步信息。将消息通过音频流发送到远端，并实现与音频流同步，该接口调用成功后，远端用户会收到 onStreamSyncInfoReceived{@link #IRTCVideoEventHandler#onStreamSyncInfoReceived} 回调。
     * @param data 消息内容。
     * @param length 消息长度。取值范围是 [1,255] 字节，建议小于 16 字节，否则可能占用较大带宽。
     * @param config 媒体流信息同步的相关配置，详见 StreamSycnInfoConfig{@link #StreamSycnInfoConfig} 。
     * @return
     *        - >=0: 消息发送成功。返回成功发送的次数。
     *        - -1: 消息发送失败。消息长度大于 255 字节。
     *        - -2: 消息发送失败。传入的消息内容为空。
     *        - -3: 消息发送失败。通过屏幕流进行消息同步时，此屏幕流还未发布。
     *        - -4: 消息发送失败。通过用麦克风或自定义设备采集到的音频流进行消息同步时，此音频流还未发布，详见错误码 ErrorCode{@link #ErrorCode}。
     * @note
     * - 调用本接口的频率建议不超过 50 次每秒。
     * - 在 `kRoomProfileTypeInteractivePodcast` 房间模式下，此消息一定会送达。在其他房间模式下，如果本地用户未说话，此消息不一定会送达。
     * @list 消息
     * @order 13
     */
    
    virtual int sendStreamSyncInfo(const uint8_t* data, int32_t length, const StreamSycnInfoConfig& config) = 0;

    /**
     * 
     * @type api
     * @region 混音
     * @brief 开启本地语音变调功能，多用于 K 歌场景。 <br>
     *        使用该方法，你可以对本地语音的音调进行升调或降调等调整。
     * @param pitch 相对于语音原始音调的升高/降低值，取值范围[-12，12]，默认值为 0，即不做调整。 <br>
     *        取值范围内每相邻两个值的音高距离相差半音，正值表示升调，负值表示降调，设置的绝对值越大表示音调升高或降低越多。 <br>
     *        超出取值范围则设置失败，并且会触发 onWarning{@link #IRTCVideoEventHandler#onWarning} 回调，提示 WarningCode{@link #WarningCode} 错误码为 `WARNING_CODE_SET_SCREEN_STREAM_INVALID_VOICE_PITCH` 设置语音音调不合法
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @list 音频处理
     * @order 2
     */
    
    virtual int setLocalVoicePitch(int pitch) = 0;

    /**
     * 
     * @deprecated since 3.45 and will be deleted in 3.51, use setPlaybackVolume{@link #IRTCVideo#setPlaybackVolume} instead.
     * @type api
     * @region 媒体流管理
     * @brief 控制本地音频流播放状态：播放/不播放
     * @param mute_state 播放状态，标识是否播放本地音频流，详见：MuteState{@link #MuteState}
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 本方法仅控制本地收到音频流的播放状态，并不影响本地音频播放设备。
     */
    
    BYTERTC_DEPRECATED virtual int muteAudioPlayback(MuteState mute_state) = 0;

    /**
     * 
     * @type api
     * @brief 订阅指定公共流 <br>
     *        无论用户是否在房间内，都可以调用本接口获取和播放指定的公共流。
     * @param public_stream_id 公共流 ID，如果指定流暂未发布，则本地客户端将在其开始发布后接收到流数据。
     * @return
     *        - 0: 成功。同时将收到 onPlayPublicStreamResult{@link #IRTCVideoEventHandler#onPlayPublicStreamResult} 回调。
     *        - !0: 失败。当参数不合法或参数为空，调用失败。
     * @note
     *        - 一个客户端最多同时播放 5 路公共流，请及时调用 stopPlayPublicStream{@link #IRTCVideo#stopPlayPublicStream} 取消订阅公共流，避免订阅的公共流数量超限。
     *        - 在调用本接口之前，建议先绑定渲染视图。
     *              - 调用 setPublicStreamVideoCanvas{@link #IRTCVideo#setPublicStreamVideoCanvas} 绑定内部渲染视图：
     *              - 调用 setPublicStreamVideoSink{@link #IRTCVideo#setPublicStreamVideoSink} 绑定自定义渲染视图：
     *        - 调用本接口后，可以通过 onFirstPublicStreamVideoFrameDecoded{@link #IRTCVideoEventHandler#onFirstPublicStreamVideoFrameDecoded} 和 onFirstPublicStreamAudioFrame{@link #IRTCVideoEventHandler#onFirstPublicStreamAudioFrame} 回调公共流的视频和音频首帧解码情况。
     *        - 调用本接口后，可以通过 onPublicStreamSEIMessageReceived{@link #IRTCVideoEventHandler#onPublicStreamSEIMessageReceived} 回调公共流中包含的 SEI 信息。
     *        - 订阅公共流之后，可以通过调用 stopPlayPublicStream{@link #IRTCVideo#stopPlayPublicStream} 接口取消订阅公共流。
     * @list 音视频传输
     * @order 19
     */
    
    virtual int startPlayPublicStream(const char* public_stream_id) = 0;
    /**
     * 
     * @type api
     * @brief 取消订阅指定公共流 <br>
     *        关于订阅公共流，查看 startPlayPublicStream{@link #IRTCVideo#startPlayPublicStream}。
     * @param public_stream_id 公共流 ID
     * @return
     *        - 0：成功
     *        - !0：失败
     * @list 音视频传输
     * @order 20
     */
    

    virtual int stopPlayPublicStream(const char* public_stream_id) = 0;
    /**
     * 
     * @type api
     * @brief 为指定公共流绑定内部渲染视图。
     * @param public_stream_id 公共流 ID
     * @param canvas 内部渲染视图，如果需要解除视频的绑定视图，把 videoCanvas 设置为空。详见 VideoCanvas{@link #VideoCanvas}。
     * @return
     *        - 0：成功
     *        - !0：失败
     * @list 音视频传输
     * @order 21
     */
    
    virtual int setPublicStreamVideoCanvas(const char* public_stream_id, const VideoCanvas& canvas) = 0;
    /**
     * 
     * @type api
     * @brief 为指定公共流绑定自定义渲染器。详见[自定义视频渲染](https:
     * @param public_stream_id 公共流 ID
     * @param video_sink 自定义视频渲染器，自定义视频渲染器，需要释放渲染器资源时，将 videoSink 设置为 `null`。参看 IVideoSink{@link #IVideoSink}
     * @param format videoSink 适用的视频帧编码格式，参看 PixelFormat{@link #PixelFormat}
     * @return 方法调用结果： <br>
     *        - 0：成功；
     *        - <0：失败。具体失败原因参看 ReturnStatus{@link #ReturnStatus}。
     * @list 音视频传输
     * @order 22
     */
    
    virtual int setPublicStreamVideoSink(const char* public_stream_id, IVideoSink* video_sink,
                                          IVideoSink::PixelFormat format) = 0;
    
    /**
     * 
     * @type api
     * @valid since 3.51
     * @brief 调节公共流的音频播放音量。
     * @param public_stream_id 公共流 ID
     * @param volume 音频播放音量值和原始音量值的比值，该比值的范围是 `[0, 400]`，单位为 %，且自带溢出保护。为保证更好的音频质量，建议设定在 `[0, 100]` 之间，其中 100 为系统默认值。
     * @return
     *         - 0: 成功调用。
     *         - -2: 参数错误。
     * @list 音视频传输
     * @order 24
     */
    
    virtual int setPublicStreamAudioPlaybackVolume(const char* public_stream_id, int volume) = 0;
    
    /**
     * 
     * @type api
     * @region 音视频处理
     * @brief 在指定视频流上添加水印。
     * @param stream_index 需要添加水印的视频流属性，参看 StreamIndex{@link #StreamIndex}。
     * @param image_path 水印图片路径，仅支持本地文件绝对路径，长度限制为 512 字节。 <br>
     *          水印图片为 PNG 或 JPG 格式。
     * @param config 水印参数，参看 RTCWatermarkConfig{@link #RTCWatermarkConfig}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 调用 clearVideoWatermark{@link #IRTCVideo#clearVideoWatermark} 移除指定视频流的水印。
     *        - 同一路流只能设置一个水印，新设置的水印会代替上一次的设置。你可以多次调用本方法来设置不同流的水印。
     *        - 若开启本地预览镜像，或开启本地预览和编码传输镜像，则远端水印均不镜像；在开启本地预览水印时，本端水印会镜像。
     *        - 开启大小流后，水印对大小流均生效，且针对小流进行等比例缩小。
     * @list 视频处理
     * @order 3
     */
    
    virtual int setVideoWatermark(StreamIndex stream_index, const char* image_path, RTCWatermarkConfig config) = 0;

    /**
     * 
     * @type api
     * @region 音视频处理
     * @brief 移除指定视频流的水印。
     * @param stream_index 需要移除水印的视频流属性，参看 StreamIndex{@link #StreamIndex}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @list 视频处理
     * @order 4
     */
    
    virtual int clearVideoWatermark(StreamIndex stream_index) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @brief 截取本地视频画面
     * @param stream_index 截图的视频流的属性，参看 StreamIndex{@link #StreamIndex}。
     * @param callback 本地截图的回调。参看 ISnapshotResultCallback{@link #ISnapshotResultCallback}。
     * @return 本地截图任务的编号，从 `1` 开始递增。
     * @note
     *        - 对截取的画面，包含本地视频处理的全部效果，包含旋转，镜像，美颜等。
     *        - 不管采用 SDK 内部采集，还是自定义采集，都可以进行截图。
     * @list 高级功能
     * @order 34
     */
    
    virtual long takeLocalSnapshot(const StreamIndex stream_index, ISnapshotResultCallback* callback) = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @brief 截取远端视频画面
     * @param stream_key 截图的视频流，参看 RemoteStreamKey{@link #RemoteStreamKey}。
     * @param callback 参看 ISnapshotResultCallback{@link #ISnapshotResultCallback}。
     * @return 远端截图任务的编号，从 `1` 开始递增。
     * @list 高级功能
     * @order 35
     */
    
    virtual long takeRemoteSnapshot(const RemoteStreamKey stream_key, ISnapshotResultCallback* callback) = 0;

    /**
     * 
     * @type api
     * @region 音视频传输
     * @brief 摄像头处于关闭状态时，使用静态图片填充本地推送的视频流。 <br>
     *        调用 `stopVideoCapture` 接口时，会开始推静态图片。若要停止发送图片，可传入空字符串或启用内部摄像头采集。 <br>
     *        可重复调用该接口来更新图片。
     * @param file_path 设置静态图片的路径。 <br>
     *        支持本地文件绝对路径，不支持网络链接，长度限制为 512 字节。 <br>
     *        静态图片支持类型为 JPEG/JPG、PNG、BMP。 <br>
     *        若图片宽高比与设置的编码宽高比不一致，图片会按编码高宽等比缩放，并裁剪长边。推流帧率与码率与设置的编码参数一致。
     * @return
     *        - 0: 成功。
     *        - -1: 失败。
     * @note
     *        - 该接口只适用于 SDK 内部摄像头采集，不适用于自定义视频采集。
     *        - 本地预览无法看到静态图片。
     *        - 进入房间前后均可调用此方法。在多房间场景中，静态图片仅在发布的房间中生效。
     *        - 针对该静态图片，滤镜和镜像效果不生效，水印效果生效。
     *        - 只有主流能设置静态图片，屏幕流不支持设置。
     *        - 开启大小流后，静态图片对大小流均生效，且针对小流进行等比例缩小。
     * @list 音视频传输
     * @order 2
     */
    
    virtual int setDummyCaptureImagePath(const char* file_path) = 0;

    /**
     * 
     * @type api
     * @region 云代理
     * @brief 开启云代理
     * @param configuration 云代理服务器信息列表。参看 CloudProxyConfiguration{@link #CloudProxyConfiguration}。
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note
     *        - 在加入房间前调用此接口
     *        - 在开启云代理后，进行通话前网络探测
     *        - 开启云代理后，并成功链接云代理服务器后，会收到 onCloudProxyConnected{@link #IRTCVideoEventHandler#onCloudProxyConnected}。
     *        - 要关闭云代理，调用 stopCloudProxy{@link #IRTCVideo#stopCloudProxy}。
     * @list 安全与加密
     * @order 0
     */
    
    virtual int startCloudProxy(const CloudProxyConfiguration& configuration) = 0;

    /**
     * 
     * @type api
     * @region 云代理
     * @brief 关闭云代理
     * @return
     *        - 0: 调用成功。
     *        - < 0 : 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明
     * @note 要开启云代理，调用 startCloudProxy{@link #IRTCVideo#startCloudProxy}。
     * @list 安全与加密
     * @order 1
     */
    
    virtual int stopCloudProxy() = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 网络管理
     * @brief 开启音视频回路测试。 <br>
     *        在进房前，用户可调用该接口对音视频通话全链路进行检测，包括对音视频设备以及用户上下行网络的检测，从而帮助用户判断是否可以正常发布和接收音视频流。 <br>
     *        开始检测后，SDK 会录制你声音或视频。如果你在设置的延时范围内收到了回放，则视为音视频回路测试正常。
     * @param echo_test_config 回路测试参数设置，参看 EchoTestConfig{@link #EchoTestConfig}。
     * @param play_delay_time 音视频延迟播放的时间间隔，用于指定在开始检测多长时间后期望收到回放。取值范围为 [2,10]，单位为秒，默认为 2 秒。
     * @return 方法调用结果： <br>
     *        - 0：成功
     *        - -1：失败，当前用户已经在检测中
     *        - -2：失败，用户已进房
     *        - -3：失败，音视频均未采集
     *        - -4：失败，参数异常
     *        - -5：失败，已经存在相同 roomId 的房间
     * @note
     *        - 调用该方法开始音视频回路检测后，你可以调用 stopEchoTest{@link #IRTCVideo#stopEchoTest} 立即结束测试，也可等待测试 60s 后自动结束，以更换设备进行下一次测试，或进房。
     *        - 在该方法之前调用的所有跟设备控制、流控制相关的方法均在开始检测时失效，在结束检测后恢复生效。
     *        - 在调用 startEchoTest{@link #IRTCVideo#startEchoTest} 和 stopEchoTest{@link #IRTCVideo#stopEchoTest} 之间调用的所有跟设备采集、流控制、进房相关的方法均不生效，并会收到 onWarning{@link #IRTCVideoEventHandler#onWarning} 回调，提示警告码为 `kWarningCodeInEchoTestMode`。
     *        - 音视频回路检测的结果会通过 onEchoTestResult{@link #IRTCVideoEventHandler#onEchoTestResult} 回调通知。
     * @list 网络管理
     * @order 7
     */
    
    virtual int startEchoTest(EchoTestConfig echo_test_config, unsigned int play_delay_time) = 0;
    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 网络管理
     * @brief 停止音视频回路测试。 <br>
     *        调用 startEchoTest{@link #IRTCVideo#startEchoTest} 开启音视频回路检测后，你必须调用该方法停止检测。
     * @return 方法调用结果： <br>
     *        - 0：成功
     *        - -1：失败，未开启回路检测
     * @note 音视频回路检测结束后，所有对系统设备及音视频流的控制均会恢复到开始检测前的状态。
     * @list 网络管理
     * @order 8
     */
    
    virtual int stopEchoTest() = 0;
    /**
     * 
     * @hidden(Linux)
     * @type api
     * @brief 创建 K 歌评分管理接口。
     * @return K 歌评分管理接口,详见 ISingScoringManager{@link #ISingScoringManager}。
     * @note 如需使用 K 歌评分功能，即调用该方法以及 `ISingScoringManager` 类下全部方法，需集成 SAMI 动态库，详情参看[按需集成插件](1108726)文档。
     * @list 在线 KTV
     * @order 22
     */
    
    virtual ISingScoringManager* getSingScoringManager() = 0;
    /**
     * 
     * @hidden(Linux)
     * @type api
     * @brief 通过 NTP 协议，获取网络时间。
     * @return 网络时间。参看 NetworkTimeInfo{@link #NetworkTimeInfo}。
     * @note
     *        - 第一次调用此接口会启动网络时间同步功能，并返回 `0`。同步完成后，会收到 onNetworkTimeSynchronized{@link #IRTCVideoEventHandler#onNetworkTimeSynchronized}，此后，再次调用此 API，即可获取准确的网络时间。
     *        - 在合唱场景下，合唱参与者应在相同的网络时间播放背景音乐。
     * @list 网络管理
     * @order 9
     */
    
    virtual NetworkTimeInfo getNetworkTimeInfo() = 0;
    /**
     * 
     * @hidden internal use
     * @valid since 3.52
     * @type api
     * @brief 调用实验性 API。 <br>
     *        调用后，可能会触发 onInvokeExperimentalAPI{@link #IRTCVideoEventHandler#onInvokeExperimentalAPI}。
     * @param param JSON 字符串，形如： <br>
     * ```{
     *   "api_name":"startPublish",
     *   "params":{
     *     "streamID":"",
     *     "observer":"",
     *     "uri":"",
     *     "option":""
     *   }
     * }```
     * @return
     *        - 0： 成功。
     *        - < 0： 失败。
     * @list 引擎管理
     * @order 7
     */
    
    virtual int invokeExperimentalAPI(const char* param) = 0;


    /**
     * 
     * @hidden(Linux,macOS)
     * @type api
     * @brief 创建 KTV 管理接口。
     * @return KTV 管理接口，参看 IKTVManager{@link #IKTVManager}。
     * @list 在线 KTV
     * @order 0
     */
    
    virtual IKTVManager* getKTVManager() = 0;

    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 音频设备
     * @brief 开启通话前回声检测
     * @param test_audio_file_path 用于回声检测的音频文件的绝对路径，路径字符串使用 UTF-8 编码格式，支持以下音频格式: mp3，aac，m4a，3gp，wav。 <br>
     *         音频文件不为静音文件，推荐时长为 10 ～ 20 秒。
     * @return 方法调用结果： <br>
     *        - 0: 成功。
     *        - -1：失败。上一次检测未结束，请先调用 stopHardwareEchoDetection{@link #IRTCVideo#stopHardwareEchoDetection} 停止检测 后重新调用本接口。
     *        - -2：失败。路径不合法或音频文件格式不支持。
     * @note
     *        - 只有当 RoomProfileType{@link #RoomProfileType} 为 `kRoomProfileTypeMeeting` 和 `kRoomProfileTypeMeetingRoom` 时支持开启本功能。
     *        - 开启检测前，你需要向用户获取音频设备的使用权限。
     *        - 开启检测前，请确保音频设备没有被静音，采集和播放音量正常。
     *        - 调用本接口后监听 onHardwareEchoDetectionResult{@link #IRTCVideoEventHandler#onHardwareEchoDetectionResult} 获取检测结果。
     *        - 检测期间，进程将独占音频设备，无法使用其他音频设备测试接口： startEchoTest{@link #IRTCVideo#startEchoTest}、startAudioDeviceRecordTest{@link #IAudioDeviceManager#startAudioDeviceRecordTest} 或 startAudioPlaybackDeviceTest{@link #IAudioDeviceManager#startAudioPlaybackDeviceTest}。
     *        - 调用 stopHardwareEchoDetection{@link #IRTCVideo#stopHardwareEchoDetection} 停止检测，释放对音频设备的占用。
     * @list 音频管理
     * @order 38
     */
    
    virtual int startHardwareEchoDetection(const char* test_audio_file_path) = 0;
    /**
     * 
     * @hidden(Linux)
     * @type api
     * @region 音频设备
     * @brief 停止通话前回声检测
     * @return 方法调用结果： <br>
     *        - 0: 成功。
     *        - -1：失败。
     * @note
     *       - 关于开启通话前回声检测，参看 startHardwareEchoDetection{@link #IRTCVideo#startHardwareEchoDetection} 。
     *       - 建议在收到 onHardwareEchoDetectionResult{@link #IRTCVideoEventHandler#onHardwareEchoDetectionResult} 通知的检测结果后，调用本接口停止检测。
     *       - 在用户进入房间前结束回声检测，释放对音频设备的占用，以免影响正常通话。
     * @list 音频管理
     * @order 39
     */
    
    virtual int stopHardwareEchoDetection() = 0;


    /**
     * 
     * @hidden(macOS, Windows, Linux)
     * @type api
     * @brief 启用蜂窝网络辅助增强，改善通话质量。
     * @param config 参看 MediaTypeEnhancementConfig{@link #MediaTypeEnhancementConfig}。
     * @return 方法调用结果： <br>
     *        - 0: 成功。
     *        - -1：失败，内部错误。
     *        - -2: 失败，输入参数错误。
     * @note 此功能默认不开启。
     */
    
    virtual int setCellularEnhancement(const MediaTypeEnhancementConfig& config) = 0;

    /**
     * 
     * @type api
     * @region 代理
     * @brief 设置本地代理。
     * @param configurations 本地代理配置参数。参看 LocalProxyConfiguration{@link #LocalProxyConfiguration}。 <br>
     *        你可以根据自己的需要选择同时设置 Http 隧道 和 Socks5 两类代理，或者单独设置其中一类代理。如果你同时设置了 Http 隧道 和 Socks5 两类代理，此时，媒体和信令采用 Socks5 代理， Http 请求采用 Http 隧道代理；如果只设置 Http 隧道 或 Socks5 一类代理，媒体、信令和 Http 请求均采用已设置的代理。 <br>
     *        调用此接口设置本地代理后，若想清空当前已有的代理设置，可再次调用此接口，选择不设置任何代理即可清空。
     * @param configuration_num 本地代理配置参数的数量。
     * @note
     *       - 该方法需要在进房前调用。
     *       - 调用该方法设置本地代理后，SDK 会触发 onLocalProxyStateChanged{@link #IRTCVideoEventHandler#onLocalProxyStateChanged} ，返回代理连接的状态。
     * @list 安全与加密
     * @order 4
     */
    
    virtual int setLocalProxy(const LocalProxyConfiguration* configurations, int configuration_num) = 0;


    /**
     * 
     * @valid since 3.57
     * @hidden(Android,iOS)
     * @type api
     * @region 音视频处理
     * @brief 设置视频暗光增强模式。 <br>
     *        对于光线不足、照明不均匀或背光等场景下推荐开启，可有效改善画面质量。
     * @param mode 默认不开启。参看 VideoEnhancementMode{@link #VideoEnhancementMode}。
     * @return
     *        - 0: API 调用成功。会立即生效，但需要等待下载和检测完成后才能看到增强后的效果。
     *        - < 0: API 调用失败。查看 ReturnStatus{@link #ReturnStatus} 获得更多错误说明。
     * @note
     *        - 开启后会影响设备性能，应根据实际需求和设备性能决定是否开启。
     *        - 对 RTC SDK 内部采集的视频和自定义采集的视频都生效。
     */
    
    virtual int setLowLightAdjusted(VideoEnhancementMode mode) = 0;
};

/**
* 
* @type api
* @region 引擎管理
* @brief 创建 IRTCVideo 实例。 <br>
*        如果当前进程中未创建引擎实例，那么你必须先使用此方法，以使用 RTC 提供的各种音视频能力。 <br>
*        如果当前进程中已创建了引擎实例，再次调用此方法时，会返回已创建的引擎实例。
* @param app_id 每个应用的唯一标识符。只有使用相同的 app_id 生成的实例，才能够进行音视频通信。
* @param event_handler SDK 回调给应用层的 Callback 对象，详见 IRTCVideoEventHandler{@link #IRTCVideoEventHandler} 。
* @param parameters 私有参数。如需使用请联系技术支持人员。
* @return
*        - IRTCVideo：创建成功。返回一个可用的 IRTCVideo{@link #IRTCVideo} 实例
*        - Null：app_id 或者 event_handler 为空, event_handler 为空。
* @list 引擎管理
* @order 1
*/

BYTERTC_API bytertc::IRTCVideo* createRTCVideo(const char* app_id,
     bytertc::IRTCVideoEventHandler *event_handler, const char* parameters);

/**
 * 
 * @hidden internal use only
 * @type api
 * @region 引擎管理
 * @brief 创建多实例引擎对象 <br>
 *        如果当前进程中未创建引擎实例，那么你必须先使用此方法，以使用 RTC 提供的各种音视频能力。 <br>
 *        如果当前进程中已创建了引擎实例，再次调用此方法时，会返回新创建的引擎实例。
 * @param context Android Application Context
 * @param appId 每个应用的唯一标识符，由 RTC 控制台随机生成的。不同的 AppId 生成的实例在 RTC 中进行音视频通话完全独立，无法互通。
 * @param handler SDK 回调给应用层的 Handler，详见 IRTCVideoEventHandler{@link #IRTCVideoEventHandler}
 * @param eglContext 如果需要支持外部纹理硬编码，则需要以 `JObject` 方式传入 `eglContext`。
 * @param parameters 私有参数。如需使用请联系技术支持人员。
 * @return
 *        - RTCVideo：创建成功。返回一个可用的 IRTCVideo{@link #IRTCVideo} 实例
 *        - Null：.so 文件加载失败，创建失败。
 * @note 你应注意保持 handler 的生命周期必须大于 IRTCVideo{@link #IRTCVideo} 的生命周期，即 handler 必须在调用 destroyRTCVideo{@link #destroyRTCVideo} 之后销毁。
 */

BYTERTC_API bytertc::IRTCVideo* createRTCVideoMulti(const char* app_id,
                                                    bytertc::IRTCVideoEventHandler* event_handler, const char* parameters);
/**
 * 
 * @type api
 * @region 引擎管理
 * @brief 销毁由 createRTCVideo{@link #createRTCVideo} 所创建的引擎实例，并释放所有相关资源。
 * @note
 *        - 请确保和需要销毁的 IRTCVideo{@link #IRTCVideo} 实例相关的业务场景全部结束后，才调用此方法。如果在多线程场景下，调用此接口后，又调用了其他 IRTCVideo{@link #IRTCVideo} 相关接口，可能导致 SDK 崩溃。该方法在调用之后，会销毁所有和此 IRTCVideo{@link #IRTCVideo} 实例相关的内存，并且停止与媒体服务器的任何交互。
 *        - 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，会导致死锁。同时此方法是耗时操作，不建议在主线程调用本方法，避免主线程阻塞。
 * @list 引擎管理
 * @order 2
 */

BYTERTC_API void destroyRTCVideo();

/**
 * 
 * @hidden internal use only
 * @type api
 * @region 引擎管理
 * @brief 销毁由 createRTCVideoMulti{@link #createRTCVideoMulti} 所创建的引擎实例，并释放所有相关资源。
 * @note
 *      - 请确保和需要销毁的 IRTCVideo{@link #IRTCVideo} 实例相关的业务场景全部结束后，才调用此方法
 *      - 该方法在调用之后，会销毁所有和此 IRTCVideo{@link #IRTCVideo} 实例相关的内存，并且停止与媒体服务器的任何交互
 *      - 调用本方法会启动 SDK 退出逻辑。引擎线程会保留，直到退出逻辑完成。因此，不要在回调线程中直接调用此 API，会导致死锁。同时此方法是耗时操作，不建议在主线程调用本方法，避免主线程阻塞。
 */

BYTERTC_API void destroyRTCVideoMulti(IRTCVideo* instance_multi);
/**
 * 
 * @type api
 * @region 错误码
 * @brief 获取 SDK 内各种错误码、警告码的描述文字。
 * @param code 需要获取描述的错误码
 * @return 错误码的描述
 * @note 该接口是通用功能，调用时不需要依赖引擎对象。
 * @list 引擎管理
 * @order 4
 */

BYTERTC_API const char* getErrorDescription(int code);
/**
 * 
 * @type api
 * @region 引擎管理
 * @brief 获取当前 SDK 版本信息。
 * @return 当前 SDK 版本信息。
 * @list 引擎管理
 * @order 2
 */

BYTERTC_API const char* getSDKVersion();

/**
 * 
 * @type api
 * @region 引擎管理
 * @brief 配置 SDK 本地日志参数，包括日志级别、存储路径、日志文件最大占用的总空间、日志文件名前缀。
 * @param log_config 本地日志参数，参看 LogConfig{@link #LogConfig}。
 * @return
 *        - 0：成功。
 *        - –1：失败，本方法必须在创建引擎前调用。
 *        - –2：失败，参数填写错误。
 * @note 本方法必须在调用 createRTCVideo{@link #createRTCVideo} 之前调用。
 * @list 引擎管理
 * @order 8
 */

BYTERTC_API int setLogConfig(const LogConfig& log_config);



}  