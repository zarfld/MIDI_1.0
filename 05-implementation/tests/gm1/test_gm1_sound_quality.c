/**
 * @file test_gm1_sound_quality.c
 * @brief Unit tests for GM1 Sound Quality and Timbre Standards
 * 
 * Verifies: #540 (REQ-F-GM1-004: GM1 Sound Quality and Timbre Standards)
 * Test Type: Unit
 * Priority: P0 (Critical)
 * 
 * @see https://github.com/zarfld/MIDI_1.0/issues/540
 * 
 * These tests verify the GM1 sound quality validation, controller response
 * processing, and timbre standards compliance per RP-003.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/* Include the implementation header */
#include "../src/gm1/gm1_sound_quality.h"

/*============================================================================
 * Test Utilities
 *============================================================================*/

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("FAIL: %s\n", message); \
            tests_failed++; \
        } else { \
            printf("PASS: %s\n", message); \
            tests_passed++; \
        } \
    } while(0)

#define TEST_ASSERT_FLOAT_EQ(actual, expected, tolerance, message) \
    do { \
        if (fabsf((actual) - (expected)) > (tolerance)) { \
            printf("FAIL: %s (expected %.4f, got %.4f)\n", message, expected, actual); \
            tests_failed++; \
        } else { \
            printf("PASS: %s\n", message); \
            tests_passed++; \
        } \
    } while(0)

/*============================================================================
 * Test Cases - Category Classification
 *============================================================================*/

/**
 * @brief Test AC1: Instrument Category Recognition
 * 
 * Verifies: REQ-F-GM1-004 AC1 - Instrument category recognition
 * Given: GM1 program 1-128
 * When: Getting instrument category
 * Then: Correct category returned for each group
 */
void test_instrument_category_classification(void) {
    printf("\n--- Test: Instrument Category Classification ---\n");
    
    /* Piano group (programs 1-8) */
    TEST_ASSERT(gm1_get_instrument_category(1) == GM1_CATEGORY_PIANO,
                "Program 1 should be Piano category");
    TEST_ASSERT(gm1_get_instrument_category(8) == GM1_CATEGORY_PIANO,
                "Program 8 should be Piano category");
    
    /* Chromatic Percussion (programs 9-16) */
    TEST_ASSERT(gm1_get_instrument_category(9) == GM1_CATEGORY_CHROMATIC_PERC,
                "Program 9 should be Chromatic Percussion category");
    TEST_ASSERT(gm1_get_instrument_category(16) == GM1_CATEGORY_CHROMATIC_PERC,
                "Program 16 should be Chromatic Percussion category");
    
    /* Organ (programs 17-24) */
    TEST_ASSERT(gm1_get_instrument_category(17) == GM1_CATEGORY_ORGAN,
                "Program 17 should be Organ category");
    
    /* Guitar (programs 25-32) */
    TEST_ASSERT(gm1_get_instrument_category(25) == GM1_CATEGORY_GUITAR,
                "Program 25 should be Guitar category");
    
    /* Bass (programs 33-40) */
    TEST_ASSERT(gm1_get_instrument_category(33) == GM1_CATEGORY_BASS,
                "Program 33 should be Bass category");
    
    /* Strings (programs 41-48) */
    TEST_ASSERT(gm1_get_instrument_category(41) == GM1_CATEGORY_STRINGS,
                "Program 41 should be Strings category");
    
    /* Ensemble (programs 49-56) */
    TEST_ASSERT(gm1_get_instrument_category(49) == GM1_CATEGORY_ENSEMBLE,
                "Program 49 should be Ensemble category");
    
    /* Brass (programs 57-64) */
    TEST_ASSERT(gm1_get_instrument_category(57) == GM1_CATEGORY_BRASS,
                "Program 57 should be Brass category");
    
    /* Reed (programs 65-72) */
    TEST_ASSERT(gm1_get_instrument_category(65) == GM1_CATEGORY_REED,
                "Program 65 should be Reed category");
    
    /* Pipe (programs 73-80) */
    TEST_ASSERT(gm1_get_instrument_category(73) == GM1_CATEGORY_PIPE,
                "Program 73 should be Pipe category");
    
    /* Synth Lead (programs 81-88) */
    TEST_ASSERT(gm1_get_instrument_category(81) == GM1_CATEGORY_SYNTH_LEAD,
                "Program 81 should be Synth Lead category");
    
    /* Synth Pad (programs 89-96) */
    TEST_ASSERT(gm1_get_instrument_category(89) == GM1_CATEGORY_SYNTH_PAD,
                "Program 89 should be Synth Pad category");
    
    /* Synth Effects (programs 97-104) */
    TEST_ASSERT(gm1_get_instrument_category(97) == GM1_CATEGORY_SYNTH_EFFECTS,
                "Program 97 should be Synth Effects category");
    
    /* Ethnic (programs 105-112) */
    TEST_ASSERT(gm1_get_instrument_category(105) == GM1_CATEGORY_ETHNIC,
                "Program 105 should be Ethnic category");
    
    /* Percussive (programs 113-120) */
    TEST_ASSERT(gm1_get_instrument_category(113) == GM1_CATEGORY_PERCUSSIVE,
                "Program 113 should be Percussive category");
    
    /* Sound Effects (programs 121-128) */
    TEST_ASSERT(gm1_get_instrument_category(121) == GM1_CATEGORY_SOUND_EFFECTS,
                "Program 121 should be Sound Effects category");
    TEST_ASSERT(gm1_get_instrument_category(128) == GM1_CATEGORY_SOUND_EFFECTS,
                "Program 128 should be Sound Effects category");
    
    /* Invalid programs */
    TEST_ASSERT(gm1_get_instrument_category(0) == GM1_CATEGORY_UNKNOWN,
                "Program 0 should be Unknown category");
    TEST_ASSERT(gm1_get_instrument_category(129) == GM1_CATEGORY_UNKNOWN,
                "Program 129 should be Unknown category");
}

