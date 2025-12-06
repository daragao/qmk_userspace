/**
 * Utilities for QMK oled animations
 *
 * Copyright (c) Marek Piechut
 * MIT License
 */
#pragma once

//-------- CONFIGURATION START --------

#ifndef ANIM_RENDER_WPM
  #define ANIM_RENDER_WPM true
#endif
#ifndef FAST_TYPE_WPM
  #define FAST_TYPE_WPM 45 //Switch to fast animation when over words per minute
#endif
#ifndef ANIM_INVERT
  #define ANIM_INVERT false //Invert animation color and background
#endif
#ifndef ANIM_BG
  #define ANIM_BG 0x00
#endif
#ifndef ANIM_FRAME_TIME
  #define ANIM_FRAME_TIME 250
#endif
//-------- CONFIGURATION END--------

#define ANIM_WPM_WIDTH 22
#define OLED_ROWS OLED_DISPLAY_HEIGHT / 4

// Function prototypes (needed by keymap.c)
void oled_render_anim_frame(const char **fast_frames, const char **slow_frames, uint8_t frames_len);
