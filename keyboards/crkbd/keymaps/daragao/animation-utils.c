#include QMK_KEYBOARD_H // Includes core QMK definitions
#include <qmk.h> // <-- Use the universal QMK header
#include "animation-utils.h" // Your custom header
#include <quantum.h>

// ... rest of the file ...

static void oled_render_wpm(void) {
  // We need to use `oled_write_P` with `get_u8_str` if WPM rendering is complex,
  // but for simplicity, we use the core functions.

  oled_set_cursor(0, 1);
  oled_write_P(PSTR("WPM"), false);
  oled_set_cursor(0, 2);
  oled_write(get_u8_str(get_current_wpm(), '0'), false);
}

void oled_render_anim_frame(const char **fast_frames, const char **slow_frames, uint8_t frames_len) {
  // ALL THE CODE FOR THE FUNCTION GOES HERE:
  static uint32_t anim_timer = 0;
  static uint8_t current_frame = 0;
  static int16_t frame_offset = ANIM_RENDER_WPM ? ANIM_WPM_WIDTH : 0;
  static QMK_UNUSED int8_t step = 8;

  const uint8_t speed = get_current_wpm();

  if (timer_elapsed32(anim_timer) > ANIM_FRAME_TIME && speed > 0) {
    oled_set_cursor(0, 0);
    anim_timer = timer_read32();

    const bool is_fast = speed > FAST_TYPE_WPM;
    const char *frame = is_fast ? fast_frames[current_frame] : slow_frames[current_frame];
    const int8_t bg = ANIM_BG;

    uint8_t frame_start_offset = ANIM_RENDER_WPM ? ANIM_WPM_WIDTH : 0;
    current_frame = (current_frame + 1) % frames_len;

    for(int offset = 0; offset <= OLED_MATRIX_SIZE; offset++) {
      uint16_t col = offset % OLED_DISPLAY_WIDTH;

      if(col < frame_start_offset) {
        continue;
      } else if (col < frame_offset || col > frame_offset + ANIM_FRAME_WIDTH) {
        oled_write_raw_byte((ANIM_INVERT ? ~bg : bg), offset);
      } else if (col <= OLED_DISPLAY_WIDTH) {
        uint8_t row = offset / OLED_DISPLAY_WIDTH;
        int frame_data_offset = ANIM_FRAME_WIDTH * row + col - frame_offset;
        uint8_t col_data = pgm_read_byte(frame + frame_data_offset);
        oled_write_raw_byte(ANIM_INVERT ? ~col_data : col_data, offset );
      }
    }

    #ifdef ANIM_SCROLL
    if(is_fast) {
      #ifdef ANIM_BOUNCE
      if(frame_offset + ANIM_FRAME_WIDTH > OLED_DISPLAY_WIDTH || frame_offset < frame_start_offset) {
        step = -step;
      }
      #endif

      frame_offset += step;

      if(frame_offset >= OLED_DISPLAY_WIDTH) {
        frame_offset = frame_start_offset - ANIM_FRAME_WIDTH + step;
      }
    } else if (frame_offset > (OLED_DISPLAY_WIDTH - ANIM_FRAME_WIDTH) || frame_offset < frame_start_offset) {
      frame_offset = frame_start_offset;
    }
    #endif
  }

  if(ANIM_RENDER_WPM) {
    oled_render_wpm();
  }
}