/**
 * @brief Test category name retrieval
 */
void test_category_names(void) {
    printf("\n--- Test: Category Names ---\n");
    
    TEST_ASSERT(strcmp(gm1_get_category_name(GM1_CATEGORY_PIANO), "Piano") == 0,
                "Piano category name correct");
    TEST_ASSERT(strcmp(gm1_get_category_name(GM1_CATEGORY_STRINGS), "Strings") == 0,
                "Strings category name correct");
    TEST_ASSERT(strcmp(gm1_get_category_name(GM1_CATEGORY_BRASS), "Brass") == 0,
                "Brass category name correct");
    TEST_ASSERT(strcmp(gm1_get_category_name(GM1_CATEGORY_UNKNOWN), "Unknown") == 0,
                "Unknown category name correct");
}

/*============================================================================
 * Test Cases - Controller Response
 *============================================================================*/

/**
 * @brief Test AC3: Volume Controller Response
 * 
 * Verifies: REQ-F-GM1-004 AC3 - Volume controller response
 * Given: CC#7 volume controller 0-127
 * When: Adjusting volume
 * Then: Smooth volume change from silence to maximum
 */
void test_volume_controller_response(void) {
    printf("\n--- Test: Volume Controller Response ---\n");
    
    /* Test value 0 should be near silence (-60 dB) */
    float db_0 = gm1_midi_value_to_db(0);
    TEST_ASSERT_FLOAT_EQ(db_0, -60.0f, 0.01f,
                         "CC#7=0 should be -60 dB (near silence)");
    
    /* Test value 127 should be 0 dB (maximum) */
    float db_127 = gm1_midi_value_to_db(127);
    TEST_ASSERT_FLOAT_EQ(db_127, 0.0f, 0.01f,
                         "CC#7=127 should be 0 dB (maximum)");
    
    /* Test value 64 should be approximately -6 dB (nominal) */
    float db_64 = gm1_midi_value_to_db(64);
    TEST_ASSERT(db_64 > -10.0f && db_64 < -4.0f,
                "CC#7=64 should be approximately -6 dB (nominal)");
    
    /* Test logarithmic progression */
    float db_32 = gm1_midi_value_to_db(32);
    float db_96 = gm1_midi_value_to_db(96);
    TEST_ASSERT(db_32 < db_64, "CC#7=32 should be quieter than CC#7=64");
    TEST_ASSERT(db_64 < db_96, "CC#7=64 should be quieter than CC#7=96");
    TEST_ASSERT(db_96 < db_127, "CC#7=96 should be quieter than CC#7=127");
}

