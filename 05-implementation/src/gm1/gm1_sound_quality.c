/**
 * @file gm1_sound_quality.c
 * @brief GM1 Sound Quality and Timbre Standards Implementation
 * 
 * Implements: #540 (REQ-F-GM1-004: GM1 Sound Quality and Timbre Standards)
 * Depends on: #537-539 (GM1 instruments, percussion, program changes)
 * Architecture: GM1 Sound Quality Module
 * Verified by: TEST-GM1-SOUND-QUALITY-*
 * 
 * @see https://github.com/zarfld/MIDI_1.0/issues/540
 * 
 * This file implements GM1 sound quality validation, controller response
 * processing, and timbre standards compliance per RP-003.
 */

#include "gm1_sound_quality.h"
#include <string.h>
#include <stdio.h>

/*============================================================================
 * Internal State (Platform-specific implementations would provide these)
 *============================================================================*/

/* Channel program state (would be provided by GM1 core module) */
static uint8_t channel_programs[16] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1  /* Channel 10 (index 9) is percussion */
};

/* Channel volume state (normalized 0.0-1.0) */
static float channel_volumes[16] = {
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f
};

/* Modulation depth state (0.0-1.0) */
static float channel_modulation[16] = {0};

/*============================================================================
 * Category Classification Implementation
 *============================================================================*/

gm1_instrument_category_t gm1_get_instrument_category(uint8_t program) {
    /* GM1 programs are numbered 1-128 */
    if (program < 1 || program > 128) {
        return GM1_CATEGORY_UNKNOWN;
    }
    
    /* Each category contains 8 programs */
    uint8_t category_index = (program - 1) / 8;
    
    switch (category_index) {
        case 0:  return GM1_CATEGORY_PIANO;           /* Programs 1-8 */
        case 1:  return GM1_CATEGORY_CHROMATIC_PERC;  /* Programs 9-16 */
        case 2:  return GM1_CATEGORY_ORGAN;           /* Programs 17-24 */
        case 3:  return GM1_CATEGORY_GUITAR;          /* Programs 25-32 */
        case 4:  return GM1_CATEGORY_BASS;            /* Programs 33-40 */
        case 5:  return GM1_CATEGORY_STRINGS;         /* Programs 41-48 */
        case 6:  return GM1_CATEGORY_ENSEMBLE;        /* Programs 49-56 */
        case 7:  return GM1_CATEGORY_BRASS;           /* Programs 57-64 */
        case 8:  return GM1_CATEGORY_REED;            /* Programs 65-72 */
        case 9:  return GM1_CATEGORY_PIPE;            /* Programs 73-80 */
        case 10: return GM1_CATEGORY_SYNTH_LEAD;      /* Programs 81-88 */
        case 11: return GM1_CATEGORY_SYNTH_PAD;       /* Programs 89-96 */
        case 12: return GM1_CATEGORY_SYNTH_EFFECTS;   /* Programs 97-104 */
        case 13: return GM1_CATEGORY_ETHNIC;          /* Programs 105-112 */
        case 14: return GM1_CATEGORY_PERCUSSIVE;      /* Programs 113-120 */
        case 15: return GM1_CATEGORY_SOUND_EFFECTS;   /* Programs 121-128 */
        default: return GM1_CATEGORY_UNKNOWN;
    }
}

const char* gm1_get_category_name(gm1_instrument_category_t category) {
    static const char* category_names[] = {
        "Piano",
        "Chromatic Percussion",
        "Organ",
        "Guitar",
        "Bass",
        "Strings",
        "Ensemble",
        "Brass",
        "Reed",
        "Pipe",
        "Synth Lead",
        "Synth Pad",
        "Synth Effects",
        "Ethnic",
        "Percussive",
        "Sound Effects",
        "Percussion (Channel 10)",
        "Unknown"
    };
    
    if (category <= GM1_CATEGORY_UNKNOWN) {
        return category_names[category];
    }
    return "Invalid";
}

/*============================================================================
 * Controller Response Implementation
 *============================================================================*/

/**
 * @brief Set vibrato depth for a channel (platform-specific stub)
 */
static void set_vibrato_depth(uint8_t channel, float depth) {
    /* Platform-specific implementation would go here */
    if (channel < 16) {
        channel_modulation[channel] = depth;
    }
}

