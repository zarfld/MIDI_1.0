/**
 * @file gm1_sound_quality.h
 * @brief GM1 Sound Quality and Timbre Standards Interface
 * 
 * Implements: #540 (REQ-F-GM1-004: GM1 Sound Quality and Timbre Standards)
 * Depends on: #537-539 (GM1 instruments, percussion, program changes)
 * Architecture: GM1 Sound Quality Module
 * Verified by: TEST-GM1-SOUND-QUALITY-*
 * 
 * @see https://github.com/zarfld/MIDI_1.0/issues/540
 * 
 * This file defines the interface for GM1 sound quality validation,
 * controller response processing, and timbre standards compliance
 * per RP-003: General MIDI System Level 1 Specification.
 */

#ifndef GM1_SOUND_QUALITY_H
#define GM1_SOUND_QUALITY_H

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================
 * Constants and Macros
 *============================================================================*/

/** @defgroup gm1_quality_constants GM1 Quality Constants
 * @{
 */

/** Minimum recognition score for instrument category (80% per GM1 spec) */
#define GM1_MIN_RECOGNITION_SCORE       0.8f

/** Maximum tuning deviation in cents (±10 cents acceptable) */
#define GM1_MAX_TUNING_DEVIATION_CENTS  10.0f

/** Maximum program change latency in milliseconds */
#define GM1_MAX_PROGRAM_SWITCH_MS       100

/** Minimum velocity layers per program (GM1 baseline) */
#define GM1_MIN_VELOCITY_LAYERS         2

/** Minimum sample rate in Hz (GM1 baseline) */
#define GM1_MIN_SAMPLE_RATE_HZ          22050

/** Recommended sample rate in Hz (CD quality) */
#define GM1_RECOMMENDED_SAMPLE_RATE_HZ  44100

/** Minimum bit depth (GM1 baseline) */
#define GM1_MIN_BIT_DEPTH               8

/** Recommended bit depth (CD quality) */
#define GM1_RECOMMENDED_BIT_DEPTH       16

/** Minimum dynamic range in dB (GM1 baseline) */
#define GM1_MIN_DYNAMIC_RANGE_DB        48.0f

/** Recommended dynamic range in dB (professional quality) */
#define GM1_RECOMMENDED_DYNAMIC_RANGE_DB 72.0f

/** Minimum polyphony (GM1 baseline) */
#define GM1_MIN_POLYPHONY               24

/** Maximum acceptable dynamic range difference in dB for cross-device consistency */
#define GM1_MAX_DYNAMIC_RANGE_DIFF_DB   12.0f

/** @} */ /* end of gm1_quality_constants */

/*============================================================================
 * GM1 Instrument Categories
 *============================================================================*/

/** @defgroup gm1_categories GM1 Instrument Categories
 * @{
 */

/**
 * @brief GM1 instrument categories for timbre classification
 * 
 * These categories define the expected modulation wheel behavior
 * and timbre characteristics for each instrument family.
 */
typedef enum {
    GM1_CATEGORY_PIANO = 0,         /**< Programs 1-8: Piano family */
    GM1_CATEGORY_CHROMATIC_PERC,    /**< Programs 9-16: Chromatic percussion */
    GM1_CATEGORY_ORGAN,             /**< Programs 17-24: Organ family */
    GM1_CATEGORY_GUITAR,            /**< Programs 25-32: Guitar family */
    GM1_CATEGORY_BASS,              /**< Programs 33-40: Bass instruments */
    GM1_CATEGORY_STRINGS,           /**< Programs 41-48: String instruments */
    GM1_CATEGORY_ENSEMBLE,          /**< Programs 49-56: Ensemble sounds */
    GM1_CATEGORY_BRASS,             /**< Programs 57-64: Brass instruments */
    GM1_CATEGORY_REED,              /**< Programs 65-72: Reed instruments */
    GM1_CATEGORY_PIPE,              /**< Programs 73-80: Pipe instruments */
    GM1_CATEGORY_SYNTH_LEAD,        /**< Programs 81-88: Synth lead */
    GM1_CATEGORY_SYNTH_PAD,         /**< Programs 89-96: Synth pad */
    GM1_CATEGORY_SYNTH_EFFECTS,     /**< Programs 97-104: Synth effects */
    GM1_CATEGORY_ETHNIC,            /**< Programs 105-112: Ethnic instruments */
    GM1_CATEGORY_PERCUSSIVE,        /**< Programs 113-120: Percussive sounds */
    GM1_CATEGORY_SOUND_EFFECTS,     /**< Programs 121-128: Sound effects */
    GM1_CATEGORY_PERCUSSION,        /**< Channel 10 percussion */
    GM1_CATEGORY_UNKNOWN            /**< Unknown/invalid category */
} gm1_instrument_category_t;