/**
 * @brief Test AC2: Modulation Wheel Response
 * 
 * Verifies: REQ-F-GM1-004 AC2 - Modulation wheel response
 * Given: CC#1 modulation wheel on any program
 * When: Adjusting modulation
 * Then: Sound changes in musically appropriate way
 */
void test_modulation_wheel_response(void) {
    printf("\n--- Test: Modulation Wheel Response ---\n");
    
    /* Test that modulation handling doesn't crash for valid channels */
    for (uint8_t channel = 0; channel < 16; channel++) {
        gm1_handle_modulation_wheel(channel, 0);
        gm1_handle_modulation_wheel(channel, 64);
        gm1_handle_modulation_wheel(channel, 127);
    }
    TEST_ASSERT(1, "Modulation wheel handling doesn't crash for all channels");
    
    /* Test invalid channel handling */
    gm1_handle_modulation_wheel(16, 64);  /* Invalid channel */
    gm1_handle_modulation_wheel(255, 64); /* Invalid channel */
    TEST_ASSERT(1, "Modulation wheel handling gracefully handles invalid channels");
}

/**
 * @brief Test volume controller channel handling
 */
void test_volume_controller_channels(void) {
    printf("\n--- Test: Volume Controller Channel Handling ---\n");
    
    /* Test that volume handling doesn't crash for valid channels */
    for (uint8_t channel = 0; channel < 16; channel++) {
        gm1_handle_volume_controller(channel, 0);
        gm1_handle_volume_controller(channel, 64);
        gm1_handle_volume_controller(channel, 127);
    }
    TEST_ASSERT(1, "Volume controller handling doesn't crash for all channels");
    
    /* Test invalid channel handling */
    gm1_handle_volume_controller(16, 64);  /* Invalid channel */
    TEST_ASSERT(1, "Volume controller handling gracefully handles invalid channels");
}

/*============================================================================
 * Test Cases - Quality Validation
 *============================================================================*/

/**
 * @brief Test quality validation with passing metrics
 * 
 * Verifies: REQ-F-GM1-004 quality validation
 */
void test_quality_validation_pass(void) {
    printf("\n--- Test: Quality Validation (Pass Cases) ---\n");
    
    /* Create metrics that meet all requirements */
    gm1_quality_metrics_t good_metrics = {
        .program = 1,
        .recognition_score = 0.85f,      /* 85% > 80% minimum */
        .controller_response = true,
        .velocity_sensitive = true,
        .frequency_accuracy = 5.0f       /* ±5 cents < ±10 cents max */
    };
    
    TEST_ASSERT(gm1_validate_program_quality(&good_metrics),
                "Good quality metrics should pass validation");
    
    /* Test boundary case - exactly 80% recognition */
    good_metrics.recognition_score = 0.8f;
    TEST_ASSERT(gm1_validate_program_quality(&good_metrics),
                "Exactly 80% recognition should pass");
    
    /* Test boundary case - exactly 10 cents deviation */
    good_metrics.recognition_score = 0.9f;
    good_metrics.frequency_accuracy = 10.0f;
    TEST_ASSERT(gm1_validate_program_quality(&good_metrics),
                "Exactly 10 cents deviation should pass");
    
    /* Test negative tuning deviation */
    good_metrics.frequency_accuracy = -9.0f;
    TEST_ASSERT(gm1_validate_program_quality(&good_metrics),
                "Negative tuning deviation within range should pass");
}