/**
 * @brief Set brightness for a channel (platform-specific stub)
 */
static void set_brightness(uint8_t channel, float brightness) {
    /* Platform-specific implementation would go here */
    (void)channel;
    (void)brightness;
}

/**
 * @brief Set LFO depth for a channel (platform-specific stub)
 */
static void set_lfo_depth(uint8_t channel, float depth) {
    /* Platform-specific implementation would go here */
    (void)channel;
    (void)depth;
}

/**
 * @brief Set channel volume (platform-specific stub)
 */
static void set_channel_volume_internal(uint8_t channel, float volume_db) {
    /* Convert dB to linear and store */
    if (channel < 16) {
        /* Convert from dB to linear (0 dB = 1.0, -60 dB ≈ 0.001) */
        channel_volumes[channel] = powf(10.0f, volume_db / 20.0f);
    }
}

/**
 * @brief Get current program for a channel
 */
static uint8_t get_channel_program(uint8_t channel) {
    if (channel < 16) {
        return channel_programs[channel];
    }
    return 1; /* Default to program 1 */
}

void gm1_handle_modulation_wheel(uint8_t channel, uint8_t value) {
    /* Validate channel */
    if (channel > 15) {
        return;
    }
    
    /* Normalize value to 0.0-1.0 range */
    float normalized_value = (float)value / 127.0f;
    
    /* Get current program for channel */
    uint8_t program = get_channel_program(channel);
    
    /* Get instrument category */
    gm1_instrument_category_t category = gm1_get_instrument_category(program);
    
    /* Apply modulation based on category (per GM1 spec) */
    switch (category) {
        case GM1_CATEGORY_STRINGS:
        case GM1_CATEGORY_ENSEMBLE:
            /* Strings/Ensemble: Modulation wheel → Vibrato depth */
            set_vibrato_depth(channel, normalized_value);
            break;
            
        case GM1_CATEGORY_BRASS:
        case GM1_CATEGORY_REED:
            /* Brass/Reed: Modulation wheel → Vibrato + brightness */
            set_vibrato_depth(channel, normalized_value);
            set_brightness(channel, normalized_value * 0.5f);
            break;
            
        case GM1_CATEGORY_SYNTH_LEAD:
            /* Synth leads: Modulation wheel → LFO depth/speed */
            set_lfo_depth(channel, normalized_value);
            break;
            
        case GM1_CATEGORY_SYNTH_PAD:
            /* Synth pads: Subtle vibrato + filter modulation */
            set_vibrato_depth(channel, normalized_value * 0.3f);
            set_lfo_depth(channel, normalized_value * 0.5f);
            break;
            
        case GM1_CATEGORY_PERCUSSION:
        case GM1_CATEGORY_PERCUSSIVE:
            /* Percussion: Modulation wheel minimal effect or none */
            break;
            
        case GM1_CATEGORY_PIANO:
        case GM1_CATEGORY_CHROMATIC_PERC:
            /* Piano/Chromatic percussion: Very subtle effect */
            set_vibrato_depth(channel, normalized_value * 0.2f);
            break;
            
        case GM1_CATEGORY_ORGAN:
            /* Organ: Leslie speed or vibrato */
            set_vibrato_depth(channel, normalized_value * 0.7f);
            break;
            
        case GM1_CATEGORY_GUITAR:
        case GM1_CATEGORY_BASS:
            /* Guitar/Bass: Subtle vibrato */
            set_vibrato_depth(channel, normalized_value * 0.4f);
            break;
            
        case GM1_CATEGORY_PIPE:
            /* Pipe instruments: Breath vibrato */
            set_vibrato_depth(channel, normalized_value * 0.6f);
            break;
            
        case GM1_CATEGORY_ETHNIC:
            /* Ethnic instruments: Moderate vibrato */
            set_vibrato_depth(channel, normalized_value * 0.5f);
            break;
            
        case GM1_CATEGORY_SYNTH_EFFECTS:
        case GM1_CATEGORY_SOUND_EFFECTS:
            /* Effects: Variable LFO modulation */
            set_lfo_depth(channel, normalized_value * 0.8f);
            break;
            
        default:
            /* General case: Subtle vibrato */
            set_vibrato_depth(channel, normalized_value * 0.5f);
            break;
    }
}