/** @} */ /* end of gm1_categories */

/*============================================================================
 * Quality Metrics Structures
 *============================================================================*/

/** @defgroup gm1_quality_structs GM1 Quality Structures
 * @{
 */

/**
 * @brief Quality metrics for a GM1 program
 * 
 * Used for quality assurance testing of GM1 implementations.
 * Recognition score is determined by human listening tests.
 */
typedef struct {
    uint8_t program;                /**< GM1 program number (1-128) */
    float   recognition_score;      /**< 0.0 - 1.0 (human listening test) */
    bool    controller_response;    /**< CC#1, CC#7 work as expected */
    bool    velocity_sensitive;     /**< Notes respond to velocity */
    float   frequency_accuracy;     /**< Tuning accuracy (cents deviation) */
} gm1_quality_metrics_t;

/**
 * @brief Velocity response configuration for a GM1 program
 * 
 * Defines how a program responds to Note On velocity values.
 */
typedef struct {
    uint8_t  velocity_layers;       /**< Number of velocity layers (2 minimum) */
    float    velocity_curve;        /**< Response curve (0.5 = linear, 1.0 = exponential) */
    bool     velocity_affects_timbre; /**< Velocity changes brightness/character */
} gm1_velocity_config_t;

/**
 * @brief Audio quality specifications for GM1 implementation
 * 
 * Defines the technical quality parameters of the implementation.
 */
typedef struct {
    uint32_t sample_rate_hz;        /**< Sample rate in Hz (22050 min, 44100 recommended) */
    uint8_t  bit_depth;             /**< Bit depth (8 min, 16 recommended) */
    float    frequency_response_min_hz;  /**< Minimum frequency (20 Hz recommended) */
    float    frequency_response_max_hz;  /**< Maximum frequency (20000 Hz recommended) */
    float    dynamic_range_db;      /**< Dynamic range in dB (48 min, 72 recommended) */
    uint8_t  polyphony_voices;      /**< Number of simultaneous voices (24 minimum) */
} gm1_audio_quality_t;

/**
 * @brief Implementation quality compliance result
 */
typedef struct {
    bool    meets_minimum;          /**< Meets GM1 minimum requirements */
    bool    meets_recommended;      /**< Meets GM1 recommended quality */
    char    issues[256];            /**< Description of any compliance issues */
} gm1_compliance_result_t;

/** @} */ /* end of gm1_quality_structs */

/*============================================================================
 * Function Prototypes - Category Classification
 *============================================================================*/

/** @defgroup gm1_category_funcs GM1 Category Functions
 * @{
 */

/**
 * @brief Get the GM1 instrument category for a program number
 * 
 * @param program GM1 program number (1-128)
 * @return gm1_instrument_category_t The category of the program
 */
gm1_instrument_category_t gm1_get_instrument_category(uint8_t program);

/**
 * @brief Get the name of a GM1 instrument category
 * 
 * @param category The instrument category
 * @return const char* Human-readable category name
 */
const char* gm1_get_category_name(gm1_instrument_category_t category);

/** @} */ /* end of gm1_category_funcs */

/*============================================================================
 * Function Prototypes - Controller Response
 *============================================================================*/

/** @defgroup gm1_controller_funcs GM1 Controller Functions
 * @{
 */

/**
 * @brief Handle modulation wheel (CC#1) for GM1 instruments
 * 
 * Applies musically appropriate modulation based on instrument category:
 * - Strings: Vibrato depth
 * - Brass: Vibrato + brightness
 * - Synth leads: LFO depth/speed
 * - Percussion: Minimal or no effect
 * 
 * @param channel MIDI channel (0-15)
 * @param value Modulation wheel value (0-127)
 */