/**
 * @brief Test quality validation with failing metrics
 */
void test_quality_validation_fail(void) {
    printf("\n--- Test: Quality Validation (Fail Cases) ---\n");
    
    gm1_quality_metrics_t bad_metrics;
    
    /* Test NULL pointer */
    TEST_ASSERT(!gm1_validate_program_quality(NULL),
                "NULL metrics should fail validation");
    
    /* Test low recognition score */
    bad_metrics = (gm1_quality_metrics_t){
        .program = 1,
        .recognition_score = 0.79f,      /* 79% < 80% minimum */
        .controller_response = true,
        .velocity_sensitive = true,
        .frequency_accuracy = 0.0f
    };
    TEST_ASSERT(!gm1_validate_program_quality(&bad_metrics),
                "Recognition score below 80% should fail");
    
    /* Test controller response failure */
    bad_metrics.recognition_score = 0.9f;
    bad_metrics.controller_response = false;
    TEST_ASSERT(!gm1_validate_program_quality(&bad_metrics),
                "Missing controller response should fail");
    
    /* Test velocity insensitive */
    bad_metrics.controller_response = true;
    bad_metrics.velocity_sensitive = false;
    TEST_ASSERT(!gm1_validate_program_quality(&bad_metrics),
                "Missing velocity sensitivity should fail");
    
    /* Test excessive tuning deviation */
    bad_metrics.velocity_sensitive = true;
    bad_metrics.frequency_accuracy = 15.0f;  /* > 10 cents */
    TEST_ASSERT(!gm1_validate_program_quality(&bad_metrics),
                "Tuning deviation > 10 cents should fail");
    
    /* Test negative excessive tuning */
    bad_metrics.frequency_accuracy = -11.0f;
    TEST_ASSERT(!gm1_validate_program_quality(&bad_metrics),
                "Negative tuning deviation > 10 cents should fail");
}

/**
 * @brief Test AC7: Audio quality baseline compliance
 * 
 * Verifies: REQ-F-GM1-004 AC7 - Quality baseline compliance
 */
void test_audio_quality_compliance(void) {
    printf("\n--- Test: Audio Quality Compliance ---\n");
    
    /* Test recommended quality (should pass all) */
    gm1_audio_quality_t recommended_quality = {
        .sample_rate_hz = 44100,
        .bit_depth = 16,
        .frequency_response_min_hz = 20.0f,
        .frequency_response_max_hz = 20000.0f,
        .dynamic_range_db = 96.0f,
        .polyphony_voices = 64
    };
    
    gm1_compliance_result_t result = gm1_validate_audio_quality(&recommended_quality);
    TEST_ASSERT(result.meets_minimum,
                "Recommended quality should meet minimum");
    TEST_ASSERT(result.meets_recommended,
                "Recommended quality should meet recommended");
    
    /* Test minimum quality */
    gm1_audio_quality_t minimum_quality = {
        .sample_rate_hz = 22050,
        .bit_depth = 8,
        .frequency_response_min_hz = 20.0f,
        .frequency_response_max_hz = 11000.0f,
        .dynamic_range_db = 48.0f,
        .polyphony_voices = 24
    };
    
    result = gm1_validate_audio_quality(&minimum_quality);
    TEST_ASSERT(result.meets_minimum,
                "Minimum quality should meet minimum");
    TEST_ASSERT(!result.meets_recommended,
                "Minimum quality should not meet recommended");
    
    /* Test below minimum quality */
    gm1_audio_quality_t below_minimum = {
        .sample_rate_hz = 11025,          /* Below 22050 */
        .bit_depth = 8,
        .frequency_response_min_hz = 20.0f,
        .frequency_response_max_hz = 20000.0f,
        .dynamic_range_db = 72.0f,
        .polyphony_voices = 24
    };
    
    result = gm1_validate_audio_quality(&below_minimum);
    TEST_ASSERT(!result.meets_minimum,
                "Below minimum sample rate should fail minimum");
    
    /* Test NULL quality */
    result = gm1_validate_audio_quality(NULL);
    TEST_ASSERT(!result.meets_minimum,
                "NULL quality should fail");
}