void gm1_handle_volume_controller(uint8_t channel, uint8_t value) {
    /* Validate channel */
    if (channel > 15) {
        return;
    }
    
    /* Convert MIDI value to dB and set volume */
    float volume_db = gm1_midi_value_to_db(value);
    set_channel_volume_internal(channel, volume_db);
}

float gm1_midi_value_to_db(uint8_t midi_value) {
    /* Handle silence case */
    if (midi_value == 0) {
        return -60.0f;  /* Near silence */
    }
    
    /* Logarithmic scaling: 0 dB at 127, approximately -6 dB at 64 */
    return 20.0f * log10f((float)midi_value / 127.0f);
}

/*============================================================================
 * Quality Validation Implementation
 *============================================================================*/

bool gm1_validate_program_quality(const gm1_quality_metrics_t* metrics) {
    if (metrics == NULL) {
        return false;
    }
    
    /* Check recognition score (80% minimum per GM1 spec) */
    if (metrics->recognition_score < GM1_MIN_RECOGNITION_SCORE) {
        return false;
    }
    
    /* Check controller response */
    if (!metrics->controller_response) {
        return false;
    }
    
    /* Check velocity sensitivity */
    if (!metrics->velocity_sensitive) {
        return false;
    }
    
    /* Check tuning accuracy (±10 cents maximum deviation) */
    if (fabsf(metrics->frequency_accuracy) > GM1_MAX_TUNING_DEVIATION_CENTS) {
        return false;
    }
    
    return true;
}

gm1_compliance_result_t gm1_validate_audio_quality(const gm1_audio_quality_t* quality) {
    gm1_compliance_result_t result = {
        .meets_minimum = true,
        .meets_recommended = true,
        .issues = {0}
    };
    
    if (quality == NULL) {
        result.meets_minimum = false;
        result.meets_recommended = false;
        snprintf(result.issues, sizeof(result.issues), "NULL quality specification");
        return result;
    }
    
    char* issues_ptr = result.issues;
    size_t remaining = sizeof(result.issues);
    
    /* Check sample rate */
    if (quality->sample_rate_hz < GM1_MIN_SAMPLE_RATE_HZ) {
        result.meets_minimum = false;
        int written = snprintf(issues_ptr, remaining, 
            "Sample rate %u Hz below minimum %u Hz. ",
            quality->sample_rate_hz, GM1_MIN_SAMPLE_RATE_HZ);
        if (written > 0 && (size_t)written < remaining) {
            issues_ptr += written;
            remaining -= written;
        }
    } else if (quality->sample_rate_hz < GM1_RECOMMENDED_SAMPLE_RATE_HZ) {
        result.meets_recommended = false;
    }
    
    /* Check bit depth */
    if (quality->bit_depth < GM1_MIN_BIT_DEPTH) {
        result.meets_minimum = false;
        int written = snprintf(issues_ptr, remaining,
            "Bit depth %u below minimum %u. ",
            quality->bit_depth, GM1_MIN_BIT_DEPTH);
        if (written > 0 && (size_t)written < remaining) {
            issues_ptr += written;
            remaining -= written;
        }
    } else if (quality->bit_depth < GM1_RECOMMENDED_BIT_DEPTH) {
        result.meets_recommended = false;
    }
    
    /* Check dynamic range */
    if (quality->dynamic_range_db < GM1_MIN_DYNAMIC_RANGE_DB) {
        result.meets_minimum = false;
        int written = snprintf(issues_ptr, remaining,
            "Dynamic range %.1f dB below minimum %.1f dB. ",
            quality->dynamic_range_db, GM1_MIN_DYNAMIC_RANGE_DB);
        if (written > 0 && (size_t)written < remaining) {
            issues_ptr += written;
            remaining -= written;
        }
    } else if (quality->dynamic_range_db < GM1_RECOMMENDED_DYNAMIC_RANGE_DB) {
        result.meets_recommended = false;
    }
    
    /* Check polyphony */
    if (quality->polyphony_voices < GM1_MIN_POLYPHONY) {
        result.meets_minimum = false;
        int written = snprintf(issues_ptr, remaining,
            "Polyphony %u voices below minimum %u. ",
            quality->polyphony_voices, GM1_MIN_POLYPHONY);
        if (written > 0 && (size_t)written < remaining) {
            issues_ptr += written;
            remaining -= written;
        }
    }
    
    /* Check frequency response */
    if (quality->frequency_response_min_hz > 20.0f) {
        result.meets_recommended = false;
    }
    if (quality->frequency_response_max_hz < 11000.0f) {
        result.meets_minimum = false;
        snprintf(issues_ptr, remaining,
            "Max frequency %.0f Hz below minimum 11000 Hz. ",
            quality->frequency_response_max_hz);
    } else if (quality->frequency_response_max_hz < 20000.0f) {
        result.meets_recommended = false;
    }
    
    return result;
}