void gm1_handle_modulation_wheel(uint8_t channel, uint8_t value);

/**
 * @brief Handle volume controller (CC#7) for GM1 instruments
 * 
 * Provides smooth logarithmic volume control:
 * - CC#7 = 0: Silence (or very quiet)
 * - CC#7 = 64: Nominal volume (-6 dB)
 * - CC#7 = 127: Maximum volume (0 dB)
 * 
 * @param channel MIDI channel (0-15)
 * @param value Volume controller value (0-127)
 */
void gm1_handle_volume_controller(uint8_t channel, uint8_t value);

/**
 * @brief Convert MIDI value (0-127) to decibels
 * 
 * Uses logarithmic scaling for natural volume perception:
 * - Value 0: -60 dB (near silence)
 * - Value 64: approximately -6 dB
 * - Value 127: 0 dB (maximum)
 * 
 * @param midi_value MIDI value (0-127)
 * @return float Volume in decibels
 */
float gm1_midi_value_to_db(uint8_t midi_value);

/** @} */ /* end of gm1_controller_funcs */

/*============================================================================
 * Function Prototypes - Quality Validation
 *============================================================================*/

/** @defgroup gm1_quality_funcs GM1 Quality Functions
 * @{
 */

/**
 * @brief Validate GM1 program quality against standards
 * 
 * Checks:
 * - Recognition score >= 80%
 * - Controller response working
 * - Velocity sensitivity present
 * - Tuning accuracy within ±10 cents
 * 
 * @param metrics Pointer to quality metrics structure
 * @return bool true if program meets GM1 quality standards
 */
bool gm1_validate_program_quality(const gm1_quality_metrics_t* metrics);

/**
 * @brief Validate audio quality specifications against GM1 requirements
 * 
 * @param quality Pointer to audio quality specifications
 * @return gm1_compliance_result_t Compliance result with details
 */
gm1_compliance_result_t gm1_validate_audio_quality(const gm1_audio_quality_t* quality);

/**
 * @brief Configure velocity response for a GM1 program
 * 
 * All GM1 programs must respond to velocity with minimum 2 layers.
 * 
 * @param program GM1 program number (1-128)
 * @param config Pointer to velocity configuration
 * @return bool true if configuration applied successfully
 */
bool gm1_configure_program_velocity(uint8_t program, const gm1_velocity_config_t* config);

/**
 * @brief Test program switching latency
 * 
 * GM1 requires program changes to complete within 100ms.
 * 
 * @param from_program Source program (1-128)
 * @param to_program Target program (1-128)
 * @return uint32_t Measured latency in milliseconds
 */
uint32_t gm1_test_program_switch_latency(uint8_t from_program, uint8_t to_program);

/**
 * @brief Check if program switch latency is GM1 compliant
 * 
 * @param latency_ms Measured latency in milliseconds
 * @return bool true if latency is within GM1 requirements (<100ms)
 */
static inline bool gm1_is_program_switch_compliant(uint32_t latency_ms) {
    return latency_ms < GM1_MAX_PROGRAM_SWITCH_MS;
}

/** @} */ /* end of gm1_quality_funcs */

/*============================================================================
 * Function Prototypes - Cross-Device Consistency
 *============================================================================*/

/** @defgroup gm1_consistency_funcs GM1 Consistency Functions
 * @{
 */

/**
 * @brief Calculate cross-device consistency score
 * 
 * Compares playback characteristics between two GM1 implementations.
 * 
 * @param device1_metrics Quality metrics from device 1
 * @param device2_metrics Quality metrics from device 2
 * @return float Consistency score (0.0-1.0, higher is more consistent)
 */
float gm1_calculate_consistency_score(
    const gm1_quality_metrics_t* device1_metrics,
    const gm1_quality_metrics_t* device2_metrics
);

/**
 * @brief Check if two implementations have acceptable consistency
 * 
 * @param consistency_score Score from gm1_calculate_consistency_score()
 * @return bool true if implementations are acceptably consistent
 */
static inline bool gm1_is_consistent(float consistency_score) {
    return consistency_score >= GM1_MIN_RECOGNITION_SCORE;
}

/** @} */ /* end of gm1_consistency_funcs */

#ifdef __cplusplus
}
#endif

#endif /* GM1_SOUND_QUALITY_H */