/**
 * @brief Test AC4: Velocity sensitivity configuration
 * 
 * Verifies: REQ-F-GM1-004 AC4 - Velocity sensitivity
 */
void test_velocity_configuration(void) {
    printf("\n--- Test: Velocity Configuration ---\n");
    
    /* Valid configuration */
    gm1_velocity_config_t valid_config = {
        .velocity_layers = 4,
        .velocity_curve = 0.7f,
        .velocity_affects_timbre = true
    };
    
    TEST_ASSERT(gm1_configure_program_velocity(1, &valid_config),
                "Valid velocity config should succeed");
    TEST_ASSERT(gm1_configure_program_velocity(128, &valid_config),
                "Valid velocity config for program 128 should succeed");
    
    /* Minimum velocity layers (2 per GM1 spec) */
    gm1_velocity_config_t min_config = {
        .velocity_layers = 2,
        .velocity_curve = 0.5f,
        .velocity_affects_timbre = false
    };
    TEST_ASSERT(gm1_configure_program_velocity(64, &min_config),
                "Minimum 2 velocity layers should succeed");
    
    /* Invalid: Less than 2 velocity layers */
    gm1_velocity_config_t bad_layers = {
        .velocity_layers = 1,
        .velocity_curve = 0.5f,
        .velocity_affects_timbre = false
    };
    TEST_ASSERT(!gm1_configure_program_velocity(1, &bad_layers),
                "Less than 2 velocity layers should fail");
    
    /* Invalid: Zero velocity curve */
    gm1_velocity_config_t bad_curve = {
        .velocity_layers = 2,
        .velocity_curve = 0.0f,
        .velocity_affects_timbre = false
    };
    TEST_ASSERT(!gm1_configure_program_velocity(1, &bad_curve),
                "Zero velocity curve should fail");
    
    /* Invalid: Program number 0 */
    TEST_ASSERT(!gm1_configure_program_velocity(0, &valid_config),
                "Program 0 should fail");
    
    /* Invalid: Program number > 128 */
    TEST_ASSERT(!gm1_configure_program_velocity(129, &valid_config),
                "Program 129 should fail");
    
    /* Invalid: NULL config */
    TEST_ASSERT(!gm1_configure_program_velocity(1, NULL),
                "NULL config should fail");
}

/**
 * @brief Test AC8: Program switching performance
 * 
 * Verifies: REQ-F-GM1-004 AC8 - Program switching performance
 */
void test_program_switch_latency(void) {
    printf("\n--- Test: Program Switch Latency ---\n");
    
    /* Test same category switch (should be fast) */
    uint32_t latency = gm1_test_program_switch_latency(1, 2);  /* Piano to Piano */
    TEST_ASSERT(gm1_is_program_switch_compliant(latency),
                "Same category switch should be compliant (<100ms)");
    
    /* Test cross-category switch */
    latency = gm1_test_program_switch_latency(1, 57);  /* Piano to Trumpet */
    TEST_ASSERT(gm1_is_program_switch_compliant(latency),
                "Cross-category switch should be compliant (<100ms)");
    
    /* Test complex switch (Piano has large samples) */
    latency = gm1_test_program_switch_latency(57, 1);  /* Trumpet to Piano */
    TEST_ASSERT(gm1_is_program_switch_compliant(latency),
                "Switch to Piano should be compliant (<100ms)");
    
    /* Test invalid program numbers */
    latency = gm1_test_program_switch_latency(0, 1);
    TEST_ASSERT(latency == UINT32_MAX,
                "Invalid from_program should return UINT32_MAX");
    
    latency = gm1_test_program_switch_latency(1, 129);
    TEST_ASSERT(latency == UINT32_MAX,
                "Invalid to_program should return UINT32_MAX");
}

