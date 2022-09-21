#ifndef __FANPLAYER_FFPLAYER_H__
#define __FANPLAYER_FFPLAYER_H__

// include header files
#include "stdefine.h"

#ifdef __cplusplus
extern "C" {
#endif

// constant definition
// message
#define MSG_FANPLAYER         (WM_APP + 1)
#define MSG_OPEN_DONE         (('O' << 24) | ('P' << 16) | ('E' << 8) | ('N' << 0))
#define MSG_OPEN_FAILED       (('F' << 24) | ('A' << 16) | ('I' << 8) | ('L' << 0))
#define MSG_PLAY_COMPLETED    (('E' << 24) | ('N' << 16) | ('D' << 8) | (' ' << 0))
#define MSG_TAKE_SNAPSHOT     (('S' << 24) | ('N' << 16) | ('A' << 8) | ('P' << 0))
#define MSG_STREAM_CONNECTED  (('C' << 24) | ('N' << 16) | ('C' << 8) | ('T' << 0))
#define MSG_STREAM_DISCONNECT (('D' << 24) | ('I' << 16) | ('S' << 8) | ('C' << 0))
#define MSG_VIDEO_RESIZED     (('S' << 24) | ('I' << 16) | ('Z' << 8) | ('E' << 0))
#define MSG_D3D_DEVICE_LOST   (('L' << 24) | ('O' << 16) | ('S' << 8) | ('T' << 0))

// adev render type
enum {
    ADEV_RENDER_TYPE_WAVEOUT,
    ADEV_RENDER_TYPE_MAX_NUM,
};

// vdev render type
enum {
    VDEV_RENDER_TYPE_GDI,
    VDEV_RENDER_TYPE_D3D,
    VDEV_RENDER_TYPE_MAX_NUM,
};

// render mode
enum {
    VIDEO_MODE_LETTERBOX,
    VIDEO_MODE_STRETCHED,
    VIDEO_MODE_MAX_NUM,
};

// visual effect
enum {
    VISUAL_EFFECT_DISABLE,
    VISUAL_EFFECT_WAVEFORM,
    VISUAL_EFFECT_SPECTRUM,
    VISUAL_EFFECT_MAX_NUM,
};

// seek flags
enum {
    SEEK_STEP_FORWARD = 1,
    SEEK_STEP_BACKWARD,
};

// param
enum {
    //++ public
    // duration & position
    PARAM_MEDIA_DURATION = 0x1000,
    PARAM_MEDIA_POSITION,

    // media detail info
    PARAM_VIDEO_WIDTH,
    PARAM_VIDEO_HEIGHT,

    // video display mode
    PARAM_VIDEO_MODE,

    // audio volume control
    PARAM_AUDIO_VOLUME,

    // playback speed control
    PARAM_PLAY_SPEED_VALUE,
    PARAM_PLAY_SPEED_TYPE,

    // visual effect mode
    PARAM_VISUAL_EFFECT,

    // audio/video sync diff
    PARAM_AVSYNC_TIME_DIFF,

    // get player init params
    PARAM_PLAYER_INIT_PARAMS,

    // definition evaluation
    PARAM_DEFINITION_VALUE,

    PARAM_DATARATE_VALUE,
    PARAM_OBJECT_DETECT,
    //-- public

    //++ for adev
    PARAM_ADEV_GET_CONTEXT = 0x2000,
    //-- for adev

    //++ for vdev
    PARAM_VDEV_GET_CONTEXT = 0x3000,
    PARAM_VDEV_POST_SURFACE,
    PARAM_VDEV_GET_D3DDEV,
    PARAM_VDEV_D3D_ROTATE,
    PARAM_VDEV_GET_OVERLAY_HDC,
    PARAM_VDEV_SET_OVERLAY_RECT,
    PARAM_VDEV_GET_VRECT,
    PARAM_VDEV_SET_BBOX,
    //-- for vdev

    //++ for render
    PARAM_RENDER_GET_CONTEXT = 0x4000,
    PARAM_RENDER_STEPFORWARD,
    PARAM_RENDER_VDEV_WIN,
    PARAM_RENDER_SOURCE_RECT,
    PARAM_RENDER_STOP,
    //-- for render

    //++ for ffrdp
    PARAM_FFRDP_SENDDATA = 0x5000,
    //-- for ffrdp
};

enum {
    AVSYNC_MODE_AUTO, // Automatic
    AVSYNC_MODE_FILE, // File playback mode
    AVSYNC_MODE_LIVE_SYNC0, // Live mode, give up audio and video synchronization
    AVSYNC_MODE_LIVE_SYNC1, // Live mode, do audio and video synchronization
};

// Initialization parameter description
// PLAYER_INIT_PARAMS is the player initialization parameter, which is passed in when player_open, and some parameter information after the video file is opened can be obtained
// The player_open function will only read the content of the passed initialization parameter, but will not change its content. Want to get the new parameter value after the video file is opened
// Need to pass player_getparam(PARAM_PLAYER_INIT_PARAMS, &params); such an interface
// In the structure, r means the parameter is read-only, w means the parameter is write-only, wr means the parameter is configurable, but whether the configuration is successful can be read and judged after player_open
// Note: When using asynchronous open mode (open_syncmode = 0), some parameters obtained after the player_open function returns may not be valid
// (such as video_vwidth, video_vheight), you need to wait until the MSG_OPEN_DONE message is received to get the valid parameters. Synchronize
// There is no such problem in the open mode, because after player_open is completed, all the initialization work has been done.
typedef struct {
    int video_vwidth; // wr video actual width
    int video_vheight; // wr video actual height
    int video_owidth; // r video output width (after rotate)
    int video_oheight; // r video output height (after rotate)
    int video_frame_rate; // wr video frame rate
    int video_stream_total; // r total number of video streams
    int video_stream_cur; // wr current video stream
    int video_thread_count; // wr video decoding thread count
    int video_hwaccel; // wr video hard decoding enable
    int video_deinterlace; // wr video deinterlace enable
    int video_rotate; // wr video rotation angle
    int video_codecid; // codecid of wr video decoder
    int video_bufpktn; // wr video pkt buffer number
    int audio_channels; // r audio channel number
    int audio_sample_rate; // r audio sample rate
    int audio_stream_total; // r total number of audio streams
    int audio_stream_cur; // wr current audio stream
    int audio_bufpktn; // wr audio pkt buffer number

    int subtitle_stream_total; // r total number of subtitle streams
    int subtitle_stream_cur; // wr current subtitle stream

    int vdev_render_type; // w vdev type
    int adev_render_type; // w adev type

    int init_timeout; // w Player initialization timeout, unit ms, set to prevent stuck when opening network streaming
    int open_syncmode; // w The player is opened synchronously, calling player_open will wait for the player to initialize successfully
    int auto_reconnect; // w The timeout period for automatic reconnection when playing streaming media, in milliseconds
    int rtsp_transport; // w rtsp transmission mode, 0-automatic, 1-udp, 2-tcp, 3-http, 4-https
    int tunnel_port;    // port for use rtsp over http
    int avts_syncmode; // wr audio and video time stamp synchronization mode, 0-automatic, 1-file playback mode, 2-live mode, do audio and video synchronization, 3-live mode, give up audio and video synchronization
    char filter_string[256]; // w custom video filter string
    char ffrdp_tx_key[32]; // aes256 encryption key used by w ffrdp protocol to send data
    char ffrdp_rx_key[32]; // aes256 decryption key used by w ffrdp protocol to receive data
    int swscale_type; // The swscale image scaling algorithm type used by w ffrender
    int waveout_device_id; // w specifies the ID of the waveout device on the windows platform
    int (*oninit)();
    void (*onopening)();
    void (*onrendering)();
    void (*onframe)();
    void (*onreconnect)();
} PLAYER_INIT_PARAMS;
// The two parameters video_stream_cur and audio_stream_cur, if set to -1, the corresponding decoding action can be prohibited
// Application scenario: When playing a video, the window returns to the background, or I just want to listen to the sound, I can set video_stream_cur to -1
// In this way, the player will only decode audio and not video, which can reduce cpu usage

//++ player common variables
typedef struct {// for internal use only
    PLAYER_INIT_PARAMS* init_params;
    int64_t start_time;
    int64_t start_tick;
    int64_t start_pts;
    int64_t apts; // current apts
    int64_t vpts; // current vpts
    int apktn; // available audio packet number in pktqueue
    int vpktn; // available video packet number in pktqueue
    void* winmsg;
} CMNVARS;
//-- player common variables

#ifdef WIN32
typedef struct {
    int srcx, srcy, srcw, srch;
    int dstx, dsty, dstw, dsth;
#define OVERLAY_COPY_BITBLT 1 // bitblt
#define OVERLAY_TRANSPARENT 2 // transparent bitblt
#define OVERLAY_CONST_ALPHA 3 // Transparent blend with fixed alpha value
#define OVERLAY_PIXEL_ALPHA 4 // Transparent blending pixel by pixel
    int type, alpha, transparent;
} RECTOVERLAY;
#endif

// Function declaration
void* player_open(char* file, void* appdata, PLAYER_INIT_PARAMS* params);
void player_close(void* hplayer);
void player_play(void* hplayer);
void player_pause(void* hplayer);
void player_seek(void* hplayer, int64_t ms, int type);
void player_setrect(void* hplayer, int type, int x, int y, int w, int h); // type: 0-video rect, 1-visual effect rect
int player_snapshot(void* hplayer, char* file, int w, int h, int waitt);
int player_record(void* hplayer, char* file);
void player_setparam(void* hplayer, int id, void* param);
void player_getparam(void* hplayer, int id, void* param);

// internal helper function
void player_send_message(void* extra, int32_t msg, void* param);
void player_load_params(PLAYER_INIT_PARAMS* params, char* str);

// Function description
/*
player_open creates a player object
    file-file path (can be the URL of network streaming media)
    appdata-win32 platform incoming window handle, android platform incoming MediaPlayer class object
    params-player initialization parameters
    Return value-void* pointer type, pointing to the player object

player_close close the player
    hplayer-points to the player object returned by player_open

player_play start playing
    hplayer-points to the player object returned by player_open

player_pause pause playback
    hplayer-points to the player object returned by player_open

player_seek jump to the specified position
    hplayer-points to the player object returned by player_open
    ms-the specified position, in milliseconds
    type-specify the type, 0 / SEEK_STEP_FORWARD / SEEK_STEP_BACKWARD
    If type is 0, it is a normal seek operation, ms specifies the position to seek, and the unit is milliseconds
    If the type is SEEK_STEP_FORWARD, it will step forward one frame and then pause
    If the type is SEEK_STEP_BACKWARD, it will step back one frame and then pause
    When using the seek method of SEEK_STEP_BACKWARD, the ms parameter needs to be passed in. The meaning of this function is
    The time range of the search back, usually -1 can be passed in. But for some special videos, pass in -1
    It may not be possible to perform the retreat operation correctly, you can try to modify it to -500 or greater

player_setrect Set the display area, there are two display areas, video display area and visual effect display area
    hplayer-points to the player object returned by player_open
    type-Specified area type 0-video rect, 1-visual effect rect
    x,y,w,h-specify the display area

player_snapshot video playback screenshot
    hplayer-points to the player object returned by player_open
    w, h-specify the image width and height, if <= 0, the video width and height are used by default
    file-image file name (currently only supports jpeg format)
    waitt-whether to drive the screenshot to complete 0-do not wait, >0 wait timeout ms as the unit

player_record save the video record of the current player
    hplayer-points to the player object returned by player_open
    file-the name of the recording file, only .mp4 format is supported, and NULL is passed in to stop recording

player_setparam set parameters
    hplayer-points to the player object returned by player_open
    id-parameter id
    param-parameter pointer

player_getparam get parameters
    hplayer-points to the player object returned by player_open
    id-parameter id
    param-parameter pointer
 */

 // Dynamic parameter description
 /*
 PARAM_MEDIA_DURATION and PARAM_MEDIA_POSITION
 Used to obtain the total length of the multimedia file and the current playback position (in milliseconds)
 LONGLONG total = 1, pos = 0;
 player_getparam(g_hplayer, PARAM_MEDIA_DURATION, &total);
 player_getparam(g_hplayer, PARAM_MEDIA_POSITION, &pos );

 PARAM_VIDEO_WIDTH and PARAM_VIDEO_HEIGHT
 The width and height of the video used to obtain the multimedia file (in pixels)
 int vw = 0, vh = 0;
 player_getparam(g_hplayer, PARAM_VIDEO_WIDTH, &vw);
 player_getparam(g_hplayer, PARAM_VIDEO_HEIGHT, &vh);

 PARAM_VIDEO_MODE
 Used to get and set the video display mode, there are two options:
     1. VIDEO_MODE_LETTERBOX-zoom to the display area proportionally
     2. VIDEO_MODE_STRETCHED-stretch to display area
 (Note: The video display area is set by player_setrect)
 int mode = 0;
 player_getparam(g_hplayer, PARAM_VIDEO_MODE, &mode);
 mode = VIDEO_MODE_STRETCHED;
 player_setparam(g_hplayer, PARAM_VIDEO_MODE, &mode);

 PARAM_AUDIO_VOLUME
 Used to set the playback volume, different from the system volume, fanplayer has a software volume control unit from -30dB to +12dB
 Volume range: [-182, 73], -182 corresponds to -30dB, 73 corresponds to +12dB
 Special value: 0 corresponds to 0dB gain, -255 corresponds to mute, +255 corresponds to maximum gain
 int volume = -0;
 player_setparam(g_hplayer, PARAM_AUDIO_VOLUME, &volume);

 PARAM_PLAY_SPEED_VALUE
 Used to set the playback speed, fanplayer supports variable speed playback
 int speed = 150;
 player_setparam(g_hplayer, PARAM_PLAY_SPEED_VALUE, &speed);
 The parameter speed is the percentage speed, 150 means playing at 150%
 The speed has no upper limit and lower limit, it is meaningless to set to 0, it will be processed as 1% internally
 The actual limit of the playback speed is determined by the processing capacity of the processor. If the processor capacity is exceeded, the playback will be stuck.

 PARAM_PLAY_SPEED_TYPE
 Used to set the variable speed type of variable speed playback, currently supports two types:
 0-Variable speed changes, use swresample to change the sampling rate to achieve variable speed
 1-Variable speed is unchanged, using soundtouch audio processing library to achieve variable speed
 int type = 1;
 player_setparam(g_hplayer, PARAM_PLAY_SPEED_TYPE, &type);

 PARAM_VISUAL_EFFECT
 Used to specify the type of visual effects, fanplayer supports visual effects, mainly for the presentation of audio visual effects
 int mode = 0;
 player_getparam(g_hplayer, PARAM_VISUAL_EFFECT, &mode);
 mode = VISUAL_EFFECT_WAVEFORM;
 player_setparam(g_hplayer, PARAM_VISUAL_EFFECT, &mode);
 There are currently three visual effects:
     1. VISUAL_EFFECT_DISABLE-Close
     2. VISUAL_EFFECT_WAVEFORM-Waveform
     3. VISUAL_EFFECT_SPECTRUM-Spectrum
 (Note: The visual effect area is set by player_setrect)

 PARAM_AVSYNC_TIME_DIFF
 Used to set the time synchronization difference between audio and video (in milliseconds)
 int diff = 100;
 player_setparam(g_hplayer, PARAM_AVSYNC_TIME_DIFF, &diff);
 When set to 100, audio will be 100ms faster than video, and set to -100 to be 100ms slower

 PARAM_PLAYER_INIT_PARAMS
 Used to get the initialization parameters of the player, how to use it:
 PLAYER_INIT_PARAMS params;
 player_getparam(g_hplayer, PARAM_PLAYER_INIT_PARAMS, &params);
 For the meaning of the parameters, please refer to the player initialization parameter description

 All parameters can be get, but not all parameters can be set, because some parameters are read-only.

// Support for avdevice input device
Support three input devices of dshow gdigrab vfwcap on windows platform
Examples of opening methods:
player_open("vfwcap", hwnd, 0, 0, NULL); // will open the camera in vfw mode for preview
player_open("gdigrab://desktop", hwnd, 0, 0, NULL); // will open the desktop in gdigrab mode for preview
player_open("dshow://video=Integrated Camera", hwnd, 0, 0, NULL); // Will open Integrated Camera in dshow

 */

#ifdef __cplusplus
}
#endif

#endif



