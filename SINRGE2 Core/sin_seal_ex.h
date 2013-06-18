/*
** SINRGE2
** Copyright (C) 2013 Su Zhang, Shy07
** Gernischt@gmail.com
**
** SealEx Header
*/
#ifndef _SIN_SEAL_EX_H_
#define _SIN_SEAL_EX_H_
#ifdef __cplusplus
extern "C" {
#endif
// err
enum seal_err_t
{
    SEAL_OK,

    SEAL_BAD_OBJ,
    SEAL_BAD_ENUM,
    SEAL_BAD_VAL,
    SEAL_BAD_OP,

    SEAL_CANNOT_OPEN_DEVICE,
    SEAL_NO_EFX,
    SEAL_NO_EXT_FUNC,
    SEAL_BAD_DEVICE,
    SEAL_CANNOT_CREATE_CONTEXT,

    SEAL_STREAM_UNOPENED,
    SEAL_MIXING_STREAM_FMT,

    SEAL_MIXING_SRC_TYPE,

    SEAL_CANNOT_OPEN_FILE,
    SEAL_BAD_AUDIO,

    SEAL_CANNOT_ALLOC_MEM,

    SEAL_BAD_PRESET,

    SEAL_BAD_WAV_SUBTYPE,
    SEAL_BAD_WAV_CHUNK,
    SEAL_BAD_WAV_CHUNK_SIZE,
    SEAL_ABSENT_WAV_CHUNK,
    SEAL_BAD_WAV_BPS,
    SEAL_BAD_WAV_NCHANNELS,
    FILE_BAD_WAV_FREQ,
    SEAL_CANNOT_REWIND_WAV,

    SEAL_CANNOT_OPEN_OV,
    SEAL_CANNOT_GET_OV_INFO,
    SEAL_CANNOT_READ_OV,
    SEAL_CANNOT_REWIND_OV,
    SEAL_CANNOT_CLOSE_OV,

    SEAL_CANNOT_INIT_MPG,
    SEAL_CANNOT_GET_MPG_INFO,
    SEAL_CANNOT_READ_MPG,
    SEAL_CANNOT_REWIND_MPG,
    SEAL_CANNOT_CLOSE_MPG,
};

typedef enum seal_err_t seal_err_t;
#define SEAL_API
//const char * 	 seal_get_err_msg (seal_err_t err);
const char* SEAL_API seal_get_err_msg(seal_err_t);

// raw
typedef struct seal_raw_t seal_raw_t;
typedef struct seal_raw_attr_t seal_raw_attr_t;

struct seal_raw_attr_t
{
    int    bit_depth;
    int    nchannels;
    int    freq;
};

struct seal_raw_t
{
    void *          data;
    size_t          size;
    seal_raw_attr_t attr;
};

//#define SEAL_RAW_ATTR_INIT_LST { 0, 0, 0 }
//#define SEAL_RAW_INIT_LST      { 0, 0, SEAL_RAW_ATTR_INIT_LST }

// fmt
enum seal_fmt_t
{
    SEAL_UNKNOWN_FMT,
    SEAL_WAV_FMT,
    SEAL_OV_FMT,
    SEAL_MPG_FMT
};

typedef enum seal_fmt_t seal_fmt_t;

// buf
struct seal_buf_t
{
    unsigned int id;
};

typedef struct seal_buf_t seal_buf_t;

seal_err_t 		 seal_init_buf (seal_buf_t * buf);
seal_err_t 		 seal_destroy_buf (seal_buf_t * buf);
seal_err_t 		 seal_load2buf (seal_buf_t * buf, const char * filename, seal_fmt_t fmt);
seal_err_t 		 seal_raw2buf (seal_buf_t * buf, seal_raw_t * raw);
seal_err_t 		 seal_get_buf_size (seal_buf_t * buf, int * psize);
seal_err_t 		 seal_get_buf_freq (seal_buf_t * buf, int * pfreq);
seal_err_t 		 seal_get_buf_bps (seal_buf_t * buf, int * pbps);
seal_err_t 		 seal_get_buf_nchannels (seal_buf_t * buf, int * pnchannels);
seal_err_t 		 seal_load (seal_raw_t * raw, const char * filename, seal_fmt_t fmt);

// stream
struct seal_stream_t
{
    /* Tagged union of identifiers used by different decoder libraries. */
    void *          id;
    seal_fmt_t      fmt;
    seal_raw_attr_t attr;
};
typedef struct seal_stream_t seal_stream_t;
seal_err_t 		 seal_open_stream (seal_stream_t * stream, const char * filename, seal_fmt_t fmt);
seal_err_t 		 seal_stream (seal_stream_t * stream, seal_raw_t * raw, size_t * psize);
seal_err_t 		 seal_rewind_stream (seal_stream_t * stream);
seal_err_t 		 seal_close_stream (seal_stream_t * stream);

// efs
struct seal_efs_t
{
    unsigned int id;
    void *       effect;
};
typedef struct seal_efs_t seal_efs_t;

seal_err_t 		 seal_init_efs (seal_efs_t * slot);
seal_err_t 		 seal_destroy_efs (seal_efs_t * slot);
seal_err_t 		 seal_set_efs_effect (seal_efs_t * slot, void * effect);
seal_err_t 		 seal_set_efs_gain (seal_efs_t * slot, float gain);
seal_err_t 		 seal_set_efs_auto (seal_efs_t * slot, char automatic);
void * 			 seal_get_efs_effect (seal_efs_t * slot);
seal_err_t 		 seal_get_efs_gain (seal_efs_t * slot, float * pgain);
seal_err_t 		 seal_is_efs_auto (seal_efs_t * slot, char * pauto);


// core
seal_err_t 		 seal_startup (const char * device_name);
void 			 seal_cleanup (void);
int 				 seal_get_per_src_effect_limit (void);
const char * 	 seal_get_version (void);
void 			 seal_sleep (unsigned int millisec);

// src
enum seal_src_type_t
{
    SEAL_UNDETERMINED,
    SEAL_STATIC,
    SEAL_STREAMING
};

enum seal_src_state_t
{
    SEAL_INITIAL,
    SEAL_PLAYING,
    SEAL_PAUSED,
    SEAL_STOPPED
};

struct seal_src_t
{
    unsigned int    id;
    seal_buf_t *    buf;
    seal_stream_t * stream;
    void *          updater;
    size_t          chunk_size   : 24;
    size_t          queue_size   : 6;
    unsigned int    looping      : 1;
    unsigned int    automatic    : 1;
};

typedef struct seal_src_t seal_src_t;
typedef enum seal_src_type_t seal_src_type_t;
typedef enum seal_src_state_t seal_src_state_t;

seal_err_t 		 seal_init_src (seal_src_t * src);
seal_err_t 		 seal_destroy_src (seal_src_t * src);
seal_err_t 		 seal_play_src (seal_src_t * src);
seal_err_t 		 seal_pause_src (seal_src_t * src);
seal_err_t 		 seal_stop_src (seal_src_t * src);
seal_err_t 		 seal_rewind_src (seal_src_t * src);
seal_err_t 		 seal_move_src (seal_src_t * src);
seal_err_t 		 seal_set_src_buf (seal_src_t * src, seal_buf_t * buf);
seal_err_t 		 seal_set_src_stream (seal_src_t * src, seal_stream_t * stream);
seal_err_t 		 seal_feed_efs (seal_src_t * src, seal_efs_t * slot, int index);
seal_err_t 		 seal_update_src (seal_src_t * src);
seal_err_t 		 seal_detach_src_audio (seal_src_t * src);
seal_err_t 		 seal_set_src_queue_size (seal_src_t * src, size_t size);
seal_err_t 		 seal_set_src_chunk_size (seal_src_t * src, size_t size);
seal_err_t 		 seal_set_src_pos (seal_src_t * src, float x, float y, float z);
seal_err_t 		 seal_set_src_vel (seal_src_t * src, float x, float y, float z);
seal_err_t 		 seal_set_src_pitch (seal_src_t * src, float pitch);
seal_err_t 		 seal_set_src_gain (seal_src_t * src, float gain);
seal_err_t 		 seal_set_src_auto (seal_src_t * src, char automatic);
seal_err_t 		 seal_set_src_relative (seal_src_t * src, char relative);
seal_err_t 		 seal_set_src_looping (seal_src_t * src, char looping);
seal_buf_t * 	 seal_get_src_buf (seal_src_t * src);
seal_stream_t * 	 seal_get_src_stream (seal_src_t * src);
seal_err_t 		 seal_get_src_queue_size (seal_src_t * src, size_t * psize);
seal_err_t 		 seal_get_src_chunk_size (seal_src_t * src, size_t * psize);
seal_err_t 		 seal_get_src_pos (seal_src_t * src, float * px, float * py, float * pz);
seal_err_t 		 seal_get_src_vel (seal_src_t * src, float * px, float * py, float * pz);
seal_err_t 		 seal_get_src_pitch (seal_src_t * src, float * ppitch);
seal_err_t 		 seal_get_src_gain (seal_src_t * src, float * pgain);
seal_err_t 		 seal_is_src_auto (seal_src_t * src, char * pauto);
seal_err_t 		 seal_is_src_relative (seal_src_t * src, char * prelative);
seal_err_t 		 seal_is_src_looping (seal_src_t * src, char * plooping);
seal_err_t 		 seal_get_src_type (seal_src_t * src, seal_src_type_t * ptype);
seal_err_t 		 seal_get_src_state (seal_src_t * src, seal_src_state_t * pstate);

// listener
seal_err_t 		 seal_move_listener (void);
seal_err_t 		 seal_set_listener_gain (float gain);
seal_err_t 		 seal_set_listener_pos (float x, float y, float z);
seal_err_t 		 seal_set_listener_vel (float x, float y, float z);
seal_err_t 		 seal_set_listener_orien (float * orien);
seal_err_t 		 seal_get_listener_gain (float * pgain);
seal_err_t 		 seal_get_listener_pos (float * px, float * py, float * pz);
seal_err_t 		 seal_get_listener_vel (float * px, float * py, float * pz);
seal_err_t 		 seal_get_listener_orien (float * orien);

// rvb
enum seal_rvb_preset_t
{
    /* Default presets. */
    SEAL_GENERIC_REVERB,
    SEAL_PADDEDCELL_REVERB,
    SEAL_ROOM_REVERB,
    SEAL_BATHROOM_REVERB,
    SEAL_LIVINGROOM_REVERB,
    SEAL_STONEROOM_REVERB,
    SEAL_AUDITORIUM_REVERB,
    SEAL_CONCERTHALL_REVERB,
    SEAL_CAVE_REVERB,
    SEAL_ARENA_REVERB,
    SEAL_HANGAR_REVERB,
    SEAL_CARPETEDHALLWAY_REVERB,
    SEAL_HALLWAY_REVERB,
    SEAL_STONECORRIDOR_REVERB,
    SEAL_ALLEY_REVERB,
    SEAL_FOREST_REVERB,
    SEAL_CITY_REVERB,
    SEAL_MOUNTAINS_REVERB,
    SEAL_QUARRY_REVERB,
    SEAL_PLAIN_REVERB,
    SEAL_PARKINGLOT_REVERB,
    SEAL_SEWERPIPE_REVERB,
    SEAL_UNDERWATER_REVERB,
    SEAL_DRUGGED_REVERB,
    SEAL_DIZZY_REVERB,
    SEAL_PSYCHOTIC_REVERB,

    /* Castle presets. */
    SEAL_CASTLE_SMALLROOM_REVERB,
    SEAL_CASTLE_SHORTPASSAGE_REVERB,
    SEAL_CASTLE_MEDIUMROOM_REVERB,
    SEAL_CASTLE_LARGEROOM_REVERB,
    SEAL_CASTLE_LONGPASSAGE_REVERB,
    SEAL_CASTLE_HALL_REVERB,
    SEAL_CASTLE_CUPBOARD_REVERB,
    SEAL_CASTLE_COURTYARD_REVERB,
    SEAL_CASTLE_ALCOVE_REVERB,

    /* Factory presets. */
    SEAL_FACTORY_SMALLROOM_REVERB,
    SEAL_FACTORY_SHORTPASSAGE_REVERB,
    SEAL_FACTORY_MEDIUMROOM_REVERB,
    SEAL_FACTORY_LARGEROOM_REVERB,
    SEAL_FACTORY_LONGPASSAGE_REVERB,
    SEAL_FACTORY_HALL_REVERB,
    SEAL_FACTORY_CUPBOARD_REVERB,
    SEAL_FACTORY_COURTYARD_REVERB,
    SEAL_FACTORY_ALCOVE_REVERB,

    /* Ice palace presets. */
    SEAL_ICEPALACE_SMALLROOM_REVERB,
    SEAL_ICEPALACE_SHORTPASSAGE_REVERB,
    SEAL_ICEPALACE_MEDIUMROOM_REVERB,
    SEAL_ICEPALACE_LARGEROOM_REVERB,
    SEAL_ICEPALACE_LONGPASSAGE_REVERB,
    SEAL_ICEPALACE_HALL_REVERB,
    SEAL_ICEPALACE_CUPBOARD_REVERB,
    SEAL_ICEPALACE_COURTYARD_REVERB,
    SEAL_ICEPALACE_ALCOVE_REVERB,

    /* Space station presets. */
    SEAL_SPACESTATION_SMALLROOM_REVERB,
    SEAL_SPACESTATION_SHORTPASSAGE_REVERB,
    SEAL_SPACESTATION_MEDIUMROOM_REVERB,
    SEAL_SPACESTATION_LARGEROOM_REVERB,
    SEAL_SPACESTATION_LONGPASSAGE_REVERB,
    SEAL_SPACESTATION_HALL_REVERB,
    SEAL_SPACESTATION_CUPBOARD_REVERB,
    SEAL_SPACESTATION_ALCOVE_REVERB,

    /* Wooden galleon presets. */
    SEAL_WOODEN_SMALLROOM_REVERB,
    SEAL_WOODEN_SHORTPASSAGE_REVERB,
    SEAL_WOODEN_MEDIUMROOM_REVERB,
    SEAL_WOODEN_LARGEROOM_REVERB,
    SEAL_WOODEN_LONGPASSAGE_REVERB,
    SEAL_WOODEN_HALL_REVERB,
    SEAL_WOODEN_CUPBOARD_REVERB,
    SEAL_WOODEN_COURTYARD_REVERB,
    SEAL_WOODEN_ALCOVE_REVERB,

    /* Sports presets. */
    SEAL_SPORT_EMPTYSTADIUM_REVERB,
    SEAL_SPORT_SQUASHCOURT_REVERB,
    SEAL_SPORT_SMALLSWIMMINGPOOL_REVERB,
    SEAL_SPORT_LARGESWIMMINGPOOL_REVERB,
    SEAL_SPORT_GYMNASIUM_REVERB,
    SEAL_SPORT_FULLSTADIUM_REVERB,
    SEAL_SPORT_STADIUMTANNOY_REVERB,

    /* Prefab presets. */
    SEAL_PREFAB_WORKSHOP_REVERB,
    SEAL_PREFAB_SCHOOLROOM_REVERB,
    SEAL_PREFAB_PRACTISEROOM_REVERB,
    SEAL_PREFAB_OUTHOUSE_REVERB,
    SEAL_PREFAB_CARAVAN_REVERB,

    /* Dome presets. */
    SEAL_DOME_TOMB_REVERB,
    SEAL_DOME_SAINTPAULS_REVERB,

    /* Pipe presets. */
    SEAL_PIPE_SMALL_REVERB,
    SEAL_PIPE_LONGTHIN_REVERB,
    SEAL_PIPE_LARGE_REVERB,
    SEAL_PIPE_RESONANT_REVERB,

    /* Outdoors presets. */
    SEAL_OUTDOORS_BACKYARD_REVERB,
    SEAL_OUTDOORS_ROLLINGPLAINS_REVERB,
    SEAL_OUTDOORS_DEEPCANYON_REVERB,
    SEAL_OUTDOORS_CREEK_REVERB,
    SEAL_OUTDOORS_VALLEY_REVERB,

    /* Mood presets. */
    SEAL_MOOD_HEAVEN_REVERB,
    SEAL_MOOD_HELL_REVERB,
    SEAL_MOOD_MEMORY_REVERB,

    /* Driving presets. */
    SEAL_DRIVING_COMMENTATOR_REVERB,
    SEAL_DRIVING_PITGARAGE_REVERB,
    SEAL_DRIVING_INCAR_RACER_REVERB,
    SEAL_DRIVING_INCAR_SPORTS_REVERB,
    SEAL_DRIVING_INCAR_LUXURY_REVERB,
    SEAL_DRIVING_FULLGRANDSTAND_REVERB,
    SEAL_DRIVING_EMPTYGRANDSTAND_REVERB,
    SEAL_DRIVING_TUNNEL_REVERB,

    /* City presets. */
    SEAL_CITY_STREETS_REVERB,
    SEAL_CITY_SUBWAY_REVERB,
    SEAL_CITY_MUSEUM_REVERB,
    SEAL_CITY_LIBRARY_REVERB,
    SEAL_CITY_UNDERPASS_REVERB,
    SEAL_CITY_ABANDONED_REVERB,

    /* Misc. presets. */
    SEAL_DUSTYROOM_REVERB,
    SEAL_CHAPEL_REVERB,
    SEAL_SMALLWATERROOM_REVERB
};

struct seal_rvb_t
{
    unsigned int id;
};
typedef struct seal_rvb_t seal_rvb_t;
typedef enum seal_rvb_preset_t seal_rvb_preset_t;


seal_err_t 		 seal_init_rvb (seal_rvb_t * rvb);
seal_err_t 		 seal_destroy_rvb (seal_rvb_t * rvb);
seal_err_t 		 seal_load_rvb (seal_rvb_t * rvb, seal_rvb_preset_t preset);
seal_err_t 		 seal_set_rvb_density (seal_rvb_t * rvb, float density);
seal_err_t 		 seal_set_rvb_diffusion (seal_rvb_t * rvb, float diffusion);
seal_err_t 		 seal_set_rvb_gain (seal_rvb_t * rvb, float gain);
seal_err_t 		 seal_set_rvb_hfgain (seal_rvb_t * rvb, float hfgain);
seal_err_t 		 seal_set_rvb_decay_time (seal_rvb_t * rvb, float time);
seal_err_t 		 seal_set_rvb_hfdecay_ratio (seal_rvb_t * rvb, float ratio);
seal_err_t 		 seal_set_rvb_reflections_gain (seal_rvb_t * rvb, float gain);
seal_err_t 		 seal_set_rvb_reflections_delay (seal_rvb_t * rvb, float delay);
seal_err_t 		 seal_set_rvb_late_gain (seal_rvb_t * rvb, float gain);
seal_err_t 		 seal_set_rvb_late_delay (seal_rvb_t * rvb, float delay);
seal_err_t 		 seal_set_rvb_air_absorbtion_hfgain (seal_rvb_t * rvb, float hfgain);
seal_err_t 		 seal_set_rvb_room_rolloff_factor (seal_rvb_t * rvb, float factor);
seal_err_t 		 seal_set_rvb_hfdecay_limited (seal_rvb_t * rvb, char limited);
seal_err_t 		 seal_get_rvb_density (seal_rvb_t * rvb, float * pdensity);
seal_err_t 		 seal_get_rvb_diffusion (seal_rvb_t * rvb, float * pdensity);
seal_err_t 		 seal_get_rvb_gain (seal_rvb_t * rvb, float * pgain);
seal_err_t 		 seal_get_rvb_hfgain (seal_rvb_t * rvb, float * phfgain);
seal_err_t 		 seal_get_rvb_decay_time (seal_rvb_t * rvb, float * ptime);
seal_err_t 		 seal_get_rvb_hfdecay_ratio (seal_rvb_t * rvb, float * pratio);
seal_err_t 		 seal_get_rvb_reflections_gain (seal_rvb_t * rvb, float * pgain);
seal_err_t 		 seal_get_rvb_reflections_delay (seal_rvb_t * rvb, float * pdelay);
seal_err_t 		 seal_get_rvb_late_gain (seal_rvb_t * rvb, float * pgain);
seal_err_t 		 seal_get_rvb_late_delay (seal_rvb_t * rvb, float * pdelay);
seal_err_t 		 seal_get_rvb_air_absorbtion_hfgain (seal_rvb_t * rvb, float * phfgain);
seal_err_t 		 seal_get_rvb_room_rolloff_factor (seal_rvb_t * rvb, float * pfactor);
seal_err_t 		 seal_is_rvb_hfdecay_limited (seal_rvb_t * rvb, char * plimited);

#ifdef __cplusplus
}
#endif
#endif // _SIN_SEAL_EX_H_