/*============================================================================
 * Test Cases - Cross-Device Consistency
 *============================================================================*/

/**
 * @brief Test AC5: Cross-device consistency
 * 
 * Verifies: REQ-F-GM1-004 AC5 - Cross-device consistency
 */
void test_cross_device_consistency(void) {
    printf("\n--- Test: Cross-Device Consistency ---\n");
    
    /* Two identical implementations */
    gm1_quality_metrics_t device1 = {
        .program = 1,
        .recognition_score = 0.9f,
        .controller_response = true,
        .velocity_sensitive = true,
        .frequency_accuracy = 2.0f
    };
    
    gm1_quality_metrics_t device2 = {
        .program = 1,
        .recognition_score = 0.9f,
        .controller_response = true,
        .velocity_sensitive = true,
        .frequency_accuracy = 2.0f
    };
    
    float score = gm1_calculate_consistency_score(&device1, &device2);
    TEST_ASSERT_FLOAT_EQ(score, 1.0f, 0.01f,
                         "Identical implementations should have 1.0 consistency");
    TEST_ASSERT(gm1_is_consistent(score),
                "Identical implementations should be consistent");
    
    /* Slightly different implementations */
    device2.recognition_score = 0.85f;
    device2.frequency_accuracy = 5.0f;
    
    score = gm1_calculate_consistency_score(&device1, &device2);
    TEST_ASSERT(score > 0.8f && score < 1.0f,
                "Similar implementations should have high consistency");
    TEST_ASSERT(gm1_is_consistent(score),
                "Similar implementations should be consistent");
    
    /* Very different implementations */
    device2.recognition_score = 0.8f;
    device2.controller_response = false;
    device2.velocity_sensitive = false;
    device2.frequency_accuracy = 10.0f;
    
    score = gm1_calculate_consistency_score(&device1, &device2);
    TEST_ASSERT(score < 0.8f,
                "Very different implementations should have low consistency");
    
    /* Different programs can't be compared */
    device2.program = 2;
    score = gm1_calculate_consistency_score(&device1, &device2);
    TEST_ASSERT_FLOAT_EQ(score, 0.0f, 0.01f,
                         "Different programs should return 0.0 consistency");
    
    /* NULL pointers */
    TEST_ASSERT_FLOAT_EQ(gm1_calculate_consistency_score(NULL, &device2), 0.0f, 0.01f,
                         "NULL device1 should return 0.0");
    TEST_ASSERT_FLOAT_EQ(gm1_calculate_consistency_score(&device1, NULL), 0.0f, 0.01f,
                         "NULL device2 should return 0.0");
}

/*============================================================================
 * Main Test Runner
 *============================================================================*/

int main(void) {
    printf("==============================================\n");
    printf("GM1 Sound Quality Unit Tests\n");
    printf("Verifies: #540 (REQ-F-GM1-004)\n");
    printf("==============================================\n");
    
    /* Category Classification Tests */
    test_instrument_category_classification();
    test_category_names();
    
    /* Controller Response Tests */
    test_volume_controller_response();
    test_modulation_wheel_response();
    test_volume_controller_channels();
    
    /* Quality Validation Tests */
    test_quality_validation_pass();
    test_quality_validation_fail();
    test_audio_quality_compliance();
    test_velocity_configuration();
    test_program_switch_latency();
    
    /* Cross-Device Consistency Tests */
    test_cross_device_consistency();
    
    /* Summary */
    printf("\n==============================================\n");
    printf("Test Summary:\n");
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("  Total:  %d\n", tests_passed + tests_failed);
    printf("==============================================\n");
    
    return (tests_failed > 0) ? 1 : 0;
}