bool gm1_configure_program_velocity(uint8_t program, const gm1_velocity_config_t* config) {
    /* Validate program number */
    if (program < 1 || program > 128) {
        return false;
    }
    
    if (config == NULL) {
        return false;
    }
    
    /* Validate velocity layers (minimum 2 per GM1 spec) */
    if (config->velocity_layers < GM1_MIN_VELOCITY_LAYERS) {
        return false;
    }
    
    /* Validate velocity curve (must be positive) */
    if (config->velocity_curve <= 0.0f || config->velocity_curve > 2.0f) {
        return false;
    }
    
    /* Platform-specific implementation would configure the actual synthesis engine here */
    /* For now, return success if validation passed */
    
    return true;
}

uint32_t gm1_test_program_switch_latency(uint8_t from_program, uint8_t to_program) {
    /* Validate program numbers */
    if (from_program < 1 || from_program > 128 ||
        to_program < 1 || to_program > 128) {
        return UINT32_MAX; /* Invalid */
    }
    
    /* Platform-specific implementation would measure actual latency here */
    /* For simulation/testing purposes, return a typical value */
    
    /* Simulated latency based on complexity difference */
    gm1_instrument_category_t from_cat = gm1_get_instrument_category(from_program);
    gm1_instrument_category_t to_cat = gm1_get_instrument_category(to_program);
    
    uint32_t base_latency = 10; /* Base 10ms */
    
    /* Category change adds complexity */
    if (from_cat != to_cat) {
        base_latency += 20;
    }
    
    /* Some categories have larger sample sets */
    if (to_cat == GM1_CATEGORY_PIANO || to_cat == GM1_CATEGORY_STRINGS) {
        base_latency += 15;
    }
    
    return base_latency; /* Should be < 100ms per GM1 spec */
}

/*============================================================================
 * Cross-Device Consistency Implementation
 *============================================================================*/

float gm1_calculate_consistency_score(
    const gm1_quality_metrics_t* device1_metrics,
    const gm1_quality_metrics_t* device2_metrics
) {
    if (device1_metrics == NULL || device2_metrics == NULL) {
        return 0.0f;
    }
    
    /* Different programs can't be compared */
    if (device1_metrics->program != device2_metrics->program) {
        return 0.0f;
    }
    
    float score = 0.0f;
    float weights_sum = 0.0f;
    
    /* Recognition score comparison (weight: 40%) */
    float recognition_diff = fabsf(device1_metrics->recognition_score - 
                                   device2_metrics->recognition_score);
    float recognition_score = 1.0f - recognition_diff;
    score += recognition_score * 0.4f;
    weights_sum += 0.4f;
    
    /* Controller response match (weight: 20%) */
    if (device1_metrics->controller_response == device2_metrics->controller_response) {
        score += 0.2f;
    }
    weights_sum += 0.2f;
    
    /* Velocity sensitivity match (weight: 20%) */
    if (device1_metrics->velocity_sensitive == device2_metrics->velocity_sensitive) {
        score += 0.2f;
    }
    weights_sum += 0.2f;
    
    /* Tuning accuracy comparison (weight: 20%) */
    float tuning_diff = fabsf(device1_metrics->frequency_accuracy - 
                             device2_metrics->frequency_accuracy);
    /* Normalize: 0 cents diff = 1.0 score, 20 cents diff = 0.0 score */
    float tuning_score = fmaxf(0.0f, 1.0f - (tuning_diff / 20.0f));
    score += tuning_score * 0.2f;
    weights_sum += 0.2f;
    
    /* Normalize by weights (should equal 1.0) */
    return score / weights_sum;
}
